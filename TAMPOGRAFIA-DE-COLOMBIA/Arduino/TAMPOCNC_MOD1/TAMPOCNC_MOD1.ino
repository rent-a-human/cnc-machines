#include <EEPROM.h>
#include <Nextion.h>

#define TAMPO1 22
#define TAMPO2 24
#define TAMPO3 26
#define TAMPO4 28

#define CARRO1 30
#define CARRO2 32
#define CARRO3 34
#define CARRO4 36

#define TINTA1 38
#define TINTA2 40
#define TINTA3 42
#define TINTA4 44

#define nextion Serial2
Nextion myNextion(nextion, 19200);

int velocidad = 100;
int reiniciar = 0;
int PRESIONIMPRIMIR = 25;
int PRESIONENTINTAR = 10;
int tiempoentintar = 100;
int tiempoimprimir = 100;
int tintas = 2;
int p0 = 5000;
int p1 = 2800;
int p2 = 2200;
float pausa = 160;
int pause = 0;
int pausamax = 160;
int pausainicial = 160;
int contador = 0;
int imprimir = 0;
int automatico = 0;
int reset = 0;
int ciclos = 0;

float TAMPOP0 = 10;
float TAMPOP1 = 80;
float TAMPOP2 = 50;
float CARROP0 = 20;
float CARROP1 = 170;
float TINTAP0 = 0;
float TINTAP1 = 120;
double APP1 = 0.025;

double presionimprimir = PRESIONIMPRIMIR / APP1;
double presionentintar = PRESIONENTINTAR / APP1;
double tampop0 = TAMPOP0 / APP1; //UNIDADES EN PASOS
double tampop1 = TAMPOP1 / APP1;
double tampop2 = TAMPOP2 / APP1;
double carrop0 = CARROP0 / APP1;
double carrop1 = CARROP1 / APP1;
double tintap0 = TINTAP0 / APP1;
double tintap1 = TINTAP1 / APP1;

int pedal = 0;
int steps = 0;

int sensor_tampo = 0;
int sensor_carro = 0;
int sensor_tinta = 0;

long PATAMPO = 0; //Paso Actual TAMPO
long PACARRO = 0;
long PATINTA = 0;

int ciclos_reinicio = 10;
int modo = 0;

void setup() {
  Serial.begin(19200);;
  Serial.print("Inicio de programa ");
  //Motor TAMPO
  pinMode(TAMPO1, OUTPUT);
  pinMode(TAMPO2, OUTPUT);
  pinMode(TAMPO3, OUTPUT);
  pinMode(TAMPO4, OUTPUT);
  //Motor Carro
  pinMode(CARRO1, OUTPUT);
  pinMode(CARRO2, OUTPUT);
  pinMode(CARRO3, OUTPUT);
  pinMode(CARRO4, OUTPUT);
  //Motor Tinta
  pinMode(TINTA1, OUTPUT);
  pinMode(TINTA2, OUTPUT);
  pinMode(TINTA3, OUTPUT);
  pinMode(TINTA4, OUTPUT);
  //Sensores:A0 TAMPO, A1 Carro, A2 Tinta,A3 Pedal
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  delay(1000);
  if (myNextion.init()) {
    Serial.println(F("Nextion Ready..."));
  } else {
    Serial.println(F("Nextion not responding..."));
  }
  delay(1000);
  //myNextion.sendCommand("bauds=19200");
  //myNextion.sendCommand("page 1");
  delay(500);
  /*myNextion.sendCommand("page1.n0.val=12");
    delay(600);
    modo=myNextion.getComponentValue("n0");
    Serial.println(modo); 
    EEPROM.write(0, velocidad);
  EEPROM.write(1, TAMPOP0);
  EEPROM.write(2, TAMPOP1);
  EEPROM.write(3, TAMPOP2);
  EEPROM.write(4, CARROP0);
  EEPROM.write(5, CARROP1);
  EEPROM.write(6, TINTAP0);
  EEPROM.write(7, TINTAP1);
  */

 

  Serial.println("Iniciando");
  delay(1000);

  velocidad = 25;
  TAMPOP0 = EEPROM.read(1);
  TAMPOP1 = EEPROM.read(2);
  TAMPOP2 = EEPROM.read(3);
  CARROP0 = EEPROM.read(4);
  CARROP1 = EEPROM.read(5);
  TINTAP0 = EEPROM.read(6);
  TINTAP1 = EEPROM.read(7);
  Serial.println("Parametros leidos de memoria correctamente");
  CalcularVariables();

  Serial.print(" FIN Inicio de programa ");
  reiniciar = 1;
}

