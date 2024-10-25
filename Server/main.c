#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

char buffer[1024];

int server_fd, client_fd;

struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);

int init_server() {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("OPT failed");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("Binding failed");
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return -1;
    }
    return 0;
}

void send_message(char const *message) {
    send(server_fd, message, strlen(message), 0);
}

void receive_message(const int current_socket) {
    read(current_socket, buffer, 1023);
}

int main(int argc, char const* argv[])
{
    int active = 1;

    if (init_server() < 0) {
        return -1;
    }

    printf("Waiting for connection...\n");

    while (active) {
        if (client_fd > 0) {
            receive_message(client_fd);

            if (strcmp(buffer, "exit") == 0) {
                printf("Client disconnected\n");
                close(client_fd);
                client_fd = -1;
            }

            else if (strcmp(buffer, "shutdown") == 0) {
                printf("Shutting down...\n");
                close(client_fd);
                active = 0;
            }

            else {
                printf("Client: %s\n", buffer);
            }

            memset(buffer, 0, sizeof(buffer));
        } else if ((client_fd = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen)) < 0) {
            perror("Accept failed");
            return -1;
        } else {
            printf("Client connected\n");
        }
    }

    close(server_fd);

    return 0;
}

