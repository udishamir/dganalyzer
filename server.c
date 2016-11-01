#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "global.h"

int clean_all(char *mem_map){
  // unmapping the file
   if(munmap(mem_map, sizeof(mem_map)) == -1){
     perror("failed to unmap");
     return -1;
   }

   return 0;
}

char *loader(){
   int fd;
   uint32_t size;
   void *mem_map;

   struct stat st;
   if(stat(WORDS, &st) != 0){
     perror("stat failed");
     exit(-1);
   }

   fd = open(WORDS, O_RDONLY);
   if(fd == -1){
     perror("failed to open words");
     exit(-1);
   }

   size = st.st_size;
   printf ("%u\n", size);

   mem_map = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
   if(mem_map == MAP_FAILED){
     perror("failed to map words file");
     close(fd);
     exit(-1);
   }

   // memory mapped, we calose the handle
   close(fd);

   return mem_map;
}

int main(){

  void *mem_map;
  int i = 0;

  // load once
  mem_map = loader();
  printf("%s\n", mem_map);

  // unmapping the file
  i = clean_all(mem_map);
  if(i == -1){
    printf("failed to munmap\n");
  }

}
