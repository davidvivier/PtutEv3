

#include <stdio.h>
#include <stdlib.h>


#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string.h>


#include "flux.h"

#ifndef INCLUDED_PROGRAM_H

void a() {

}

int Main() {
   a();
   //testSensor(0);

   /*
	while(1) {
		listenSocket();
		sleep(1);
	}*/
   listenSocket();
	return 0;
}

#endif

char* listenSocket() {

   //int *a = calloc(4, sizeof(int));
   //int b = a[50];
 
   int sockfd, newsockfd, portno, clilen;
   //char buffer[256];
   char* buffer = malloc(256*sizeof(char));
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   int yes = 1;
   setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
   setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
   
   setsockopt(newsockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
   setsockopt(newsockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
   
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
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   
   printf("listening on port %d...\n", portno);
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }
   
   /* If connection is established then start communicating */
   //int i = 0;
   //while (i < 10)  {

      //printf("i=%d | ", i);
      //i++;

      printf("1-");

      bzero(buffer,256);

      printf("2-");

      n = read( newsockfd,buffer,255 );
      
      printf("3-");

      if (n < 0) {
         perror("ERROR reading from socket");
         exit(1);
      }

      printf("4-");

      //printf("Here is the message: %d - size = %d - client : %d\n",buffer[0], sizeof(buffer), cli_addr.sin_port);
      printf("Received : \n");
      int i = 0;
      for (i = 0; i < 16; i++) {
      		printf("0x%02x-", buffer[i]);
      }
      int *a = malloc(4*sizeof(int));
      int b = a[50];
      printf("\n");
      printf("\n5-");

      /* Write a response to the client */
      //n = write(newsockfd,"May the Force be with you",25);


      char res = 0;

      for (i = 0; i < 255; i++) {
         res = res ^ buffer[i];
      }
      printf("Renvoi a la tablette : %d\n", res);
      n = write(newsockfd, &res, 1);
      printf("6-");

      if (n < 0) {
         perror("ERROR writing to socket");
         exit(1);
      }
      printf("7-");

   //}

   close(sockfd);
   close(newsockfd);
      
   return buffer;
}



int sendSocket(char msg[]) {
 
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   
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
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   
   printf("listening on port %d...\n", portno);
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }
   
   /* If connection is established then start communicating */
   bzero(buffer,256);
   n = read( newsockfd,buffer,255 );
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   
   //printf("Here is the message: %d - size = %d - client : %d\n",buffer[0], sizeof(buffer), cli_addr.sin_port);
   printf("Received : \n");
   int i = 0;
   for (i = 0; i < sizeof(buffer); i++) {
         printf("%d-", buffer[i]);
   }
   printf("\n");

   if (buffer[3] == OCTET_DEMANDE_MSG)
   /* Write a response to the client */
   n = write(newsockfd, msg,sizeof(msg));
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }

   close(sockfd);
   close(newsockfd);
      
   return buffer[3];
}



