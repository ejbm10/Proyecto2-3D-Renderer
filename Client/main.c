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

char input_buffer[1024];
char msg_buffer[1024];
char block_buffer[1024];

int init_client() {
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "192.168.0.19", &serv_addr.sin_addr) <= 0) {
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

        printf("Enter message: ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            perror("Error reading input");
            active = 0;
        }
        input_buffer[strcspn(input_buffer, "\n")] = '\0';

        if (strcmp(input_buffer, "exit") == 0 || strcmp(input_buffer, "shutdown") == 0) active = 0;

        int blocks =(int) messageToASCII(input_buffer, -1, key);

        for (int i = 0; i < blocks; i++) {
            snprintf(block_buffer, sizeof(block_buffer), "%llu|", rsa_encrypt(messageToASCII(input_buffer, i, key), key));
    
            strcat(msg_buffer, block_buffer);
        }

        if (send(client_fd, msg_buffer, strlen(msg_buffer), 0) < 0) {
            perror("Error sending message");
            active = 0;
        }

        memset(msg_buffer, 0, sizeof(msg_buffer));
    }
    free(key);
    close(client_fd);
    return 0;
}


