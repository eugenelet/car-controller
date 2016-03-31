/* Creates a datagram server.  The port 
   number is passed as an argument.  This
   server runs forever */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

int sock, length, n;
socklen_t fromlen;
struct sockaddr_in server;
struct sockaddr_in from;
char buf[1024];
unsigned char recvPacket[1024];

void error_receiver(const char *msg)
{
	perror(msg);
	exit(0);
}

void receiver_init(char *port)
{
	//if (argc < 2) {
	// 	fprintf(stderr, "ERROR, no port provided\n");
	// 	exit(0);
	//}
	
	sock=socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) error_receiver("Opening socket");
	length = sizeof(server);
	bzero(&server,length);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(atoi(port));
	if (bind(sock,(struct sockaddr *)&server,length)<0) 
		error_receiver("binding");
	fromlen = sizeof(struct sockaddr_in);
	 //	write(1,"Received a datagram: ",21);
	 //	 snprintf(buf, 1024,"%u",recvPacket[0]);
	 //	write(1,buf,n);
	// 	n = sendto(sock,"Got your message\n",17,
	// 	           0,(struct sockaddr *)&from,fromlen);
	// 	if (n  < 0) error_receiver("sendto");
}

unsigned char* receiver(){
	n = recvfrom(sock,recvPacket,1024,0,(struct sockaddr *)&from,&fromlen);
	if (n < 0) error_receiver("recvfrom");
//	cout << hex << recvPacket[0]<<endl;
	return recvPacket;
}
