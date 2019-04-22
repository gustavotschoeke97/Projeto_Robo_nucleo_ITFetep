#include <Wire.h>
#define PIN_2 2
#define DIRECAO 3
int dir;
unsigned long duration;
void setup() {
  // put your setup code here, to run once:
  Wire.begin(13);
  Serial.begin(9600);
  pinMode(PIN_2,  INPUT);
}

void loop() {


  if (duration = pulseIn(PIN_2, HIGH) > 0 ) {
    dir = digitalRead(DIRECAO);
    float valor = (duration * 2 * 15) / 1000000.0;

    valor = 1 / valor;
    int RPM = valor * 60; // multiplica por 60 para obter o valor do 

    String str = "D|" + String(dir) + "|" + String(RPM); //roda Esq/Dir|sentido da roda|RPM
    char c[10];
    str.toCharArray(c, 10);//// necessario converter para um char array antes de enviar

    Wire.beginTransmission(8); // transmite para o dispositvo de endereco 0x08
    Wire.write(c);
    Wire.endTransmission();
  }
  delay(500);
}
