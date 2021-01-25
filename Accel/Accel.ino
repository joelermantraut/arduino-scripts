/*
 * Autor: Joel Ermantraut
 * Ultima Modificacion: 20/11/2015
 * Descripcion: Algoritmo que controla un prototipo electronico. El mismo
 * es un vehiculo que se controla inalambricamente con Bluetooth, realiza
 * la traccion mediante un motor de continua, el giro mediante un servo
 * motor, posee una alarma y permite ubicar una camara para realizar
 * la grabacion.
 */

#include <Servo.h> // Para controlar los servos
#include <SoftwareSerial.h> // Para el modulo Bluetooth
#include <avr/sleep.h> // Para suspender el Arduino
#include "Definiciones.h" // Definiciones de pines y tiempos

Servo giro; // Servo que controla la dirección
Servo camara; // Servo que controla la cámara
SoftwareSerial BT(2,4); // TX | RX
 
volatile boolean resetear = true; // Almacena si se recibieron datos durante los últimos segundos
volatile boolean conectado = false; // Recuerdo si el módulo fue conectado
volatile byte value = 0; // Almacena el último valor de traccion
volatile byte gradosGiro = 0; // Almacena los grados de movimiento del servo de dirección
volatile byte gradosCamara = 0; // Almacena los grados de movimiento del servo de cámara
volatile byte pos = 1;
volatile unsigned long tiempo = 0; // Almacena el último valor de tiempo
volatile int arrayDireccion[10];
volatile int arrayCamara[10];
volatile int ruidoDireccion = 0;
volatile int ruidoCamara = 0;
volatile int titilar = 0;
volatile int totalGiro = 0;
volatile int totalCamara = 0;

void interrupcion() {
  if (BT.read() == 'd') {
    sleep_disable(); // Desactiva el modo de suspensión
    detachInterrupt(0); // Desactiva el temporizador
  }
}

void Sleep() { // Suspende el vehículo
  freno(); // Frena el vehículo, cambiando enable a LOW
  attachInterrupt(0,interrupcion,RISING);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Establecemos el modo de bajo consumo
  // [SLEEP_MODE_PWM_DOWN] es el modo que más ahorra, también hay otros modos disponibles
  // Mientras más ahorra, más timers desactiva
  sleep_enable(); // Inicializamos el modo bajo consumo
  sleep_mode(); // Ponemos el modo bajo consumo
  // Configura la interrupción
}

void traccion(int a, int b) { // Modifica el movimiento del motor de continua
  analogWrite(adelante, a);
  analogWrite(atras, b);
  delayMicroseconds(10000);
}

void posicion(int valor) { // Determina los valores del motor de continua
  value = map(abs(valor), 0, 10, 0, 255); // MAPEA EL VALOR ENTRE LO QUE RECIBIO Y EL RANGO DE SALIDA
  if (valor > 0) { // Si el valor es mayor a 0
    if (pos == 2) {freno();}
    traccion(value, 0);
    pos = 0;
  } else if (valor < 0) {
    if (pos == 0) {freno();}
    traccion(0, value);
    pos = 2;
  } else if (pos != 1) {
    freno();
  }
}

void direccion(int valor) { // Determina el valor del servo de giro
  arrayDireccion[ruidoDireccion] = valor;
  totalGiro = totalGiro + arrayDireccion[ruidoDireccion];
  if (ruidoDireccion == ruidoMax) {
    ruidoDireccion = 0;
    valor = map(totalGiro/ruidoMax, -10, 10, giroMin, giroMax);
    if (valor > gradosGiro) { // Si la posicion en la que esta es menor que la que tiene que ir
      for (int i=gradosGiro;i<valor;i++) {
        giro.write(i);
        delayMicroseconds(delayServo);
        // Avanza
      }
    } else if (valor < gradosGiro) { // Si es mayor
      for (int i=gradosGiro;i>valor;i--) {
        giro.write(i);
        delayMicroseconds(delayServo);
        // Retrocede
      }
    }
    gradosGiro = valor;
  } else {
    ruidoDireccion++;
  }
}

void girarCamara(int valor) { // Modifique el giro de la cámara
  arrayCamara[ruidoCamara] = valor;
  totalCamara = totalCamara + arrayCamara[ruidoCamara];
  if (ruidoCamara == ruidoMax) {
    ruidoCamara = 0;
    if (valor > gradosCamara) { // Si la posicion en la que esta es menor que la que tiene que ir
      for (int i=gradosCamara;i<valor;i++) {
        camara.write(i);
        delayMicroseconds(delayServo);
        // Avanza
      }
    } else if (valor < gradosCamara) { // Si es mayor
      for (int i=gradosCamara;i>valor;i--) {
        camara.write(i);
        delayMicroseconds(delayServo);
        // Retrocede
      }
    }
    gradosCamara = valor;
  } else {
    ruidoCamara = 0;
  }
}

