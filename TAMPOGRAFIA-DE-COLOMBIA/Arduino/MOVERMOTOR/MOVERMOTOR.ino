#define MOTOR1 8
#define MOTOR2 9
#define MOTOR3 10
#define MOTOR4 11

int pausa = 140;
int p0 = 180;
int p1 = 90;
int p2 = 50;
int p3 = 36;
int p4 = 22;


double APP = 0.046;

void setup() {
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  pinMode(MOTOR3, OUTPUT);
  pinMode(MOTOR4, OUTPUT);
  
}

void loop() {
  AvanceACC(230 / APP);
  delay(500);
  RetrocederACC(230 / APP);
  delay(500);
}

void AvanceACC2(int steps) {
  for (int i = 0; i < steps / 2; i++) {
    Avance(1);
    pausa = -0.07 * i + 140.07;
  }
  for (int i = 0; i < steps / 2; i++) {
    Avance(1);
    pausa = 0.07 * i + 69.93;
  }
}

void AvanceACC(float steps) {
  pausa = p0 ;
  for (int i = 0; i < steps / 20; i++) {
    Avance(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 20; i++) {
    Avance(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 20); i++) {
    Avance(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 20); i++) {
    Avance(1);
  }
  pausa = p4;
  for (int i = 0; i < ((steps * 3)/ 5); i++) {
    Avance(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 20); i++) {
    Avance(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 20); i++) {
    Avance(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 20; i++) {
    Avance(1);
  }
  pausa = p0;
  for (int i = 0; i < steps / 20; i++) {
    Avance(1);
  }
}

void RetrocederACC(float steps) {
  pausa = p0 ;
  for (int i = 0; i < steps / 20; i++) {
    Retroceder(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 20; i++) {
    Retroceder(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 20); i++) {
    Retroceder(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 20); i++) {
    Retroceder(1);
  }
  pausa = p4;
  for (int i = 0; i < ((steps * 3) / 5); i++) {
    Retroceder(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 20); i++) {
    Retroceder(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 20); i++) {
    Retroceder(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 20; i++) {
    Retroceder(1);
  }
  pausa = p0;
  for (int i = 0; i < steps / 20; i++) {
    Retroceder(1);
  }
}

void Avance(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(MOTOR1,  LOW);
    digitalWrite(MOTOR2,  HIGH);
    digitalWrite(MOTOR3,  LOW);
    digitalWrite(MOTOR4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(MOTOR1,  HIGH);
    digitalWrite(MOTOR2,  LOW);
    digitalWrite(MOTOR3,  LOW);
    digitalWrite(MOTOR4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(MOTOR1,  HIGH);
    digitalWrite(MOTOR2,  LOW);
    digitalWrite(MOTOR3,  HIGH);
    digitalWrite(MOTOR4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(MOTOR1,  LOW);
    digitalWrite(MOTOR2,  HIGH);
    digitalWrite(MOTOR3,  HIGH);
    digitalWrite(MOTOR4,  LOW);
    delayMicroseconds(pausa);
  }
}

void Retroceder(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(MOTOR1,  LOW);
    digitalWrite(MOTOR2,  HIGH);
    digitalWrite(MOTOR3,  LOW);;
    digitalWrite(MOTOR4,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(MOTOR1,  LOW);
    digitalWrite(MOTOR2,  HIGH);
    digitalWrite(MOTOR3,  HIGH);
    digitalWrite(MOTOR4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(MOTOR1,  HIGH);
    digitalWrite(MOTOR2,  LOW);
    digitalWrite(MOTOR3,  HIGH);
    digitalWrite(MOTOR4,  LOW);
    delayMicroseconds(pausa);
    digitalWrite(MOTOR1,  HIGH);
    digitalWrite(MOTOR2,  LOW);
    digitalWrite(MOTOR3,  LOW);
    digitalWrite(MOTOR4,  HIGH);
    delayMicroseconds(pausa);
  }
}
