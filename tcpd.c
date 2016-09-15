#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MSS 1000
#define LOCAL_TCPD_PORT 9999
#define REMOTE_TCPD_PORT 8888
#define LOCAL_ADDRESS "127.0.0.1"


int main(int argc, char *argv[])
{

    /* Set up the sockets for local and remote communication */
	int local_sock;
	int remote_sock;

    struct sockaddr_in local_name;
    struct sockaddr_in remote_name;

    /*create local port*/
    local_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(local_sock < 0) {
	   perror("opening local datagram socket");
	   exit(1);
	}

    local_name.sin_family = AF_INET;
    local_name.sin_port = LOCAL_TCPD_PORT;
    local_name.sin_addr.s_addr = inet_addr(LOCAL_ADDRESS);

    if(bind(local_sock, (struct sockaddr *)&local_name, sizeof(local_name)) < 0) {
	perror("getting local socket name");
	exit(2);
    }

    printf("%s\n", "Local socket bound");

    /*create remote port*/
    remote_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(remote_sock < 0) {
	   perror("opening remote datagram socket");
	   exit(1);
	}
    remote_name.sin_family = AF_INET;
    remote_name.sin_port = REMOTE_TCPD_PORT;
    remote_name.sin_addr.s_addr = inet_addr(LOCAL_ADDRESS);

    if(bind(remote_sock, (struct sockaddr *)&remote_name, sizeof(remote_name)) < 0) {
	   perror("getting remote socket name");
	   exit(1);
    }

    printf("%s\n", "Remote socket bound.");


    /* If this is running on the client, set up the remote port*/
    struct sockaddr_in remote_server_tcpd;
    struct sockaddr_in local_server_name;
    if (strcmp(argv[1], "1") == 0) {
        remote_server_tcpd.sin_family = AF_INET;
        remote_server_tcpd.sin_port = REMOTE_TCPD_PORT;
        remote_server_tcpd.sin_addr.s_addr = inet_addr(argv[2]);
        printf("%s\n", "Set up remote tcpd information");
    } else if (strcmp(argv[1], "0") == 0) {
        local_server_name.sin_family = AF_INET;
        local_server_name.sin_port = -1;
        local_server_name.sin_addr.s_addr = inet_addr(LOCAL_ADDRESS);
    } else {
        printf("%s\n", "Invalid argument. Not a 0 or a 1.");
        exit(1);
    }

    printf("%s\n", "After the type check");

    //set for holding remote and local sockets to listen to
    fd_set readfds;
    //buffers to hold data sent to or received from the local and remote sockets
    char local_buf[MSS] = {0}, remote_buf[MSS] = {0};
    //to keep track of the maximum amount of data the server can accept
    int ftpsMaxData = -1;
    int serverPort = -1;
    //to keep track of the amount of data received
    int amtRecvd = 0;

    while(1) {

        // clear the set 
        FD_ZERO(&readfds);

        // add our descriptors to the set
        FD_SET(local_sock, &readfds);
        FD_SET(remote_sock, &readfds);

        // Block until input arrives on one or more sockets
        if(select(FD_SETSIZE, &readfds, NULL, NULL, NULL) < 0) {
            fprintf(stderr, "%s\n", "There was an issue with select() in tcpd");
            exit(1);
        }
        //Grab data from any socket that is ready
        if (FD_ISSET(local_sock, &readfds)) {
            //receive data from the local socket
            amtRecvd = recvfrom(local_sock, local_buf, sizeof(local_buf), 0, NULL, NULL);
            
            printf("%s: %d %s\n", "received data from the local socket", amtRecvd, local_buf);

            if (local_buf[0] == '1') {
                //forward the data to the remote tcpd through troll
                printf("%s\n", "Just received a message from the client");

                //remove the header
                char* local_buf_no_header = local_buf + 1;
                printf("%s\n", local_buf_no_header);

                sendto(remote_sock, local_buf_no_header, amtRecvd - 1, 0, 
                    (struct sockaddr *)&remote_server_tcpd, sizeof(remote_server_tcpd));

            } else if (local_buf[0] == '0') { 
                printf("%s\n", "Just received a message from the server");

                int* max_length = (int*)(&local_buf[1]); 
                ftpsMaxData = *max_length;

                int* port_value = (int*)(&local_buf[5]);
                serverPort = *port_value;
                local_server_name.sin_port = serverPort;

                printf("MAX DATA: %d, PORT: %d\n", ftpsMaxData, serverPort);

             } else {
                fprintf(stderr, "%s\n", "Message from unknown source");
                 exit(1);
             } 
        } 
        if (FD_ISSET(remote_sock, &readfds)) {
            //receive data from the remote socket
            amtRecvd = recvfrom(remote_sock, remote_buf, sizeof(remote_buf), 0, NULL, NULL);

            printf("%s%d\n", "Just received data on the remote socket: ", amtRecvd);
            printf("%s\n", remote_buf);

            //forward data up to ftps
          //  if (NULL == local_server_name) {
          //      fprintf(stderr, "%s\n", "The server is not ready for packets");
           //     exit(1);
           // } else {
                if (sendto(remote_sock, remote_buf, amtRecvd, 0, (struct sockaddr *)&local_server_name, sizeof(local_server_name)) < 0) {
                    fprintf(stderr, "%s\n", "Error sending to server from local tcpd");
                    exit(1);
                }
            //}
        }
    } // end while
}

