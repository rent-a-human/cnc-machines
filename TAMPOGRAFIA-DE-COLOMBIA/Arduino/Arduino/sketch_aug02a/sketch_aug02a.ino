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

int ciclo = 0;
int reiniciartampo = 0;
int reiniciartinta = 0;
int mezclar = 0;
int tampoatras = 0;
int bt6p1 = 0;
int bt5p1 = 0;
int bt4p1 = 0;
int bt5p2 = 0;
int bt4p2 = 0;
int bt5p3 = 0;
int bt4p3 = 0;
int modo = 0;
int conf = 0;
int pagina = 0;
int cambio = 0;
int CARROP0AUX = 0;
int CARROP1AUX = 0;
int PRESIONIMPRIMIR1AUX = 0;
int PRESIONIMPRIMIR2AUX = 0;
int PRESIONENTINTARAUX = 0;
int ANCHO1AUX = 0;
int ANCHO2AUX = 0;
int TAMPOP0AUX = 0;
int TAMPOP1AUX = 0;
int probarp1 = 0;
int probarp2 = 0;
int probarp3 = 0;
int probarp4 = 0;
int probarp5 = 0;
int probarp6 = 0;
int velocidad = 90;
int reiniciar = 0;
int reiniciarcarro = 0;
int tiempoentintar = 100;
int tiempoimprimir = 150;
int tintas = 1;
int p0 = 12000;
int p1 = 7000;
int p2 = 5000;
int p3 = 4000;
int p4 = 3500;
float pausa = 82;
float pausatampo = 150;
int pause = 0;
int pauseAUX = 0;
int pausamax = 164;
int pausainicial = 164;
int contador = 0;
int imprimir = 0;
int automatico = 0;
int reset = 0;
int ciclos = 0;

float TAMPOP0 = 30;
float TAMPOP1 = 60;
float TAMPOP2 = 50;
float CARROP0 = 20;
float CARROP1 = 140;
float TINTAP0 = 0;
float TINTAP1 = 130;
float PRESIONIMPRIMIR1 = 20;
float PRESIONIMPRIMIR2 = 20;
float PRESIONENTINTAR = 20;
float ANCHO = 70;
float ANCHO1 = 25;
float ANCHO2 = 25;
double APP1 = 0.025;

int ReinicioCiclos = 0;
int ReinicioCiclosAUX = 0;

double presionimprimir1 = PRESIONIMPRIMIR1 / APP1;
double presionimprimir2 = PRESIONIMPRIMIR2 / APP1;
double presionentintar = PRESIONENTINTAR / APP1;
double tampop0 = TAMPOP0 / APP1; //UNIDADES EN PASOS
double tampop1 = TAMPOP1 / APP1;
double tampop2 = TAMPOP2 / APP1;
double carrop0 = CARROP0 / APP1;
double carrop1 = CARROP1 / APP1;
double tintap0 = TINTAP0 / APP1;
double tintap1 = TINTAP1 / APP1;
double ancho = ANCHO / APP1;
double ancho1 = ANCHO1 / APP1;
double ancho2 = ANCHO2 / APP1;

int pedal = 0;
int steps = 0;

int sensor_tampo = 0;
int sensor_carro = 0;
int sensor_tinta = 0;

long PATAMPO = 0; //Paso Actual TAMPO
long PACARRO = 0;
long PATINTA = 0;

int ciclos_reinicio = 10;
int ciclos_reinicioAUX = 0;

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
  myNextion.sendCommand("page 1");
  delay(500);
  /*myNextion.sendCommand("page1.n0.val=12");
    delay(600);
    modo=myNextion.getComponentValue("n0");
    Serial.println(modo);
    EEPROM.write(0, velocidad);  */


  Serial.println("Iniciando");
  delay(1000);

  velocidad = 95;
  TAMPOP0 = EEPROM.read(1);
  TAMPOP1 = EEPROM.read(2);
  TAMPOP2 = EEPROM.read(3);
  CARROP0 = EEPROM.read(4);
  CARROP1 = EEPROM.read(5);
  //TINTAP0 = EEPROM.read(6);
  //TINTAP1 = EEPROM.read(7);
  PRESIONIMPRIMIR1 = EEPROM.read(8);
  PRESIONENTINTAR = EEPROM.read(9);
  PRESIONIMPRIMIR2 = EEPROM.read(10);
  ciclos_reinicio = EEPROM.read(11);
  ReinicioCiclos = EEPROM.read(12);
  ANCHO1 = EEPROM.read(13);
  ANCHO2 = EEPROM.read(14);
  pause = EEPROM.read(15);

  Serial.println("Parametros leidos de memoria correctamente");
  CalcularVariables();

  Serial.print(" FIN Inicio de programa ");
  reiniciar = 1;
}

