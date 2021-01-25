/*
 * Autor: Joel Ermantraut
 * Ultima Modificacion: 7/5/2015
 * Descripcion: Algoritmo que controla secuencialmente un motor
 * paso a paso para verificar su funcionamiento.
 * 
 * Motor paso a paso bipolar con interfaz basada en ULN2803.
 */

int pos=1; // Variable que almacena el ultimo cable del PaP al que se envio un pulsos
int t=100; // Variable que determina el periodo de retardo

void reset() {
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  // Resetea las salida del Arduino
}

void izq() {
  // Funcion que gira el motor hacia la derecha
  pos++; // Ajusta la posición al siguiente cable
  if (pos > 5) {
    // La posición máxima es 5, por lo que si es mayor, se coloca en el primer cable
    pos = 2;
  }
  digitalWrite(pos, LOW); // Se envia un 0 al cable indicado
  delay(t); // Se frena el programa por un tiempo t
  reset(); // Se resetean las salidas del Arduino
}

void der() {
  // Función que gira el motor hacia la izquierda
  pos--; // Ajusta la posición a el cable anterior
  if (pos < 2) {
    // La posición mínima es 2, por lo que si es menor, se coloca en el ultimo cable
    pos = 5;
  }
  digitalWrite(pos, LOW); // Se envía un 0 al cable indicado
  delay(t); // Se frena el programa por un tiempo t
  reset(); // Se resetean las salidas del Arduino
}

void setup() {
  Serial.begin(9600);
  // Inicia el serial
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  // Configura los pines 2, 3, 4, 5 como salidas
}

void loop() {
  while(Serial.available()>0) {
    // El programa se ejecuta mientras el serial este disponible
    for (int i=0;i<30;i++) {
      der();
      // Realiza 30 pasos hacia la derecha
    }
    delay(2000);
    for (int i=0;i<60;i++) {
      izq();
      // Realiza 60 pasos hacia la izquierda
    }
    delay(2000);
    for (int i=0;i<30;i++) {
      der();
      // Realiza 30 pasos hacia la derecha
    }
    delay(2000);
  }
}
