#include <iostream>
#include <winsock2.h>
#include <string>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set up the server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
    server_addr.sin_port = htons(12345);

    // Set up the client address
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
    client_addr.sin_port = htons(12346);

    // Bind the socket to a port
    if (bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }

    while (true) {
        // Read a message from the console
        std::string message;
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        // Send the message to the server
        if (sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Failed to send message" << std::endl;
            return 1;
        }

        // Receive the echoed message from the server
        char buffer[256];
        socklen_t server_addr_len = sizeof(server_addr);
        int bytes_received = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr*)&server_addr, &server_addr_len);
        if (bytes_received < 0) {
            std::cerr << "Failed to receive message" << std::endl;
            return 1;
        }

        // Print the echoed message
        std::cout << "Received echoed message: " << std::string(buffer).substr(0, bytes_received) << std::endl;
    }

    // Close the socket
    closesocket(sockfd);

    iResult = WSACleanup();
    if (iResult != 0) {
        std::cerr << "WSACleanup failed: " << iResult << std::endl;
        return 1;
    }

    return 0;
}