/*
/ Eric Olson and James Baker
/ CSE 5462 Network Programming
/ Lab 3 - September 15, 2016
/ 
/ This file contains our implementations of TCP functions using UDP.
*/


/* Prototypes for each function */ 
ssize_t SEND(int socket, const void* buffer, size_t length, int flags);
ssize_t RECV(int socket, const void* buffer, size_t length, int flags);
int BIND(int socket, const struct sockaddr *address,
       socklen_t address_len);
int ACCEPT(int socket, struct sockaddr *restrict address,
       socklen_t *restrict address_len);
int CONNECT(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen);
int SOCKET(int domain, int type, int protocol);




int ACCEPT(int socket, struct sockaddr *restrict address,
       socklen_t *restrict address_len) {
	return 0;
}

int CONNECT(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen) {
	return 0;
}