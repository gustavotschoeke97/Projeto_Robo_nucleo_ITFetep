#include <Wire.h>
#define PINO_PWM     3                   // pino de aceleração
#define PINO_FREIO   4
#define PINO_REVERSO 5                  //  pino inverte rotação
#define TEMPO        100               //   tempo (ms) em que o motor gira
#define PINO_FREIO_DIR 8
#define PINO_REVERSO_DIR 7
#define PINO_PWM_DIR 9
char read_value; 
bool freio = false;

void setup() {
  Wire.begin();
  pinMode(PINO_PWM    , OUTPUT);
  pinMode(PINO_REVERSO, OUTPUT);
  pinMode(PINO_FREIO  , OUTPUT);
  pinMode(PINO_PWM_DIR    , OUTPUT);
  pinMode(PINO_REVERSO_DIR, OUTPUT);
  pinMode(PINO_FREIO_DIR  , OUTPUT);
  Serial.begin(9600);
}

void loop(){
  Wire.requestFrom(0x08,1);           //   slave may send less than requested
  while(Wire.available()){           //     receive a byte as character
    int c = Wire.read();           //    print the character                    
    Serial.println(c);        
  }
  delay(250);
  
  if (Serial.available() > 0 ){
    read_value = Serial.read();
  }
  switch(read_value){
    case '1':
      freio = false;
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      delay(300);
      digitalWrite(PINO_REVERSO, LOW);
      digitalWrite(PINO_REVERSO_DIR, LOW);
      delay(300);
      analogWrite(PINO_PWM, 70);
      analogWrite(PINO_PWM_DIR, 85);

      break;
    case '2':
      freio = false;
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      delay(300);
      digitalWrite(PINO_REVERSO, HIGH);
      digitalWrite(PINO_REVERSO_DIR, HIGH);
      delay(300);
      analogWrite(PINO_PWM, 70);
      analogWrite(PINO_PWM_DIR, 85);
      break;
    case '3':
      freio = true;
      Movimenta(freio);
      digitalWrite(PINO_FREIO, 0);
      digitalWrite(PINO_FREIO_DIR,0);
      delay(500);
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR,1);
      break;
  }
  delay(100);
}

void Movimenta(bool freio) {
  if (freio == true) {
      analogWrite(PINO_PWM, 0);
      analogWrite(PINO_PWM_DIR, 0);
  }
  else {
      for (int i = 0; i < 90; i++){      // acelera para frente
        analogWrite(PINO_PWM, i);
        analogWrite(PINO_PWM_DIR, i);
        delay(TEMPO);
        if (i > 0 ) {
           Serial.print("Valor do PWM: ");
           Serial.println(i);
        }
      }       
  }
  read_value=0;
}
    
