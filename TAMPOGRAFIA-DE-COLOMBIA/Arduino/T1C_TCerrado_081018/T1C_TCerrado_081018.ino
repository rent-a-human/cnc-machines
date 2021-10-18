#include <EEPROM.h>
#include <Nextion.h>

#define CARRO1 22
#define CARRO2 24
#define CARRO3 26
#define CARRO4 28

#define TAMPO1 30
#define TAMPO2 32
#define TAMPO3 34
#define TAMPO4 36


#define SENSOR_CARRO A2
#define SENSOR_TAMPO A1
#define PEDAL A3

#define nextion Serial2

int noparar = 0;
int reiniciarcarro = 0;
int reiniciartampo = 0;
int pausaimpresion = 0;
int piAUX = 0;
int ctAUX = 0;
int ccAUX = 0;
int ccuAUX = 0;
int contador = 0;
int cont = 0;
double sincrono = 0;
int  bt4p6 = 0;
int bt4p7 = 0;
int bt0p3 = 0;
int IBAUX = 0;
int FBAUX = 0;
int ANCHOAUX = 0;
int PCAAUX = 0;

Nextion myNextion(nextion, 38400);
//Nextion myNextion(nextion, 19200);

int pausa = 100;
int pausa2 = 80;
int pausainicial = 160;
int p0 = 150;
int p1 = 70;
int p2 = 40;
int p3 = 25;
int p4 = 12;


int tampo = 1;
int pagina = 0;
int cambio = 0;
int imprimir = 0;
int automatico = 0;
int reset = 0;
int ciclos = 0;
int ciclo = 0;
int pedal = 0;
int steps = 0;
int reiniciar = 0;
int tiempoentintar = 50;
int tiempoimprimir = 50;
int tampoatras = 0;
int tampoabajo = 1;
int conf = 0;

int sensor_tampo = 0;
int sensor_carro = 0;
//Distancia max 230mm

float ALTURA_TAMPO_IMPRIMIR = 30;
float ALTURA_TAMPO_ENTINTAR = 30;
float PRESIONIMPRIMIR = 20;
float PRESIONENTINTAR = 20;
float ANCHO = 70;
int ciclos_tampo = 20;
int ciclos_carro = 25;
double APPCARRO = 0.065;
double APPTAMPO = 0.0125;
double MAX = 180 / APPCARRO;


double altura_tampo_imprimir = ALTURA_TAMPO_IMPRIMIR / APPTAMPO;
double altura_tampo_entintar = ALTURA_TAMPO_ENTINTAR / APPTAMPO;
double presionimprimir = PRESIONIMPRIMIR / APPTAMPO;
double presionentintar = PRESIONENTINTAR / APPTAMPO;
double ancho = ANCHO / APPCARRO;

long PACARRO = 2;
long PATAMPO = 10000;
int modo = 1;

int reiniciar_carro = 0;
int reiniciar_tampo = 0;

int bt2p1 = 0;
int PIAUX = 0;
int PEAUX = 0;
int ATIAUX = 0;
int ATEAUX = 0;
int PCARRAUX = 0;
int PCABAUX = 0;
int bt0p2 = 0;
int bt1p2 = 0;
int bt2p2 = 0;
int bt3p2 = 0;
int bt4p2 = 0;
int bt5p2 = 0;
int bt6p2 = 0;
int bt7p2 = 0;
int bt8p2 = 0;
int bt9p2 = 0;
int bt10p2 = 0;
int bt11p2 = 0;
int bt12p2 = 0;

int bt0p6 = 0;

