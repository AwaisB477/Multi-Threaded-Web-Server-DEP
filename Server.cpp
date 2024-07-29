#include "Server.h"
#include "RequestHandler.h"
#include <iostream>
#include <thread>
#include <vector>

Server::Server(int port, const std::string& directory) : port(port), directory(directory) {
    initializeWinsock();
}

void Server::initializeWinsock() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(iResult));
    }
}

void Server::start() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        WSACleanup();
        throw std::runtime_error("Failed to create socket: " + std::to_string(WSAGetLastError()));
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(serverSocket);
        WSACleanup();
        throw std::runtime_error("Failed to bind to port: " + std::to_string(WSAGetLastError()));
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(serverSocket);
        WSACleanup();
        throw std::runtime_error("Failed to listen on socket: " + std::to_string(WSAGetLastError()));
    }

    std::cout << "Server is listening on port " << port << std::endl;

    while (true) {
        sockaddr_in clientAddr{};
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::thread(&Server::handleClient, this, clientSocket).detach();
    }

    closesocket(serverSocket);
    WSACleanup();
}

void Server::handleClient(SOCKET clientSocket) {
    RequestHandler requestHandler(directory);
    requestHandler.handle(clientSocket);
    closesocket(clientSocket);
}
