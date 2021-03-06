/**************************************************************************************
* Server TCP Program
* @file - server.c
* Reference: https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
*
***************************************************************************************/
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <fcntl.h>
#include <unistd.h>

#include <sys/msg.h> 
#include <syslog.h>


#define MAX 80 
#define PORT 9000 
#define SA struct sockaddr 


int msgid;
// structure for message queue
struct mesg_buffer {
	long mesg_type;
	char mesg_text[100];
} message;


// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
	//char buff[MAX]; //using message queue instead of this buffer
	int n; 
	// infinite loop for chat 
	for (;;) { 
		//bzero(buff, MAX); 

		// read the message from client and copy it in buffer 
		read(sockfd, message.mesg_text, sizeof(message.mesg_text)); 

		//message queue
		message.mesg_type = 1;
		msgsnd(msgid,&messgae,sizeof(message),0);
		// print buffer which contains the client contents 
		printf("From client: %s\t To client : ", message.mesg_text); 
		memset(message.mesg_text, 0x0, (100*sizeof(char)));
		} 
	} 
} 

// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	//initialization for message queues
	key_t key;
	key = ftok("progfile",65);
	msgid = msgget(key,0666 | IPC_CREAT);
	

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, (socklen_t*)&len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 

