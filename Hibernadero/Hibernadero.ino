/*
 * Autor: Joel Ermantraut
 * Ultima Modificacion: 8/8/2017
 * Descripcion: Algoritmo de un hibernadero que incorpora:
 *  - Sensor de temperatura.
 *  - Sensor de humedad.
 *  - Sensor de luminosidad.
 *  - Sensor de humedad del suelo.
 *  - Servo motor.
 * Con esto, intenta que todos estos parametros permanezcan
 * en los valores ideales, variando aspecto como la luz que
 * ingresa (mediante un foco y abriendo/cerrando persianas),
 * la humedad (mediante ventilacion), etc.
 * 
 * Incorpora ademas un menu para elegir los valores ideales
 * de estos parametros.
 * 
 * IMPORTANTE: Este programa recibio modificaciones que no
 * fueron probadas en el prototipo, por lo que puede no ser
 * completamente funcional.
 */

#include <Wire.h> // Para la comunicacion I2C del BH1750
#include "BH1750/BH1750.cpp"
#include "DHT11/DHT.cpp"
#include <Servo.h>
#include <LiquidCrystal.h> // LCD

// Librerias

// La directiva DEFINE, es una orden para el compilador que reemplaza una palabra con el texto que le sigue.
#define DHTPIN              6
#define servo1              46
#define servo2              47
#define BH1750_ADD          51
#define BH1750_SDA          20
#define BH1750_SCL          21
#define YL69_A0             15
#define YL69_D0             48
#define LCD_D7              12
#define LCD_D6              11
#define LCD_D5              10
#define LCD_D4              9
#define LCD_E               8
#define LCD_RS              7
#define COOLERS             53
#define PULS_AR             30
#define PULS_AB             31
#define PULS_DER            32
#define PULS_IZQ            33
#define FOCO                34
#define BOMBA               35
#define LUZ_DE_DIA          50 // Valor arbitario, sujeto a pruebas

// DEFINEs

int tempRango,
    humRango,
    humSueloRango,
    lumRango;
// Rangos
int hora,
    minuto,
    segundo;
// Tiempo
int item;
// Menu

// VARIABLES

BH1750 medidorLuz(0x23);
// Definicion en la direccion 0x23
DHT dht(DHTPIN, DHT11);
// Le pasamos al DHT su pin y su modelo
Servo motor1;
Servo motor2;
// Motores
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
// Declaracion del LCD
 
// Inicializacion

float leerTemperatura() {

  float t;

  do {

    t =  dht.readTemperature();

  } while (isnan(t));
  // Hasta que la medicion sea exitosa

  return t;
  
}

int leerHumedad() {

  float h;

  do {

    h = dht.readHumidity();

  } while (isnan(h));
  // Hasta que la medicion sea exitosa

  return h;
  
}

int leerHumedadSuelo() {

  return analogRead(YL69_A0) * 1023 / 100;
  
}

uint16_t leerLuminosidad() {

  uint16_t lux = medidorLuz.readLightLevel();

  return lux;

}

void prenderFoco() {
  digitalWrite(FOCO, HIGH);
}

void apagarFoco() {
  digitalWrite(FOCO, LOW);
}

void abrirPersianas() {
  motor1.write(180);
  motor2.write(180);
  // Elegir rotacion adecuada
}

void cerrarPersianas() {
  motor1.write(0);
  motor2.write(0);
}

bool esDeDia() { 
  if (leerLuminosidad() > LUZ_DE_DIA) {
    return true;
  } else {
    return false;
  }
}

void iluminar() {

  if (esDeDia()) {
    abrirPersianas();
    apagarFoco();
  } else {
    prenderFoco();
    cerrarPersianas();
  }
  
}

void ventilar(int estado) {
  digitalWrite(COOLERS, estado);
}

void alternarBomba(int estado) {
  digitalWrite(BOMBA, estado);
}

void configurarHora() {

  hora = 0;
  minuto = 0;
  segundo = 0;
  bool salir = false;

  int curs = 0;

  lcd.clear();

  while (salir == false) {

    lcd.setCursor(0, 0);
    lcd.print(hora + ':' + minuto + ':' + segundo);

    if (digitalRead(PULS_AR)) {

      switch (curs) {

        case 0:
          hora++;
          if (hora > 23) hora = 0;
          break;

        case 1:
          minuto++;
          if (minuto > 59) minuto = 0;
          break;

        case 2:
          segundo++;
          if (segundo > 59) segundo = 0;
          break;
                  
      }
      
    } else if (digitalRead(PULS_AB)) {

      switch (curs) {

        case 0:
          hora--;
          if (hora < 0) hora = 23;
          break;

        case 1:
          minuto--;
          if (minuto < 0) minuto = 59;
          break;

        case 2:
          segundo--;
          if (segundo < 0) segundo = 59;
          break;
                  
      }

    } else if (digitalRead(PULS_DER)) {

        curs++;
        if (curs > 3) curs = 0;
      
    } else if (digitalRead(PULS_IZQ)) {

        salir = true;

    }
    
  }

}

