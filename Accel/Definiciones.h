// MOTORES
#define adelante 5 // PIN para que el motor de continua gire hacia adelante
#define atras 6 // PIN para que el motor de continua gire hacia atrás
// SERVOS
#define servoGiro 3 // PIN donde va conectado el servo de dirección
#define servoCamara 9 // PIN donde va conectado el servo de cámara
// LIMITES
#define giroMin 0 // Determina el giro mínimo del servo de dirección
#define giroMax 180 // Determina el giro máximo del servo de dirección
// LUCES
#define lucesBajas 7 // PIN donde van conectadas las luces bajas
#define lucesAltas 8 // PIN donde van conectadas las luces altas
#define lucesAtras 11 // PIN donde van conectadas las luces traseras
#define lucesAlarma 13 // PIN donde van conectadas la luces de la alarma
// RETARDOS
#define delayServo 10000 // Determina el retardo entre movimientos (en microsegundos)
#define delayFreno 2000 // Tiempo que debe pasar para que termine de frenar
#define reinicio 2000 // Tiempo que debe pasar sin recibir datos para reiniciar el módulo
// ALARMA
#define maxFrecuencia 800 // Frecuencia máxima del parlante de la alarma
#define minFrecuencia 500 // Frecuencia mínima del parlante de la alarma
// OTROS
#define BtVcc 12 // PIN para el Vcc del módulo BT
#define parlante 10 // PIN donde va conectado el parlante
#define ruidoMax 15 // Cantidad de valores que promedia para eliminar el ruido
