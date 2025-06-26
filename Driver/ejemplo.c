#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd = open("/dev/arduino_cnc", O_WRONLY);
    if (fd < 0) {
        perror("No se pudo abrir el dispositivo arduino_cnc");
        return 1;
    }

    char *comando = "X-LEFT\n";
    write(fd, comando, strlen(comando));
    close(fd);

    return 0;
}
