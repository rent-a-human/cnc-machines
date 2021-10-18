
#define POT1 A0
#define POT2 A1
#define POT3 A2
#define PULSADOR A3
#define SENSOR A4
int i = 0;

#define MOTORA1 8
#define MOTORA2 9
#define MOTORA3 10
#define MOTORA4 11

#define MOTORB1 4
#define MOTORB2 5
#define MOTORB3 6
#define MOTORB4 7

int pot1 = 0;
int pot2 = 0;
int pot3 = 0;
int pulsador = 0;
int sensor = 0;
int pausa1 = 0;
int pausa2 = 0;
int pmin1 = 40;
int pmax1 = 2000;
int pmin2 = 100;
int pmax2 = 2000;
int tiempo_espera = 0;
int arrancar = 0;
int contador = 0;
int sentido1 = 0;
int sentido2 = 0;
int pasos_giro = 0;
int pasos=0;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicio de programa ");
  pinMode(POT1, INPUT);
  pinMode(POT2, INPUT);
  pinMode(POT3, INPUT);
  pinMode(PULSADOR, INPUT);
  pinMode(SENSOR, INPUT);

  pinMode(MOTORA1, OUTPUT);
  pinMode(MOTORA2, OUTPUT);
  pinMode(MOTORA3, OUTPUT);
  pinMode(MOTORA4, OUTPUT);

  pinMode(MOTORB1, OUTPUT);
  pinMode(MOTORB2, OUTPUT);
  pinMode(MOTORB3, OUTPUT);
  pinMode(MOTORB4, OUTPUT);
}

void loop() {
  LeerEntradas();
  while (contador < pasos_giro && arrancar == 1) {
    if (sentido2 == 0) {
      MoverM2(1);
    } else {
      RetroM2(1);
    }
    LeerEntradas();
    contador++;
  }
  contador = 0;
  while ((pulsador == 0 || pasos<60) && arrancar == 1) {
    if (sentido1 == 0) {
      MoverM1(1);
    } else {
      RetroM1(1);
    }
    LeerEntradas();
    pasos++;
  }
  pasos=0;
  while (pulsador == 1 && arrancar == 1) {
    if (sentido1 == 0) {
      MoverM1(1);
    } else {
      RetroM1(1);
    }
    LeerEntradas();
  }

  //ProbarEntradas();
  //delay(200);
}

void LeerEntradas() {
  pot1 = analogRead(POT1);
  pot2 = analogRead(POT2);
  pot3 = analogRead(POT3);
  pulsador = digitalRead(PULSADOR);
  sensor = digitalRead(SENSOR);

  sentido1 = 1;
  pausa1 = map(pot1, 1023, 0, pmin1, pmax1);

  sentido2 = 0;
  pausa2 = map(pot2, 1023, 0, pmin2, pmax2);

  pasos_giro = 5 * pot3;
  if (pulsador == 1) {
    arrancar = 1;
  }
}


void ProbarEntradas() {
  pot1 = analogRead(POT1);
  pot2 = analogRead(POT2);
  pot3 = analogRead(POT3);
  pulsador = digitalRead(PULSADOR);
  sensor = digitalRead(SENSOR);
  Serial.print("Pot1:   ");
  Serial.print(pot1);
  Serial.print("   Pot2:   ");
  Serial.print(pot2);
  Serial.print("   Pot3:   ");
  Serial.print(pot3);
  Serial.print("   Pulsador:   ");
  Serial.print(pulsador);
  Serial.print("   Sensor:   ");
  Serial.println(sensor);
}

void MoverM1(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(MOTORA1,  LOW);
    digitalWrite(MOTORA2,  HIGH);
    digitalWrite(MOTORA3,  LOW);
    digitalWrite(MOTORA4,  HIGH);
    delayMicroseconds(pausa1);
    digitalWrite(MOTORA1,  HIGH);
    digitalWrite(MOTORA2,  LOW);
    digitalWrite(MOTORA3,  LOW);
    digitalWrite(MOTORA4,  HIGH);
    delayMicroseconds(pausa1);
    digitalWrite(MOTORA1,  HIGH);
    digitalWrite(MOTORA2,  LOW);
    digitalWrite(MOTORA3,  HIGH);
    digitalWrite(MOTORA4,  LOW);
    delayMicroseconds(pausa1);
    digitalWrite(MOTORA1,  LOW);
    digitalWrite(MOTORA2,  HIGH);
    digitalWrite(MOTORA3,  HIGH);
    digitalWrite(MOTORA4,  LOW);
    delayMicroseconds(pausa1);
  }
  MoverM2(pot2 / 50 + 2);
}

