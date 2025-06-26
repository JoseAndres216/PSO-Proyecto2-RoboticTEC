#include <Servo.h>

// Matriz teclado QWERTY 6x5
const char teclado[5][6] = {
  {'q', 'w', 'e', 'r', 't', 'y'},
  {'u', 'i', 'o', 'p', 'a', 's'},
  {'d', 'f', 'g', 'h', 'j', 'k'},
  {'l', 'z', 'x', 'c', 'v', 'b'},
  {'n', 'm', '.', ',', '\'', '-'}
};

const int ancho = 6;
const int alto = 5;

int posX = 0;
int posY = 0;

Servo motorLR;
Servo motorUD;

const int VEL_DER = 60;
const int VEL_IZQ = 120;
const int VEL_ABAJO = 60;
const int VEL_ARRIBA = 120;
const int STOP = 90;

// Tiempos reducidos para pasos más pequeños
const int DUR_H = 420;    
const int DUR_V = 620;    

void setup() {
  motorLR.attach(9);
  motorUD.attach(10);

  motorLR.write(STOP);
  motorUD.write(STOP);

  Serial.begin(9600);
  Serial.println("Inicio en 'q' (0,0). Envía letras para mover el cursor.");
  imprimirPosicion();
}

void loop() {
  if (Serial.available()) {
    char letra = Serial.read();

    if (letra >= 'A' && letra <= 'Z') {
      letra = letra + ('a' - 'A');
    }

    Serial.print("Letra recibida: ");
    Serial.println(letra);

    int destinoX = -1;
    int destinoY = -1;

    for (int y = 0; y < alto; y++) {
      for (int x = 0; x < ancho; x++) {
        if (teclado[y][x] == letra) {
          destinoX = x;
          destinoY = y;
          break;
        }
      }
      if (destinoX != -1) break;
    }

    if (destinoX == -1 || destinoY == -1) {
      Serial.println("Letra no encontrada en el teclado.");
      return;
    }

    Serial.print("Mover de (");
    Serial.print(posX);
    Serial.print(", ");
    Serial.print(posY);
    Serial.print(") a (");
    Serial.print(destinoX);
    Serial.print(", ");
    Serial.print(destinoY);
    Serial.println(")");

    while (posX < destinoX) {
      moverDerecha();
      posX++;
      imprimirPosicion();
    }
    while (posX > destinoX) {
      moverIzquierda();
      posX--;
      imprimirPosicion();
    }
    while (posY < destinoY) {
      moverAbajo();
      posY++;
      imprimirPosicion();
    }
    while (posY > destinoY) {
      moverArriba();
      posY--;
      imprimirPosicion();
    }
  }
}

void moverDerecha() {
  motorLR.write(VEL_DER);
  delay(DUR_H);
  motorLR.write(STOP);
}

void moverIzquierda() {
  motorLR.write(VEL_IZQ);
  delay(DUR_H);
  motorLR.write(STOP);
}

void moverAbajo() {
  motorUD.write(VEL_ABAJO);
  delay(DUR_V);
  motorUD.write(STOP);
}

void moverArriba() {
  motorUD.write(VEL_ARRIBA);
  delay(DUR_V);
  motorUD.write(STOP);
}

void imprimirPosicion() {
  Serial.print("Posición actual: (");
  Serial.print(posX);
  Serial.print(", ");
  Serial.print(posY);
  Serial.println(")");
}
