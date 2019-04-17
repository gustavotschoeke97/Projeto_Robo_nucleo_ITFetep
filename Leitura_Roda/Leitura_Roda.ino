#include <Wire.h>
#define PIN_2 2
#define DIRECAO 3

volatile int cont, dir, rpm;
volatile unsigned long timer;
volatile float rps;
volatile bool isr_happened;


void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(PIN_2,  INPUT);
  pinMode(DIRECAO, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_2), CalcVeloRoda, RISING);

}

void loop() {
  if (isr_happened == true && cont == 15 ){
    String str = String(dir)+"|"+String(rpm); // sentido da roda|RPM 
    char c[10];
    str.toCharArray(c,10);
    Wire.beginTransmission(8); // transmite para o dispositvo de endereco 0x08
    Wire.write(c);
    Wire.endTransmission();   
    isr_happened = false;
    cont = 0;
  }
  delay(50);
}

void CalcVeloRoda() {
  isr_happened = true;
  if (cont == 0) {
    timer   = millis();
    //Serial.print(digitalRead(DIRECAO)); // módulo esquerdo DIRECAO = 0 vai para frente DIRECAO = 1 para trás || módulo  Direito  DIRECAO = 1 vai para frente DIRECAO = 0 para trás
  }
  cont++;

  if (cont == 15) {
    timer = (millis() - timer) / 1000;
    rps     = 1.0 / timer; // para dividir floats colocar os números com casas decimais ex: 1.0
    rpm     = rps * 60;
    dir     = digitalRead(DIRECAO);
  }
}
