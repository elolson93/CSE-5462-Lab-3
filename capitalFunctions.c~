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
#define LOCALPORT 9999

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

int CONNECT(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen) {
	return 0;
}

int SOCKET(int domain, int type, int protocol) {
	return socket(AF_INET, SOCK_DGRAM, 0);
}

int BIND(int socket, struct sockaddr *my_addr, socklen_t addrlen) {
        return bind(socket, (struct sockaddr *)&my_addr, addrlen);
}

ssize_t SEND(int socket, const void* buffer, size_t length, int flags) {
	//define address of local socket to pass data to 
	struct sockaddr_in name;
	name.sin_family = AF_INET;
        name.sin_port = LOCALPORT;
        name.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//pass data to local socket in tcpd
	return sendto(socket, buffer, length, 0, (struct sockaddr *)&name, sizeof(name));
}

ssize_t RECV(int socket, void* buffer, size_t length, int flags) {

	socklen_t * size;
	struct sockaddr_in name;

	//recieve from local socket in tcpd
	return recvfrom(socket, buffer, length, 0, (struct sockaddr*)&name, size);
}

