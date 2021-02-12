/*
 * Autor: Joel Ermantraut
 * Ultima Modificacion: 01/12/2017
 * Descripcion: Este programa permitia incorporar dos relojes, uno
 * decimal y otro binario, ubicados en una maqueta simulando ser un
 * edificio, donde en una de sus paredes se podía leer la hora mediante
 * un formato decimal (usando 7-segmentos) y en otra pared se podía leer
 * usando LEDs que representaban bits de los mismos números en binario.
 * 
 * IMPORTANTE: La librería fue modificada y este programa ya no es
 * compatible con la misma.
 */

#include <DS1307RTC.h>

// Librerias

#define D1 22
#define D2 24
#define D3 26
#define D4 28
// Datos

#define S1 52
#define S2 50
#define S3 48
#define S4 46
// Strobes

#define L1 2
#define L2 4
#define L3 8
#define L4 11
// LEDs

#define M 4

// DEFINICIONES

int datos[10][4] = {

  {0, 0, 0, 0},
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {1, 1, 0, 0},
  {0, 0, 1, 0},
  {1, 0, 1, 0},
  {0, 1, 1, 0},
  {1, 1, 1, 0},
  {0, 0, 0, 1},
  {1, 0, 0, 1}

};

// VARIABLES

void muestreoDisplay(int indice, int valor) {

  digitalWrite(indice, LOW);
  delay(1);

  digitalWrite(D1, datos[valor][0]);
  digitalWrite(D2, datos[valor][1]);
  digitalWrite(D3, datos[valor][2]);
  digitalWrite(D4, datos[valor][3]);

  digitalWrite(indice, HIGH);

}

void mostrarDisplay(tmElements_t reloj) {

  int unidad, decena;

  if (reloj.Minute <= 9) {

    muestreoDisplay(S4, reloj.Minute);
    muestreoDisplay(S3, 0);

  } else {

    unidad = reloj.Minute % 10;
    decena = reloj.Minute / 10;

    muestreoDisplay(S4, unidad);
    muestreoDisplay(S3, decena);

  }

  if (reloj.Hora <= 9) {

    muestreoDisplay(S2, reloj.Hora);
    muestreoDisplay(S1, 0);

  } else {

    unidad = reloj.Hora % 10;
    decena = reloj.Hora / 10;

    muestreoDisplay(S2, unidad);
    muestreoDisplay(S1, decena);

  }
}

/* ***************************************************** */

void mostrarBinario(Reloj reloj) {

  int unidad, decena;

  if (reloj.Minute <= 9) {

    muestreoBinario(4, reloj.Minute);
    muestreoBinario(3, 0);
    // ACA ME QUEDE

  } else {

    unidad = reloj.Minute % 10;
    decena = reloj.Minute / 10;
    
    muestreoBinario(4, unidad);
    muestreoBinario(3, decena);

  }

  if (reloj.Hora <= 9) {

    muestreoBinario(2, reloj.Hora);
    muestreoBinario(1, 0);

  } else {

    unidad = reloj.Hora % 10;
    decena = reloj.Hora / 10;
    
    muestreoBinario(2, unidad);
    muestreoBinario(1, decena);

  }

}

// FUNCIONES

void setup() {

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  // Datos de displays
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S4, OUTPUT);
  // Strobes de displays

  int i;
  for (i = L1; i < L4; i++) {

    pinMode(i, OUTPUT);

  }
  // LEDs del reloj binario

}

void loop() {

  tmElements_t reloj;
 
  RTC.read(reloj);

  mostrarDisplay(reloj);

  mostrarBinario(reloj);
 

}
