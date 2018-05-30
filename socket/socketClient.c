#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define BUFF_SIZE 1024

int main(int argc, char** argv) {
	
	int client_socket;
	struct sockaddr_in server_addr;

	char buff[BUFF_SIZE + 5];

	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(client_socket == -1) {
		printf("fail to create client socket\n");
		exit(1);
	}

	// initiate
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4000);
	/*
	 * in_addr_t inet_addr(const char *cp) : converts the internet host address "cp" from IPv4
	 * numbers and dots notation in network byte order. 
	 */
	server_addr.sin_addr.s_addr = inet_addr("192.168.0.41"); // set ip address of server_addr

	// attempt to make a connection on a socket.
	if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) 
	{
		printf("fail to connect to server\n");
		exit(1);
	}

	printf("success to connect\n");


	return 0;
}
