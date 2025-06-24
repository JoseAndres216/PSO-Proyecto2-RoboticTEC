#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 65536
#define XOR_KEY 0x55

// Función para descifrar XOR (es la misma que cifrar)
void xorDecrypt(char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] ^= XOR_KEY;
    }
}

int main(int argc, char *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    FILE *file;

    // Create the socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error creating the socket.");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error binding.");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Error listening.");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Error accepting the connection.");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Open the file to save the decrypted file
    file = fopen("../Files/DecryptedFile.txt", "wb");
    if (file == NULL) {
        perror("Error opening the file");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Receive the file, decrypts it, and save it
    ssize_t bytes_read;
    while ((bytes_read = read(new_socket, buffer, BUFFER_SIZE)) > 0) {

        // Decrypt the buffer using XOR
        xorDecrypt(buffer, bytes_read);
        
        fwrite(buffer, sizeof(char), bytes_read, file);
    }

    printf("Decrypted file saved as 'DecryptedFIle.txt'\n");

    // Close the file and sockets
    fclose(file);
    close(new_socket);
    close(server_fd);

    return 0;
}