 // módulo esquerdo DIRECAO = 0 vai para frente DIRECAO = 1 para trás | módulo  Direito  DIRECAO = 1 vai para frente DIRECAO = 0 para trás

#include <Wire.h>
#include <string.h>
#define PINO_PWM     3                   // pino de aceleração
#define PINO_FREIO   4
#define PINO_REVERSO 5                  //  pino inverte rotação
#define TEMPO        100               //   tempo (ms) em que o motor gira
#define PINO_FREIO_DIR 8
#define PINO_REVERSO_DIR 7
#define PINO_PWM_DIR 9


char read_value,wheel;
int  dir_esq,dir_dir, rpm_esq,rpm_dir, pwd_control_esq, pwd_control_esq_rev, pwd_control_dir, pwd_control_dir_rev;
bool freio = false;

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(PINO_PWM    , OUTPUT);
  pinMode(PINO_REVERSO, OUTPUT);
  pinMode(PINO_FREIO  , OUTPUT);
  pinMode(PINO_PWM_DIR    , OUTPUT);
  pinMode(PINO_REVERSO_DIR, OUTPUT);
  pinMode(PINO_FREIO_DIR  , OUTPUT);
  pwd_control_esq     = 43;
  pwd_control_dir     = 85;
  pwd_control_esq_rev = 43;
  pwd_control_dir_rev = 85;

}

void loop() {
  controleRPM(wheel);

  if (Serial.available()) {
    read_value = Serial.read();
  }
  switch (read_value) {
    case '1':
      freio = false;
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      delay(300);
      digitalWrite(PINO_REVERSO, LOW);
      digitalWrite(PINO_REVERSO_DIR, LOW);
      delay(300);
      analogWrite(PINO_PWM, 48);
      analogWrite(PINO_PWM_DIR, 83);
      break;
    case '2':
      freio = false;
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      delay(300);
      digitalWrite(PINO_REVERSO, HIGH);
      digitalWrite(PINO_REVERSO_DIR, HIGH);
      delay(300);
      analogWrite(PINO_PWM, 30);
      analogWrite(PINO_PWM_DIR, 85);
      break;
    case '3':
      freio = true;
      Movimenta(freio);
      digitalWrite(PINO_FREIO, 0);
      digitalWrite(PINO_FREIO_DIR, 0);
      delay(500);
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      break;
  }
  delay(100);
}

void Movimenta(bool freio) {
  if (freio == true) {
    analogWrite(PINO_PWM, 0);
    analogWrite(PINO_PWM_DIR, 0);
  }
  read_value = 0;
}

void receiveEvent(int howMany) {
  String Receive_Data;
  while (Wire.available()) {   // loop through all but the last
    Receive_Data += char(Wire.read());       // receive byte as a character
  }
  char c[sizeof(Receive_Data) + 1];
  Receive_Data.toCharArray(c, sizeof(c));
  Serial.println(c);
  char* token = strtok(c, "|");
  int i = 0;
  while (token != NULL) {
    i++;
    if (i == 1) {
      wheel = atoi(token);
    }
    else if (i == 2) {
      if (wheel == 'D') {
        dir_dir = atoi(token);
      }
      else {
        dir_esq =  atoi(token);
      }
    }
    else if (i == 3) {
      if (wheel == 'D') {
        rpm_dir = atoi(token);
      }
      else {
        rpm_esq = atoi(token);
      }
    }

    token = strtok(NULL, "|");
  }
}

void controleRPM(char wheel) {
  if (wheel == 'D') {
    if (dir_dir = 1 && rpm_dir < 40 ){
       analogWrite(PINO_PWM_DIR, pwd_control_dir++);  
    }
    else if(dir_dir = 0 && rpm_dir < 40){
      analogWrite(PINO_REVERSO_DIR, pwd_control_dir_rev++);
    }
  
  }
  else if (wheel == 'E') {
     if (dir_esq = 0 && rpm_esq < 40 ){
      analogWrite(PINO_PWM, pwd_control_esq++);
    }
    else if(dir_esq = 1 && rpm_esq < 40){
      analogWrite(PINO_REVERSO, pwd_control_esq_rev++);
    }
  }
}
