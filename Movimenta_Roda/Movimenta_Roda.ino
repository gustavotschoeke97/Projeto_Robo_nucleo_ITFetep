#include <Wire.h>
#define PINO_PWM     3      //pino de aceleração
#define PINO_FREIO   4
#define PINO_REVERSO 5      // pino inverte rotação
#define TEMPO        100  //tempo (ms) em que o motor gira
#define PINO_FREIO_DIR 8
#define PINO_REVERSO_DIR 7
#define PINO_PWM_DIR 9
char read_value;
bool freio = false;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(PINO_PWM    , OUTPUT);
  pinMode(PINO_REVERSO, OUTPUT);
  pinMode(PINO_FREIO  , OUTPUT);
  pinMode(PINO_PWM_DIR    , OUTPUT);
  pinMode(PINO_REVERSO_DIR, OUTPUT);
  pinMode(PINO_FREIO_DIR  , OUTPUT);
}

void loop() {
  Wire.requestFrom(0x08, 6);
  delay(200);
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    delay(20);
    Serial.print(c);         // print the character
  }
  delay(300);
  
  if (Serial.available() > 0 ){
    read_value = Serial.read();
    Serial.println(read_value);
  }
  switch (read_value) {
    case '1':
      freio = false;
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      digitalWrite(PINO_REVERSO, LOW);
      digitalWrite(PINO_REVERSO_DIR, LOW);
      delay(100);
      Movimenta(freio);

      break;
    case '2':
      freio = false;
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      digitalWrite(PINO_REVERSO, HIGH);
      digitalWrite(PINO_REVERSO_DIR, HIGH);
      delay(100);
      Movimenta(freio);
      break;
    case '3':
      freio = true;
      Movimenta(freio);
      digitalWrite(PINO_FREIO, 0);
      digitalWrite(PINO_FREIO_DIR,0);
      break;
  }
  delay(100);
}

void Movimenta(bool freio) {
  float duty_cicle = 0;
  if (freio == true) {
      analogWrite(PINO_PWM, 0);
      analogWrite(PINO_PWM_DIR, 0);
  }
  else {
      for (int i = 0; i < 200; i++) {      //acelera para frente
        analogWrite(PINO_PWM, i);
        analogWrite(PINO_PWM_DIR, i);
        delay(TEMPO);
        if (i > 0 ) {
           //duty_cicle = (i/256);
          // duty_cicle = duty_cicle * 100;
           Serial.print("Valor do PWM: ");
           Serial.println(i);
        }
      }   
      
  }
  read_value=0;
}
    
