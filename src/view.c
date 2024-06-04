#ifdef _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #define CLOSESOCKET closesocket
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #define CLOSESOCKET close
#endif

#include "view.h"
#include <string.h>

void send_response(int client_socket, const char *message) {
    send(client_socket, message, strlen(message), 0);
}
