#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <string>
#include <winsock2.h>

class RequestHandler {
public:
    explicit RequestHandler(const std::string& baseDir);
    void handle(SOCKET clientSocket);
private:
    std::string baseDir;
    std::string parseRequest(const std::string& request);
    std::string getContentType(const std::string& filePath);
};

#endif // REQUESTHANDLER_H