void setup() {
  Serial.begin(19200);;
  Serial.println("Inicio de programa ");
  pinMode(CARRO1, OUTPUT);
  pinMode(CARRO2, OUTPUT);
  pinMode(CARRO3, OUTPUT);
  pinMode(CARRO4, OUTPUT);
  pinMode(TAMPO1, OUTPUT);
  pinMode(TAMPO2, OUTPUT);
  pinMode(TAMPO3, OUTPUT);
  pinMode(TAMPO4, OUTPUT);
  pinMode(SENSOR_CARRO, INPUT);
  pinMode(SENSOR_TAMPO, INPUT);
  pinMode(PEDAL, INPUT);
  delay(500);
  if (myNextion.init()) {
    //Serial.println(F("Nextion Ready..."));
  } else {
    //Serial.println(F("Nextion not responding..."));
  }
  delay(500);

  ALTURA_TAMPO_IMPRIMIR = EEPROM.read(1);
  ALTURA_TAMPO_ENTINTAR = EEPROM.read(2);
  PRESIONIMPRIMIR = EEPROM.read(9);
  PRESIONENTINTAR = EEPROM.read(10);
  ANCHO = EEPROM.read(11);
  ciclos_tampo = EEPROM.read(12);
  ciclos_carro = EEPROM.read(13);
  pausaimpresion = EEPROM.read(15);

  /*
    EEPROM.write(1, ALTURA_TAMPO_IMPRIMIR);
    EEPROM.write(2, ALTURA_TAMPO_ENTINTAR);
    EEPROM.write(5, POSICION_CUCHILLA_ARRIBA);
    EEPROM.write(6, POSICION_CUCHILLA_ABAJO);
    EEPROM.write(11, ANCHO);
    EEPROM.write(12, ciclos_tampo);
    EEPROM.write(13, ciclos_carro);
  */
  //myNextion.sendCommand("bauds=38400");
  //Serial.println("Parametros leidos de memoria correctamente");
  delay(100);
  myNextion.sendCommand("page 1");
  delay(500);
  //Serial.println(" FIN Inicio de programa ");
  reiniciar = 0;
}

void loop() {

  CalcularVariables();
  if (reiniciarcarro == 1) {
    ReiniciarCarro();
    reiniciarcarro = 0;
  }
  if (reiniciartampo == 1) {
    delay(50);
    ReiniciarTampo();
    reiniciartampo = 0;
  }
  //Serial.print("DISTANCIA_CARRO_IMPRIMIR: ");
  //Serial.println(distancia_carro_imprimir);
  //delay(10);
  if (reiniciar == 1) {
    ReiniciarTodos();
    reiniciar = 0;
  }
  if (imprimir < 1) {
    myNextion.setComponentText("g0", " En Linea...");
  }
  reiniciar_carro = ciclos % ciclos_carro + 1;
  reiniciar_tampo = ciclos % ciclos_tampo + 1;

  if (reiniciar_carro == 1 && (ciclos > 0) && imprimir == 1) {
    ReiniciarCarro();

  }
  if (reiniciar_tampo == 1 && (ciclos > 0) && imprimir == 1) {
    ReiniciarTampo();
  }
  if (noparar == 0) {
    delay(100);
    Configurar();
  }

  if (imprimir == 1) {
    IMPRIMIR();
  }

  //LeerSensores();
  //IMPRIMIR();
  //
  // Demo();
  //PruebaMotorCuchilla();
  //PruebaMotorTinta();
  //PruebaMotorTampo();
}

void ReiniciarTodos() {
  ReiniciarTampo();
  ReiniciarCarro();
}

