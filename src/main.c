#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #define CLOSESOCKET closesocket
    #define SOCKET_TYPE SOCKET
    #define INVALID_SOCKET_VALUE INVALID_SOCKET
    #define SOCKET_ERROR_VALUE SOCKET_ERROR
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #define CLOSESOCKET close
    #define SOCKET_TYPE int
    #define INVALID_SOCKET_VALUE -1
    #define SOCKET_ERROR_VALUE -1
#endif

#include "controller.h"
#include "log_manager.h"

#define PORT 8080

#define LOG(level, message) log_message(level, message, __FILE__, __LINE__)
#define LOG_CLIENT(level, message, client) log_client_info(level, message, __FILE__, __LINE__, client)

int main() {
    init_log("server.log");
    LOG(LOG_INFO, "Server starting...");

#ifdef _WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        LOG(LOG_ERROR, "WSAStartup failed.");
        printf("WSAStartup failed: %d\n", iResult);
        close_log();
        return 1;
    }
#endif

    SOCKET_TYPE server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET_VALUE) {
        LOG(LOG_ERROR, "Socket creation failed.");
        perror("Socket failed");
#ifdef _WIN32
        WSACleanup();
#endif
        close_log();
        exit(EXIT_FAILURE);
    }
    LOG(LOG_INFO, "Socket created successfully.");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR_VALUE) {
        LOG(LOG_ERROR, "Bind failed.");
        perror("Bind failed");
        CLOSESOCKET(server_fd);
#ifdef _WIN32
        WSACleanup();
#endif
        close_log();
        exit(EXIT_FAILURE);
    }
    LOG(LOG_INFO, "Bind successful.");

    if (listen(server_fd, 3) == SOCKET_ERROR_VALUE) {
        LOG(LOG_ERROR, "Listen failed.");
        perror("Listen failed");
        CLOSESOCKET(server_fd);
#ifdef _WIN32
        WSACleanup();
#endif
        close_log();
        exit(EXIT_FAILURE);
    }
    LOG(LOG_INFO, "Listening on port 8080.");

    while (1) {
        struct sockaddr_in client_address;
        SOCKET_TYPE client_socket = accept(server_fd, (struct sockaddr *)&client_address, &addrlen);
        if (client_socket == INVALID_SOCKET_VALUE) {
            LOG(LOG_ERROR, "Accept failed.");
            perror("Accept failed");
            CLOSESOCKET(server_fd);
#ifdef _WIN32
            WSACleanup();
#endif
            close_log();
            exit(EXIT_FAILURE);
        }
        LOG_CLIENT(LOG_INFO, "Client connected.", &client_address);
        handle_client(client_socket);
        LOG_CLIENT(LOG_INFO, "Client connection closed.", &client_address);
    }

    CLOSESOCKET(server_fd);
#ifdef _WIN32
    WSACleanup();
#endif
    LOG(LOG_INFO, "Server stopped.");
    close_log();
    return 0;
}
