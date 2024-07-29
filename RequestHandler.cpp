#include "RequestHandler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

RequestHandler::RequestHandler(const std::string& baseDir) : baseDir(baseDir) {}

void RequestHandler::handle(SOCKET clientSocket) {
    char buffer[4096];
    std::string request;
    int bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        request.append(buffer, bytesRead);
        if (request.find("\r\n\r\n") != std::string::npos) {
            break;
        }
    }

    if (bytesRead == SOCKET_ERROR) {
        std::cerr << "Error reading request: " << WSAGetLastError() << std::endl;
        return;
    }

    std::string filePath = parseRequest(request);
    std::string fullPath = baseDir + filePath;

    std::ifstream file(fullPath);
    if (!file.is_open()) {
        std::string notFound = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>";
        send(clientSocket, notFound.c_str(), notFound.size(), 0);
        return;
    }

    std::stringstream fileContent;
    fileContent << file.rdbuf();
    std::string content = fileContent.str();

    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: " + getContentType(fullPath) + "\r\n\r\n" + content;
    send(clientSocket, response.c_str(), response.size(), 0);
}

std::string RequestHandler::parseRequest(const std::string& request) {
    std::istringstream requestStream(request);
    std::string method, path;
    requestStream >> method >> path;
    if (path == "/") {
        path = "/index.html";
    }
    return path;
}

bool ends_with(const std::string& value, const std::string& ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string RequestHandler::getContentType(const std::string& filePath) {
    if (ends_with(filePath, ".html")) {
        return "text/html";
    }
    else if (ends_with(filePath, ".css")) {
        return "text/css";
    }
    else if (ends_with(filePath, ".js")) {
        return "application/javascript";
    }
    return "text/plain";
}