void Configurar() {
  delay(50);
  pagina = myNextion.getComponentValue("bt0");
  //Serial.print("pagina :  ");
  //Serial.println(pagina);
  while (pagina <= 0) {
    delay(50);
    pagina = myNextion.getComponentValue("pagina");
    //Serial.print("pagina :  ");
    //Serial.println(pagina);
    //myNextion.setComponentText("g0","-1 err");
  }

  switch (pagina) {
    case 0:
      myNextion.sendCommand("page 1");
      delay(50);
      break;
    case 1://BOTONES DE PAG 1
      {
        delay(50);
        bt2p1 = myNextion.getComponentValue("bt2");
        while (bt2p1 < 0 && cont < 3) {
          delay(40);
          bt2p1 = myNextion.getComponentValue("bt2");
          cont++;
          ////Serial.print("contador:   ");
          ////Serial.println(contador);
          ////Serial.print("bt2p1:   ");
          ////Serial.println(bt2p1);
        }
        cont = 0;
        if (bt2p1 == 0) {
          noparar = 0;
          imprimir = 0;
          contador = 0;
          int bt7p1 = myNextion.getComponentValue("bt7");
          delay(50);
          if (bt7p1 == 0) {
            myNextion.setComponentValue("n0", ciclos);
          } else if (bt7p1 == 1) {
            ciclos = 0;
            myNextion.setComponentValue("n0", ciclos);
            myNextion.setComponentValue("bt7", 0);
          }
          delay(50);
          cambio = myNextion.getComponentValue("cambio");
          while (cambio < 0) {
            cambio = myNextion.getComponentValue("cambio");
          }
          if (cambio == 0) {
            myNextion.setComponentValue("bt3", automatico); delay(2);
            myNextion.setComponentValue("bt4", tampoatras); delay(2);
            myNextion.setComponentValue("bt8", tampoabajo); delay(2);
            myNextion.setComponentValue("bt5", modo); delay(2);
            myNextion.setComponentValue("bt6", tampo); delay(2);
            myNextion.setComponentValue("cambio", 1); delay(2);
          } else if (cambio == 1) {
            delay(50);
            tampoatras = myNextion.getComponentValue("bt4");
            if (tampoatras == 0) {
              delay(50);
              tampoatras = myNextion.getComponentValue("bt4");
              if (tampoatras == 0) {
                //Serial.println("Tampoatras en 1");
                if (ciclo == 0) {
                  ReiniciarTampo();
                  ReiniciarCarro();
                  ciclo = 1;
                }
                while (PACARRO > MAX+1) {
                  RetroCarro(1);
                }
                while (PACARRO < MAX) {
                  AvanceCarro(1);
                }
              }
            } else if (tampoatras == 1) {
              delay(50);
              tampoatras = myNextion.getComponentValue("bt4");
              if (tampoatras == 1) {
                if (ciclo == 0) {
                  ReiniciarTampo();
                  ReiniciarCarro();
                  ciclo = 1;
                }
                while (PACARRO >= 1) {
                  RetroCarro(1);
                }
                while (PACARRO < 0) {
                  AvanceCarro(1);
                }
              }
            }
            delay(50);
            tampoabajo = myNextion.getComponentValue("bt8");
            if (tampoabajo == 0) {
              delay(50);
              tampoabajo = myNextion.getComponentValue("bt8");
              if (tampoabajo == 0) {
                //Serial.println("Tampoabajo en 1");
                if (ciclo == 0) {
                  ReiniciarTampo();
                  ReiniciarCarro();
                  ciclo = 1;
                }
                if (tampoatras == 0) {
                  while (PATAMPO < altura_tampo_entintar) {
                    AvanceTampo(1);
                  }
                  while (PATAMPO > altura_tampo_entintar) {
                    RetroTampo(1);
                  }
                } else {
                  while (PATAMPO < altura_tampo_imprimir) {
                    AvanceTampo(1);
                  }
                  while (PATAMPO > altura_tampo_imprimir) {
                    RetroTampo(1);
                  }
                }
              }
            } else if (tampoabajo == 1) {
              delay(50);
              tampoabajo = myNextion.getComponentValue("bt8");
              if (tampoabajo == 1) {
                //Serial.println("Tampoabajo en 0");
                if (ciclo == 0) {
                  ReiniciarTampo();
                  ReiniciarCarro();
                  ciclo = 1;
                }
                while (PATAMPO > 0) {
                  RetroTampo(1);
                }
              }
            }
            delay(50);
            modo = myNextion.getComponentValue("bt5");
            while (modo < 0) {
              delay(50);
              modo = myNextion.getComponentValue("bt5");
            }
            delay(50);
            tampo = myNextion.getComponentValue("bt6");
            while (tampo < 0) {
              delay(50);
              tampo = myNextion.getComponentValue("bt6");
            }
            delay(50);
            automatico = myNextion.getComponentValue("bt3");
            while (automatico < 0) {
              delay(50);
              automatico = myNextion.getComponentValue("bt3");
            }
            delay(50);
          }
          delay(50);
          reset = myNextion.getComponentValue("bt0");
          if (reset == 1) {
            //Serial.println("reiniciando desde pantalla");
            myNextion.setComponentText("g0", "Reiniciando...");
            ReiniciarTampo();
            ReiniciarCarro();
            myNextion.setComponentValue("bt0", 0);
            delay(2);
            myNextion.setComponentText("g0", "Reiniciado!");
            delay(1000);
          }
          delay(50);
          conf = myNextion.getComponentValue("bt1");
          if (conf == 1) {
            myNextion.sendCommand("page 2");
            delay(50);
          }
        } else if (bt2p1 == 1) { // VAMOS A IMPRIMIR
          noparar = 1;
          imprimir = 1;
          delay(20);
          bt2p1 = myNextion.getComponentValue("bt2");
          if (bt2p1 == 1) {
            myNextion.setComponentText("g0", "Imprimiendo...");
            //Serial.println("Imprimiendo... ");
            IMPRIMIR();
            delay(20);
            myNextion.setComponentText("g0", "Impreso!");
            delay(20);
          }
        }
        break;
      }

    case 2://PAG 2 CONFIG
      {
        delay(100);
        cambio = myNextion.getComponentValue("cambio");
        delay(50);
        cambio = myNextion.getComponentValue("cambio");
        if (cambio == 0) {
          myNextion.setComponentValue("h0", 70-ALTURA_TAMPO_IMPRIMIR); delay(2);
          myNextion.setComponentValue("h1", 70-ALTURA_TAMPO_ENTINTAR); delay(2);
          myNextion.setComponentValue("n2", PRESIONIMPRIMIR); delay(2);
          myNextion.setComponentValue("n4", ALTURA_TAMPO_IMPRIMIR); delay(2);
          myNextion.setComponentValue("n5", ALTURA_TAMPO_ENTINTAR); delay(2);
          myNextion.setComponentValue("n0", ciclos_tampo); delay(2);
          myNextion.setComponentValue("n3", ANCHO); delay(2);
          delay(500);
        } else if (cambio == 1) {
          delay(100);
          PIAUX = myNextion.getComponentValue("n2"); delay(2);
          ATIAUX = myNextion.getComponentValue("n4"); delay(2);
          ATEAUX = myNextion.getComponentValue("n5"); delay(2);
          ANCHOAUX = myNextion.getComponentValue("n3"); delay(2);
          ctAUX = myNextion.getComponentValue("n0"); delay(2);
          if (ATIAUX >= 0) {
            ALTURA_TAMPO_IMPRIMIR = ATIAUX;
            EEPROM.write(1, ALTURA_TAMPO_IMPRIMIR);
          }
          if (ATEAUX >= 0) {
            ALTURA_TAMPO_ENTINTAR = ATEAUX;
            EEPROM.write(2, ALTURA_TAMPO_ENTINTAR);
          }
          if (PIAUX >= 0) {
            PRESIONIMPRIMIR = PIAUX;
            EEPROM.write(9, PRESIONIMPRIMIR);
          }
          if (ANCHOAUX >= 0) {
            ANCHO = ANCHOAUX;
            EEPROM.write(11, ANCHO);
          }
          if (ctAUX >= 0) {
            ciclos_tampo = ctAUX;
            EEPROM.write(12, ciclos_tampo);
          }
          CalcularVariables();
        }
        delay(100);
        bt1p2 = myNextion.getComponentValue("bt1");
        delay(50);
        bt1p2 = myNextion.getComponentValue("bt1");
        delay(50);
        if (bt1p2 == 1) {
          myNextion.sendCommand("page 1");
        }
        delay(50);
        bt0p2 = myNextion.getComponentValue("bt0");
        delay(50);
        bt0p2 = myNextion.getComponentValue("bt0");
        delay(50);
        if (bt0p2 == 1) {
          myNextion.sendCommand("page 3");
        }
        break;
      }

    default:
      myNextion.setComponentText("g0", "Error!");
  }
}


