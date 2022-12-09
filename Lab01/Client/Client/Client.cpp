#include <winsock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

int main(int argc, char* argv[]) {
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	std::cout << "Connected!\n";
	std::cout << "Enter your message: ";
	char msg[256]{};
	std::cin.getline(msg, 256);

	send(Connection, msg, sizeof(msg), NULL);

	recv(Connection, msg, sizeof(msg), NULL);

	std::string encoded_msg = std::string(msg);
	std::cout << "Key: ";
	std::cout << encoded_msg.back() << std::endl;
	std::cout << "Encoded word: " + encoded_msg.substr(0, encoded_msg.length() - 1) << std::endl;

	system("pause");
	return 0;
}