#include "log_manager.h"
#include <string.h> // Add this include

static FILE *log_file = NULL;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

void init_log(const char *filename) {
    pthread_mutex_lock(&log_mutex);
    log_file = fopen(filename, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Failed to open log file: %s\n", filename);
    }
    pthread_mutex_unlock(&log_mutex);
}

void close_log() {
    pthread_mutex_lock(&log_mutex);
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
    pthread_mutex_unlock(&log_mutex);
}

void log_message(LogLevel level, const char *message, const char *file, int line) {
    pthread_mutex_lock(&log_mutex);
    if (log_file == NULL) {
        fprintf(stderr, "Log file is not open.\n");
        pthread_mutex_unlock(&log_mutex);
        return;
    }

    time_t now;
    time(&now);
    char timestamp[26];
#ifdef _WIN32
    ctime_s(timestamp, sizeof(timestamp), &now);
#else
    strncpy(timestamp, ctime(&now), sizeof(timestamp));
#endif
    timestamp[strlen(timestamp) - 1] = '\0'; // Remove newline

    const char *level_str;
    switch (level) {
        case LOG_INFO:
            level_str = "INFO";
            break;
        case LOG_WARNING:
            level_str = "WARNING";
            break;
        case LOG_ERROR:
            level_str = "ERROR";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }

    fprintf(log_file, "[%s] [%s] [%s:%d]: %s\n", timestamp, level_str, file, line, message);
    fflush(log_file);
    pthread_mutex_unlock(&log_mutex);
}

void log_client_info(LogLevel level, const char *message, const char *file, int line, struct sockaddr_in *client) {
    pthread_mutex_lock(&log_mutex);
    if (log_file == NULL) {
        fprintf(stderr, "Log file is not open.\n");
        pthread_mutex_unlock(&log_mutex);
        return;
    }

    char client_info[100];
    snprintf(client_info, sizeof(client_info), "Client IP: %s, Port: %d",
             inet_ntoa(client->sin_addr), ntohs(client->sin_port));

    time_t now;
    time(&now);
    char timestamp[26];
#ifdef _WIN32
    ctime_s(timestamp, sizeof(timestamp), &now);
#else
    strncpy(timestamp, ctime(&now), sizeof(timestamp));
#endif
    timestamp[strlen(timestamp) - 1] = '\0'; // Remove newline

    const char *level_str;
    switch (level) {
        case LOG_INFO:
            level_str = "INFO";
            break;
        case LOG_WARNING:
            level_str = "WARNING";
            break;
        case LOG_ERROR:
            level_str = "ERROR";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }

    fprintf(log_file, "[%s] [%s] [%s:%d]: %s - %s\n", timestamp, level_str, file, line, message, client_info);
    fflush(log_file);
    pthread_mutex_unlock(&log_mutex);
}

void log_client_request(LogLevel level, int client_socket, const char *request, const char *file, int line) {
    pthread_mutex_lock(&log_mutex);
    if (log_file == NULL) {
        fprintf(stderr, "Log file is not open.\n");
        pthread_mutex_unlock(&log_mutex);
        return;
    }

    struct sockaddr_in client;
    int len = sizeof(client);
    getpeername(client_socket, (struct sockaddr*)&client, &len);

    time_t now;
    time(&now);
    char timestamp[26];
#ifdef _WIN32
    ctime_s(timestamp, sizeof(timestamp), &now);
#else
    strncpy(timestamp, ctime(&now), sizeof(timestamp));
#endif
    timestamp[strlen(timestamp) - 1] = '\0'; // Remove newline

    const char *level_str;
    switch (level) {
        case LOG_INFO:
            level_str = "INFO";
            break;
        case LOG_WARNING:
            level_str = "WARNING";
            break;
        case LOG_ERROR:
            level_str = "ERROR";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }

    fprintf(log_file, "[%s] [%s] [%s:%d]: Client IP: %s, Port: %d, Request:\n%s\n", timestamp, level_str, file, line,
            inet_ntoa(client.sin_addr), ntohs(client.sin_port), request);
    fflush(log_file);
    pthread_mutex_unlock(&log_mutex);
}
