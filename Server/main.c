#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "RSA/rsa.h"
#include <mpi/mpi.h>
#include <time.h>
#include "Renderer/Renderer_alg.h"
#include "GL/glut.h"
#include "Validation/syntax.h"

#include "biblioteca.h"

#define PORT 8080

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
    MPI_Init(&argc, &argv);

    srand(time(NULL));

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    struct RSAKeyPair *keys = generate_keys();

    if (rank == 0) {

        char buffer[1024];

        int active = 1;
        if (init_server() < 0) {
            return -1;
        }

        printf("Public key: (%lld, %lld)\nPrivate key: (%lld, %lld)\n\n", keys->public_key->modulus, keys->public_key->exponent, keys->private_key->modulus, keys->private_key->exponent);

        printf("Waiting for connection...\n");

        while (active) {
            if (client_fd > 0) {
                if (recv(client_fd, buffer, 1023, 0) < 0) {
                    perror("Received failed");
                    close(client_fd);
                    return -1;
                }

                char final_msg[1024] = { 0 };
                int count;
                char **substrings = split_string(buffer, '|', &count);
                for (int i = 0; i < count; i++) {
                    strcat(final_msg, ASCIIToMessage(rsa_decrypt(strtoull(substrings[i], NULL, 10), keys->private_key)));
                }
                free_split_string(substrings, count);

                if (strcmp(final_msg, "exit") == 0) {
                    printf("Client disconnected\n\n");
                    close(client_fd);
                    client_fd = -1;
                }

                else if (strcmp(final_msg, "shutdown") == 0) {
                    printf("Server shutting down...\n");
                    close(client_fd);
                    active = 0;
                    if (size > 1) MPI_Bcast(buffer, sizeof(buffer), MPI_CHAR, 0, MPI_COMM_WORLD);
                }

                else if (validate_instruction(final_msg)) {
                    system("rm -rf ../Resources/*");

                    printf("Node %d sent: %s\n", rank, buffer);

                    if (size > 0 ) MPI_Bcast(buffer, sizeof(buffer), MPI_CHAR, 0, MPI_COMM_WORLD);

                    process_partial_STL(rank, size, final_msg);

                    MPI_Barrier(MPI_COMM_WORLD);    // Waits for all nodes to finish writing the partial STL files

                    process_STL(size);

                    send_serial(); //Enviar al Hardware
                }

                memset(buffer, 0, sizeof(buffer));

            } else if ((client_fd = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen)) < 0) {
                perror("Accept failed");
                return -1;
            } else {
                printf("Client connected\n\n");
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
    }
    else {
        int active = 1;

        while (active) {
            char buffer[1024];

            MPI_Bcast(buffer, sizeof(buffer), MPI_CHAR, 0, MPI_COMM_WORLD);

            char final_msg[1024] = { 0 };
            int count;
            char **substrings = split_string(buffer, '|', &count);
            for (int i = 0; i < count; i++) {
                strcat(final_msg, ASCIIToMessage(rsa_decrypt(strtoull(substrings[i], NULL, 10), keys->private_key)));
            }
            free_split_string(substrings, count);

            if (strcmp(final_msg, "shutdown") == 0) active = 0;
            else {
                printf("Node %d received: %s\n", rank, final_msg);

                process_partial_STL(rank, size, final_msg);

                MPI_Barrier(MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();
    return 0;
}

