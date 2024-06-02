#ifndef CONTROLLER_H
#define CONTROLLER_H

void send_file(int client_socket, const char *filename, const char *content_type);
void handle_cgi(int client_socket, const char *program);
void handle_raylib(int client_socket);
void handle_client(int client_socket);

#endif // CONTROLLER_H