void CalcularVariables() {
  p0 = 5000 / velocidad;
  p1 = 2800 / velocidad;
  p2 = 2200 / velocidad;
  pausainicial = pausa;
  presionimprimir = PRESIONIMPRIMIR / APP1;
  presionentintar = PRESIONENTINTAR / APP1;
  tampop0 = TAMPOP0 / APP1; //UNIDADES EN PASOS
  tampop1 = TAMPOP1 / APP1;
  carrop0 = CARROP0 / APP1;
  carrop1 = CARROP1 / APP1;
  tintap0 = TINTAP0 / APP1;
  tintap1 = TINTAP1 / APP1;
  Serial.println("Variables recalculadas");
}

void loop() {
  delay(10);
  LeerPedal();
  IMPRIMIR();
  //Configurar();
  //myNextion.setComponentText("g0", "En Linea...");
}

void Configurar() {

}

void IMPRIMIR() {
  Serial.println("Imprimiendo...");
  CalcularVariables();
  reiniciar = ciclos % ciclos_reinicio + 1;
  if (reiniciar == 1 || ciclos == 0) {
    ReinicioSincrono();
    ReiniciarTodos();
    Serial.println("Reiniciado");
    reiniciar = 0;
  }
  pausa = pausa / 2;
  while (PATINTA < tintap0 && PACARRO < carrop0) {
    AvanceTinta(1);
    AvanceCarro(1);
  }
  pausa = 2 * pausa;
  while (PATINTA < tintap0) {
    AvanceTinta(1);
  }
  while (PACARRO <  carrop0) {
    AvanceCarro(1);
  }
  while (PATAMPO < tampop0) {
    AvanceTampo(1);
  }
  while (PATINTA >= tintap0) {
    RetroTinta(1);
  }
  while (PACARRO >=  carrop0) {
    RetroCarro(1);
  }
  while (PATAMPO >= tampop0) {
    RetroTampo(1);
  }

 Serial.println("En punto de impresion");

  if (automatico == 0) {
    LeerPedal();
  } else {
    delay(1000 * pause);
    }
  delay(100);

  if (ciclos == 0) {
    AvanceTinta(tintap1-tintap0);
    RetroTinta(tintap1-tintap0);
  }
  AvanceTampo(tampop2 - tampop0 - presionentintar);
  AvanceTampo(presionentintar);
  delay(tiempoentintar);
  RetroTampo(tampop2 - tampop0);
  AvanceTinta(tintap1 - tintap0);
  AvanceTampo(tampop1 - tampop0 - presionimprimir);
  AvanceTampo(presionimprimir);
  delay(tiempoimprimir);
  RetroTampo(tampop1 - tampop0);
  if (tintas > 1) {
    pausa = pausa / 2;
    RSTINTACARROACC(tintap1 - tintap0);
    ASTAMPOCARROACC((carrop1 - carrop0) - (tintap1-tintap0));
    while (PATAMPO < tampop2) {
      AvanceTampo(1);
    }
    pausa = 2 * pausa;
    AvanceTampo(presionentintar);
    delay(tiempoentintar);
    RetroTampo(tampop2);
    AvanceTinta(tintap1);
    AvanceTampo(tampop1 - presionimprimir);
    AvanceTampo(presionimprimir);
    delay(tiempoimprimir);
    RSTAMPOCARROACC(tampop1);
    while (PATINTA > tintap0) {
      RetroTinta(1);
    }
  }  else {
    RetroTinta(tintap1 - tintap0);
  }
  ciclos++;
  contador++;
  myNextion.setComponentValue("n0", contador);
  Serial.println("Ciclo de impresion terminado");
}


