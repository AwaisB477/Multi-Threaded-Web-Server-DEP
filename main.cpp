#include "Server.h"
#include<iostream>
int main() {
    try {
        Server server(8080, "C:/Users/Awais Raza/source/repos/Multi-Threaded Web Server/www");
        server.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Error starting server: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
