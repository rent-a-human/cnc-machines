#include <EEPROM.h>
#include <Nextion.h>
#define nextion Serial1
Nextion myNextion(nextion, 9600);

int pausa=114;
int pause=0;
int contador=0;
int imprimir=0;
int automatico=0;
int multicolor=0;
int reset=0;
int va0=0;
int va1=0;
int velocidad=0;
int pausamax=100;
int pausainicial=115;
int reiniciar=1;
int diametro=50;
int angulo=0;
int anchoMax=290;
int ANCHO=120;
int centro=150;
int BSP0=centro-ANCHO/2;
int BSP1=centro+ANCHO/2;
int CSP0=20;
int CSP1=90;
int TSP0=80;//recoger
int TSP1=90;//imprimir
int TSP2=87;//entintar
int OFFSET_DER=20;
int OFFSET_IZQ=20;
double APP1=0.025;
double APP2=diametro/127.32366;
double frecuencia=APP2/APP1;

double tsp0=TSP0/APP1;//UNIDADES EN PASOS
double tsp1=TSP1/APP1;
double tsp2=TSP2/APP1;
double bsp0=BSP0/APP1;
double bsp1=BSP1/APP1;
double csp0=CSP0/APP1;
double csp1=CSP1/APP1;
double offset_der=OFFSET_DER/APP1;
double offset_izq=OFFSET_IZQ/APP1;
double ancho=ANCHO/APP2;

int pedal=0;

int steps=0;

int sensor_bandeja=0;
int sensor_carro=0;
int sensor_tinta=0;
int sensor_envase=0;

long PAB=0;//Paso Actual Bandeja
long PAC=0;
long PAT=0;
long PAE=0;

int modo=0;

void setup() {
   Serial.begin(9600);;
   Serial.print("Inicio de programa ");
 //Motor Bandeja
 pinMode(2, OUTPUT);
 pinMode(3, OUTPUT);
 pinMode(4, OUTPUT);
 pinMode(5, OUTPUT);
 //Motor Carro
 pinMode(9, OUTPUT);
 pinMode(10, OUTPUT);
 pinMode(11, OUTPUT);
 pinMode(12, OUTPUT);
 //Motor Tinta
 pinMode(22, OUTPUT);
 pinMode(24, OUTPUT);
 pinMode(26, OUTPUT);
 pinMode(28, OUTPUT);
 //Motor Envase
 pinMode(30, OUTPUT);
 pinMode(32, OUTPUT);
 pinMode(34, OUTPUT);
 pinMode(36, OUTPUT);
 //Sensores:A0 Bandeja, A1 Carro, A2 Tinta, A3 Envase, A4 Pedal
 pinMode(A0, INPUT);
 pinMode(A1, INPUT);
 pinMode(A2, INPUT);
 pinMode(A3, INPUT);
 pinMode(A4, INPUT); 

if (myNextion.init()) {
    Serial.println(F("Nextion Ready..."));
  } else {
    Serial.println(F("Nextion not responding..."));
  }
 delay(1000);
 //myNextion.sendCommand("bauds=57600");
myNextion.sendCommand("page 1");
/*myNextion.sendCommand("page1.n0.val=12");
delay(600);
modo=myNextion.getComponentValue("n0");
Serial.println(modo);
*/
 velocidad = EEPROM.read(0);
 diametro = EEPROM.read(1);
 ANCHO = EEPROM.read(2);
 angulo = EEPROM.read(3);
 angulo = 2*angulo;
 centro = EEPROM.read(4);
 CSP0 = EEPROM.read(5);
 CSP1 = EEPROM.read(6);
 TSP0 = EEPROM.read(7);
 TSP1 = EEPROM.read(8);
 TSP2 = EEPROM.read(9);
 OFFSET_DER = EEPROM.read(10);
 CalcularVariables();

 Serial.print(" FIN Inicio de programa ");
 
}

void loop() {
  Configurar();
  myNextion.setComponentText("g0","En Linea..."); 

}

