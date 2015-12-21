

#include <stdio.h>
#include <stdlib.h>

//#include "flux.h"

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main() {
	while(1) {
		listenSocket();
		sleep(1);
	}
	return 0;
}


int listenSocket() {
 
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

   /* Write a response to the client */
   n = write(newsockfd,"May the Force be with you",25);
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }

   close(sockfd);
   close(newsockfd);
      
   return buffer[3];
}

