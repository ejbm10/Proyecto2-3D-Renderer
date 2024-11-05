#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "RSA/rsa.h"

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

int main(int argc, char const* argv[])
{
    srand(time(NULL));
    int active = 1;
    if (init_server() < 0) {
        return -1;
    }
    struct RSAKeyPair *keys = generate_keys();
    printf("Public key: (%lld, %lld)\nPrivate key: (%lld, %lld)\n\n", keys->public_key->modulus, keys->public_key->exponent, keys->private_key->modulus, keys->private_key->exponent);

    while (active) {
        if (client_fd > 0) {
            if (recv(client_fd, buffer, 1023, 0) < 0) {
                perror("Received failed");
                close(client_fd);
                return -1;
            }

            printf("Incoming: %s\n", buffer);

            char final_msg[1024] = { 0 };
            char* token = strtok(buffer, "|");

            while (token != NULL) {
                strcat(final_msg, ASCIIToMessage(rsa_decrypt(strtoull(token, NULL, 10), keys->private_key)));
                token = strtok(NULL, "|");
            }

            if (strcmp(final_msg, "exit") == 0) {
                printf("Client disconnected!\n\n");
                close(client_fd);
                client_fd = -1;
            }

            else if (strcmp(final_msg, "shutdown") == 0) {
                printf("Shutting down...\n");
                close(client_fd);
                active = 0;
            }

            else {
                printf("Client: %s\n", final_msg);
            }

            memset(buffer, 0, sizeof(buffer));

        } else if ((client_fd = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen)) < 0) {
            perror("Accept failed");
            return -1;
        } else {
            printf("Client connected!\n\n");
            // Send the public key to the client after connection
            char key_buffer[256];
            snprintf(key_buffer, sizeof(key_buffer), "%lld,%lld", keys->public_key->modulus, keys->public_key->exponent);
            if (send(client_fd, key_buffer, strlen(key_buffer), 0) == -1) {
                perror("Error sending public key");
                close(client_fd);
                client_fd = -1;
            }
        }
    }
    close(server_fd);
    return 0;
}