void Configurar(){
  va0=myNextion.getComponentValue("va0");
  delay(2);
    switch (va0){
    case 1:
    va1=myNextion.getComponentValue("va1");
    if(va1==0){
      myNextion.setComponentValue("n0",contador);
    } else if(va1==1){
      contador==0;
      myNextion.setComponentValue("va1",0);
    }
    reset=myNextion.getComponentValue("bt3");
      if(reset==1){
        myNextion.setComponentText("g0","Reiniciando...");
        ReiniciarTodos();
        AvanceTinta(tsp0);
        AvanceCarro(csp0);
        AvanceBandeja(bsp0-offset_der);
        AvanceTinta(tsp2-tsp0);
        myNextion.setComponentValue("bt3",0);
        delay(2);
        myNextion.setComponentText("g0","Reiniciado!");
        delay(1000);
        reiniciar=0;
       }
     imprimir=myNextion.getComponentValue("bt2");
     if(imprimir==1){
      automatico=myNextion.getComponentValue("bt1");
      if(automatico==1){
        pause=myNextion.getComponentValue("n1");
      }
      multicolor=myNextion.getComponentValue("bt0");
      myNextion.setComponentText("g0","Imprimiendo...");
      IMPRIMIR();
      myNextion.setComponentText("g0","Impreso!");
     }
    break;
    
    case 2:
       va1=myNextion.getComponentValue("va1");
       if(va1==0){
        myNextion.setComponentValue("h0",centro); delay(2);        
        myNextion.setComponentValue("n0",centro); delay(2);
        myNextion.setComponentValue("h1",CSP1); delay(2);        
        myNextion.setComponentValue("n1",CSP1); delay(2);
        myNextion.setComponentValue("h2",CSP0); delay(2);        
        myNextion.setComponentValue("n2",CSP0); delay(2);
        myNextion.setComponentValue("h3",100-TSP0); delay(2);        
        myNextion.setComponentValue("n3",TSP0); delay(2);
        myNextion.setComponentValue("h4",ANCHO); delay(2);        
        myNextion.setComponentValue("n4",ANCHO); delay(2);
        myNextion.setComponentValue("h5",diametro); delay(2);        
        myNextion.setComponentValue("n5",diametro); delay(2);
        myNextion.setComponentValue("h6",velocidad); delay(2);        
        myNextion.setComponentValue("n6",velocidad); delay(2);
        myNextion.setComponentValue("h7",angulo); delay(2);        
        myNextion.setComponentValue("n7",angulo); delay(2);
        myNextion.setComponentValue("h8",20-TSP1-TSP0); delay(2);        
        myNextion.setComponentValue("n8",TSP1-TSP0); delay(2);
        myNextion.setComponentValue("h9",20-TSP2-TSP0); delay(2);        
        myNextion.setComponentValue("n9",TSP2-TSP0); delay(2);
        myNextion.setComponentValue("h10",OFFSET_DER); delay(2);        
        myNextion.setComponentValue("n10",0FFSET_DER); delay(2);
        } else if(va1==1){
          contador==0;
          myNextion.setComponentValue("va1",0);
          }
      break;
    default:
    myNextion.setComponentText("g0","Error!");
    }
}

void IMPRIMIR(){
  if(reiniciar==1){
    ReiniciarTodos();
    AvanceTinta(tsp0);
    AvanceCarro(csp0);
    AvanceBandeja(bsp0-offset_der);
    AvanceTinta(tsp2-tsp0);
    reiniciar=0;
  } 
  if(automatico==0){
    LeerPedal();
  }
  
  AvanceCarro(csp1-csp0);
  AvanceBandeja(offset_der-(tsp1-tsp2));
  ASBT(tsp1-tsp2);
  ASBE(ancho);
  ASBTC(tsp1-tsp0);
  ASBC(offset_izq-(tsp1-tsp0));
  ASCT(tsp2-tsp0);
  RSBC(PAC);
  pausa=pausa/2;
  RetroBandeja(PAB-bsp0);
  pausa=2*pausa;
  RSBT(tsp2-tsp0);
  RetroBandeja(offset_der-(tsp2-tsp0));
  AvanceTinta(tsp2-tsp0);
  AvanceBandeja(offset_der);
}

void CalcularVariables() {
  pausa=-4*velocidad+510;
  pausainicial=pausa;
  BSP0=centro-ANCHO/2;
  BSP1=centro+ANCHO/2;
  APP2=diametro/127.32366;
  frecuencia=APP2/APP1;
  TSP1=TSP1+TSP0;
  TSP2=TSP2+TSP0;
  OFFSET_IZQ=OFFSET_DER;
  tsp0=TSP0/APP1;//UNIDADES EN PASOS
  tsp1=TSP1/APP1;
  tsp2=TSP2/APP1;
  bsp0=BSP0/APP1;
  bsp1=BSP1/APP1;
  csp0=CSP0/APP1;
  csp1=CSP1/APP1;
  offset_der=OFFSET_DER/APP1;
  offset_izq=OFFSET_IZQ/APP1;
  ancho=ANCHO/APP2;
}

