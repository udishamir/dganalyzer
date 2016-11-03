/*
___________      ____
\__    ___/____ /_   | ____   ____
  |    |  \__  \ |   |/  _ \ /    \
  |    |   / __ \|   (  <_> )   |  \
  |____|  (____  /___|\____/|___|  /
               \/                \/

Copyright (c) 2016, Ehud Shamir
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "global.h"

struct map{
  uint32_t size;
  void *mapping;
};
  
int clean_all(char *mem_map, struct map *p){
  // unmapping the file
   if(munmap(mem_map, sizeof(mem_map)) == -1){
     perror("failed to unmap");
   }

   // free the allocated struct
   free(p);

   return 0;
}

struct map *loader(){
   int fd;
 
   struct map *mp;
   struct stat st;

   mp = malloc(sizeof(struct map));
   if (mp == NULL){
     perror("failed to allocate memory");

     return NULL;
   }

   if(stat(WORDS, &st) != 0){
     perror("stat failed");
     free(mp);
    
     return NULL;
   }

   fd = open(WORDS, O_RDONLY);
   if(fd == EXIT_ERR){
     free(mp);
     perror("failed to open db file");
     
     return NULL;
   }

   mp->size = st.st_size;

   // allocating memory for the mapping 
   mp->mapping = malloc(sizeof(char *) * mp->size + 1);
   if(mp->mapping == NULL){
    perror("malloc failed for mapping section");
    close(fd);
    free(mp);

    return NULL;
   }

   // mapping the file to memory
   mp->mapping = mmap(0, mp->size, PROT_READ, MAP_PRIVATE, fd, 0);
   if(mp->mapping== MAP_FAILED){
     perror("failed to map words file");
     close(fd);
     free(mp);

     return NULL;
   }

   // memory mapped, we can close the handle
   close(fd);

   // returning the struct 
   return mp;
}

int main(){
  struct map *p;
  int i = 0;

  uint32_t t;
  int s, b, l,  s2, len;
  struct sockaddr_un local, remote;
  char msg[MAX_MSG]; 

  // load once
  p = loader();
  if(p == NULL){
    printf("loader failed, cannot proceed\n");
    exit(-1);
  }


  /*
    https://beej.us/guide/bgipc/output/html/multipage/unixsock.html

    
    server goes here 
    
  */

  // create domain, stream socket
  s = socket(AF_UNIX, SOCK_STREAM, 0);
  if(s == EXIT_ERR){
     i = clean_all(p->mapping, p);
     if(i == EXIT_ERR){
        printf("cleanup return with error\n");
        exit(EXIT_ERR);
     }
     exit(EXIT_ERR);
   }


  local.sun_family = AF_UNIX;
  /*
      create the sock file (check global.h for the file name, or to change it).
      we will communicate bi-directional using this file.
  */
  strcpy(local.sun_path, SOCK_PATH);

  unlink(local.sun_path);

  len = strlen(local.sun_path) + sizeof(local.sun_family);

  // bind 
  b = bind(s, (struct sockaddr *)&local, len);
  if(b == EXIT_ERR){
    printf("bind failed, exiting\n");
    i = clean_all(p->mapping, p);
    if(i == EXIT_ERR){
       printf("cleanup return with error\n");
       exit(EXIT_ERR);
    }
    exit(EXIT_ERR);
  }

 // we hold queue of max 5 connections, check global.h to change the queue size
  l = listen(s, MAX_QUEUE);
  if(l == EXIT_ERR){
     printf("bind failed, exiting\n");
     i = clean_all(p->mapping, p);
     if(i == EXIT_ERR){
        printf("cleanup return with error\n");
        exit(EXIT_ERR);
     }
     exit(EXIT_ERR);
  }

  // listen and block for connection
  for(;;){
      int done, n;
      printf("waiting for connection\n");
      t = sizeof(remote); 
      s2 = accept(s, (struct sockaddr *)&remote, &t);
      if(s2 == EXIT_ERR){
         printf("accept failed, existing\n");
         //cleanup
         i = clean_all(p->mapping, p);
         if(i == EXIT_ERR){
            printf("cleanup return with error\n");
            exit(EXIT_ERR);
         }
         exit(EXIT_ERR);
      }
      
      printf("received scannig request\n");

      done = 0;
      do{
         n = recv(s2, msg, MAX_MSG, 0);
         // we got a connection without any data 
         if(n <= 0){
            // recv failed  
            if(n < 0){
               perror("recv");
               // exit the loop
               done = 1;
               close(s2);
             }
          }
      } while (!done);
    
      close(s2);
  }
     
  // cleanup
  i = clean_all(p->mapping, p);
  if(i == EXIT_ERR){
     printf("cleanup return with error\n");
  }
  
  p = NULL;

  return 0;
}
