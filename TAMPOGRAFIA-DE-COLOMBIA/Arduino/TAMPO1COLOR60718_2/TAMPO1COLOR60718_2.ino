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

#define CUCHILLA1 50
#define CUCHILLA2 52

#define SENSOR_CARRO A0
#define SENSOR_CUCHILLA A2
#define SENSOR_TAMPO A1
#define PEDAL A3

#define nextion Serial3
Nextion myNextion(nextion, 19200);
//Nextion myNextion(nextion, 19200);

int pausa = 300;
int pausainicial = 160;
int p0 = 180;
int p1 = 90;
int p2 = 50;
int p3 = 36;
int p4 = 22;

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
int tiempoentintar = 100;
int tiempoimprimir = 100;
int tampoatras = 0;
int conf = 0;

int sensor_tampo = 0;
int sensor_carro = 0;
int sensor_cuchilla = 0;
//Distancia max 230mm

float ALTURA_TAMPO_IMPRIMIR = 30;
float ALTURA_TAMPO_ENTINTAR = 30;
float DISTANCIA_CARRO_IMPRIMIR = 60;
float DISTANCIA_CARRO_ENTINTAR = 160;
float POSICION_CUCHILLA_ARRIBA = 1;//De 0 a 100, 0 lo mas arriba
//, 100 lo mas abajo, máximo m vueltas
float POSICION_CUCHILLA_ABAJO = 50;
float INICIO_BARRIDO = 20;
float FIN_BARRIDO = 90;
float PRESIONIMPRIMIR = 20;
float PRESIONENTINTAR = 20;
float ANCHO = 70;
int ciclos_tampo = 20;
int ciclos_carro = 25;
int ciclos_cuchilla = 30;
double APPCARRO = 0.046;
double APPTAMPO = 0.025;
double APPCUCHILLA = 0.2;
double MAX = 230 / APPCARRO;

double altura_tampo_imprimir = ALTURA_TAMPO_IMPRIMIR / APPTAMPO;
double altura_tampo_entintar = ALTURA_TAMPO_ENTINTAR / APPTAMPO;
double distancia_carro_imprimir = DISTANCIA_CARRO_IMPRIMIR / APPCARRO;
double distancia_carro_entintar = DISTANCIA_CARRO_ENTINTAR / APPCARRO;
double posicion_cuchilla_arriba = POSICION_CUCHILLA_ARRIBA / APPCUCHILLA;
double posicion_cuchilla_abajo = POSICION_CUCHILLA_ABAJO / APPCUCHILLA;
double inicio_barrido = INICIO_BARRIDO / APPCARRO;
double fin_barrido = FIN_BARRIDO / APPCARRO;
double presionimprimir = PRESIONIMPRIMIR / APPTAMPO;
double presionentintar = PRESIONENTINTAR / APPTAMPO;
double ancho = ANCHO / APPCARRO;

long PACARRO = 0;
long PATAMPO = 0;
long PACUCHILLA = 0;
int modo = 0;

int reiniciar_carro = 0;
int reiniciar_cuchilla = 0;
int reiniciar_tampo = 0;