void loop() {
  delay(10);
  pausa = 75;
  myNextion.setComponentText("g0", "En Linea...");
  if (reiniciar = !1) {
    reiniciar = ciclos % ciclos_reinicio + 1;
  }
  if (reiniciarcarro = !1) {
    reiniciarcarro = ciclos % ReinicioCiclos + 1;
  }

  if (reiniciarcarro == 1) {
    ReiniciarCarro();
    reiniciarcarro = 0;
  }
  if (reiniciartampo == 1) {
    ReiniciarTampo();
    reiniciartampo = 0;
  }
  if (reiniciartinta == 1) {
    ReiniciarTinta();
    reiniciartinta = 0;
  }
  //LeerPedal();
  //IMPRIMIR();
  Configurar();

}


void Configurar() {
  delay(50);
  pagina = myNextion.getComponentValue("pagina");
  Serial.print("pagina :  ");
  Serial.println(pagina);
  while (pagina < 0) {
    pagina = myNextion.getComponentValue("pagina");
    Serial.print("pagina :  ");
    Serial.println(pagina);
    //myNextion.setComponentText("g0","-1 err");
  }

  // Serial.print("Pagina: ");
  //Serial.println(va0);
  switch (pagina) {
    case 1:
      myNextion.sendCommand("page 2");
      break;
    case 2://BOTONES DE PAG 2
      {
        delay(50);
        bt5p1 = myNextion.getComponentValue("bt5");
        while (bt5p1 < 0) {
          bt5p1 = myNextion.getComponentValue("bt5");
        }
        if (bt5p1 == 0) {
          bt6p1 = myNextion.getComponentValue("bt6");
          while (bt6p1 < 0) {
            bt6p1 = myNextion.getComponentValue("bt6");
          }
          delay(50);
          if (bt6p1 == 0) {
            myNextion.setComponentValue("n0", contador);
          } else if (bt6p1 == 1) {
            contador = 0;
            ciclos = 0;
            myNextion.setComponentValue("n0", contador);
            myNextion.setComponentValue("bt6", 0);
          }
          delay(50);
          cambio = myNextion.getComponentValue("cambio");
          while (cambio < 0) {
            cambio = myNextion.getComponentValue("cambio");
          }
          if (cambio == 0) {
            myNextion.setComponentValue("bt7", tintas - 1); delay(2);
            myNextion.setComponentValue("bt0", automatico); delay(2);
            myNextion.setComponentValue("bt1", tampoatras); delay(2);
            myNextion.setComponentValue("bt2", modo); delay(2);
            myNextion.setComponentValue("cambio", 1); delay(2);
          } else if (cambio == 1) {
            tintas = myNextion.getComponentValue("bt7");
            while (tintas < 0) {
              tintas = myNextion.getComponentValue("bt7");
            }
            if (tintas >= 0) {
              tintas = tintas + 1;
            }
            delay(50);
            tampoatras = myNextion.getComponentValue("bt1");
            if (tampoatras == 1) {
              if (ciclo == 0) {
                ReiniciarTampo();
                ReiniciarTinta();
                ciclo = 1;
              }
              while (PATINTA > tintap0) {
                RetroTinta(1);
              }
            } else if (tampoatras == 0) {
              if (ciclo == 0) {
                ReiniciarTampo();
                ReiniciarTinta();
                ciclo = 1;
              }
              while (PATINTA < tintap1) {
                AvanceTinta(1);
              }
            }
            delay(50);
            modo = myNextion.getComponentValue("bt2");
            while (modo < 0) {
              modo = myNextion.getComponentValue("bt2");
            }
            delay(50);
            automatico = myNextion.getComponentValue("bt0");
            while (automatico < 0) {
              automatico = myNextion.getComponentValue("bt0");
            }
            delay(50);
          }
          delay(50);
          reset = myNextion.getComponentValue("bt3");
          if (reset == 1) {
            Serial.println("reiniciando desde pantalla");
            myNextion.setComponentText("g0", "Reiniciando...");
            ReiniciarTodos();
            myNextion.setComponentValue("bt3", 0);
            delay(2);
            myNextion.setComponentText("g0", "Reiniciado!");
            delay(1000);
            reiniciar = 1;
          }
          delay(50);
          mezclar = myNextion.getComponentValue("bt8");
          while (mezclar < 0) {
            mezclar = myNextion.getComponentValue("bt8");
          }
          if (mezclar == 1) {
            if (ciclo == 0) {
              ReiniciarTampo();
              ReiniciarTinta();

              ciclo = 1;
            }
            delay(500);
            if (tampoatras == 1) {
              for (int i = 0; i < 4; i++) {
                AvanceTintaACC2(ancho);
                delay(2 * tiempoimprimir);
                RetroTintaACC2(ancho);
                delay(2 * tiempoimprimir);
              }
            }
            else {
              for (int i = 0; i < 4; i++) {
                RetroTintaACC2(ancho);
                delay(2 * tiempoimprimir);
                AvanceTintaACC2(ancho);
                delay(2 * tiempoimprimir);
              }
            }
            delay(1000);
            ReiniciarTinta();
            myNextion.setComponentValue("bt8", 0);
          }
          delay(50);
          conf = myNextion.getComponentValue("bt4");
          if (conf == 1) {
            myNextion.sendCommand("page 2");
          }
        } else if (bt5p1 == 1) { // VAMOS A IMPRIMIR
          imprimir = 1;
          myNextion.setComponentText("g0", "Imprimiendo...");
          Serial.println("Imprimiendo... ");
          tintas = myNextion.getComponentValue("bt7");
          while (tintas < 0) {
            tintas = myNextion.getComponentValue("bt7");
          }
          if (tintas >= 0) {
            tintas = tintas + 1;
          }
          IMPRIMIR();
          delay(20);
          myNextion.setComponentText("g0", "Impreso!");
          delay(20);
        }
        break;
      }

    case 3://Pag 3 CONFIG
      {
        delay(100);
        Serial.print("pagina :  ");
        Serial.println(pagina);
        cambio = myNextion.getComponentValue("cambio");
        while (cambio < 0) {
          cambio = myNextion.getComponentValue("cambio");
        }
        if (cambio == 0) {
          myNextion.setComponentValue("h0", CARROP0); delay(2);
          myNextion.setComponentValue("n0", CARROP0); delay(2);
          myNextion.setComponentValue("h1", CARROP1); delay(2);
          myNextion.setComponentValue("n1", CARROP1); delay(2);
          myNextion.setComponentValue("n2", PRESIONENTINTAR); delay(2);
          myNextion.setComponentValue("h2", PRESIONENTINTAR); delay(2);
          myNextion.setComponentValue("n3", PRESIONIMPRIMIR1); delay(2);
          myNextion.setComponentValue("n4", PRESIONIMPRIMIR2); delay(2);
          delay(500);
        } else if (cambio == 1) {
          delay(100);
          CARROP0AUX = myNextion.getComponentValue("n0"); delay(2);
          CARROP1AUX = myNextion.getComponentValue("n1"); delay(2);
          PRESIONIMPRIMIR1AUX = myNextion.getComponentValue("n3"); delay(2);
          PRESIONIMPRIMIR2AUX = myNextion.getComponentValue("n4"); delay(2);
          PRESIONENTINTARAUX = myNextion.getComponentValue("n2"); delay(2);
          if (CARROP0AUX >= 0) {
            CARROP0 = CARROP0AUX;
            EEPROM.write(4, CARROP0);
          }
          if (CARROP1AUX > 0) {
            CARROP1 = CARROP1AUX;
            EEPROM.write(5, CARROP1);
          }
          if (PRESIONIMPRIMIR1AUX > 0) {
            PRESIONIMPRIMIR1 = PRESIONIMPRIMIR1AUX;
            EEPROM.write(8, PRESIONIMPRIMIR1);
          }
          if (PRESIONIMPRIMIR2AUX > 0) {
            PRESIONIMPRIMIR2 = PRESIONIMPRIMIR2AUX;
            EEPROM.write(10, PRESIONIMPRIMIR2);
          }
          if (PRESIONENTINTARAUX > 0) {
            PRESIONENTINTAR = PRESIONENTINTARAUX;
            EEPROM.write(9, PRESIONENTINTAR);
          }
          CalcularVariables();
        }
        delay(100);
        probarp1 = myNextion.getComponentValue("bt0");
        if (probarp1 == 1) {
          Serial.println("Imprimiendo p1");
          CalcularVariables();
          ReiniciarCarro();
          AvanceCarroACC2(carrop0);
          myNextion.setComponentValue("bt0", 0);
          Serial.println("Imprimir p1 ok");
        }
        delay(100);
        probarp2 = myNextion.getComponentValue("bt1");
        if (probarp2 == 1) {
          Serial.println("Imprimiendo p2");
          CalcularVariables();
          ReiniciarCarro();
          AvanceCarroACC(carrop1);
          myNextion.setComponentValue("bt1", 0);
          Serial.println("Imprimir p2 ok");
        }
        delay(100);
        probarp3 = myNextion.getComponentValue("bt3");
        if (probarp3 == 1) {
          Serial.println("Imprimiendo p3");
          CalcularVariables();
          ReiniciarCarro();
          ReiniciarTampo();
          AvanceCarroACC(carrop0);
          while (PATINTA < tintap1) {
            AvanceTinta(1);
          }
          AvanceTampoACC(tampop0);
          AvanceTampo(presionimprimir1);
          delay(2000);
          RetroTampo(presionimprimir1);
          reiniciar = 1;
          myNextion.setComponentValue("bt3", 0);
          Serial.println("Imprimir p3 ok");
        }
        delay(100);
        probarp4 = myNextion.getComponentValue("bt2");
        if (probarp4 == 1) {
          Serial.println("Imprimiendo p4");
          CalcularVariables();
          ReiniciarTampo();
          ReiniciarTinta();
          AvanceTampoACC(tampop1);
          pausainicial = pausa;
          pausa = pausatampo;
          AvanceTampo(presionentintar);
          delay(2000);

          RetroTampo(presionentintar);
          pausa = pausainicial;
          reiniciar = 1;
          myNextion.setComponentValue("bt2", 0);
          Serial.println("Imprimir p4 ok");
        }
        delay(100);
        probarp5 = myNextion.getComponentValue("bt6");
        if (probarp5 == 1) {
          Serial.println("Imprimiendo p5");
          CalcularVariables();
          ReiniciarCarro();
          ReiniciarTampo();
          AvanceCarroACC(carrop1);
          while (PATINTA < tintap1) {
            AvanceTinta(1);
          }
          AvanceTampoACC(tampop0);
          AvanceTampo(presionimprimir2);
          delay(2000);
          RetroTampo(presionimprimir2);
          reiniciar = 1;
          myNextion.setComponentValue("bt6", 0);
          Serial.println("Imprimir p3 ok");
        }
        delay(100);
        bt5p2 = myNextion.getComponentValue("bt5");
        if (bt5p2 == 1) {
          ReiniciarTampo();
          myNextion.sendCommand("page 1");

        }
        delay(100);
        bt4p2 = myNextion.getComponentValue("bt4");
        if (bt4p2 == 1) {
          myNextion.sendCommand("page 3");
        }
        break;
      }
    case 4://PAG 4 CONFIG
      {
        delay(100);
        cambio = myNextion.getComponentValue("cambio");
        if (cambio == 0) {
          myNextion.setComponentValue("n0", TAMPOP0); delay(2);
          myNextion.setComponentValue("n4", TAMPOP1); delay(2);
          myNextion.setComponentValue("n3", ANCHO1); delay(2);
          myNextion.setComponentValue("n5", ANCHO2); delay(2);
          myNextion.setComponentValue("n1", ciclos_reinicio); delay(2);
          myNextion.setComponentValue("n2", ReinicioCiclos); delay(2);
          myNextion.setComponentValue("n6", pause); delay(2);
          delay(500);
        } else if (cambio == 1) {
          delay(100);
          TAMPOP0AUX = myNextion.getComponentValue("n0"); delay(2);
          TAMPOP1AUX = myNextion.getComponentValue("n4"); delay(2);
          ANCHO1AUX = myNextion.getComponentValue("n3"); delay(2);
          ANCHO2AUX = myNextion.getComponentValue("n5"); delay(2);
          ciclos_reinicioAUX = myNextion.getComponentValue("n1"); delay(2);
          ReinicioCiclosAUX = myNextion.getComponentValue("n2"); delay(2);
          pauseAUX = myNextion.getComponentValue("n6"); delay(2);
          if (TAMPOP0AUX >= 0) {
            TAMPOP0 = TAMPOP0AUX;
            EEPROM.write(1, TAMPOP0);
          }
          if (TAMPOP1AUX >= 0) {
            TAMPOP1 = TAMPOP1AUX;
            EEPROM.write(2, TAMPOP1);
          }
          if (ANCHO1AUX > 0) {
            ANCHO1 = ANCHO1AUX;
            EEPROM.write(13, ANCHO1);
          }
          if (ANCHO2AUX > 0) {
            ANCHO2 = ANCHO2AUX;
            EEPROM.write(14, ANCHO2);
          }
          if (ciclos_reinicioAUX > 0) {
            ciclos_reinicio = ciclos_reinicioAUX;
            EEPROM.write(11, ciclos_reinicio);
          }
          if (ReinicioCiclosAUX > 0) {
            ReinicioCiclos = ReinicioCiclosAUX;
            EEPROM.write(12, ReinicioCiclos);
          }
          if (pauseAUX >= 0) {
            pause = pauseAUX;
            EEPROM.write(15, pause);
          }
          CalcularVariables();
        }
        delay(100);
        probarp5 = myNextion.getComponentValue("bt1");
        if (probarp5 == 1) {
          Serial.println("Imprimiendo p5");
          CalcularVariables();
          ReiniciarTampo();
          ReiniciarCarro();
          while (PATINTA < tintap1) {
            AvanceTinta(1);
          }
          AvanceCarroACC2(carrop0);
          AvanceTampoACC(tampop0);
          delay(1000);
          AvanceTampo(presionimprimir1);
          delay(10 * tiempoimprimir);
          RetroTampo(presionimprimir1);
          reiniciar = 1;
          myNextion.setComponentValue("bt1", 0);
          Serial.println("Imprimir p5 ok");
        }
        delay(100);
        probarp6 = myNextion.getComponentValue("bt0");
        if (probarp6 == 1) {
          Serial.println("Imprimiendo p6");
          CalcularVariables();
          ReiniciarTampo();
          ReiniciarTinta();
          AvanceTampoACC(tampop1);
          delay(1000);
          AvanceTampo(presionentintar);
          delay(10 * tiempoentintar);
          RetroTampo(presionentintar);
          reiniciar = 1;
          myNextion.setComponentValue("bt0", 0);
          Serial.println("Imprimir p6 ok");
        }
        delay(100);
        bt5p3 = myNextion.getComponentValue("bt5");
        if (bt5p3 == 1) {
          myNextion.sendCommand("page 2");

        }
        delay(100);
        bt4p3 = myNextion.getComponentValue("bt4");
        if (bt4p3 == 1) {
          ReiniciarTampo();
          myNextion.sendCommand("page 1");

        }
        break;
      }

    default:
      myNextion.setComponentText("g0", "Error!");
  }
}

