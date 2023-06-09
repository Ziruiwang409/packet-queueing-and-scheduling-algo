#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <time.h>

#define PORT 50000

/* package Interval */
#define S1 200
#define S2 30
#define S3 300

/* package size */
#define S1Len 100
#define S2Len 50
#define S3Len 100

/* package number */
#define S1Num 100
#define S2Num 1000
#define S3Num 500

static const char alphanum[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

int stringLength = sizeof(alphanum) - 1;

void broadcast(int serverNumber, int milliseconds, int messageLen)
{
    int server_fd;                          /* Socket File Descriptor */
    int opt = 1;                            /* Socket opt to set permission to broadcast */
    struct sockaddr_in broadcastAddress;    /* Broadcast address sockaddr_in - references elements 
                                            of the socket address. "in" for internet */
    char *broadcastIP = "127.255.255.255";  /* FF:FF:FF:FF:FF:FF

    /* Create socket for sending datagrams */
    if ((server_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) /* creates socket, SOCK_STREAM is for TCP. SOCK_DGRAM for UDP */
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    /* Set socket to allow broadcast */
    if (setsockopt(server_fd, SOL_SOCKET, SO_BROADCAST, /* SOL_SOCKET is the socket layer itself */
                        (void *) &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    memset(&broadcastAddress, 0, sizeof(broadcastAddress));
    broadcastAddress.sin_family = AF_INET;                      /* Internet address family. For IPv6, it's AF_INET6. 29 others exist like AF_UNIX etc.  */
    broadcastAddress.sin_addr.s_addr = inet_addr(broadcastIP);  /* Broadcast IP address */
    broadcastAddress.sin_port = htons(PORT);                    /* Broadcast port. Htons converts to Big Endian - Left to Right. RTL is Little Endian */

    srand(time(0));
    char *message = new char[messageLen];
    int maxCount;

    if (serverNumber == 1){
        message[0] = '1';
        maxCount = S1Num;
    }
    else if  (serverNumber == 2){
        message[0] = '2';
        maxCount = S2Num;
    }
    else if (serverNumber == 3){
        message[0] = '3';
        maxCount = S3Num;
    }

    /* Send periodically, endlessly */
    for(int count = 0; count < maxCount; count++)
    {
        for(int i = 1; i < messageLen; i++)
            message[i] = alphanum[rand() % stringLength]

        /* Broadcast hello message every 5 seconds*/
        if (sendto(server_fd, message, messageLen, 0, (struct sockaddr *) 
           &broadcastAddress, sizeof(broadcastAddress)) != messageLen)
        {
            perror("Sendto Failure");
            exit(EXIT_FAILURE);
        }

        usleep(milliseconds * 1000);
    }
}

int main(int argc, char *argv[])
{
    int pid = fork();
    if(pid == 0)
    {
        broadcast(1, S1, S1Len);
    }
    else
    {
        pid = fork();
        if(pid == 0)
        {
            broadcast(2, S2, S2Len);
        }
        else
        {
            broadcast(3, S3, S3Len);
        }
    }
}