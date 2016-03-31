/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define DATAGRAM_SIZE	1024
void error_transmit(const char *);
int transmit(char *ip_addr, char* port, unsigned char* data)
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256];
  // if (argc != 3) { printf("Usage: server port\n");
  //                  exit(1);
  // }
//  sock= socket(AF_INET, SOCK_DGRAM, 0);
	sock = socket(AF_INET, SOCK_STREAM, 0);
   if (sock < 0) error_transmit("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(ip_addr);
   if (hp==0) error_transmit("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(port));
   length=sizeof(struct sockaddr_in);
   //printf("Please enter the message: ");
   //bzero(buffer,256);
   //fgets(buffer,255,stdin);
	//scanf("%u", test);
    if (connect(sock,(struct sockaddr *) &server,sizeof(server)) < 0)
        error_transmit("ERROR connecting");

//   n=sendto(sock,data,
  //          DATAGRAM_SIZE,0,(const struct sockaddr *)&server,length);
	n = write(sock,data, DATAGRAM_SIZE);

   if (n < 0) error_transmit("Write");
  // n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);
  // if (n < 0) error_transmit("recvfrom");
  // write(1,"Got an ack: ",12);
  // write(1,buffer,n);
   close(sock);
   return 0;
}

void error_transmit(const char *msg)
{
    perror(msg);
    exit(0);
}
