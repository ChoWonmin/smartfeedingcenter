#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024

int main(void) {
	
	int server_socket;
	int client_socket;
	int client_addr_size;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	char buff_rcv[BUFF_SIZE+5];
	char buff_snd[BUFF_SIZE+5] = "get";

	/*
	 * socket(int domain, int type, int protocol) 
	 * domain : set whether communicate through internet or between processes in the same system.
	 * type : set a type of transmission of data
	 * protocol : a variable which set using a specific protocol in communication, generally use "0"
	 */

	// PF_INET : use IPv4 
	// SOCK_STREAM : use TCP/IP protocol
	// create socket and return
	// this is a just socket descriptor
	// When a socket is created, it exists in a name space(address family) but has no address assigned to it.
	server_socket = socket(PF_INET, SOCK_STREAM, 0); 
	if(server_socket == -1) {
		printf("fail to create server socket\n");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr)); // initiate memory to 0
	server_addr.sin_family = AF_INET; // IPv4 internet protocol
	// uint16_t htons(uint16_t hostshort) : converts the unsigned short integer hostshort from host byte order to network byte order 
	server_addr.sin_port = htons(4000); // port number 4000 for using
	// uint32_t htonl(uint32_t hostlong) : convert the unsigned integer hostlong from host byte order to network byte order
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 32bit IPv4 address

	// int bind(nt sockfd, const struct sockaddr *addr, socklen_t addrlen) : assigns the address specified by "addr" to the socket 
	// referred to by the file descriptor sockfd.
	if(bind(server_socket,(struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		printf("bind execute error\n");
		exit(1);
	}

	// check request of client connection
	// int listen(int sockfd, int backlog) : marks the socket referred to by sockfd as a passive socket
	if(listen(server_socket, 5) == -1) {
		printf("fail to set listen mode\n");
		exit(1); 
	}

	printf("listening..\n");

	while(1) {

		client_addr_size = sizeof(client_addr);
		// int accept( int sockfd, struct sockaddr *addr, socklen_t *addrlen) : extracts the first connection request 
		// on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and
		// return a new file descriptor referring to that socket.
		client_socket = accept( server_socket,(struct sockaddr*)&client_addr, &client_addr_size); // allow client to connect
		if(client_socket == -1) {
			printf("fail to allow client connection\n");
			exit(1);
		}

		printf("accept client\n");

		sleep(10);
		write(client_socket, buff_snd, strlen(buff_snd) + 1);
		puts("write success\n");
		read(client_socket, buff_rcv, BUFF_SIZE);
		printf("receive : %s\n", buff_rcv);

		close(client_socket);
	}

}
