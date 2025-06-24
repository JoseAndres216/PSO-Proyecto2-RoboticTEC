#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 65536


int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    FILE *file, *file_encrypted, *file_original;

    // Open the file for reading
    file = fopen(argv[2], "rb");
    if (file == NULL) {
        perror("Error opening the file.");
        exit(EXIT_FAILURE);
    }

    // Create the socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error creating the socket.");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert the IP address
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address.");
        fclose(file);
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to the server.");
        fclose(file);
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Open the file to save the original file
    file_original = fopen("../Files/OriginalFile.txt", "rb");
    if (file_original == NULL) {
        perror("Error opening the original file.");
        fclose(file);
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Open the file to save the encrypted file
    file_encrypted = fopen("../Files/EncryptedFile.txt", "w");
    if (file_encrypted == NULL) {
        perror("Error opening the file to save the encrypted file.");
        fclose(file);
        fclose(file_original);
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Read and encrypt the file
    ssize_t bytes_read;
    while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {

        // Send the encrypted data to the server
        send(sock, buffer, bytes_read, 0);
    }

    printf("Original file saved as 'OriginalFile.txt.'\n");
    printf("Encrypted file saved as 'EncryptedFile.txt' and sent to the server.\n");

    // Close the files and socket
    fclose(file);
    fclose(file_original);
    fclose(file_encrypted);
    close(sock);

    return 0;
}