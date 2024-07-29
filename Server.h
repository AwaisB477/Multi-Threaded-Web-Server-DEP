#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class Server {
public:
    Server(int port, const std::string& directory);
    void start();
private:
    int port;
    std::string directory;
    void handleClient(SOCKET clientSocket);
    void initializeWinsock();
};

#endif // SERVER_H
