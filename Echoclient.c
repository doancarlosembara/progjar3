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
    struct sockaddr_in server_addr;
    int sockfd;
    char msg[100], server_msg[100];

    // Step 1: Configure the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4002);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.38");

    // Step 2: Create the socket
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }
    printf("Socket created successfully\n");

    // Step 3: Connect to the server
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sockfd);
        return 1;
    }
    printf("Connected to the server at 192.168.1.38\n");

    // Step 4: Communicate with the server
    while (1) {
        // Input message from user
        printf("Write message: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = '\0'; // Remove trailing newline

        // Send the message to the server
        if (send(sockfd, msg, strlen(msg), 0) < 0) {
            perror("Send failed");
            break;
        }

        // Clear and receive server response
        memset(server_msg, 0, sizeof(server_msg));
        if (recv(sockfd, server_msg, sizeof(server_msg), 0) < 0) {
            perror("Receive failed");
            break;
        }
        printf("Server reply: %s\n", server_msg);

        // Exit if the server or client sends "bye"
        if (strcmp(server_msg, "bye") == 0 || strcmp(msg, "bye") == 0) {
            printf("Exiting...\n");
            break;
        }
    }

    // Step 5: Close the socket
    close(sockfd);
    return 0;
}