int bt2p1 = 0;
int PIAUX = 0;
int PEAUX = 0;
int ATIAUX = 0;
int ATEAUX = 0;
int DCIAUX = 0;
int DCEAUX = 0;
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
  pinMode(CUCHILLA1, OUTPUT);
  pinMode(CUCHILLA2, OUTPUT);
  pinMode(SENSOR_CARRO, INPUT);
  pinMode(SENSOR_CUCHILLA, INPUT);
  pinMode(SENSOR_TAMPO, INPUT);
  pinMode(PEDAL, INPUT);
  delay(500);
  if (myNextion.init()) {
    Serial.println(F("Nextion Ready..."));
  } else {
    Serial.println(F("Nextion not responding..."));
  }
  delay(500);
  /*ALTURA_TAMPO_IMPRIMIR = EEPROM.read(1);
    ALTURA_TAMPO_ENTINTAR = EEPROM.read(2);
    DISTANCIA_CARRO_IMPRIMIR = EEPROM.read(3);
    DISTANCIA_CARRO_ENTINTAR = EEPROM.read(4);
    POSICION_CUCHILLA_ARRIBA = EEPROM.read(5);
    POSICION_CUCHILLA_ABAJO = EEPROM.read(6);
    INICIO_BARRIDO = EEPROM.read(7);
    FIN_BARRIDO = EEPROM.read(8);
    PRESIONIMPRIMIR = EEPROM.read(9);
    PRESIONENTINTAR = EEPROM.read(10);
    ANCHO = EEPROM.read(11);
    ciclos_tampo = EEPROM.read(12);
    ciclos_carro = EEPROM.read(13);
    ciclos_cuchilla = EEPROM.read(14);
  */
  EEPROM.write(1, ALTURA_TAMPO_IMPRIMIR);
  EEPROM.write(2, ALTURA_TAMPO_ENTINTAR);
  EEPROM.write(3, DISTANCIA_CARRO_IMPRIMIR);
  EEPROM.write(4, DISTANCIA_CARRO_ENTINTAR);
  EEPROM.write(5, POSICION_CUCHILLA_ARRIBA);
  EEPROM.write(6, POSICION_CUCHILLA_ABAJO);
  EEPROM.write(7, INICIO_BARRIDO);
  EEPROM.write(8, FIN_BARRIDO);
  EEPROM.write(9, PRESIONIMPRIMIR);
  EEPROM.write(10, PRESIONENTINTAR);
  EEPROM.write(11, ANCHO);
  EEPROM.write(12, ciclos_tampo);
  EEPROM.write(13, ciclos_carro);
  EEPROM.write(14, ciclos_cuchilla);
  //myNextion.sendCommand("bauds=19200");
  Serial.println("Parametros leidos de memoria correctamente");
delay(50);
  myNextion.sendCommand("page 2");
  delay(500);
  Serial.println(" FIN Inicio de programa ");
  reiniciar = 1;
}

void loop() {
  delay(100);
  myNextion.setComponentText("g0", "En Linea...");
  /*if ((reiniciar_carro = !1) && (ciclos = !0)) {
    reiniciar_carro = ciclos % ciclos_carro + 1;
  }
  if ((reiniciar_tampo = !1) && (ciclos = !0)) {
    reiniciar_tampo = ciclos % ciclos_tampo + 1;
  }
  if ((reiniciar_cuchilla = !1) && (ciclos = !0)) {
    reiniciar_cuchilla = ciclos % ciclos_cuchilla + 1;
  }
  if (reiniciar_carro == 1) {
    ReiniciarCarro();
  }
  if (reiniciar_tampo == 1) {
    ReiniciarTampo();
  }
  if (reiniciar_cuchilla == 1) {
    ReiniciarCarro();
    ReiniciarCuchilla();
  }*/

  //LeerSensores();
  //IMPRIMIR();
  Configurar();
  //Demo();
}