void LeerPedal(){
  pedal=analogRead(A4);
  while(pedal<50){
    pedal=analogRead(A4);
    Serial.println("Esperando pedal");
    myNextion.setComponentText("g0","Esperando pedal");        
    delay(10);
  }
}

void RSBT(int steps){
  pausa=pausamax;
  pausa=pausa/2;
  for(int i=0;i<steps;i++){
    RetroBandeja(1);
    RetroTinta(1);    
  }
  pausa=pausainicial;
}

void RSBC(int steps){
  pausa=pausamax;
  pausa=pausa/2;
  for(int i=0;i<steps;i++){
    RetroBandeja(1);
    RetroCarro(1);
  }
  pausa=pausainicial;
}

void ASCT(int steps){
  pausa=pausamax;
  pausa=pausa/2;
  for(int i=0;i<steps;i++){
    AvanceTinta(1);
    RetroCarro(1);
  }
  pausa=pausainicial;
}

void ASBC(int steps){
  pausa=pausamax;
  pausa=pausa/2;
  for(int i=0;i<steps;i++){
    AvanceBandeja(1);
    RetroCarro(1);
  }
  pausa=pausainicial;
}

void ASBTC(int steps){
  pausa=pausamax;
  pausa=pausa/3;
  for(int i=0;i<steps;i++){
    AvanceBandeja(1);
    RetroTinta(1);
    RetroCarro(1);
  }
  pausa=pausainicial;
}

void ASBE(int steps){
  pausa=pausa/2;
  for(int i=0;i<steps;i++){
    AvanceBandeja(frecuencia);
    AvanceEnvase(1);
  }
  pausa=2*pausa;
}

void ASBT(int steps){
  pausa=pausamax;
  pausa=pausa/2;
  for(int i=0;i<steps;i++){
    AvanceBandeja(1);
    AvanceTinta(1);
  }
  pausa=pausainicial;
}

void ReiniciarBandeja(){
  sensor_bandeja=analogRead(A0);
  while(sensor_bandeja>500){
    sensor_bandeja=analogRead(A0);
    RetroBandeja(10);
  }
  while(sensor_bandeja<50){
    sensor_bandeja=analogRead(A0);
    AvanceBandeja(1);
  }
  PAB=0;
}

void ReiniciarCarro(){
  sensor_carro=analogRead(A1);
  while(sensor_carro>500){
    sensor_carro=analogRead(A1);
    RetroCarro(10);
  }
  while(sensor_carro<50){
    sensor_carro=analogRead(A1);
    AvanceCarro(1);
  }
  PAC=0;
}

void ReiniciarTinta(){
  sensor_tinta=analogRead(A2);
  while(sensor_tinta>500){
    sensor_tinta=analogRead(A2);
    RetroTinta(10);
  }
  while(sensor_tinta<50){
    sensor_tinta=analogRead(A2);
    AvanceTinta(1);
  }
  PAT=0;
}

void ReiniciarTodos(){
  ReiniciarBandeja();
  ReiniciarCarro();
  ReiniciarTinta();
}

