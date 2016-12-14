#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "global.h"

// dictionary mapper
struct mapper{
   char *bufferdb;
   int size;
   FILE *fp;
};

struct mapper *loader(){
   struct mapper *mp;
   struct stat st;

   FILE *fp;

   if(stat(WORDS, &st) != 0){
     perror("dictionary file was not found");
     exit(EXIT_FAILURE);
   }

   // allocating struct //
   mp = malloc(sizeof(struct mapper));
   if(mp == NULL){
     perror("failed to allocate struct memory");
     exit(EXIT_FAILURE);
   }

   mp->size = st.st_size;

   mp->bufferdb = malloc(sizeof(char) * mp->size);
   if(mp->bufferdb == NULL){
     perror("failed to allocate memory for dictionary");
     free(mp);
     exit(EXIT_FAILURE);
   }

   printf("allocated:%lu memory\n", sizeof(char) * mp->size);

   /* open dictionary file */
   if((mp->fp = fopen(WORDS, "r")) == NULL){
    perror("open dictionary file failed");
    free(mp->bufferdb);
    free(mp);
    exit(EXIT_FAILURE);
   }

   // allocated struct
   return mp;
}

// computing edit distance algorithm //
int D(struct mapper *mp, char *msg){
   int l;

   // rewind to the beginning of the file 
   fseek(mp->fp, 0, SEEK_SET);

   // reading dictionary 
   while(fgets(mp->bufferdb, mp->size, mp->fp)){
    // remove trailing line
    mp->bufferdb [strlen(mp->bufferdb) -1] = 0;
    // calling levenshtein
    l = levenshtein(msg,  mp->bufferdb);
    // this needs to be tested 
    if(l <= BENIGN){
      return 0;
    }
   }
   // number of transformations 
   return l;
}

// socket handler //
void msghandler (int sock, struct mapper *mp) {
  int n, l;
  char buffer[MAX_MSG], response[MAX_MSG];

  bzero(buffer, MAX_MSG);
  n = read(sock, buffer, MAX_MSG);
   
  if (n < 0) {
     perror("ERROR reading from socket");
     exit(1);
  }
  
   // remove trailing line received from socket
  buffer[strlen(buffer) -1] = 0;
   // check if in cache

  l = D(mp, buffer);
   // send analysis result//
  if(l <= BENIGN){
     snprintf(response, MAX_MSG, "non dga algorithm, number of transformation:%d", l);
     n = write(sock, response, strlen(response));
  }else{
     snprintf(response, MAX_MSG, "potential dga detect, number of transformation:%d", l);
     n = write(sock, response, strlen(response));
  }
   
  if (n < 0) {
     perror("ERROR writing to socket");
     exit(1);
  }
}

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];

   struct sockaddr_in serv_addr, cli_addr;
   struct mapper *mp;
 
   int n, pid;

   printf("starting server\n");
   // allocating memory and open file //
   mp = loader();

   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 5001;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* bind */
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(EXIT_FAILURE);
   }
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   while (1) {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,  (socklen_t *)&clilen);
        
      if (newsockfd < 0) {
         perror("ERROR on accept");
         exit(EXIT_FAILURE);
      }
      
      /* fork */
      pid = fork();
        
      if (pid < 0) {
         perror("ERROR on fork");
         exit(EXIT_FAILURE);
      }
      
      if (pid == 0) {
         /* client process */
         close(sockfd);
         // scanner message 
         msghandler(newsockfd, mp);
         exit(EXIT_SUCCESS);
      }
      else {
         close(newsockfd);
      }
        
   }
}