void Configurar() {
  delay(50);
  pagina = myNextion.getComponentValue("pagina");
  Serial.print("pagina :  ");
  Serial.println(pagina);
  while (pagina < 0) {
    delay(50);
    pagina = myNextion.getComponentValue("pagina");
    Serial.print("pagina :  ");
    Serial.println(pagina);
    //myNextion.setComponentText("g0","-1 err");
  }

  switch (pagina) {
    case 0:
      myNextion.sendCommand("page 1");
      break;
    case 1://BOTONES DE PAG 1
      {
        delay(50);
        bt2p1 = myNextion.getComponentValue("bt2");
        while (bt2p1 < 0) {
          bt2p1 = myNextion.getComponentValue("bt2");
        }
        if (bt2p1 == 0) {
          int bt7p1 = myNextion.getComponentValue("bt7");
          while (bt7p1 < 0) {
            bt7p1 = myNextion.getComponentValue("bt7");
          }
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
            myNextion.setComponentValue("bt5", modo); delay(2);
            myNextion.setComponentValue("bt6", tampo); delay(2);
            myNextion.setComponentValue("cambio", 1); delay(2);
          } else if (cambio == 1) {
            tampoatras = myNextion.getComponentValue("bt4");
            if (tampoatras == 1) {
              if (ciclo == 0) {
                ReiniciarTampo();
                ReiniciarCarro();
                ciclo = 1;
              }
              while (PACARRO > 0) {
                RetroCarro(1);
              }
            } else if (tampoatras == 0) {
              if (ciclo == 0) {
                ReiniciarTampo();
                ReiniciarCarro();
                ciclo = 1;
              }
              while (PACARRO < MAX) {
                AvanceCarro(1);
              }
            }
            delay(50);
            modo = myNextion.getComponentValue("bt5");
            while (modo < 0) {
              modo = myNextion.getComponentValue("bt5");
            }
            delay(50);
            tampo = myNextion.getComponentValue("bt6");
            while (tampo < 0) {
              tampo = myNextion.getComponentValue("bt6");
            }
            delay(50);
            automatico = myNextion.getComponentValue("bt3");
            while (automatico < 0) {
              automatico = myNextion.getComponentValue("bt3");
            }
            delay(50);
          }
          delay(50);
          reset = myNextion.getComponentValue("bt0");
          if (reset == 1) {
            Serial.println("reiniciando desde pantalla");
            myNextion.setComponentText("g0", "Reiniciando...");
            ReiniciarTampo();
            ReiniciarCarro();
            ReiniciarCuchilla();
            myNextion.setComponentValue("bt0", 0);
            delay(2);
            myNextion.setComponentText("g0", "Reiniciado!");
            delay(1000);
          }
          delay(50);
          conf = myNextion.getComponentValue("bt1");
          if (conf == 1) {
            myNextion.sendCommand("page 2");
          }
        } else if (bt2p1 == 1) { // VAMOS A IMPRIMIR
          imprimir = 1;
          myNextion.setComponentText("g0", "Imprimiendo...");
          Serial.println("Imprimiendo... ");
          IMPRIMIR();
          delay(20);
          myNextion.setComponentText("g0", "Impreso!");
          delay(20);
        }
        break;
      }

    case 2://Pag 2 CONFIG
      {
        delay(100);
        Serial.print("pagina :  ");
        Serial.println(pagina);
        delay(50);
        bt0p2 = myNextion.getComponentValue("bt0");
        if (bt0p2 == 1) {
          myNextion.sendCommand("page 3");
        }
        delay(50);
        bt1p2 = myNextion.getComponentValue("bt1");
        if (bt1p2 == 1) {
          myNextion.sendCommand("page 4");
        }
        delay(50);
        bt2p2 = myNextion.getComponentValue("bt2");
        if (bt2p2 == 1) {
          myNextion.sendCommand("page 5");
        }
        delay(50);
        bt3p2 = myNextion.getComponentValue("bt3");
        if (bt3p2 == 1) {
          myNextion.sendCommand("page 6");
        }
        delay(50);
        bt4p2 = myNextion.getComponentValue("bt4");
        if (bt4p2 == 1) {
          myNextion.sendCommand("page 7");
        }
        delay(50);
        bt5p2 = myNextion.getComponentValue("bt5");
        if (bt5p2 == 1) {
          myNextion.sendCommand("page 8");
        }
        delay(50);
        bt6p2 = myNextion.getComponentValue("bt6");
        if (bt6p2 == 1) {
          myNextion.sendCommand("page 9");
        }
        delay(50);
        bt7p2 = myNextion.getComponentValue("bt7");
        if (bt7p2 == 1) {
          myNextion.sendCommand("page 3");
        }
        delay(50);
        bt8p2 = myNextion.getComponentValue("bt8");
        if (bt8p2 == 1) {
          myNextion.sendCommand("page 6");
        }
        delay(50);
        bt9p2 = myNextion.getComponentValue("bt9");
        if (bt9p2 == 1) {
          myNextion.sendCommand("page 7");
        }
        delay(50);
        bt10p2 = myNextion.getComponentValue("bt10");
        if (bt10p2 == 1) {
          myNextion.sendCommand("page 8");
        }
        delay(50);
        bt11p2 = myNextion.getComponentValue("bt11");
        if (bt11p2 == 1) {
          myNextion.sendCommand("page 9");
        }
        delay(50);
        bt12p2 = myNextion.getComponentValue("bt12");
        if (bt12p2 == 1) {
          myNextion.sendCommand("page 1");
        }
        break;
      }
    case 3://PAG 3 CONFIG ALTURAS
      {
        delay(100);
        cambio = myNextion.getComponentValue("cambio");
        if (cambio == 0) {
          myNextion.setComponentValue("n0", ALTURA_TAMPO_IMPRIMIR); delay(2);
          myNextion.setComponentValue("h0", 100 - ALTURA_TAMPO_IMPRIMIR); delay(2);
          myNextion.setComponentValue("n1", ALTURA_TAMPO_ENTINTAR); delay(2);
          myNextion.setComponentValue("h1", 100 - ALTURA_TAMPO_ENTINTAR); delay(2);
          delay(500);
        } else if (cambio == 1) {
          delay(100);
          ATIAUX = myNextion.getComponentValue("n0"); delay(2);
          ATEAUX = myNextion.getComponentValue("n4"); delay(2);
          if (ATIAUX >= 0) {
            ALTURA_TAMPO_IMPRIMIR = ATIAUX;
            EEPROM.write(1, ALTURA_TAMPO_IMPRIMIR);
          }
          if (ATEAUX >= 0) {
            ALTURA_TAMPO_ENTINTAR = ATEAUX;
            EEPROM.write(2, ALTURA_TAMPO_ENTINTAR);
          }
          CalcularVariables();
        }
        delay(100);
        int bt0p3 = myNextion.getComponentValue("bt0");
        if (bt0p3 == 1) {
          Serial.println("Imprimiendo p5");
          CalcularVariables();
          ReiniciarTampo();
          ReiniciarCarro();
          ReiniciarCuchilla();
          AvanceCarroACC(MAX - distancia_carro_imprimir);
          delay(1000);
          AvanceTampo(altura_tampo_imprimir);
          delay(2000);
          RetroTampo(altura_tampo_imprimir);
          myNextion.setComponentValue("bt0", 0);
        }
        delay(100);
        int bt1p3 = myNextion.getComponentValue("bt1");
        if (bt1p3 == 1) {
          Serial.println("Imprimiendo p5");
          CalcularVariables();
          ReiniciarTampo();
          ReiniciarCarro();
          ReiniciarCuchilla();
          AvanceCarroACC(MAX - distancia_carro_entintar);
          delay(1000);
          AvanceTampo(altura_tampo_entintar);
          delay(2000);
          RetroTampo(altura_tampo_entintar);
          myNextion.setComponentValue("bt1", 0);
        }
        delay(100);
        int  bt2p3 = myNextion.getComponentValue("bt2");
        if (bt2p3 == 1) {
          myNextion.sendCommand("page 2");
        }
        delay(100);
        int bt3p3 = myNextion.getComponentValue("bt3");
        if (bt3p3 == 1) {
          ReiniciarTampo();
          myNextion.sendCommand("page 4");
        }
        delay(100);
        int bt4p6 = myNextion.getComponentValue("bt4");
        if (bt4p6 == 1) {
          myNextion.sendCommand("page 1");
        }
        break;
      }

    case 4://PAG 4 CONFIG PRESIONES
      {
        delay(100);
        cambio = myNextion.getComponentValue("cambio");
        if (cambio == 0) {
          myNextion.setComponentValue("n0", PRESIONIMPRIMIR); delay(2);
          myNextion.setComponentValue("h0", 40 - PRESIONIMPRIMIR); delay(2);
          myNextion.setComponentValue("n1", PRESIONENTINTAR); delay(2);
          myNextion.setComponentValue("h1", 40 - PRESIONENTINTAR); delay(2);
          delay(500);
        } else if (cambio == 1) {
          delay(100);
          PIAUX = myNextion.getComponentValue("n0"); delay(2);
          PEAUX = myNextion.getComponentValue("n1"); delay(2);
          if (PIAUX >= 0) {
            PRESIONIMPRIMIR = PIAUX;
            EEPROM.write(9, PRESIONIMPRIMIR);
          }
          if (PEAUX >= 0) {
            PRESIONENTINTAR = PEAUX;
            EEPROM.write(10, PRESIONENTINTAR);
          }
          CalcularVariables();
        }
        delay(100);
        int  bt0p4 = myNextion.getComponentValue("bt0");
        if (bt0p4 == 1) {
          Serial.println("Imprimiendo p5");
          CalcularVariables();
          ReiniciarTampo();
          ReiniciarCarro();
          ReiniciarCuchilla();
          AvanceCarroACC(MAX - distancia_carro_imprimir);
          delay(1000);
          AvanceTampo(altura_tampo_imprimir);
          delay(1000);
          AvanceTampo(presionimprimir);
          delay(2000);
          RetroTampo(presionimprimir);
          myNextion.setComponentValue("bt0", 0);
        }
        delay(100);
        int  bt1p4 = myNextion.getComponentValue("bt1");
        if (bt1p4 == 1) {
          Serial.println("Imprimiendo p5");
          CalcularVariables();
          ReiniciarTampo();
          ReiniciarCarro();
          ReiniciarCuchilla();
          AvanceCarroACC(MAX - distancia_carro_entintar);
          delay(1000);
          AvanceTampo(altura_tampo_entintar);
          delay(1000);
          AvanceTampo(presionentintar);
          delay(2000);
          RetroTampo(presionentintar);
          myNextion.setComponentValue("bt1", 0);
        }
        delay(100);
        int bt2p3 = myNextion.getComponentValue("bt2");
        if (bt2p3 == 1) {
          myNextion.sendCommand("page 3");
        }
        delay(100);
        int bt3p3 = myNextion.getComponentValue("bt3");
        if (bt3p3 == 1) {
          ReiniciarTampo();
          myNextion.sendCommand("page 5");
        }
        delay(100);
        int bt4p6 = myNextion.getComponentValue("bt4");
        if (bt4p6 == 1) {
          myNextion.sendCommand("page 1");
        }
        break;
      }
    case 5://PAG 4 CONFIG posiciones tampo
      {
        delay(100);
        cambio = myNextion.getComponentValue("cambio");
        if (cambio == 0) {
          myNextion.setComponentValue("n0", DISTANCIA_CARRO_IMPRIMIR); delay(2);
          myNextion.setComponentValue("h0", 100 - DISTANCIA_CARRO_IMPRIMIR); delay(2);
          myNextion.setComponentValue("n1", DISTANCIA_CARRO_ENTINTAR); delay(2);
          myNextion.setComponentValue("h1", 230 - DISTANCIA_CARRO_ENTINTAR); delay(2);
          delay(500);
        } else if (cambio == 1) {
          delay(100);
          DCIAUX = myNextion.getComponentValue("n0"); delay(2);
          DCEAUX = myNextion.getComponentValue("n1"); delay(2);
          if (DCIAUX >= 0) {
            DISTANCIA_CARRO_IMPRIMIR = DCIAUX;
            EEPROM.write(3, DISTANCIA_CARRO_IMPRIMIR);
          }
          if (DCEAUX >= 0) {
            DISTANCIA_CARRO_ENTINTAR = DCEAUX;
            EEPROM.write(4, DISTANCIA_CARRO_ENTINTAR);
          }
          CalcularVariables();
        }
        delay(100);
        int bt0p5 = myNextion.getComponentValue("bt0");
        if (bt0p5 == 1) {
          Serial.println("Imprimiendo p5");
          CalcularVariables();
          ReiniciarTampo();
          ReiniciarCarro();
          ReiniciarCuchilla();
          AvanceCarroACC(MAX - distancia_carro_imprimir);
          delay(1000);
          AvanceTampo(altura_tampo_imprimir);
          delay(1000);
          AvanceTampo(presionimprimir);
          delay(2000);
          RetroTampo(presionimprimir);
          myNextion.setComponentValue("bt0", 0);
        }
        delay(100);
        int  bt1p5 = myNextion.getComponentValue("bt1");
        if (bt1p5 == 1) {
          Serial.println("Imprimiendo p5");
          CalcularVariables();
          ReiniciarTampo();
          ReiniciarCarro();
          ReiniciarCuchilla();
          AvanceCarroACC(MAX - distancia_carro_entintar);
          delay(1000);
          AvanceTampo(altura_tampo_entintar);
          delay(1000);
          AvanceTampo(presionentintar);
          delay(2000);
          RetroTampo(presionentintar);
          myNextion.setComponentValue("bt1", 0);
        }
        delay(100);
        int bt2p5 = myNextion.getComponentValue("bt2");
        if (bt2p5 == 1) {
          myNextion.sendCommand("page 4");
        }
        delay(100);
        int bt3p5 = myNextion.getComponentValue("bt3");
        if (bt3p5 == 1) {
          ReiniciarTampo();
          myNextion.sendCommand("page 2");
        }
        delay(100);
        int   bt4p6 = myNextion.getComponentValue("bt4");
        if (bt4p6 == 1) {
          myNextion.sendCommand("page 1");
        }
        break;
      }
    case 6://PAG 6 CONFIG ALTURAS CUCHILLA
      {
        delay(100);
        cambio = myNextion.getComponentValue("cambio");
        if (cambio == 0) {
          myNextion.setComponentValue("n0", POSICION_CUCHILLA_ARRIBA); delay(2);
          myNextion.setComponentValue("h0", 100 - POSICION_CUCHILLA_ARRIBA); delay(2);
          myNextion.setComponentValue("n1", POSICION_CUCHILLA_ABAJO); delay(2);
          myNextion.setComponentValue("h1", 100 - POSICION_CUCHILLA_ABAJO); delay(2);
          delay(500);
        } else if (cambio == 1) {
          delay(100);
          PCARRAUX = myNextion.getComponentValue("n0"); delay(2);
          PCABAUX = myNextion.getComponentValue("n1"); delay(2);
          if (PCARRAUX >= 0) {
            POSICION_CUCHILLA_ARRIBA = PCARRAUX;
            EEPROM.write(5, POSICION_CUCHILLA_ARRIBA);
          }
          if (PCABAUX >= 0) {
            POSICION_CUCHILLA_ABAJO = PCABAUX;
            EEPROM.write(6, POSICION_CUCHILLA_ABAJO);
          }
          CalcularVariables();
        }
        delay(100);
        int bt0p6 = myNextion.getComponentValue("bt0");
        if (bt0p6 == 1) {
          Serial.println("Imprimiendo p6");
          CalcularVariables();
          ReiniciarTampo();
          ReiniciarCarro();
          ReiniciarCuchilla();
          CerrarCuchilla(posicion_cuchilla_arriba);
          delay(1000);
          CerrarCuchilla(posicion_cuchilla_abajo);
          delay(2000);
          AbrirCuchilla(posicion_cuchilla_abajo);
          delay(2000);
          ReiniciarCuchilla();
          myNextion.setComponentValue("bt0", 0);
        }
        delay(100);
        int  bt4p6 = myNextion.getComponentValue("bt4");
        if (bt4p6 == 1) {
          myNextion.sendCommand("page 1");
        }
        delay(100);
        int  bt2p6 = myNextion.getComponentValue("bt2");
        if (bt2p6 == 1) {
          myNextion.sendCommand("page 5");
        }
        delay(100);
        int  bt3p6 = myNextion.getComponentValue("bt3");
        if (bt3p6 == 1) {
          ReiniciarTampo();
          myNextion.sendCommand("page 7");
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
  distancia_carro_imprimir = DISTANCIA_CARRO_IMPRIMIR / APPCARRO;
  distancia_carro_entintar = DISTANCIA_CARRO_ENTINTAR / APPCARRO;
  posicion_cuchilla_arriba = POSICION_CUCHILLA_ARRIBA / APPCUCHILLA;
  posicion_cuchilla_abajo = POSICION_CUCHILLA_ABAJO / APPCUCHILLA;
  inicio_barrido = INICIO_BARRIDO / APPCARRO;
  fin_barrido = FIN_BARRIDO / APPCARRO;
  presionimprimir = PRESIONIMPRIMIR / APPTAMPO;
  presionentintar = PRESIONENTINTAR / APPTAMPO;
  ancho = ANCHO / APPCARRO;
  Serial.println("Variables recalculadas");
}

void LeerPedal() {
  pedal = digitalRead(PEDAL);
  while (pedal < 1) {
    pedal = analogRead(PEDAL);
    Serial.println("Esperando pedal");
    myNextion.setComponentText("g0", "Esperando Pedal");
    delay(50);
    bt2p1 = myNextion.getComponentValue("bt2");
    if (bt2p1 == 0) {
      pedal = 1;
    }
  }
}

void VerificarPedal() {
  pedal = digitalRead(PEDAL);
  if (pedal > 0) {
    myNextion.setComponentText("g0", "Pausado por pedal");
    Serial.println("Pausado por pedal");
    delay(1000);
    LeerPedal();
  }
}
void LeerSensores() {
  sensor_tampo = digitalRead(SENSOR_TAMPO);
  Serial.print("Sensor Tampo:  ");
  Serial.print(sensor_tampo);

  sensor_carro = digitalRead(SENSOR_CARRO);
  Serial.print("     Sensor Carro: ");
  Serial.print(sensor_carro);

  sensor_cuchilla = digitalRead(SENSOR_CUCHILLA);
  Serial.print("     Sensor Cuchilla: ");
  Serial.print(sensor_cuchilla);

  pedal = digitalRead(PEDAL);
  Serial.print("     Pedal: ");
  Serial.println(pedal);
  delay(200);
}

void Demo() {
  //AvanceCarro(20 / APPCARRO);
  //CerrarCuchilla(200 / APPCUCHILLA);
 // delay(500);
  //RetroCarro(20 / APPCARRO);
  //AbrirCuchilla(200 / APPCUCHILLA);
 // delay(500);
 ReiniciarCuchilla();
 //ReiniciarTampo();
}

void ReiniciarTampo() {
  if (tampo == 1) {
    sensor_tampo = digitalRead(SENSOR_TAMPO);
    Serial.print("Sensor Tampo: ");
    Serial.println(sensor_tampo);
    while (sensor_tampo > 0) {
      sensor_tampo = digitalRead(SENSOR_TAMPO);
      RetroTampo(10);
    }
    delay(100);
    while (sensor_tampo < 1) {
      sensor_tampo = digitalRead(SENSOR_TAMPO);
      AvanceTampo(1);
    }
    PATAMPO = 0;
  }
}

void ReiniciarCarro() {
  sensor_carro = digitalRead(SENSOR_CARRO);
  Serial.print("Sensor Carro: ");
  Serial.println(sensor_carro);
  while (sensor_carro > 0) {
    sensor_carro = digitalRead(SENSOR_CARRO);
    RetroCarro(10);
  }
  delay(100);
  while (sensor_carro < 1) {
    sensor_carro = digitalRead(SENSOR_CARRO);
    AvanceCarro(1);
  }
  PACARRO = 0;
}

void ReiniciarCuchilla() {
  sensor_cuchilla = digitalRead(SENSOR_CUCHILLA);
  Serial.print("Sensor Cuchilla: ");
  Serial.println(sensor_cuchilla);
  while (sensor_cuchilla > 0) {
    sensor_cuchilla = digitalRead(SENSOR_CUCHILLA);
    AbrirCuchilla(10);
  }
  delay(100);
  while (sensor_cuchilla < 1) {
    sensor_cuchilla = digitalRead(SENSOR_CUCHILLA);
    CerrarCuchilla(1);
  }
  PACUCHILLA = 0;
}
void AvanceCarroACC(float steps) {
  pausa = p0 ;
  for (int i = 0; i < steps / 20; i++) {
    AvanceCarro(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 20; i++) {
    AvanceCarro(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 20); i++) {
    AvanceCarro(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 20); i++) {
    AvanceCarro(1);
  }
  pausa = p4;
  for (int i = 0; i < ((steps * 3) / 5); i++) {
    AvanceCarro(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 20); i++) {
    AvanceCarro(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 20); i++) {
    AvanceCarro(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 20; i++) {
    AvanceCarro(1);
  }
  pausa = p0;
  for (int i = 0; i < steps / 20; i++) {
    AvanceCarro(1);
  }
}

void RetroCarroACC(float steps) {
  pausa = p0 ;
  for (int i = 0; i < steps / 20; i++) {
    RetroCarro(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 20; i++) {
    RetroCarro(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 20); i++) {
    RetroCarro(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 20); i++) {
    RetroCarro(1);
  }
  pausa = p4;
  for (int i = 0; i < ((steps * 3) / 5); i++) {
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

void AvanceTampo(int steps) {
  if (tampo == 1) {
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
}

void RetroTampo(int steps) {
  if (tampo == 1) {
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
}

void AvanceTampoACC(double steps) {
  pausainicial = pausa;
  pausa = p0;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p4;
  for (int i = 0; i < (steps * 2 / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 10); i++) {
    AvanceTampo(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = p0;
  for (int i = 0; i < steps / 10; i++) {
    AvanceTampo(1);
  }
  pausa = pausainicial;
}

void RetroTampoACC(double steps) {
  pausainicial = pausa;
  pausa = p0;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTampo(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTampo(1);
  }
  pausa = p4;
  for (int i = 0; i < (steps * 2 / 10); i++) {
    RetroTampo(1);
  }
  pausa = p3;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTampo(1);
  }
  pausa = p2;
  for (int i = 0; i < (steps / 10); i++) {
    RetroTampo(1);
  }
  pausa = p1;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = p0;
  for (int i = 0; i < steps / 10; i++) {
    RetroTampo(1);
  }
  pausa = pausainicial;
}


void CerrarCuchilla(int steps) {
  digitalWrite(CUCHILLA1,  LOW);
  for (int i = 0; i < steps; i++) {
    digitalWrite(CUCHILLA2,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(CUCHILLA2,  LOW);
    delayMicroseconds(pausa);
  }
  PACUCHILLA = PACUCHILLA + steps;
}

void AbrirCuchilla(int steps) {
  digitalWrite(CUCHILLA1,  HIGH);
  for (int i = 0; i < steps; i++) {
    digitalWrite(CUCHILLA2,  HIGH);
    delayMicroseconds(pausa);
    digitalWrite(CUCHILLA2,  LOW);
    delayMicroseconds(pausa);
  }
  PACUCHILLA = PACUCHILLA - steps;

}


void IMPRIMIR() {
  Serial.println("Imprimiendo...");
  myNextion.setComponentText("g0", "Imprimiendo");
  delay(100);
  CalcularVariables();

  //Impresion Plana
  if (automatico == 1) {
    LeerPedal();
  }
  bt2p1 = myNextion.getComponentValue("bt2");
  while (bt2p1 < 0) {
    bt2p1 = myNextion.getComponentValue("bt2");
  }
  if (bt2p1 == 1) {
    if (ciclos == 0) {
      ReiniciarCarro();
      ReiniciarCuchilla();
      ReiniciarTampo();
      AvanceCarroACC(MAX - distancia_carro_imprimir);
      RetroCarro(inicio_barrido);
      CerrarCuchilla(posicion_cuchilla_abajo - posicion_cuchilla_arriba);
      RetroCarroACC(fin_barrido);
      AbrirCuchilla(posicion_cuchilla_abajo - posicion_cuchilla_arriba);
      RetroCarro(PACARRO);
    }
    VerificarPedal();
    AvanceCarro(MAX - distancia_carro_entintar);
    VerificarPedal();
    if ((altura_tampo_entintar - PATAMPO) > 0) {
      if ((altura_tampo_entintar - PATAMPO) > 600) {
        AvanceTampoACC(altura_tampo_entintar - PATAMPO);
      } else {
        AvanceTampo(altura_tampo_entintar - PATAMPO);
      }
    } else {
      RetroTampo(PATAMPO - altura_tampo_entintar);
    }
    VerificarPedal();
    AvanceTampo(presionentintar);
    delay(tiempoentintar);
    RetroTampo(presionentintar);
    VerificarPedal();
    AvanceCarroACC(MAX - distancia_carro_entintar - distancia_carro_imprimir);
    VerificarPedal();
    if ((altura_tampo_imprimir - PATAMPO) > 0) {
      if ((altura_tampo_imprimir - PATAMPO) > 600) {
        AvanceTampoACC(altura_tampo_imprimir - PATAMPO);
      } else {
        AvanceTampo(altura_tampo_imprimir - PATAMPO);
      }
    } else {
      RetroTampo(PATAMPO - altura_tampo_imprimir);
    }
    VerificarPedal();
    AvanceTampo(presionimprimir);
    delay(tiempoimprimir);
    if (modo == 1) {
      AvanceCarro(ancho);
    }
    RetroTampo(presionimprimir);
    if (modo == 1) {
      RetroCarro(ancho);
    }
    VerificarPedal();
    RetroCarro(inicio_barrido);
    VerificarPedal();
    CerrarCuchilla(posicion_cuchilla_abajo - posicion_cuchilla_arriba);
    RetroCarroACC(fin_barrido);
    VerificarPedal();
    AbrirCuchilla(posicion_cuchilla_abajo - posicion_cuchilla_arriba);
    RetroCarro(PACARRO);
  } else {
    ReiniciarTampo();
    ReiniciarCarro();
    ReiniciarCuchilla();
  }
  //Fin Impresion Plana
  ciclos++;
  myNextion.setComponentValue("n0", ciclos);
  Serial.println("Ciclo de impresion terminado");
}


