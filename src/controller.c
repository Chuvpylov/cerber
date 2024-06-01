#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "controller.h"
#include "model.h"
#include "view.h"

#ifdef _WIN32
    #define PATH_SEP "\\"
#else
    #define PATH_SEP "/"
#endif

void send_file(int client_socket, const char *filename, const char *content_type) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "..%spublic%s%s", PATH_SEP, PATH_SEP, filename);

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Could not open file");
        const char *response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Connection: close\r\n"
            "\r\n"
            "404 Not Found";
        send(client_socket, response, strlen(response), 0);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *file_content = malloc(file_size + 1);
    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0';
    fclose(file);

    char response_header[256];
    snprintf(response_header, sizeof(response_header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Connection: close\r\n"
             "\r\n",
             content_type, file_size);
    send(client_socket, response_header, strlen(response_header), 0);
    send(client_socket, file_content, file_size, 0);
    free(file_content);
}

void handle_cgi(int client_socket, const char *program) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "java -cp ../bin %s", program);

    FILE *fp = _popen(buffer, "r");
    if (fp == NULL) {
        perror("Failed to run Java program");
        const char *response =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/plain\r\n"
            "Connection: close\r\n"
            "\r\n"
            "500 Internal Server Error";
        send(client_socket, response, strlen(response), 0);
        return;
    }

    char output[4096];
    size_t output_size = fread(output, 1, sizeof(output) - 1, fp);
    output[output_size] = '\0';
    _pclose(fp);

    char response_header[256];
    snprintf(response_header, sizeof(response_header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/plain\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n", output_size);
    send(client_socket, response_header, strlen(response_header), 0);
    send(client_socket, output, output_size, 0);
}

void handle_raylib(int client_socket) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "cd ..%sbin && raylib_app", PATH_SEP);

#ifdef _WIN32
    strcat(buffer, ".exe");
#endif

    FILE *fp = _popen(buffer, "r");
    if (fp == NULL) {
        perror("Failed to run Raylib application");
        const char *response =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/plain\r\n"
            "Connection: close\r\n"
            "\r\n"
            "500 Internal Server Error";
        send(client_socket, response, strlen(response), 0);
        return;
    }

    char output[4096];
    size_t output_size = fread(output, 1, sizeof(output) - 1, fp);
    output[output_size] = '\0';
    _pclose(fp);

    char response_header[256];
    snprintf(response_header, sizeof(response_header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/plain\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n", output_size);
    send(client_socket, response_header, strlen(response_header), 0);
    send(client_socket, output, output_size, 0);
}

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    int valread = recv(client_socket, buffer, 1024, 0);
    if (valread > 0) {
        buffer[valread] = '\0';
        printf("Request:\n%s\n", buffer);

        if (strncmp(buffer, "GET / ", 6) == 0) {
            send_file(client_socket, "index.html", "text/html");
        } else if (strncmp(buffer, "GET /customers.html", 19) == 0) {
            send_file(client_socket, "customers.html", "text/html");
        } else if (strncmp(buffer, "GET /orders.html", 16) == 0) {
            send_file(client_socket, "orders.html", "text/html");
        } else if (strncmp(buffer, "GET /runjava ", 13) == 0) {
            handle_cgi(client_socket, "HelloWorld");
        } else if (strncmp(buffer, "GET /runraylib", 14) == 0) {
            handle_raylib(client_socket);
        } else {
            const char *response =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n"
                "\r\n"
                "404 Not Found";
            send(client_socket, response, strlen(response), 0);
        }
    } else {
        printf("No message received from client.\n");
    }
    closesocket(client_socket);
}