void CalcularVariables() {
  altura_tampo_imprimir = ALTURA_TAMPO_IMPRIMIR / APPTAMPO;
  altura_tampo_entintar = ALTURA_TAMPO_ENTINTAR / APPTAMPO;
  presionimprimir = PRESIONIMPRIMIR / APPTAMPO;
  presionentintar = presionimprimir;
  ancho = ANCHO / APPCARRO;
  //Serial.println("Variables recalculadas");
}

void LeerPedal() {
  pedal = digitalRead(PEDAL);
  while (pedal < 1) {
    pedal = digitalRead(PEDAL);
    delay(30);
    //Serial.println("Esperando pedal");
    myNextion.setComponentText("g0", "Esperando Pedal");
    delay(30);
    tampo = myNextion.getComponentValue("bt6");
    while (tampo < 0) {
      delay(20);
      tampo = myNextion.getComponentValue("bt6");
    }
    bt2p1 = myNextion.getComponentValue("bt2");
    if (bt2p1 == 0) {
      bt2p1 = myNextion.getComponentValue("bt2");
      if (bt2p1 == 0) {
        pedal = 1;
        imprimir = 0;
        noparar = 0;
      }
    }
  }
  delay(50);
  myNextion.setComponentText("g0", "Imprimiendo");
  delay(250);
}

