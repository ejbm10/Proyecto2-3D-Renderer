#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "RSA/rsa.h"

#define PORT 8080

int status, client_fd;
struct sockaddr_in serv_addr;

char buffer[1024];

int init_client() {
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    return 0;
}

void send_message(char const *message) {
    send(client_fd, message, strlen(message), 0);
}

void receive_message() {
    read(client_fd, buffer, 1023);
}

int main(int argc, char const* argv[])
{
    int active = 1;

    if (init_client() < 0) {
        return -1;
    }

    printf("Successfully connected\n");

    char key_buffer[256];
    if (recv(client_fd, key_buffer, sizeof(key_buffer), 0) <= 0) {
        perror("Error receiving public key");
        close(client_fd);
        return -1;
    }

    // Parse the received public key
    struct RSAKey *key = malloc(sizeof(struct RSAKey));

    sscanf(key_buffer, "%lld,%lld", &key->modulus, &key->exponent);
    printf("Public Key: (%lld, %lld)\n", key->modulus, key->exponent);

    while (active) {

        scanf("%s", buffer);

        if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "shutdown") == 0) {
            active = 0;
        }

        printf("ASCII: %lld\n", messageToASCII(buffer));
        printf("Encriptado: %lld\n", rsa_encrypt(messageToASCII(buffer), key));
        snprintf(buffer, sizeof(buffer), "%lld", rsa_encrypt(messageToASCII(buffer), key));
        send_message(buffer);
    }

    close(client_fd);

    return 0;
}


