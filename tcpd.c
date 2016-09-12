#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MSS 1000
#define local_port = 9999

main(void)
{
	int local_sock;
	int remote_sock;

	//set using remote function?
	long remote_ip;
	short remote_port;


        struct local_sockaddr_in name;
        struct remote_sockaddr_in name;

        /*create local port*/
        local_sock = socket(AF_INET, SOCK_DGRAM, 0);
        if(local_sock < 0) {
		perror("opening local datagram socket");
		exit(1);
    	}

	/* create name with parameters and bind name to socket */
        name.sin_family = AF_INET;
	//set port to designated local host
        name.sin_port = local_port;
	//set ip to localhost
        name.sin_addr.s_addr = inet_addr("127.0.0.1");

        if(bind(local_sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
		perror("getting local socket name");
		exit(2);
    	}

        /*create remote port*/
        remote_sock = socket(AF_INET, SOCK_DGRAM, 0);
        if(remotesock < 0) {
		perror("opening remote datagram socket");
		exit(1);
    	}

	/* create name with parameters and bind name to socket */
        name.sin_family = AF_INET;
	//set port to designated local host
        name.sin_port = remote_port;
	//set ip to localhost
        name.sin_addr.s_addr = remote_ip;

        if(bind(local_sock, (struct sockaddr *)&name, sizeof(name)) < 0) {
		perror("getting remote socket name");
		exit(2);
    	}

}