void LeerPedal() {
  pedal = analogRead(A3);
  while (pedal < 400) {
    pedal = analogRead(A3);
    Serial.println("Esperando pedal");
    delay(50);
  }
}

void ReiniciarTampo() {
  sensor_tampo = analogRead(A0);
  Serial.print("Sensor Tampo: ");
  Serial.println(sensor_tampo);
  while (sensor_tampo > 150) {
    sensor_tampo = analogRead(A0);
    RetroTampo(10);
    Serial.print("Sensor Tampo: ");
  Serial.println(sensor_tampo);
  }
  while (sensor_tampo < 50) {
    sensor_tampo = analogRead(A0);
    AvanceTampo(1);
    Serial.print("Sensor Tampo: ");
  Serial.println(sensor_tampo);
  }
  PATAMPO = 0;
}

void ReiniciarCarro() {
  sensor_carro = analogRead(A1);
  while (sensor_carro > 150) {
    sensor_carro = analogRead(A1);
    RetroCarro(10);
  }
  while (sensor_carro < 50) {
    sensor_carro = analogRead(A1);
    AvanceCarro(1);
  }
  PACARRO = 0;
}

void ReiniciarTinta() {
  sensor_tinta = analogRead(A2);
  while (sensor_tinta < 100) {
    sensor_tinta = analogRead(A2);
    RetroTinta(10);
  }
  while (sensor_tinta > 200) {
    sensor_tinta = analogRead(A2);
    AvanceTinta(1);
  }
  PATINTA = 0;
}

void ReinicioSincrono() {
  sensor_tampo = analogRead(A0);
  sensor_carro = analogRead(A1);
  sensor_tinta = analogRead(A2);
  pausa = pausa / 3;
  while (sensor_tampo > 500 && sensor_carro > 500 && sensor_tinta > 500) {
    for (int i = 0; i < 10; i++) {
      RetroTampo(1);
      RetroCarro(1);
      RetroTinta(1);
    }
    sensor_tampo = analogRead(A0);
    sensor_carro = analogRead(A1);
    sensor_tinta = analogRead(A2);
  }
  pausa = 3 * pausa / 2;
  while (sensor_carro > 500 && sensor_tinta > 500) {
    for (int i = 0; i < 10; i++) {
      RetroCarro(1);
      RetroTinta(1);
    }
    sensor_carro = analogRead(A1);
    sensor_tinta = analogRead(A2);
  }
  while (sensor_tampo > 500 && sensor_tinta > 500) {
    for (int i = 0; i < 10; i++) {
      RetroTampo(1);
      RetroTinta(1);
    }
    sensor_tampo = analogRead(A0);
    sensor_tinta = analogRead(A2);
  }
  while (sensor_tampo > 500 && sensor_carro > 500) {
    for (int i = 0; i < 10; i++) {
      RetroTampo(1);
      RetroCarro(1);
    }
    sensor_tampo = analogRead(A0);
    sensor_carro = analogRead(A1);
  }
  pausa = 2 * pausa;
  ReiniciarTampo();
  ReiniciarCarro();
  ReiniciarTinta();
}

void ReiniciarTodos() {
  ReiniciarTampo();
  ReiniciarCarro();
  ReiniciarTinta();
}

