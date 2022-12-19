#include <iostream>
#include <winsock2.h>
#include <process.h>

#pragma warning(disable: 4996)
#pragma comment(lib, "ws2_32.lib")

std::string CaesarEncode(std::string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == 'Z') str[i] = 'A';
        else str[i]++;
    }
    str += "1";
    return str;
}

void daemon(void* arg) {
    SOCKET newConnection = *(SOCKET*)arg;

    char msg[256];
    recv(newConnection, msg, sizeof(msg), NULL);
    std::cout << "Message was encoded!\n";
    send(newConnection, CaesarEncode(std::string(msg)).c_str(), sizeof(msg), NULL);
    closesocket(newConnection);
}

int main(int argc, char* argv[]) {
    // WSASStartup
    WSAData wsadata;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsadata) != 0) {
        std::cout << "Error" << std::endl;
        exit(1);
    }
    std::cout << "Server started!" << std::endl;

    SOCKADDR_IN addr;
    int sizeOfAddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    // Accept 10 clients
    for (int i = 0; i < 10; i++) {
        SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeOfAddr);
        if (newConnection == 0) {
            std::cout << "Error #2\n";
        }
        else {
            std::cout << "Client Connected!\n";
            _beginthread(daemon, 0, &newConnection);
        }
    }

    system("pause");
    return 0;
}
