#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "global.h"

int main(){
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

 mem_map = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
 if(mem_map == MAP_FAILED){
     perror("failed to map words file");
     close(fd);
     exit(-1);
 }

 printf("%s\n", mem_map);

 // unmapping the file
 if(munmap(mem_map, st.st_size) == -1){
   perror("failed to unmap");
   close(fd);
   exit(-1);
 }

 close(fd);
}