void AvanceCarro(int steps) {
  VerificarPedal1();
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
  VerificarPedal3();
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

void CalcularVariables() {
  p0 = 12000 / velocidad;
  p1 = 7000 / velocidad;
  p2 = 4600 / velocidad;
  p3 = 4200 / velocidad;
  p4 = 3800 / velocidad;
  pausainicial = pausa;
  presionimprimir1 = PRESIONIMPRIMIR1 / APP1;
  presionimprimir2 = PRESIONIMPRIMIR2 / APP1;
  presionentintar = PRESIONENTINTAR / APP1;
  tampop0 = TAMPOP0 / APP1; //UNIDADES EN PASOS
  tampop1 = TAMPOP1 / APP1;
  carrop0 = CARROP0 / APP1;
  carrop1 = CARROP1 / APP1;
  tintap0 = TINTAP0 / APP1;
  tintap1 = TINTAP1 / APP1;
  ancho = ANCHO / APP1;
  ancho1 = ANCHO1 / APP1;
  ancho2 = ANCHO2 / APP1;
  Serial.println("Variables recalculadas");
}


void LeerPedal() {
  pedal = analogRead(A3);
  while (pedal < 400) {
    pedal = analogRead(A3);
    Serial.println("Esperando pedal");
    myNextion.setComponentText("g0", "Esperando Pedal");
    delay(50);
    bt5p1 = myNextion.getComponentValue("bt5");
    if (bt5p1 == 0) {
      pedal = 500;
    }
  }
}

void VerificarPedal1() {
  pedal = analogRead(A3);
  if (pedal > 400) {
    myNextion.setComponentText("g0", "Pausado por pedal");
    Serial.println("Pausado por pedal");
    delay(1000);
    reiniciarcarro = 1;
    LeerPedal();

  }
}

void VerificarPedal2() {
  pedal = analogRead(A3);
  if (pedal > 400) {
    myNextion.setComponentText("g0", "Pausado por pedal");
    Serial.println("Pausado por pedal");
    delay(1000);
    reiniciartampo = 1;
    LeerPedal();

  }
}

void VerificarPedal3() {
  pedal = analogRead(A3);
  if (pedal > 400) {
    myNextion.setComponentText("g0", "Pausado por pedal");
    Serial.println("Pausado por pedal");
    delay(1000);
    reiniciartinta = 1;
    LeerPedal();

  }
}

void ReiniciarTampo() {
  sensor_tampo = analogRead(A0);
  Serial.print("Sensor Tampo: ");
  Serial.println(sensor_tampo);
  while (sensor_tampo > 150) {
    sensor_tampo = analogRead(A0);
    RetroTampo(10);
  }
  delay(50);
  while (sensor_tampo < 50) {
    sensor_tampo = analogRead(A0);
    AvanceTampo(1);
  }
  PATAMPO = 0;
}

void ReiniciarCarro() {
  sensor_carro = analogRead(A1);
  while (sensor_carro > 150) {
    sensor_carro = analogRead(A1);
    RetroCarro(10);
  }
  delay(50);
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
  delay(50);
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
  VerificarPedal2();
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


void RSTINTACARROACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / (3.5);
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
    RetroTinta(1);
  }
  pausa = p1 / (6);
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
    RetroTinta(1);
  }
  pausa = p2 / (13);
  for (int i = 0; i < (steps / 10); i++) {
    RetroCarro(1);
    RetroTinta(1);
  }
  pausa = p3 / (40);
  for (int i = 0; i < (steps  / 10); i++) {
    RetroCarro(1);
    RetroTinta(1);
  }
  pausa = p4 / (80);
  for (int i = 0; i < (steps * 2  / 10); i++) {
    RetroCarro(1);
    RetroTinta(1);
  }
  pausa = p3 / (40);
  for (int i = 0; i < (steps  / 10); i++) {
    RetroCarro(1);
    RetroTinta(1);
  }
  pausa = p2 / (13);
  for (int i = 0; i < (steps / 10); i++) {
    RetroCarro(1);
    RetroTinta(1);
  }
  pausa = p1 / (6);
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
    RetroTinta(1);
  }
  pausa = p0 / (3.5);
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
    RetroTinta(1);
  }
  pausa = pausainicial;
}

void AvanceTampoACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = p1 / 1.1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = p2 / 1.2;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p3 / 1.3;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p4 / 1.4;
  for (int i = 0; i < (steps * 2 / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p3 / 1.3;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p2 / 1.2;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p1 / 1.1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = pausainicial;
}

void RetroTampoACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = p1 / 1.1;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = p2 / 1.2;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTampo(1);
  }
  pausa = p3 / 1.3;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTampo(1);
  }
  pausa = p4 / 1.4;
  for (int i = 0; i < (steps * 2 / 10); i++) {
    RetroTampo(1);
  }
  pausa = p3 / 1.3;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTampo(1);
  }
  pausa = p2 / 1.2;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTampo(1);
  }
  pausa = p1 / 1.1;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = pausainicial;
}

void AvanceCarroACC(double steps) {

  pausainicial = pausa;
  pausa = p0 / 1.1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = p1 / 1.3;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = p2 / 1.5;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p3 / 1.7;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p4 / 1.9;
  for (int i = 0; i < (steps * 2 / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p3 / 1.7;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p2 / 1.5;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p1 / 1.3;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = p0 / 1.1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = pausainicial;
}

void AvanceCarroACC2(double steps) {
  pausainicial = pausa;
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = p1 / 1.2;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = p2 / 1.4;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p3 / 1.6;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p4 / 1.7;
  for (int i = 0; i < (steps * 2 / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p3 / 1.6;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p2 / 1.4;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceCarro(1);
  }
  pausa = p1 / 1.2;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceCarro(1);
  }
  pausa = pausainicial;
}

void RetroCarroACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
  }
  pausa = p1 / 1.2;
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
  }
  pausa = p2 / 1.4;
  for (int i = 0; i < (steps / 10); i++) {
    RetroCarro(1);
  }
  pausa = p3 / 1.6;
  for (int i = 0; i < (steps / 10); i++) {
    RetroCarro(1);
  }
  pausa = p4 / 1.8;
  for (int i = 0; i < (steps * 2 / 10); i++) {
    RetroCarro(1);
  }
  pausa = p3 / 1.6;
  for (int i = 0; i < (steps / 10); i++) {
    RetroCarro(1);
  }
  pausa = p2 / 1.4;
  for (int i = 0; i < (steps / 10); i++) {
    RetroCarro(1);
  }
  pausa = p1 / 1.2;
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
  }
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    RetroCarro(1);
  }
  pausa = pausainicial;
}

void AvanceTintaACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / 1.;
  for (int i = 0; i < steps / 20; i++) {
    AvanceTinta(1);
  }
  pausa = p1 / 1;
  for (int i = 0; i < steps / 20; i++) {
    AvanceTinta(1);
  }
  pausa = p2 / 1.2;
  for (int i = 0; i < (steps / 20); i++) {
    AvanceTinta(1);
  }
  pausa = p3 / 1.4;
  for (int i = 0; i < (steps  / 20); i++) {
    AvanceTinta(1);
  }
  pausa = p4 / 1.6;
  for (int i = 0; i < (steps * 12 / 20); i++) {
    AvanceTinta(1);
  }
  pausa = p3 / 1.4;
  for (int i = 0; i < (steps  / 20); i++) {
    AvanceTinta(1);
  }
  pausa = p2 / 1.2;
  for (int i = 0; i < (steps / 20); i++) {
    AvanceTinta(1);
  }

  pausa = p1 / 1;
  for (int i = 0; i < steps / 20; i++) {
    AvanceTinta(1);
  }
  pausa = p0 / 1;
  for (int i = 0; i < steps / 20; i++) {
    AvanceTinta(1);
  }
  pausa = pausainicial;
}

void AvanceTintaACC2(double steps) {
  pausainicial = pausa;
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTinta(1);
  }
  pausa = p1 / 1.2;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTinta(1);
  }
  pausa = p2 / 1.3;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceTinta(1);
  }
  pausa = p3 / 1.4;
  for (int i = 0; i < (steps  / 10); i++) {
    AvanceTinta(1);
  }
  pausa = p4 / 1.5;
  for (int i = 0; i < (steps * 2 / 10); i++) {
    AvanceTinta(1);
  }
  pausa = p3 / 1.4;
  for (int i = 0; i < (steps  / 10); i++) {
    AvanceTinta(1);
  }
  pausa = p2 / 1.3;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceTinta(1);
  }

  pausa = p1 / 1.2;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTinta(1);
  }
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTinta(1);
  }
  pausa = pausainicial;
}

void RetroTintaCarroACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / 2;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
    RetroCarro(1);
  }
  pausa = p1 / 2.3;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
    RetroCarro(1);
  }
  pausa = p2 / 2.7;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTinta(1);
    RetroCarro(1);
  }
  pausa = p3 / 3.1;
  for (int i = 0; i < (steps  / 10); i++) {
    RetroTinta(1);
    RetroCarro(1);
  }
  pausa = p4 / 3.5;
  for (int i = 0; i < (steps * 2 / 10); i++) {
    RetroTinta(1);
    RetroCarro(1);
  }
  pausa = p3 / 3.1;
  for (int i = 0; i < (steps  / 10); i++) {
    RetroTinta(1);
    RetroCarro(1);
  }
  pausa = p2 / 2.7;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTinta(1);
    RetroCarro(1);
  }

  pausa = p1 / 2.3;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
    RetroCarro(1);
  }
  pausa = p0 / 2;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
    RetroCarro(1);
  }
  pausa = pausainicial;
}

void RetroTintaACC(double steps) {
  pausainicial = pausa;
  pausa = p0 / 1.;
  for (int i = 0; i < steps / 20; i++) {
    RetroTinta(1);
  }
  pausa = p1 / 1;
  for (int i = 0; i < steps / 20; i++) {
    RetroTinta(1);
  }
  pausa = p2 / 1.2;
  for (int i = 0; i < (steps / 20); i++) {
    RetroTinta(1);
  }
  pausa = p3 / 1.4;
  for (int i = 0; i < (steps  / 20); i++) {
    RetroTinta(1);
  }
  pausa = p4 / 1.6;
  for (int i = 0; i < (steps * 12 / 20); i++) {
    RetroTinta(1);
  }
  pausa = p3 / 1.4;
  for (int i = 0; i < (steps  / 20); i++) {
    RetroTinta(1);
  }
  pausa = p2 / 1.2;
  for (int i = 0; i < (steps / 20); i++) {
    RetroTinta(1);
  }

  pausa = p1 / 1;
  for (int i = 0; i < steps / 20; i++) {
    RetroTinta(1);
  }
  pausa = p0 / 1;
  for (int i = 0; i < steps / 20; i++) {
    RetroTinta(1);
  }
  pausa = pausainicial;
}

