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
        return bind(socket, (struct sockaddr *)my_addr, addrlen);
}

ssize_t SEND(int socket, const void* buffer, size_t length, int flags) {
	//define address of local socket to pass data to 
	struct sockaddr_in daemon_addr;
	daemon_addr.sin_family = AF_INET;
	daemon_addr.sin_port = LOCALPORT;
	daemon_addr.sin_addr.s_addr = inet_addr(LOCALADDRESS);

	char header = '1';
	char buffer_with_header[length + 1];
	buffer_with_header[0] = header;
	strcat(buffer_with_header, buffer);
	printf("%s\n", buffer_with_header);

	//pass data to local socket in tcpd
	return sendto(socket, buffer_with_header, sizeof(buffer_with_header), flags, 
		(struct sockaddr *)&daemon_addr, sizeof(daemon_addr));
}

ssize_t RECV(int socket, void* buffer, size_t length, int flags) {
	struct sockaddr_in daemon_addr;
	daemon_addr.sin_family = AF_INET;
	daemon_addr.sin_port = LOCALPORT;
	daemon_addr.sin_addr.s_addr = inet_addr(LOCALADDRESS);

	//Build the message to let the local tcpd that the server is ready
	char header = '0';
	char packet[50];
	packet[0] = header;
	int* max_amt_pointer = (int *)(&packet[1]);
	*max_amt_pointer = length;
	int* port = (int*)(&packet[5]);
	*port = 6789;

	sendto(socket, packet, sizeof(packet), 0, 
	 	(struct sockaddr *)&daemon_addr, sizeof(daemon_addr));

	//recieve from local socket in tcpd
	int temp = -1;
	while(temp == -1) {
		temp = recvfrom(socket, buffer, length, flags, NULL, NULL);
	}
	printf("The capital recvfrom is returning %d\n", temp);
	return temp;
}