void VerificarPedal() {
  pedal = digitalRead(PEDAL);
  if (pedal > 0) {
    pedal = digitalRead(PEDAL);
    delay(100);
    if (pedal == 1) {
      myNextion.setComponentText("g0", "Pausado por pedal");
      noparar = 0;
      tampo = myNextion.getComponentValue("bt6");
      while (tampo < 0) {
        delay(20);
        tampo = myNextion.getComponentValue("bt6");
      }
      delay(50);
      bt2p1 = myNextion.getComponentValue("bt2");
      while (bt2p1 < 0) {
        bt2p1 = myNextion.getComponentValue("bt2");
      }
      //Serial.println("Pausado por pedal");
      delay(1000);
      pausa = 100;
      reiniciarcarro = 1;
      LeerPedal();
    }
  }
}
void VerificarPedal2() {
  pedal = digitalRead(PEDAL);
  if (pedal > 0) {
    pedal = digitalRead(PEDAL);
    delay(100);
    if (pedal == 1) {
      myNextion.setComponentText("g0", "Pausado por pedal");
      noparar = 0;
      tampo = myNextion.getComponentValue("bt6");
      while (tampo < 0) {
        delay(20);
        tampo = myNextion.getComponentValue("bt6");
      }
      delay(50);
      bt2p1 = myNextion.getComponentValue("bt2");
      while (bt2p1 < 0) {
        bt2p1 = myNextion.getComponentValue("bt2");
      }
      //Serial.println("Pausado por pedal");

      delay(1000);
      pausa = 100;
      reiniciartampo = 1;
      LeerPedal();
    }
  }
}
void LeerSensores() {
  sensor_tampo = digitalRead(SENSOR_TAMPO);
  Serial.print("Sensor Tampo:  ");
  Serial.print(sensor_tampo);

  sensor_carro = digitalRead(SENSOR_CARRO);
  Serial.print("     Sensor Carro: ");
  Serial.print(sensor_carro);

  pedal = digitalRead(PEDAL);
  Serial.print("     Pedal: ");
  Serial.println(pedal);
  delay(200);
}

void Demo() {
  //ReiniciarCuchilla();
  //AvanceCarro(20 / APPCARRO);
  //CerrarCuchilla(200 / APPCUCHILLA);
  // delay(500);
  //RetroCarro(20 / APPCARRO);
  //AbrirCuchilla(200 / APPCUCHILLA);
  // delay(500);
  //ReiniciarCuchilla();
}

void ReiniciarTampo() {

  sensor_tampo = digitalRead(SENSOR_TAMPO);
  //Serial.print("Sensor Tampo: ");
  //Serial.println(sensor_tampo);
  while (sensor_tampo > 0) {
    sensor_tampo = digitalRead(SENSOR_TAMPO);
    RetroTampo(1);
  }
  delay(100);
  while (sensor_tampo < 1) {
    sensor_tampo = digitalRead(SENSOR_TAMPO);
    AvanceTampo(1);
  }
  PATAMPO = 0;
  reiniciartampo = 0;
}

void ReiniciarCarro() {
  pausainicial = pausa;
  pausa = 140;
  sensor_carro = digitalRead(SENSOR_CARRO);
  //Serial.print("Sensor Carro: ");
  //Serial.println(sensor_carro);
  while (sensor_carro > 0) {
    sensor_carro = digitalRead(SENSOR_CARRO);
    RetroCarro(5);
  }
  delay(20);
  while (sensor_carro < 1) {
    sensor_carro = digitalRead(SENSOR_CARRO);
    AvanceCarro(1);
  }
  PACARRO = 0;
  pausa = pausainicial;
  reiniciarcarro = 0;

}

