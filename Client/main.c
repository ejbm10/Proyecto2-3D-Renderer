#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

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

unsigned long long stringToInteger(const char *str) {
    unsigned long long num = 0;
    size_t len = strlen(str);

    // Combine ASCII values into a single integer
    for (size_t i = 0; i < len; i++) {
        num = num * 256 + (unsigned char)str[i]; // Shift and add the ASCII value
    }

    return num;
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

    //if (init_client() < 0) {
        //return -1;
    //}

    //printf("Successfully connected\n");

    while (active) {
        scanf("%s", buffer);

        if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "shutdown") == 0) {
            active = 0;
        }

        printf("%lld\n", stringToInteger(buffer));

        printf("%c\n", integerToString(stringToInteger(buffer)));

        //send_message(buffer);
    }

    //close(client_fd);

    return 0;
}


