/*
 * Autor: Joel Ermantraut
 * Ultima Modificacion: 10/10/2016
 * Descripcion: Juego de luces, con distintas combinacion, originalmente
 * diseñado para comunicarse serialmente con un modulo RF y poder elegir
 * la combinacion con el mismo.
 */

char dato;

void clarear() {
  for (int i=2; i < 10; i++) {
    digitalWrite(i, LOW);
  }
}
// 00000000

void barridoIzquierda() {
  for (int i=2;i < 10;i++) {
    if (i == 2) {digitalWrite(9, LOW);}
    else {digitalWrite(i-1, LOW);} // Apaga el anterior
    digitalWrite(i, HIGH); // Prende el actual
    delay(100);
    if (Serial.available()) {break;}
  }
}
// 00000001
// 00000010
// 00000100
// ...
// 10000000

void barridoDerecha() {
  for (int i=9;i > 1;i--) {
    if (i == 9) {digitalWrite(2, LOW);}
    else {digitalWrite(i+1, LOW);}
    digitalWrite(i, HIGH);
    delay(100);
    if (Serial.available()) {break;}
  }
}
// 10000000
// 01000000
// 00100000
// ...
// 00000001

void barridoBidireccional() {
  barridoDerecha();
  barridoIzquierda();
}
// 10000000
// 01000000
// ...
// 00000001
// 00000010
// ...
// 10000000

void escalera() {
  clarear();
  byte intensidad = 0;
  for (int i2=0; i2 < 3;i2++) {
    for (int i=2; i < 5; i++) {
      digitalWrite(i, HIGH);
      digitalWrite(11-i, HIGH);
      delay(500);
      if (Serial.available()) {break;}
    }
    clarear();
    intensidad += 85;
    analogWrite(5, intensidad);
    analogWrite(6, intensidad);
    delay(500);
  }
}
// 10000001
// 01000010
// 00100100
// 000.33.33000
// Repite 3 veces

void parpadeo() {
  for (int i2=0;i2 < 2;i2++) {
    clarear();
    delay(100);
    for (int i=2;i < 6;i++) {digitalWrite(i, HIGH);}
    delay(100);
    clarear();
    delay(100);
    for (int i=6;i < 10;i++) {digitalWrite(i, HIGH);}
    delay(100);
  }
}
// 11110000
// 00001111

void alAzar() {
  clarear();
  digitalWrite(random(2, 10), HIGH);
  delay(100);
}

void acumulativo() {
  for (int i=2; i < 10; i++) {
    digitalWrite(i, HIGH);
    delay(100);
    if (Serial.available()) {break;}
  }
  for (int i=9; i > 1; i--) {
    digitalWrite(i, LOW);
    delay(100);
    if (Serial.available()) {break;}
  }
}
// 10000000
// 11000000
// 11100000
// ...
// 11111111
// 11111110
// 11111100
// ...
// 00000000

void espejo() {
  for (int i=2; i < 6; i++) {
    digitalWrite(i, HIGH);
    digitalWrite(11-i, HIGH);
    delay(250);
    clarear();
    if (Serial.available()) {break;}
  }
  for (int i=5; i > 1; i--) {
    digitalWrite(i, HIGH);
    digitalWrite(11-i, HIGH);
    delay(250);
    clarear();
    if (Serial.available()) {break;}
  }
}
// 10000001
// 01000010
// ...
// 00011000
// 00100100
// ..
// 10000001

void elegir() {
  switch (dato) {
    case '1':
      barridoIzquierda();
      break;
    case '2':
      barridoDerecha();
      break;
    case '3':
      barridoBidireccional();
      break;
    case '4':
      escalera();
      break;
    case '5':
      parpadeo();
      break;
    case '6':
      alAzar();
      break;
    case '7':
      acumulativo();
      break;
    case '8':
      espejo();
      break;
    default:
      break;
  }
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    dato = Serial.read();
    elegir();
  } else {
    elegir();
  }
}
