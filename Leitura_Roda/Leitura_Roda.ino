#include <Wire.h>
#define PIN_2 2
#define DIR 3
int cont = 0;
unsigned long timer;
void setup() {
  Wire.begin(0x08);
  delay(50);
  Wire.onRequest(requestEvent);
  pinMode(PIN_2, INPUT_PULLUP);
  pinMode(DIR, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_2), CalcVeloRoda, RISING);
 
  Serial.begin(9600);
}

void loop() {
  delay(100);
}

void CalcVeloRoda() {
  float rpm;
  if (cont == 0) {
    timer = millis();
    Serial.print("valor porta 3:");
    Serial.println(digitalRead(DIR)); // módulo esquerdo dir = 0 vai para frente dir = 1 para trás || módulo direito  dir = 1 vai para frente dir = 0 para trás 
  }
  cont++;
  Serial.print("valor: ");
  Serial.println(cont);
  if (cont == 15) {
    timer = (millis() - timer)/1000;
    rpm =1.0/timer; // para dividir floats clocar os números com casas decimais ex: 1.0
    Serial.println(rpm);
    rpm =rpm * 60;
    Serial.print("valor RPM: ");
    Serial.println(rpm);
    cont = 0;
  }
}

void requestEvent(){
   Wire.write("hello ");
}
