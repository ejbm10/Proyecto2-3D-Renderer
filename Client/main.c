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

int main(int argc, char const* argv[])
{
    int active = 1;
    if (init_client() < 0) return -1;
    char key_buffer[256];
    if (recv(client_fd, key_buffer, sizeof(key_buffer), 0) <= 0) {
        perror("Error receiving public key");
        close(client_fd);
        return -1;
    }
    // Parse the received public key
    struct RSAKey *key = malloc(sizeof(struct RSAKey));
    sscanf(key_buffer, "%lld,%lld", &key->modulus, &key->exponent);

    printf("Successfully connected!\nPublic key: (%lld, %lld)\n\n", key->modulus, key->exponent);

    while (active) {
        scanf("%s", buffer);
        if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "shut") == 0) active = 0;
        printf("ASCII: %llu\n", messageToASCII(buffer));
        snprintf(buffer, sizeof(buffer), "%llu", rsa_encrypt(messageToASCII(buffer), key));
        printf("%s\n", buffer);
        if (send(client_fd, buffer, strlen(buffer), 0) < 0) {
            perror("Error sending message");
            active = 0;
        }
    }
    close(client_fd);
    return 0;
}