void freno() { // Frena el vehículo
  luces(lucesAtras, 1);
  if (pos == 0) {
    for (int i=value;i>0;i--) {
      traccion(i,0);
      delay(5);
    }
  } else if (pos == 2) {
    for (int i=value;i<0;i++) {
      traccion(0,i);
      delay(5);
    }
  }
  luces(lucesAtras, 0);
  pos = 1;
}

void luces(int luz, int valor) {
  digitalWrite(luz, valor);
}

void bocina(int valor) {
  Serial.println(valor);
  if (valor == 1) {tone(parlante,750);}
  else {noTone(parlante);}
}

void decodificar() { // Se activa cuando se recibe un dato BT y lo decodifica
  resetear = false; // Cada vez que recibe un dato se pone en falso, para evitar que el temporizador desconecte al módulo BT
  char dato = BT.read(); // Recibe el caracter inicial
  float valor = BT.parseFloat(); // INICIALIZAR ANTES DE ENTRAR EN SWITCH, NO EN CASES, PRODUCE ERRORES
  Serial.print(dato);
  Serial.println(valor);
  switch (dato) { // Según que dato sea, ejecuta cada función
    case 'p': // Recibe el caracter de posición
      posicion(valor); // Modifica la posición
      break; // ESTO EVITA QUE SIGA CON EL OTRO CASE
    case 'l': // Recibe el caracter de dirección
      direccion(valor); // Modifica la dirección
      break;
    case 'c': // Recibe el caracter de cámara
      girarCamara(valor); // Modifica la posición de la cámara
      break;
    case 's': // Recibe el caracter de freno
      freno(); // Frena el vehículo
      break;
    case 'b':
      bocina(valor);
      break;
    case 'u': // Recibe el caracter de luces bajas
      luces(lucesBajas, valor); // false se refiere a luces bajas
      break;
    case 'U': // Recibe el caracter de luces altas
      luces(lucesAltas, valor); // true se refiere a luces altas
      break;
    case 'd': // Recibe el caracter de suspensión
      Sleep(); // Entra en el modo de suspensión
      break;
  }
}

void efecto2() {
  tone(parlante,400,200);
  delay(200);
}

void efecto1() {
  static boolean upDown = false;
  static int i=minFrecuencia;
  if (upDown == false) {
    i++;
    if (i == maxFrecuencia) {upDown = true;}
  } else {
    i--;
    if (i == minFrecuencia) {upDown = false;}
  }
  tone(parlante,i,10);
}

void alarma() {
  static boolean efecto=true;
  static int cantidad=0;
  while (!(BT.available())) {
    if (efecto) {
      if (cantidad == 4) {efecto=!efecto;cantidad=0;}
      else {cantidad++;}
      efecto1();
    } else {
      if (cantidad == 10) {efecto=!efecto;cantidad=0;}
      else {cantidad++;}
      efecto2();
    }
    delay(10);
  }
}

void verificar() {
  if (resetear == true) { // Resetea el módulo BT
    digitalWrite(BtVcc, LOW);
    delay(10);
    digitalWrite(BtVcc, HIGH);
    delay(10);
    // El reseteo del módulo consiste en cortar y restituir la alimentación
    alarma(); // Inicia la alarma
  }
  resetear = true; // Esto se hace para que si se recibe se coloque en false y se advierta en el próximo censo
}

void setup() {
  BT.begin(9600); // Se inicializa la conexión Bluetooth
  Serial.begin(9600); // SACAR
  // Pines motor de continua
  pinMode(BtVcc, OUTPUT); // BT Vcc
  pinMode(lucesBajas, OUTPUT); // LEDs Bajos
  pinMode(lucesAltas, OUTPUT); // LEDs Altos
  pinMode(lucesAtras, OUTPUT); // LEDs Atras
  pinMode(lucesAlarma, OUTPUT); // LEDs Alarma
  digitalWrite(BtVcc, LOW); // Alimenta el módulo Bluetooth
  giro.attach(servoGiro);
  camara.attach(servoCamara);
  // Se vincula los servos con los pines entre paréntesis
  direccion(0); // Centra el eje de giro al inicio
  girarCamara(90); // Centra el eje de cámara al inicio
}

void loop() {
  if ((pos == 2) && (millis() >= (tiempo + 500))) {
    digitalWrite(lucesAtras, !titilar);
    titilar = digitalRead(lucesAtras);
  }
  tiempo = millis();
  if (BT.available()) { // Si hay un dato disponible
    conectado = true; // Significa que esta conectado
    decodificar();
  }
}