void cambiaRango(int item) {

  int n = 0;
  bool salir = false;

  while (salir == false) {

    if (digitalRead(PULS_AR)) {
      n++;
    } else if (digitalRead(PULS_AB)) {
      n--;
    } else if (digitalRead(PULS_DER) || digitalRead(PULS_IZQ)) {

      switch (item) {

        case 0:
          tempRango = n;
        case 1:
          humRango = n;
        case 2:
          humSueloRango = n;
        case 3:
          lumRango = n;

      }

      salir = true;

    }

  }

}

void configurarRangos() {

  int item = 0;
  int n = 0;
  bool salir = false;

  while (salir == false) {

    lcd.clear();
    lcd.setCursor(0, 0);
    switch (item) {

      case 0:
        lcd.print("Temperatura...");
        break;
      case 1:
        lcd.print("Humedad...");
        break;
      case 2:
        lcd.print("Humedad de suelo...");
        break;
      case 3:
        lcd.print("Luminosidad...");
        break;

    }

    if (digitalRead(PULS_AR)) {
      item++;
      if (item > 3) item = 0;
    } else if (digitalRead(PULS_AB)) {
      item--;
      if (item < 0) item = 3;
    } else if (digitalRead(PULS_DER)) {
      cambiaRango(item);
    } else if (digitalRead(PULS_IZQ)) {
      salir = true;
    }
    
  }
  
}

void mostrarMenu() {
  
  item = 0;
  bool salir = false;

  while (salir == false) {

    lcd.clear();
  
    if (item == 0) {
      float v = leerTemperatura();
      lcd.setCursor(0, 0);
      lcd.print("Temperatura: ");
      lcd.setCursor(0, 13);
      lcd.print(v);
      v = leerHumedad();
      lcd.setCursor(1, 0);
      lcd.print("Humedad Amb: ");
      lcd.setCursor(1, 13);
      lcd.print(v);
    }
    if (item == 1) {
      float v = leerHumedadSuelo();
      lcd.setCursor(0, 0);
      lcd.print("Hum Suelo: ");
      lcd.setCursor(0, 11);
      lcd.print(v);
      uint16_t l = leerLuminosidad();
      lcd.setCursor(1, 0);
      lcd.print("Luminosidad: ");
      lcd.setCursor(1, 13);
      lcd.print(l);
    }

    if (digitalRead(PULS_AB)) {
      item++;
      if (item == 2) item = 0;
    } else if (digitalRead(PULS_AR)) {
      item--;
      if (item == -1) item = 1;
    } else if (digitalRead(PULS_AR)) {
      salir = true;
    }

  }
  
}

// Funciones

void setup() {

  medidorLuz.begin(BH1750_ONE_TIME_HIGH_RES_MODE_2);
  // Tiene diferentes modos de medicion, este modo mide solo cuando le pedis y despues se apaga (alta resolucion)
  dht.begin();
  // Lo arrancamos
  motor1.attach(servo1);
  motor2.attach(servo2);
  // Motores
  lcd.begin(16, 2);
  // Le pasamos las dimensiones del display

  pinMode(FOCO, OUTPUT);
  pinMode(BOMBA, OUTPUT);
  pinMode(COOLERS, OUTPUT);
  pinMode(PULS_AR, INPUT);
  pinMode(PULS_AB, INPUT);
  pinMode(PULS_DER, INPUT);
  pinMode(PULS_IZQ, INPUT);
  // Declaracion de pines

  configurarHora();
  configurarRangos();
}

void loop() {

  if (leerTemperatura() < tempRango - 20) {
    prenderFoco(); 
  } else if (leerTemperatura() > tempRango + 20) {
    apagarFoco();
  }

  if (leerHumedad() < humRango - 20) {
    ventilar(false); 
  } else if (leerHumedad() > humRango + 20) {
    ventilar(true);
  }

  if (leerHumedadSuelo() < humSueloRango - 20) {
    alternarBomba(true);   
  } else if (leerHumedadSuelo() > humSueloRango + 20) {
    alternarBomba(false);
  }

  iluminar();

  if (digitalRead(PULS_DER) || digitalRead(PULS_IZQ)) {
    lcd.clear();
    lcd.print("Mostrando Menu...");
    delay(1000);
    mostrarMenu();
  }

  delay(1000);

}
