#ifndef ARDUINO_CNC_DRIVER_H
#define ARDUINO_CNC_DRIVER_H

int iniciar_serial();
void cerrar_serial(int fd);
int enviar_letra(int fd, char c);

#endif
