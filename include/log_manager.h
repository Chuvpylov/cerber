#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#include <pthread.h>

// Log levels
typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// Function declarations
void log_message(LogLevel level, const char *message, const char *file, int line);
void init_log(const char *filename);
void close_log();
void log_client_info(LogLevel level, const char *message, const char *file, int line, struct sockaddr_in *client);
void log_client_request(LogLevel level, int client_socket, const char *request, const char *file, int line);

#endif // LOG_MANAGER_H
