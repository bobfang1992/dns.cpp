#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    // Step 1: Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    // Step 2: Define server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7); // Echo service port

    if (inet_pton(AF_INET, "193.204.114.233", &server_addr.sin_addr) <= 0) { // IP address of sntp1.inrim.it
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sockfd);
        return -1;
    }

    // Step 3: Send data
    const char *message = "Hello, UDP Echo Server!";
    int message_len = strlen(message);

    if (sendto(sockfd, message, message_len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to send message" << std::endl;
        close(sockfd);
        return -1;
    }

    // Step 4: Receive response
    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&server_addr, &addr_len);
    if (n < 0) {
        std::cerr << "Failed to receive response" << std::endl;
    } else {
        buffer[n] = '\0';
        std::cout << "Server response: " << buffer << std::endl;
    }

    // Step 5: Close socket
    close(sockfd);
    return 0;
}
