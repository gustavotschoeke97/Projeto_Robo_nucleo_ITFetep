#include <Wire.h>
#define PIN_2 2
#define DIRECAO 3

int cont = 0;
unsigned long timer;
float rps;

void setup(){
  Wire.begin(0x08);
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
   // Serial.print("modulo direito:");
  //  Serial.println(digitalRead(DIRECAO)); // módulo esquerdo DIRECAO = 0 vai para frente DIRECAO = 1 para trás || módulo  Direito  DIRECAO = 1 vai para frente DIRECAO = 0 para trás 
  }
  cont++;
  //Serial.print("valor do contador de interrupçoes: ");
  //Serial.println("Cont: ");
  //Serial.println(cont);
  if (cont == 15) {
    timer = (millis() - timer)/1000;
    rps = 1.0/timer; // para dividir floats colocar os números com casas decimais ex: 1.0
    rps = rps * 60;
  //  Serial.print("valor do RPS: ");
  //  Serial.println("RPS: ");
    //Serial.println(rps);
    cont = 0;
  }
}

void requestEvent(){
   Wire.write(digitalRead(DIRECAO));
}