void AvanceTampo(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(TAMPO1,  LOW);
    digitalWrite(TAMPO2,  HIGH);
    digitalWrite(TAMPO3,  LOW);
    digitalWrite(TAMPO4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(TAMPO1,  HIGH);
    digitalWrite(TAMPO2,  LOW);
    digitalWrite(TAMPO3,  LOW);
    digitalWrite(TAMPO4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(TAMPO1,  HIGH);
    digitalWrite(TAMPO2,  LOW);
    digitalWrite(TAMPO3,  HIGH);
    digitalWrite(TAMPO4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(TAMPO1,  LOW);
    digitalWrite(TAMPO2,  HIGH);
    digitalWrite(TAMPO3,  HIGH);
    digitalWrite(TAMPO4,  LOW);
    delayMicroseconds(pausa);
  }
  PATAMPO = PATAMPO + steps;
}

void RetroTampo(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(TAMPO1,  LOW);
    digitalWrite(TAMPO2,  HIGH);
    digitalWrite(TAMPO3,  LOW);;
    digitalWrite(TAMPO4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(TAMPO1,  LOW);
    digitalWrite(TAMPO2,  HIGH);
    digitalWrite(TAMPO3,  HIGH);
    digitalWrite(TAMPO4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(TAMPO1,  HIGH);
    digitalWrite(TAMPO2,  LOW);
    digitalWrite(TAMPO3,  HIGH);
    digitalWrite(TAMPO4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(TAMPO1,  HIGH);
    digitalWrite(TAMPO2,  LOW);
    digitalWrite(TAMPO3,  LOW);
    digitalWrite(TAMPO4,  HIGH);
    delayMicroseconds(pausa);
  }
  PATAMPO = PATAMPO - steps;
}

void ASTAMPOCARROACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / (2 * velocidad);
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
    AvanceCarro(1);
  }
  pausa = p1 / (2 * velocidad);
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
    AvanceCarro(1);
  }
  pausa = p2 / (2 * velocidad);
  for (int i = 0; i < (steps * 6 / 10); i++) {
    AvanceTampo(1);
    AvanceCarro(1);
  }
  pausa = p1 / (2 * velocidad);
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
    AvanceCarro(1);
  }
  pausa = p0 / (2 * velocidad);
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
    AvanceCarro(1);
  }
  pausa = pausainicial;
}

void RSTAMPOCARROACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / (2 * velocidad);
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
    RetroTampo(1);
  }
  pausa = p1 / (2 * velocidad);
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
    RetroTampo(1);
  }
  pausa = p2 / (2 * velocidad);
  for (int i = 0; i < (steps * 8 / 10); i++) {
    RetroCarro(1);
    RetroTampo(1);
  }

  for (int i = 0; i < (PACARRO); i = i + 4) {
    RetroCarro(4);
    RetroTinta(1);
  }

  pausa = 3 * pausa;
}

void RSTINTACARROACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / (2 * velocidad);
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
    RetroTinta(1);
  }
  pausa = p1 / (2 * velocidad);
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
    RetroTinta(1);
  }
  pausa = p2 / (2 * velocidad);
  for (int i = 0; i < (steps * 6 / 10); i++) {
    AvanceCarro(1);
    RetroTinta(1);
  }
  pausa = p1 / (2 * velocidad);
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
    RetroTinta(1);
  }
  pausa = p0 / (2 * velocidad);
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
    RetroTinta(1);
  }
  pausa = pausainicial;
}

void AvanceTampoACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = p2 / velocidad;
  for (int i = 0; i < (steps * 6 / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = pausainicial;
}

void RetroTampoACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = p2 / velocidad;
  for (int i = 0; i < (steps * 6 / 10); i++) {
    RetroTampo(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = pausainicial;
}

void AvanceCarroACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = p2 / velocidad;
  for (int i = 0; i < (steps * 6 / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = pausainicial;
}

void RetroCarroACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
  }
  pausa = p2 / velocidad;
  for (int i = 0; i < (steps * 6 / 10); i++) {
    RetroCarro(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
  }
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
  }
  pausa = pausainicial;
}

void AvanceTintaACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTinta(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTinta(1);
  }
  pausa = p2 / velocidad;
  for (int i = 0; i < (steps * 6 / 10); i++) {
    AvanceTinta(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTinta(1);
  }
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTinta(1);
  }
  pausa = pausainicial;
}

void RetroTintaACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
  }
  pausa = p2 / velocidad;
  for (int i = 0; i < (steps * 6 / 10); i++) {
    RetroTinta(1);
  }
  pausa = p1 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
  }
  pausa = p0 / velocidad;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
  }
  pausa = pausainicial;
}

void AvanceCarro(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(CARRO1,  LOW);
    digitalWrite(CARRO2,  HIGH);
    digitalWrite(CARRO3,  LOW);
    digitalWrite(CARRO4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(CARRO1,  HIGH);
    digitalWrite(CARRO2,  LOW);
    digitalWrite(CARRO3,  LOW);
    digitalWrite(CARRO4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(CARRO1,  HIGH);
    digitalWrite(CARRO2,  LOW);
    digitalWrite(CARRO3,  HIGH);
    digitalWrite(CARRO4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(CARRO1,  LOW);
    digitalWrite(CARRO2,  HIGH);
    digitalWrite(CARRO3,  HIGH);
    digitalWrite(CARRO4,  LOW);
    delayMicroseconds(pausa);
  }
  PACARRO = PACARRO + steps;
}

void RetroCarro(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(CARRO1,  LOW);
    digitalWrite(CARRO2,  HIGH);
    digitalWrite(CARRO3,  LOW);;
    digitalWrite(CARRO4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(CARRO1,  LOW);
    digitalWrite(CARRO2,  HIGH);
    digitalWrite(CARRO3,  HIGH);
    digitalWrite(CARRO4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(CARRO1,  HIGH);
    digitalWrite(CARRO2,  LOW);
    digitalWrite(CARRO3,  HIGH);
    digitalWrite(CARRO4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(CARRO1,  HIGH);
    digitalWrite(CARRO2,  LOW);
    digitalWrite(CARRO3,  LOW);
    digitalWrite(CARRO4,  HIGH);
    delayMicroseconds(pausa);
  }
  PACARRO = PACARRO - steps;
}

void AvanceTinta(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(TINTA1,  LOW);
    digitalWrite(TINTA2,  HIGH);
    digitalWrite(TINTA3,  LOW);
    digitalWrite(TINTA4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(TINTA1,  HIGH);
    digitalWrite(TINTA2,  LOW);
    digitalWrite(TINTA3,  LOW);
    digitalWrite(TINTA4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(TINTA1,  HIGH);
    digitalWrite(TINTA2,  LOW);
    digitalWrite(TINTA3,  HIGH);
    digitalWrite(TINTA4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(TINTA1,  LOW);
    digitalWrite(TINTA2,  HIGH);
    digitalWrite(TINTA3,  HIGH);
    digitalWrite(TINTA4,  LOW);
    delayMicroseconds(pausa);
  }
  PATINTA = PATINTA + steps;
}

void RetroTinta(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(TINTA1,  LOW);
    digitalWrite(TINTA2,  HIGH);
    digitalWrite(TINTA3,  LOW);;
    digitalWrite(TINTA4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(TINTA1,  LOW);
    digitalWrite(TINTA2,  HIGH);
    digitalWrite(TINTA3,  HIGH);
    digitalWrite(TINTA4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(TINTA1,  HIGH);
    digitalWrite(TINTA2,  LOW);
    digitalWrite(TINTA3,  HIGH);
    digitalWrite(TINTA4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(TINTA1,  HIGH);
    digitalWrite(TINTA2,  LOW);
    digitalWrite(TINTA3,  LOW);
    digitalWrite(TINTA4,  HIGH);
    delayMicroseconds(pausa);
  }
  PATINTA = PATINTA - steps;
}


