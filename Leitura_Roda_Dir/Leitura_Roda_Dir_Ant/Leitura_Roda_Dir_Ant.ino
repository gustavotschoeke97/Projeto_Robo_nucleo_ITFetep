#include <Wire.h>
#define PIN_2 2
#define DIRECAO 3

volatile int cont, dir, rpm;
volatile double timerfinal, timer;
volatile float rps;
volatile bool isr_happened;
byte error;

void setup() {
  delay(75);
  Wire.begin(9);
  Serial.begin(9600);
  pinMode(PIN_2,  INPUT);
  pinMode(DIRECAO, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_2), CalcVeloRoda, RISING);

}

void loop() {

  if (isr_happened == true && cont == 15 ) {
    Serial.println(rpm);
    String str = String(dir) + "|" + String(rpm); // sentido da roda|RPM
    char c[10];
    str.toCharArray(c, 10);
    Wire.beginTransmission(8); // transmite para o dispositvo de endereco 0x08
    Wire.write(c);
    Serial.println("passou no write");
    Wire.endTransmission();
//    error = Wire.endTransmission();
//    switch (error) {
//      case 0:
//        Serial.println("0:success ");
//        break;
//      case 1:
//        Serial.println("1:data too long to fit in transmit buffer ");
//        break;
//      case 2:
//        Serial.println("2:received NACK on transmit of address ");
//        break;
//      case 3:
//        Serial.println("3:received NACK on transmit of data ");
//        break;
//      case 4:
//        Serial.println("4:other error ");
//        break;
//    }
    isr_happened = false;
    cont = 0;
  }
  delay(100);
}

void CalcVeloRoda() {
  isr_happened = true;
  if (cont == 0) {
    timer   = millis();
    //Serial.print(digitalRead(DIRECAO)); // módulo esquerdo DIRECAO = 0 vai para frente DIRECAO = 1 para trás || módulo  Direito  DIRECAO = 1 vai para frente DIRECAO = 0 para trás
  }
  cont++;

  if (cont == 15) {
    timerfinal = (millis() - timer) / 1000;
    rps     = 1.0 / timerfinal; // para dividir floats colocar os números com casas decimais ex: 1.0
    rpm     = rps * 60;
    dir     = digitalRead(DIRECAO);
  }
}
