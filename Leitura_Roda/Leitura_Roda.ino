#include <Wire.h>
#define PIN_2 2
#define DIRECAO 3

int cont = 0;
unsigned long timer;
int rps;

void setup(){
  Wire.begin(0x0A);
  Wire.onRequest(requestEvent);
  pinMode(PIN_2,  INPUT);
  pinMode(DIRECAO,INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_2), CalcVeloRoda, RISING);
  Serial.begin(9600);
}

void loop(){
  delay(100);
}

void CalcVeloRoda(){
  
  if (cont == 0){
    timer = millis();
    Serial.print("valor porta 3:");
    Serial.println(digitalRead(DIRECAO)); // módulo esquerdo DIRECAO = 0 vai para frente DIRECAO = 1 para trás || módulo  Direito  DIRECAO = 1 vai para frente DIRECAO = 0 para trás 
  }
  cont++;
  Serial.print("valor: ");
  Serial.println(cont);
  if (cont == 15) {
    timer = (millis() - timer)/1000;
    rps =1.0/timer; // para dividir floats clocar os números com casas decimais ex: 1.0
    Serial.println(rps);
    rps =rps * 60;
    Serial.print("valor RPS: ");
    Serial.println(rps);
    cont = 0;
    rps =  0;
  }
}

void requestEvent(){
   Wire.write(rps);
}
