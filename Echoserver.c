#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

// Doan Carlos Embara
// 21120121140032

int main() { 
    struct sockaddr_in server_addr, client_addr; 
    int sockfd, client_sockfd; 
    socklen_t client_addr_size = sizeof(client_addr);
    char msg[100];

    // Step 1: Create the socket
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("Socket creation failed"); 
        return 1; 
    } 
    printf("Socket created successfully\n");

    // Step 2: Configure the server address
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(4002); 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

    // Step 3: Bind the socket
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) { 
        perror("Socket bind failed"); 
        close(sockfd);
        return 1; 
    } 
    printf("Socket successfully bound to port 4002\n");

    // Step 4: Listen for incoming connections
    if (listen(sockfd, 5) < 0) { 
        perror("Socket listen failed"); 
        close(sockfd);
        return 1; 
    } 
    printf("Listening for connections (max queue: 5)\n");

    // Step 5: Accept incoming connections
    while (1) { 
        if ((client_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_addr_size)) < 0) { 
            perror("Failed to accept connection"); 
            continue; 
        } 
        printf("Accepted connection from %s\n", inet_ntoa(client_addr.sin_addr));

        // Step 6: Receive and send messages
        while (1) { 
            memset(msg, 0, sizeof(msg)); // Clear the message buffer
            int bytes_received = recv(client_sockfd, msg, sizeof(msg), 0); 
            
            if (bytes_received < 0) {
                perror("Failed to receive message"); 
                break; 
            } else if (bytes_received == 0) {
                printf("Client disconnected\n");
                break;
            }

            printf("Message received: %s\n", msg); 

            send(client_sockfd, msg, bytes_received, 0); 

            if (strcmp(msg, "bye") == 0) { 
                printf("Client said 'bye', closing connection...\n"); 
                break; 
            } 
        }

        close(client_sockfd); // Close client connection
    } 

    // Step 7: Close the server socket
    close(sockfd); 
    return 0; 
}
