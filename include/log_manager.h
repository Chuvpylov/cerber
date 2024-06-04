#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <arpa/inet.h> // Include this for inet_ntoa on Unix systems

#ifdef _WIN32
    #include <winsock2.h>
    #define CLOSESOCKET closesocket
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #define CLOSESOCKET close
#endif

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

void init_log(const char *filename);
void close_log();
void log_message(LogLevel level, const char *message, const char *file, int line);
void log_client_info(LogLevel level, const char *message, const char *file, int line, struct sockaddr_in *client);
void log_client_request(LogLevel level, int client_socket, const char *request, const char *file, int line);

#endif
