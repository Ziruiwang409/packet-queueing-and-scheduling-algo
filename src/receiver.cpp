#include <cstdio>
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>  
#include <string.h>   
#include <unistd.h>   
#include <time.h>
#include <cstring>

using namespace std;

#define PORT 50001
#define MAXrecv_msg 100

int main(int argc, char *argv[])
{
    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast Address */

    /* Create a best-effort datagram socket using UDP */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    /* Construct bind structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
    broadcastAddr.sin_port = htons(PORT);               /* Broadcast port */

    /* Bind to the broadcast port */
    if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    char recv_msg[MAXrecv_msg + 1];
    int recv_bytes;

    int count = 0;

    while(1)
    {
        if ((recv_bytes = recvfrom(sock, recv_msg, MAXrecv_msg, 0, NULL, 0)) < 0)
        {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }
        
        if (recv_msg[0] == '1')
            printf("S1\n");
        else if (recv_msg[0] == '2')
            printf("S2\n");
        else if (recv_msg[0] == '3')
            printf("S3\n");
        else 
            continue;
    }
    
    close(sock);
    exit(0);
}