void AvanceBandeja(int steps){
  for(int i=0;i<steps;i++){
  digitalWrite(2,  LOW);
  digitalWrite(3,  HIGH);
  digitalWrite(4,  LOW);
  digitalWrite(5,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(2,  HIGH);
  digitalWrite(3,  LOW);
  digitalWrite(4,  LOW);
  digitalWrite(5,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(2,  HIGH);
  digitalWrite(3,  LOW);
  digitalWrite(4,  HIGH);
  digitalWrite(5,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(2,  LOW);
  digitalWrite(3,  HIGH);
  digitalWrite(4,  HIGH);
  digitalWrite(5,  LOW);
  delayMicroseconds(pausa);
  }
  PAB=PAB+steps;
}

void RetroBandeja(int steps){
  for(int i=0;i<steps;i++){
  digitalWrite(2,  LOW);
  digitalWrite(3,  HIGH);
  digitalWrite(4,  LOW);;
  digitalWrite(5,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(2,  LOW);
  digitalWrite(3,  HIGH);
  digitalWrite(4,  HIGH);
  digitalWrite(5,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(2,  HIGH);
  digitalWrite(3,  LOW);
  digitalWrite(4,  HIGH);
  digitalWrite(5,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(2,  HIGH);
  digitalWrite(3,  LOW);
  digitalWrite(4,  LOW);
  digitalWrite(5,  HIGH);
  delayMicroseconds(pausa);  
  }
  PAB=PAB-steps;
}

void AvanceCarro(int steps){
  for(int i=0;i<steps;i++){
  digitalWrite(9,  LOW);
  digitalWrite(10,  HIGH);
  digitalWrite(11,  LOW);
  digitalWrite(12,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(8,  HIGH);
  digitalWrite(10,  LOW);
  digitalWrite(11,  LOW);
  digitalWrite(12,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(9,  HIGH);
  digitalWrite(10,  LOW);
  digitalWrite(11,  HIGH);
  digitalWrite(12,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(9,  LOW);
  digitalWrite(10,  HIGH);
  digitalWrite(11,  HIGH);
  digitalWrite(12,  LOW);
  delayMicroseconds(pausa);
  }
  PAC=PAC+steps;
}

void RetroCarro(int steps){
  for(int i=0;i<steps;i++){
  digitalWrite(9,  LOW);
  digitalWrite(10,  HIGH);
  digitalWrite(11,  LOW);;
  digitalWrite(12,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(9,  LOW);
  digitalWrite(10,  HIGH);
  digitalWrite(11,  HIGH);
  digitalWrite(12,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(9,  HIGH);
  digitalWrite(10,  LOW);
  digitalWrite(11,  HIGH);
  digitalWrite(12,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(9,  HIGH);
  digitalWrite(10,  LOW);
  digitalWrite(11,  LOW);
  digitalWrite(12,  HIGH);
  delayMicroseconds(pausa);  
  }
  PAC=PAC-steps;
}

void AvanceTinta(int steps){
  for(int i=0;i<steps;i++){
  digitalWrite(22,  LOW);
  digitalWrite(24,  HIGH);
  digitalWrite(26,  LOW);
  digitalWrite(28,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(22,  HIGH);
  digitalWrite(24,  LOW);
  digitalWrite(26,  LOW);
  digitalWrite(28,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(22,  HIGH);
  digitalWrite(24,  LOW);
  digitalWrite(26,  HIGH);
  digitalWrite(28,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(22,  LOW);
  digitalWrite(24,  HIGH);
  digitalWrite(26,  HIGH);
  digitalWrite(28,  LOW);
  delayMicroseconds(pausa);
  }
  PAT=PAT+steps;
}

void RetroTinta(int steps){
  for(int i=0;i<steps;i++){
  digitalWrite(22,  LOW);
  digitalWrite(24,  HIGH);
  digitalWrite(26,  LOW);;
  digitalWrite(28,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(22,  LOW);
  digitalWrite(24,  HIGH);
  digitalWrite(26,  HIGH);
  digitalWrite(28,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(22,  HIGH);
  digitalWrite(24,  LOW);
  digitalWrite(26,  HIGH);
  digitalWrite(28,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(22,  HIGH);
  digitalWrite(24,  LOW);
  digitalWrite(26,  LOW);
  digitalWrite(28,  HIGH);
  delayMicroseconds(pausa);  
  }
  PAT=PAT-steps;
}

void AvanceEnvase(int steps){
  for(int i=0;i<steps;i++){
  digitalWrite(30,  LOW);
  digitalWrite(32,  HIGH);
  digitalWrite(34,  LOW);
  digitalWrite(36,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(30,  HIGH);
  digitalWrite(32,  LOW);
  digitalWrite(34,  LOW);
  digitalWrite(36,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(30,  HIGH);
  digitalWrite(32,  LOW);
  digitalWrite(34,  HIGH);
  digitalWrite(36,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(30,  LOW);
  digitalWrite(32,  HIGH);
  digitalWrite(34,  HIGH);
  digitalWrite(36,  LOW);
  delayMicroseconds(pausa);
  }
  PAE=PAE+steps;
}

void RetroEnvase(int steps){
  for(int i=0;i<steps;i++){
  digitalWrite(30,  LOW);
  digitalWrite(32,  HIGH);
  digitalWrite(34,  LOW);;
  digitalWrite(36,  HIGH);
  delayMicroseconds(pausa);
  digitalWrite(30,  LOW);
  digitalWrite(32,  HIGH);
  digitalWrite(34,  HIGH);
  digitalWrite(36,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(30,  HIGH);
  digitalWrite(32,  LOW);
  digitalWrite(34,  HIGH);
  digitalWrite(36,  LOW);
  delayMicroseconds(pausa);
  digitalWrite(30,  HIGH);
  digitalWrite(32,  LOW);
  digitalWrite(34,  LOW);
  digitalWrite(36,  HIGH);
  delayMicroseconds(pausa);  
  }
  PAE=PAE-steps;
}