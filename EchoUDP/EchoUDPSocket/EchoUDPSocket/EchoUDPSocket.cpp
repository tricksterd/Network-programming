#include <iostream>
#include <winsock2.h>

#pragma warning(disable: 4996)
#pragma comment(lib, "ws2_32.lib")

int main() {
    // Initialize Winsock
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 1;
    }

    // Create a UDP socket
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set up the server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
    server_addr.sin_port = htons(12345);

    // Bind the socket to a port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }

    // Set up the client address
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    while (true) {
        // Receive a message from the client
        char buffer[256];
        int bytes_received = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (bytes_received == SOCKET_ERROR) {
            std::cerr << "Failed to receive message" << std::endl;
            return 1;
        }
        
        
        std::string received_message_without_empty_bytes = std::string(buffer).substr(0, bytes_received);

        // Print the message
        std::cout << "Received message: " << received_message_without_empty_bytes << std::endl;

        // Echo the message back to the client
        if (sendto(sockfd, received_message_without_empty_bytes.c_str(), received_message_without_empty_bytes.length(), 0, (struct sockaddr*)&client_addr, client_addr_len) == SOCKET_ERROR) {
            std::cerr << "Failed to send message" << std::endl;
            return 1;
        }
    }

    // Close the socket
    closesocket(sockfd);

    // Clean up Winsock
    WSACleanup();

    return 0;
}