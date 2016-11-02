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
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

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

  // load once
  p = loader();
  if(p == NULL){
    printf("loader failed, cannot proceed\n");
    exit(-1);
  }

  printf("%s\n", p->mapping);

  /*

    server goes here 
    
  */

  // cleanup
  i = clean_all(p->mapping, p);
  if(i == EXIT_ERR){
    printf("cleanup return with error\n");
  }


  p = NULL;

  return 0;
}
