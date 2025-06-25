#include "Library.h"
#include "arduino_cnc_driver.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char palabra[100]; // definición

int leer_palabra(const char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) return -1;
    fscanf(archivo, "%99s", palabra);
    fclose(archivo);
    return 0;
}

char letra_mas_repetida() {
    int frecuencia[256] = {0};
    for (int i = 0; palabra[i]; i++) {
        frecuencia[(unsigned char)palabra[i]]++;
    }

    char letra_max = '\0';
    int max = 0;
    for (int i = 0; i < 256; i++) {
        if (frecuencia[i] > max) {
            max = frecuencia[i];
            letra_max = i;
        }
    }
    return letra_max;
}

int enviar_palabra_a_arduino() {
    int fd = iniciar_serial();
    if (fd < 0) return -1;

    for (int i = 0; palabra[i] != '\0'; i++) {
        if (enviar_letra(fd, palabra[i]) < 0) {
            cerrar_serial(fd);
            return -2;
        }
        usleep(500000); // 0.5s entre letras
    }

    cerrar_serial(fd);
    return 0;
}