void RetroM1(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(MOTORA1,  LOW);
    digitalWrite(MOTORA2,  HIGH);
    digitalWrite(MOTORA3,  LOW);
    digitalWrite(MOTORA4,  HIGH);
    delayMicroseconds(pausa1);
    digitalWrite(MOTORA1,  LOW);
    digitalWrite(MOTORA2,  HIGH);
    digitalWrite(MOTORA3,  HIGH);
    digitalWrite(MOTORA4,  LOW);
    delayMicroseconds(pausa1);
    digitalWrite(MOTORA1,  HIGH);
    digitalWrite(MOTORA2,  LOW);
    digitalWrite(MOTORA3,  HIGH);
    digitalWrite(MOTORA4,  LOW);
    delayMicroseconds(pausa1);
    digitalWrite(MOTORA1,  HIGH);
    digitalWrite(MOTORA2,  LOW);
    digitalWrite(MOTORA3,  LOW);
    digitalWrite(MOTORA4,  HIGH);
    delayMicroseconds(pausa1);
  }
  MoverM2(pot2 / 50 + 2);
}

void MoverM2(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(MOTORB1,  LOW);
    digitalWrite(MOTORB2,  HIGH);
    digitalWrite(MOTORB3,  LOW);
    digitalWrite(MOTORB4,  HIGH);
    delayMicroseconds(pausa2);
    digitalWrite(MOTORB1,  HIGH);
    digitalWrite(MOTORB2,  LOW);
    digitalWrite(MOTORB3,  LOW);
    digitalWrite(MOTORB4,  HIGH);
    delayMicroseconds(pausa2);
    digitalWrite(MOTORB1,  HIGH);
    digitalWrite(MOTORB2,  LOW);
    digitalWrite(MOTORB3,  HIGH);
    digitalWrite(MOTORB4,  LOW);
    delayMicroseconds(pausa2);
    digitalWrite(MOTORB1,  LOW);
    digitalWrite(MOTORB2,  HIGH);
    digitalWrite(MOTORB3,  HIGH);
    digitalWrite(MOTORB4,  LOW);
    delayMicroseconds(pausa2);
  }
}

void RetroM2(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(MOTORB1,  LOW);
    digitalWrite(MOTORB2,  HIGH);
    digitalWrite(MOTORB3,  LOW);
    digitalWrite(MOTORB4,  HIGH);
    delayMicroseconds(pausa2);
    digitalWrite(MOTORB1,  LOW);
    digitalWrite(MOTORB2,  HIGH);
    digitalWrite(MOTORB3,  HIGH);
    digitalWrite(MOTORB4,  LOW);
    delayMicroseconds(pausa2);
    digitalWrite(MOTORB1,  HIGH);
    digitalWrite(MOTORB2,  LOW);
    digitalWrite(MOTORB3,  HIGH);
    digitalWrite(MOTORB4,  LOW);
    delayMicroseconds(pausa2);
    digitalWrite(MOTORB1,  HIGH);
    digitalWrite(MOTORB2,  LOW);
    digitalWrite(MOTORB3,  LOW);
    digitalWrite(MOTORB4,  HIGH);
    delayMicroseconds(pausa2);
  }
}

void RetroMOTORA(int steps) {//Giro inverso
  for (int i = 0; i < steps; i++) {
    digitalWrite(MOTORA1,  LOW);
    digitalWrite(MOTORA2,  HIGH);
    digitalWrite(MOTORA3,  LOW);;
    digitalWrite(MOTORA4,  HIGH);
    delayMicroseconds(pausa1);
    digitalWrite(MOTORA1,  LOW);
    digitalWrite(MOTORA2,  HIGH);
    digitalWrite(MOTORA3,  HIGH);
    digitalWrite(MOTORA4,  LOW);
    delayMicroseconds(pausa1);
    digitalWrite(MOTORA1,  HIGH);
    digitalWrite(MOTORA2,  LOW);
    digitalWrite(MOTORA3,  HIGH);
    digitalWrite(MOTORA4,  LOW);
    delayMicroseconds(pausa1);
    digitalWrite(MOTORA1,  HIGH);
    digitalWrite(MOTORA2,  LOW);
    digitalWrite(MOTORA3,  LOW);
    digitalWrite(MOTORA4,  HIGH);
    delayMicroseconds(pausa1);
  }
}