void RetroTintaACC2(double steps) {
  pausainicial = pausa;
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
  }
  pausa = p1 / 1.2;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
  }
  pausa = p2 / 1.4;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTinta(1);
  }
  pausa = p3 / 1.5;
  for (int i = 0; i < (steps  / 10); i++) {
    RetroTinta(1);
  }
  pausa = p4 / 1.6;
  for (int i = 0; i < (steps * 2 / 10); i++) {
    RetroTinta(1);
  }
  pausa = p3 / 1.5;
  for (int i = 0; i < (steps  / 10); i++) {
    RetroTinta(1);
  }
  pausa = p2 / 1.4;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTinta(1);
  }

  pausa = p1 / 1.2;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
  }
  pausa = p0 / 1;
  for (int i = 0; i < steps / 10; i++) {
    RetroTinta(1);
  }
  pausa = pausainicial;
}



void IMPRIMIR() {
  Serial.println("Imprimiendo...");
  CalcularVariables();
  reiniciar = ciclos % ciclos_reinicio + 1;
  if (reiniciar == 1 || ciclos == 0) {
    ReiniciarTodos();
    Serial.println("Reiniciado");
    reiniciar = 0;
  }
  reiniciarcarro = ciclos % ReinicioCiclos + 1;
  if (reiniciarcarro == 1) {
    ReiniciarCarro();
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
  while (PATAMPO < tampop1) {
    AvanceTampo(1);
  }
  while (PATINTA >= tintap0) {
    RetroTinta(1);
  }
  while (PACARRO >=  carrop0) {
    RetroCarro(1);
  }
  while (PATAMPO >= tampop1) {
    RetroTampo(1);
  }

  Serial.println("En punto de impresion");
  if (modo == 0) {
    if (automatico == 1) {
      LeerPedal();
    }
    myNextion.setComponentText("g0", "Imprimiendo");
    bt5p1 = myNextion.getComponentValue("bt5");
    while (bt5p1 < 0) {
      bt5p1 = myNextion.getComponentValue("bt5");
    }
    if (bt5p1 == 1) {

      if (ciclos == 0) {
        ReiniciarTampo();
        AvanceTintaACC(tintap1 - tintap0);
        delay(500);
        RetroTintaACC(tintap1 - tintap0);
      }
      while (PATAMPO < tampop1) {
        AvanceTampo(1);
      }
      pausainicial = pausa;

      AvanceTampoACC(presionentintar);
      delay(tiempoentintar);
      pausa = pausatampo;
      RetroTampoACC(presionentintar);
      delay(tiempoentintar);
      pausa = pausainicial;
      AvanceTintaACC(tintap1 - tintap0);

      if ((tampop0 - tampop1) > 0) {
        if ((tampop0 - tampop1) > 800) {
          AvanceTampoACC(tampop0 - tampop1);
        }
        else {
          AvanceTampo(tampop0 - tampop1);
        }
      } else {
        if ((tampop1 - tampop0) > 800) {
          RetroTampoACC(tampop1 - tampop0);
        } else {
          RetroTampo(tampop1 - tampop0);
        }

      }
      AvanceTampoACC(presionimprimir1);
      delay(tiempoimprimir);
      RetroTampoACC(presionimprimir1);

      if (tintas > 1) {
        delay(1000 * pause);
        AvanceCarroACC(carrop1 - carrop0);

        AvanceTampoACC(presionimprimir2);
        delay(tiempoimprimir);
        RetroTampoACC(presionimprimir2);

        if ((tampop0 - tampop1) > 0) {
          if ((tampop0 - tampop1) > 800) {
            RetroTampoACC(tampop0 - tampop1);
          } else {
            RetroTampo(tampop0 - tampop1);
          }
        } else {
          if ((tampop1 - tampop0) > 800) {
            AvanceTampoACC(tampop1 - tampop0);
          } else {
            AvanceTampo(tampop1 - tampop0);
          }
        }
        delay(tiempoimprimir);
        RSTINTACARROACC(tintap1 - tintap0);
        RetroCarroACC(PACARRO - carrop0);
      }  else {
        if ((tampop0 - tampop1) > 0) {
          RetroTampoACC(tampop0 - tampop1);
        } else {
          AvanceTampo(tampop1 - tampop0);
        }
        delay(tiempoimprimir);
        RetroTintaACC(tintap1 - tintap0);
        while (PACARRO > carrop0) {
          RetroCarro(1);
        }
      }
      ciclos++;
      contador++;
      myNextion.setComponentValue("n0", contador);
      Serial.println("Ciclo de impresion terminado");
    } else {
      ReiniciarTodos();
      reiniciar = 1;
    }

  } else if (modo == 1) { //Impresion cilindrica
    if (automatico == 1) {
      LeerPedal();
    }
    myNextion.setComponentText("g0", "Imprimiendo");
    delay(100);
    bt5p1 = myNextion.getComponentValue("bt5");
    while (bt5p1 < 0) {
      bt5p1 = myNextion.getComponentValue("bt5");
    }
    if (bt5p1 == 1) {

      if (ciclos == 0) {
        ReiniciarTampo();
        delay(tiempoimprimir);
        AvanceTintaACC(tintap1 - tintap0);
        delay(500);
        RetroTinta(tintap1 - tintap0);
      }
      while (PATAMPO < tampop1) {
        AvanceTampo(1);
      }
      pausainicial = pausa;

      AvanceTampo(presionentintar);
      delay(tiempoentintar);
      pausa = pausatampo;
      RetroTampo(presionentintar);

      delay(tiempoentintar);
      pausa = pausainicial;
      AvanceTintaACC(tintap1 - tintap0);

      if ((tampop0 - tampop1) > 0) {
        if ((tampop0 - tampop1) > 800) {
          AvanceTampoACC(tampop0 - tampop1);
        }
        else {
          AvanceTampo(tampop0 - tampop1);
        }
      } else {
        if ((tampop1 - tampop0) > 800) {
          RetroTampoACC(tampop1 - tampop0);
        } else {
          RetroTampo(tampop1 - tampop0);
        }

      }

      AvanceTampo(presionimprimir1);
      AvanceCarroACC2(ancho1);
      RetroTampo(presionimprimir1);

      if (tintas > 1) {
        delay(1000 * pause);
        AvanceCarroACC(carrop1 - carrop0 - ancho);

        AvanceTampo(presionimprimir2);
        delay(tiempoimprimir);

        AvanceCarroACC2(ancho2);

        RetroTampo(presionimprimir2);

        if ((tampop0 - tampop1) > 0) {
          RetroTampoACC(tampop0 - tampop1);
        } else {
          AvanceTampoACC(tampop1 - tampop0);
        }
        delay(tiempoimprimir);

        RSTINTACARROACC(tintap1 - tintap0);

        RetroCarroACC(PACARRO - carrop0);
      }  else {

        if ((tampop0 - tampop1) > 0) {
          if ((tampop0 - tampop1) > 800) {
            RetroTampoACC(tampop0 - tampop1);
          } else {
            RetroTampo(tampop0 - tampop1);
          }
        } else {
          if ((tampop1 - tampop0) > 800) {
            AvanceTampoACC(tampop1 - tampop0);
          } else {
            AvanceTampo(tampop1 - tampop0);
          }
        }
        delay(tiempoimprimir);

        RetroTintaCarroACC(ancho1);

        RetroTintaACC(tintap1 - tintap0 - ancho1);
        ReiniciarTinta();
      }

      ciclos++;
      contador++;
      myNextion.setComponentValue("n0", contador);
      Serial.println("Ciclo de impresion terminado");
    } else {
      ReiniciarTodos();
      reiniciar = 1;
    }
  }
}

