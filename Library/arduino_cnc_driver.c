#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_PATH 256

int disable_conflicting_services()
{
    // Intenta detener servicios comunes que bloquean el puerto
    system("sudo systemctl stop ModemManager.service 2>/dev/null");
    system("sudo systemctl stop brltty.service 2>/dev/null");
    return 0;
}

char *detect_arduino_port()
{
    static char path[MAX_PATH];
    DIR *dir = opendir("/dev");
    if (!dir)
        return NULL;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strstr(entry->d_name, "ttyACM") || strstr(entry->d_name, "ttyUSB"))
        {
            snprintf(path, sizeof(path), "/dev/%s", entry->d_name);
            closedir(dir);
            return path;
        }
    }

    closedir(dir);
    return NULL;
}

int open_serial(const char *device)
{
    int fd = open(device, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        perror("Error abriendo el puerto serial");
        return -1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0)
    {
        perror("Error al obtener atributos");
        close(fd);
        return -1;
    }

    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);
    tty.c_cflag |= (CLOCAL | CREAD); // Habilitar la lectura y no controlar la línea
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;     // 8 bits
    tty.c_cflag &= ~PARENB; // sin paridad
    tty.c_cflag &= ~CSTOPB; // 1 bit de parada
#ifdef CRTSCTS
    tty.c_cflag &= ~CRTSCTS; // sin control de flujo por hardware si está definido
#endif                       // sin control de flujo

    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        perror("Error al aplicar configuración");
        close(fd);
        return -1;
    }

    return fd;
}

int enviar_letra(int fd, char c)
{
    return write(fd, &c, 1);
}
