/*
/ Eric Olson and James Baker
/ CSE 5462 Network Programming
/ Lab 3 - September 15, 2016
/ 
/ This file contains our implementations of TCP functions using UDP.
*/
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define LOCALPORT 9999
#define LOCALADDRESS "127.0.0.1"

/* Prototypes for each function */ 
ssize_t SEND(int socket, const void* buffer, size_t length, int flags);
ssize_t RECV(int socket, void* buffer, size_t length, int flags);
int BIND(int socket, struct sockaddr *my_addr, socklen_t addrlen);
int ACCEPT(int sock, struct sockaddr *address, socklen_t *address_len);
int CONNECT(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen);
int SOCKET(int domain, int type, int protocol);



int ACCEPT(int sock, struct sockaddr *address, socklen_t *address_len) {
	return 0;
}

int CONNECT(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	return 0;
}

int SOCKET(int domain, int type, int protocol) {
	return socket(AF_INET, SOCK_DGRAM, 0);
}

int BIND(int socket, struct sockaddr *my_addr, socklen_t addrlen) {
        return bind(socket, (struct sockaddr *)&my_addr, addrlen);
}

ssize_t SEND(int socket, const void* buffer, size_t length, int flags) {
	//pass data to local socket in tcpd
	//define address of local socket to pass data to 
	struct sockaddr_in daemon_addr;
	daemon_addr.sin_family = AF_INET;
	daemon_addr.sin_port = LOCALPORT;
	daemon_addr.sin_addr.s_addr = inet_addr(LOCALADDRESS);

	//adds 1 byte header to buffer "1" indicates data is to be sent
	// const char *header = "1";
	// char *buffer_with_header = malloc(strlen(buffer)+strlen(buffer_with_header)+1);
	// strcat(buffer_with_header, header);
	// strcat(buffer_with_header, buffer);
	// //for debugging
	// printf("%s\n", buffer_with_header);
	// //free malloc
	// free(buffer_with_header);

	char header = '1';
	char buffer_with_header[length + 1];
	buffer_with_header[0] = header;
	strcat(buffer_with_header, buffer);
	printf("%s\n", buffer_with_header);

	//pass data to local socket in tcpd
	return sendto(socket, buffer_with_header, sizeof(buffer_with_header), flags, 
		(struct sockaddr *)&daemon_addr, sizeof(daemon_addr));

	// return sendto(socket, buffer, length, flags, 
	// 	(struct sockaddr *)&daemon_addr, sizeof(daemon_addr));
}

ssize_t RECV(int socket, void* buffer, size_t length, int flags) {
	//send packet to the local tcpd to let it know the server is waiting
	//send 0 to request to send data
	
	const char *header = "0";
	char max_length[MSS] = "";
	char packet[MSS] = "";
	snprintf(max_length, sizeof(max_length), "%zu", length);
	strcat(packet, header);
	strcat(packet, max_length);
	//for debugging
	printf("%s\n", packet);
	//buffer waitingMsg
	sendto(socket, packet, sizeof(packet), 0, 
		(struct sockaddr *)&daemon_addr, sizeof(daemon_addr));

	//socklen_t * size;
	//struct sockaddr_in name;

	//recieve from local socket in tcpd
	return recvfrom(socket, buffer, length, flags, NULL, NULL);
}