void calcularP(float steps) {
  p0 = -0.04 * steps + 330;
  p1 = -0.02 * steps + 180;
  p2 = -0.015 * steps + 120;
  p3 = -0.01 * steps + 80;
  p4 = -0.007 * steps + 46;
}
void calcularP2(float steps) {//para acc tampo
  p0 = -0.03 * steps + 200;
  p1 = -0.02 * steps + 140;
  p2 = -0.01 * steps + 90;
  p3 = -0.007 * steps + 60;
  p4 = -0.0055 * steps + 50;
}

void calcularP4(float steps) {//para acc tampo largo
  p0 = -0.03 * steps + 200;
  p1 = -0.02 * steps + 140;
  p2 = -0.01 * steps + 90;
  p3 = -0.007 * steps + 60;
  p4 = -0.0055 * steps + 50;

  //p0 = -0.03 * steps + 280;
  //p1 = -0.02 * steps + 140;
  //p2 = -0.011 * steps + 90;
  //p3 = -0.009 * steps + 65;
  //p4 = -0.005 * steps + 58;
}

void calcularP3(float steps) {
  p0 = -0.04 * steps + 350;
  p1 = -0.02 * steps + 170;
  p2 = -0.011 * steps + 110;
  p3 = -0.009 * steps + 90;
  p4 = -0.005 * steps + 56;;
}

void AvanceCarroACC(float steps) {

  calcularP(steps);
  pausainicial = pausa;
  if (reiniciarcarro < 1) {
    pausa = p0 ;
  }
  for (int i = 0; i < steps / 20; i++) {
    AvanceCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p1;
  }
  for (int i = 0; i < steps / 20; i++) {
    AvanceCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p2;
  }
  for (int i = 0; i < (steps / 20); i++) {
    AvanceCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p3;
  }
  for (int i = 0; i < (steps / 20); i++) {
    AvanceCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p4;
  }
  for (int i = 0; i < ((steps * 3) / 5); i++) {
    AvanceCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p3;
  }
  for (int i = 0; i < (steps / 20); i++) {
    AvanceCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p2;
  }
  for (int i = 0; i < (steps / 20); i++) {
    AvanceCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p1;
  }
  for (int i = 0; i < steps / 20; i++) {
    AvanceCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p0;
  }
  for (int i = 0; i < steps / 20; i++) {
    AvanceCarro(1);
  }
  pausa = pausainicial;
}

void RetroCarroACC(float steps) {
  calcularP(steps);
  pausainicial = pausa;
  if (reiniciarcarro < 1) {
    pausa = p0 ;
  }
  for (int i = 0; i < steps / 20; i++) {
    RetroCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p1;
  }
  for (int i = 0; i < steps / 20; i++) {
    RetroCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p2;
  }
  for (int i = 0; i < (steps / 20); i++) {
    RetroCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p3;
  }
  for (int i = 0; i < (steps / 20); i++) {
    RetroCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p4;
  }
  for (int i = 0; i < ((steps * 3) / 5); i++) {
    RetroCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p3;
  }
  for (int i = 0; i < (steps / 20); i++) {
    RetroCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p2;
  }
  for (int i = 0; i < (steps / 20); i++) {
    RetroCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p1;
  }
  for (int i = 0; i < steps / 20; i++) {
    RetroCarro(1);
  }
  if (reiniciarcarro < 1) {
    pausa = p0;
  }
  for (int i = 0; i < steps / 20; i++) {
    RetroCarro(1);
  }
  pausa = pausainicial;
}

