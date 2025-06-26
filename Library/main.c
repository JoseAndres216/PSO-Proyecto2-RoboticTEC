#include <stdio.h>
#include "Library.h"

int main() {
    if (leer_palabra("palabra.txt") != 0) {
        fprintf(stderr, "Error leyendo archivo.\n");
        return 1;
    }

    printf("Palabra: %s\n", palabra);
    char letra = letra_mas_repetida();
    printf("Letra más repetida: %c\n", letra);

    if (enviar_palabra_a_arduino() == 0) {
        printf("Enviado correctamente.\n");
    } else {
        printf("Error al enviar.\n");
    }

    return 0;
}
