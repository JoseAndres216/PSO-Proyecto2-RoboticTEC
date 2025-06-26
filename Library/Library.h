#ifndef LIBRARY_H
#define LIBRARY_H

extern char palabra[100]; // variable global

int leer_palabra(const char* nombre_archivo);
char letra_mas_repetida();
int enviar_palabra_a_arduino();

#endif