void RetroCarroACC2(float steps) {
  calcularP(steps);
  if (pausa > p0) {
    pausa = p0;
  }
  for (int i = 0; i < steps / 20; i++) {
    RetroCarro(1);
  }
  if (pausa > p1) {
    pausa = p1;
  }
  for (int i = 0; i < steps / 20; i++) {
    RetroCarro(1);
  }
  if (pausa > p2) {
    pausa = p2;
  }
  for (int i = 0; i < (steps / 20); i++) {
    RetroCarro(1);
  }
  if (pausa > p3) {
    pausa = p3;
  }
  for (int i = 0; i < (steps / 20); i++) {
    RetroCarro(1);
  }

  pausa = p4;
  for (int i = 0; i < ((steps * 12) / 20); i++) {
    RetroCarro(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 20); i++) {
    RetroCarro(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 20); i++) {
    RetroCarro(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 20; i++) {
    RetroCarro(1);
  }
  pausa = p0;
  for (int i = 0; i < steps / 20; i++) {
    RetroCarro(1);
  }
  //pausa = pausainicial;

}

void AvanceCarro(int steps) {
  VerificarPedal();
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
  VerificarPedal();
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
  VerificarPedal2();
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

void AvanceTampoACC(double steps) {
  //delay(20);
  //tampo = myNextion.getComponentValue("bt6");
  // delay(50);
  //    //Serial.print("Tampo bt6: ");
  //  //Serial.println(tampo);
  // tampo = myNextion.getComponentValue("bt6");

  if (tampo == 1) {
    calcularP2(steps);
    pausainicial = pausa;
    if (reiniciartampo < 1) {
      pausa = p0;
    }
    for (int i = 0; i < steps / 20; i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p1;
    }
    for (int i = 0; i < steps / 20; i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p2;
    }
    for (int i = 0; i < (steps / 20); i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p3;
    }
    for (int i = 0; i < (steps / 20); i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p4;
    }
    for (int i = 0; i < (steps * 12 / 20); i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p3;
    }
    for (int i = 0; i < (steps / 20); i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p2;
    }
    for (int i = 0; i < (steps / 20); i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p1;
    }
    for (int i = 0; i < steps / 20; i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p0;
    }
    for (int i = 0; i < steps / 20; i++) {
      AvanceTampo(1);
    }
    pausa = pausainicial;
  }
}
void AvanceTampoACC2(double steps) {
  //delay(20);
  //tampo = myNextion.getComponentValue("bt6");
  // delay(50);
  //    //Serial.print("Tampo bt6: ");
  //  //Serial.println(tampo);
  // tampo = myNextion.getComponentValue("bt6");

  if (tampo == 1) {
    calcularP4(steps);
    pausainicial = pausa;
    if (reiniciartampo < 1) {
      pausa = p0;
    }
    for (int i = 0; i < steps / 20; i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p1;
    }
    for (int i = 0; i < steps / 20; i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p2;
    }
    for (int i = 0; i < (steps / 20); i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p3;
    }
    for (int i = 0; i < (steps / 20); i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p4;
    }
    for (int i = 0; i < (steps * 12 / 20); i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p3;
    }
    for (int i = 0; i < (steps / 20); i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p2;
    }
    for (int i = 0; i < (steps / 20); i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p1;
    }
    for (int i = 0; i < steps / 20; i++) {
      AvanceTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p0;
    }
    for (int i = 0; i < steps / 20; i++) {
      AvanceTampo(1);
    }
    pausa = pausainicial;
  }
}

void RetroTampoACC(double steps) {
  // delay(20);
  // tampo = myNextion.getComponentValue("bt6");
  //    delay(50);
  //    //Serial.print("Tampo bt6: ");
  //   //Serial.println(tampo);
  //  tampo = myNextion.getComponentValue("bt6");


  if (tampo == 1) {
    calcularP2(steps);
    pausainicial = pausa;
    if (reiniciartampo < 1) {
      pausa = p0;
    }
    for (int i = 0; i < steps / 20; i++) {
      RetroTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p1;
    }
    for (int i = 0; i < steps / 20; i++) {
      RetroTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p2;
    }
    for (int i = 0; i < (steps / 20); i++) {
      RetroTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p3;
    }
    for (int i = 0; i < (steps / 20); i++) {
      RetroTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p4;
    }
    for (int i = 0; i < (steps * 12 / 20); i++) {
      RetroTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p3;
    }
    for (int i = 0; i < (steps / 20); i++) {
      RetroTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p2;
    }
    for (int i = 0; i < (steps / 20); i++) {
      RetroTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p1;
    }
    for (int i = 0; i < steps / 20; i++) {
      RetroTampo(1);
    }
    if (reiniciartampo < 1) {
      pausa = p0;
    }
    for (int i = 0; i < steps / 20; i++) {
      RetroTampo(1);
    }
    pausa = pausainicial;
  }
}


void PruebaMotorTinta() {
  pausa = 100;
  Serial.println("Avance Tinta");
  delay(200);
  AvanceCarroACC(120 / APPCARRO);
  Serial.println("Retroceder Tinta");
  delay(200);
  RetroCarroACC(120 / APPCARRO);
}

void PruebaMotorTampo() {
  pausa = 80;
  Serial.println("Avance Tinta");
  delay(200);
  AvanceTampoACC(40 / APPTAMPO);
  Serial.println("Retroceder Tinta");
  delay(200);
  RetroTampoACC(40 / APPTAMPO);
}

void IMPRIMIR() {
  if (noparar > 0) {
    bt2p1 = 1;
  }
  //Serial.println("Imprimiendo...");
  myNextion.setComponentText("g0", "Imprimiendo");
  //delay(100);
  //CalcularVariables();

  //Impresion Plana
  if (automatico == 0) {
    LeerPedal();
  }

  delay(800 * pausaimpresion);

  if (bt2p1 == 1) {
    if (contador == 0) {
      ReiniciarTampo();
      ReiniciarCarro();
      AvanceCarroACC(MAX);
      contador = 1;
    }

    while (PACARRO < 0) {
      AvanceCarro(1);
    }
    while (PACARRO > 0) {
      RetroCarro(1);
    }
    Serial.print("Posicion Carro: ");
    Serial.println(PACARRO);
    if ((altura_tampo_entintar - PATAMPO) > 0) {
      if ((altura_tampo_entintar - PATAMPO) > 200) {
        if (tampo == 1) {
          AvanceTampoACC2(altura_tampo_entintar - PATAMPO);
        }
      } else {
        delay(50);
        if (tampo == 1) {
          AvanceTampo(altura_tampo_entintar - PATAMPO);
        }
      }
    } else {
      if ((PATAMPO - altura_tampo_entintar) > 200) {
        if (tampo == 1) {
          RetroTampoACC(PATAMPO - altura_tampo_entintar);
        }
      } else {
        delay(50);
        if (tampo == 1) {
          RetroTampo(PATAMPO - altura_tampo_entintar);
        }
      }
    }
    if (tampo == 1) {
      AvanceTampoACC(presionentintar);
    }
    delay(tiempoentintar);
    if (tampo == 1) {
      RetroTampoACC(presionentintar);
    }
    VerificarPedal();
    if(modo==0){
      AvanceCarroACC(MAX-ancho);
    } else if (modo==1){
      AvanceCarroACC(MAX);
    }
    
    if ((altura_tampo_imprimir - PATAMPO) > 0) {
      if ((altura_tampo_imprimir - PATAMPO) > 800) {
        if (tampo == 1) {
          AvanceTampoACC(altura_tampo_imprimir - PATAMPO);
        }
      } else {
        pausainicial = pausa;
        pausa = pausa2;
        if (tampo == 1) {
          AvanceTampo(altura_tampo_imprimir - PATAMPO);
        }
        pausa = pausainicial;

      }
    } else {
      if ((PATAMPO - altura_tampo_imprimir) > 800) {
        if (tampo == 1) {
          RetroTampoACC(PATAMPO - altura_tampo_imprimir);
        }
      } else {
        if (tampo == 1) {
          RetroTampo(PATAMPO - altura_tampo_imprimir);
        }
      }

    }

    if (tampo == 1) {
      AvanceTampoACC(presionimprimir);
    }
    delay(tiempoimprimir);
    if (modo == 0) {//modo cilindrico
      AvanceCarroACC(ancho);
      }
    if (tampo == 1) {
      RetroTampoACC(presionimprimir);
    }
    if (tampo == 1) {
      while (PATAMPO > altura_tampo_entintar) {
        RetroTampo(1);
      }
      while (PATAMPO < altura_tampo_entintar) {
        AvanceTampo(1);
      }
    }
    VerificarPedal();
    
      RetroCarroACC(MAX);
    
    VerificarPedal();
    ReiniciarCarro();
    ciclos++;
    
    myNextion.setComponentValue("n0", ciclos);
    //Serial.println("ValorCuchilla ");
    //Serial.println(PACUCHILLA);
  } else if (bt2p1 == 0) {
    bt2p1 = myNextion.getComponentValue("bt2");
    if (bt2p1 == 0) {
      ReiniciarTampo();
      ReiniciarCarro();
      contador = 0;
    }
  }
  //Fin Impresion Plana

  //Serial.println("Ciclo de impresion terminado");
}
