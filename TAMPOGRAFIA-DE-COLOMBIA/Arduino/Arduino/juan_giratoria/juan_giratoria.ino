
#define POT1 A0
#define POT2 A1
#define SENSOR A4

#define MOTORA1 8
#define MOTORA2 9
#define MOTORA3 10
#define MOTORA4 11

#define MOTORB1 13
#define MOTORB2 12

int pot1 = 0;
int pot2 = 0;

int sensor = 0;
int pausa1 = 100;
int pausa2 = 100;
int pmin1 = 10;
int pmax1 = 300;
int pmin2 = 40;
int pmax2 = 600;
int tiempo_espera = 0;
int arrancar = 0;
int contador = 0;
int sentido1 = 0;
int sentido2 = 0;
int pasos_giro = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicio de programa ");
  pinMode(POT1, INPUT);
  pinMode(POT2, INPUT);
  pinMode(SENSOR, INPUT);

  pinMode(MOTORA1, OUTPUT);
  pinMode(MOTORA2, OUTPUT);
  pinMode(MOTORA3, OUTPUT);
  pinMode(MOTORA4, OUTPUT);

  pinMode(MOTORB1, OUTPUT);
  pinMode(MOTORB2, OUTPUT);
}

void loop() {
/*
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
  while (sensor == 0 && arrancar == 1) {
    if (sentido1 == 0) {
      MoverM1(1);
    } else {
      RetroM1(1);
    }
    LeerEntradas();
  }
  while (sensor == 1 && arrancar == 1) {
    if (sentido1 == 0) {
      MoverM1(1);
    } else {
      RetroM1(1);
    }
    LeerEntradas();
  }
*/
  //ProbarEntradas();
  //PruebaMotor2();
  
  RetroM2(1);
  RetroM1(1);
  pot1 = analogRead(POT1);
  pot2 = analogRead(POT2);
  pausa1 = map(pot1, 1024, 0, pmin1, pmax1);
  pausa2 = map(pot2, 1024, 0, pmin2, pmax2);
 
  //delay(200);
}

void PruebaMotor1() {
  pausa1 = 100;
  Serial.println("horario: ");
  delay(500);
  MoverM1(10000);
  delay(1000);
  Serial.println("Antihorario:");
  delay(500);
  RetroM1(10000);
  delay(1000);
}

void PruebaMotor2() {
  pausa2 = 100;
  Serial.println("horario: ");
  delay(500);
  MoverM2(10000);
  delay(1000);
  Serial.println("Antihorario:");
  delay(500);
  RetroM2(10000);
  delay(1000);
}

void LeerEntradas() {
  pot1 = analogRead(POT1);
  pot2 = analogRead(POT2);
  sensor = digitalRead(SENSOR);
  sentido1 = 1;
  pausa1 = map(pot1, 1023, 0, pmin1, pmax1);

  sentido2 = 0;
  pausa2 = map(pot2, 1023, 0, pmin2, pmax2);

}

void ProbarEntradas() {
  pot1 = analogRead(POT1);
  pot2 = analogRead(POT2);
  Serial.print("Pot1:   ");
  Serial.print(pot1);
  Serial.print("   Pot2:   ");
  Serial.println(pot2);
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
  //MoverM2(pot2 / 50 + 2);
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
  //MoverM2(pot2 / 50 + 2);
}

void MoverM2(int steps) {
  digitalWrite(MOTORB1,  LOW);
  for (int i = 0; i < steps; i++) {
    digitalWrite(MOTORB2,  HIGH);
    delayMicroseconds(pausa2);
    digitalWrite(MOTORB2,  LOW);
    delayMicroseconds(pausa2);
  }
}

void RetroM2(int steps) {
    digitalWrite(MOTORB1,  HIGH);
    for (int i = 0; i < steps; i++) {
      digitalWrite(MOTORB2,  LOW);
      delayMicroseconds(pausa2);
      digitalWrite(MOTORB2,  HIGH);
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
