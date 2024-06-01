#include <winsock2.h>
#include "view.h"

void send_response(int client_socket, const char *message) {
    send(client_socket, message, strlen(message), 0);
}
