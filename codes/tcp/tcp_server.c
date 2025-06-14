#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void handle_client(int newsockfd) {
    char buffer[256];
    bzero(buffer, sizeof(buffer));

    /* Read message from client */
    if (read(newsockfd, buffer, sizeof(buffer)) < 0) 
        error("ERROR reading from socket");
    printf("[+] Client sent: %s\n", buffer);

    /* Send ACK to client */
    if (write(newsockfd, "Thanks, I got your message", 26) < 0) 
        error("ERROR writing to socket");

    close(newsockfd); // Close client socket
    exit(0);          // End child process
}

int main()
{
    int sockfd, newsockfd, portno;
    char portnumber[10];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    // Prevent zombie processes by auto-reaping child processes
    signal(SIGCHLD, SIG_IGN);

    printf("Server requires a port number to operate.\n");
    printf("Please provide a port number: ");
    fgets(portnumber, sizeof(portnumber), stdin);
    portno = atoi(portnumber);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind socket
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    // Listen for connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    printf("[+] Server started on port %d. Waiting for clients...\n", portno);

    // Accept connections forever
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        printf("[+] New client connected\n");

        // Create a child process to handle this client
        if (fork() == 0) {
            close(sockfd); // Child doesn't need the listening socket
            handle_client(newsockfd);
        } else {
            close(newsockfd); // Parent doesn't need this socket
        }
    }

    close(sockfd);
    return 0;
}
