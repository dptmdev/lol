/* UDP Server: multi-client capable by default (loop-based) */
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5000
#define MAXLINE 1000

int main()
{   
    char buffer[100];
    char *message = "Hello Client";
    int listenfd, len, n;
    struct sockaddr_in servaddr, cliaddr;

    bzero(&servaddr, sizeof(servaddr));

    // Create a UDP socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listenfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    // Bind address and port
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    // Infinite loop to handle multiple client messages
    while (1) {
        len = sizeof(cliaddr);
        bzero(buffer, sizeof(buffer));

        // Receive datagram from client
        n = recvfrom(listenfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr*)&cliaddr, &len);
        buffer[n] = '\0';
        printf("Received from client [%s:%d]: %s\n",
               inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buffer);

        // Send response to the same client
        sendto(listenfd, message, MAXLINE, 0,
               (struct sockaddr*)&cliaddr, len);
    }

    return 0;
}
