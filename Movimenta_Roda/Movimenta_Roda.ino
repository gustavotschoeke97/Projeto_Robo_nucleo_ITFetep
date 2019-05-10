// módulo esquerdo DIRECAO = 0 vai para frente DIRECAO = 1 para trás | módulo  Direito  DIRECAO = 1 vai para frente DIRECAO = 0 para trás

#include <Wire.h>
#include <Thread.h>
#include <string.h>

#define PINO_PWM         3                   // pino de aceleração
#define PINO_FREIO       4
#define PINO_REVERSO     5                  //  pino inverte rotação
#define TEMPO            100               //   tempo (ms) em que o motor gira
#define PINO_FREIO_DIR   8
#define PINO_REVERSO_DIR 7
#define PINO_PWM_DIR     9
#define PWM_DIR          83
#define PWM_ESQ          90
#define PWM_DIR_REV      83
#define PWM_ESQ_REV      45

char read_value;
String wheel;
int  dir_esq, dir_dir, pwd_control_esq, pwd_control_esq_rev, pwd_control_dir, pwd_control_dir_rev;
int  rpm_esq, rpm_dir;
bool freio       = false;
bool lib_control = false;
Thread t[4];


void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  pinMode(PINO_PWM        , OUTPUT);
  pinMode(PINO_REVERSO    , OUTPUT);
  pinMode(PINO_FREIO      , OUTPUT);
  pinMode(PINO_PWM_DIR    , OUTPUT);
  pinMode(PINO_REVERSO_DIR, OUTPUT);
  pinMode(PINO_FREIO_DIR  , OUTPUT);
  
  t[0].onRun(controleRPM);
  t[0].setInterval(50);
  t[1].onRun(controleRPM);
  t[1].setInterval(50);
  t[2].onRun(FrenteDir);
  t[2].setInterval(75);
  t[3].onRun(FrenteEsq);
  t[3].setInterval(50);
}

void loop() {

  if (lib_control == true) {
    if (wheel == "D") {
      t[0].run();
    }
    else {
      t[1].run();
    }
  }

  if (Serial.available() > 0) {
    read_value = char(Serial.read());
    //Serial.println(read_value);
  }
  switch (read_value) {
    case '1': // girar para frente
      pwd_control_esq     = 332;
      pwd_control_dir     = 70;
      freio = false;
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      delay(300);
      digitalWrite(PINO_REVERSO, LOW);
      digitalWrite(PINO_REVERSO_DIR, LOW);
      delay(500);
      t[2].run();
      t[3].run();
      

      /*for(int i = 0;i< 90;i++){
          if(pwd_control_dir == PWM_DIR ){
            continue;
          }

          else if( pwd_control_dir  < PWM_DIR ){
            analogWrite(PINO_PWM_DIR, pwd_control_dir++);
          }

          if(pwd_control_esq == PWM_ESQ ){
            continue;
          }
          else if( pwd_control_esq  < PWM_ESQ ){
            analogWrite(PINO_PWM, pwd_control_esq+=6);
          }

          if(pwd_control_dir >= PWM_DIR && pwd_control_esq  >= PWM_ESQ){
            break;
          }
          delay(70);
        }*/

      lib_control = true;
      read_value = 0;
      break;

    case '2': // girar para tras
      pwd_control_esq_rev = 74;
      pwd_control_dir_rev = 46;
      freio = false;
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      delay(300);
      digitalWrite(PINO_REVERSO, HIGH);
      digitalWrite(PINO_REVERSO_DIR, HIGH);
      delay(300);
      analogWrite(PINO_PWM, pwd_control_esq_rev);
      analogWrite(PINO_PWM_DIR, pwd_control_dir_rev);
      lib_control = true;
      read_value = 0;
      break;

    case '3': // freiar tudo
      freio = true;
      read_value = Movimenta(freio);
      digitalWrite(PINO_FREIO, 0);
      digitalWrite(PINO_FREIO_DIR, 0);
      while (rpm_dir > 0 && rpm_esq > 0) {
        Serial.println("ta no while");
      }
      Serial.println("Saiu");
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      lib_control = false;
      read_value = 0;
      break;

    case '4':// virar esquerda
      freio = true;
      read_value = Movimenta(freio);
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      analogWrite(PINO_PWM, pwd_control_esq);
      lib_control = false;
      break;

    case '5': // virar direita
      freio = true;
      read_value = Movimenta(freio);
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      lib_control = false;
      break;
  }
  delay(100);
}

int Movimenta(bool freio) {
  if (freio == true) {
    analogWrite(PINO_PWM, 0);
    analogWrite(PINO_PWM_DIR, 0);
    return  0;
  }
  return -1;
}

void receiveEvent(int howMany) {

  String Receive_Data;
  while (Wire.available()) {                  // loop through all but the last
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
      wheel = token;
    }
    else if (i == 2) {
      if (wheel == "D") {
        dir_dir = atoi(token);
      }
      else {
        dir_esq =  atoi(token);
      }
    }
    else if (i == 3) {
      if (wheel == "D") {
        rpm_dir = atoi(token);
      }
      else {
        rpm_esq = atoi(token);
      }
    }

    token = strtok(NULL, "|");
  }
}

void controleRPM() {
  if (wheel == "D") {
    if (dir_dir == 1 ) {
      if (rpm_dir < 50 ) {
        analogWrite(PINO_PWM_DIR, pwd_control_dir_rev++);
        delay(100);
      }
      else if (rpm_dir > 50 ) {
        analogWrite(PINO_PWM_DIR, pwd_control_dir_rev--);
        delay(100);
      }
    }
    else if (dir_dir == 0 ) {
      if (rpm_dir < 50) {
        analogWrite(PINO_PWM_DIR, pwd_control_dir++);
        delay(100);
      }
    }
    else if (rpm_dir > 50) {
      analogWrite(PINO_PWM_DIR, pwd_control_dir--);
      delay(100);
    }
  }
  else if (wheel == "E") {
    if (dir_esq == 0) {
      if (rpm_esq < 50 ) {
        analogWrite(PINO_PWM, pwd_control_esq_rev++);
        delay(200);
      }
      else if (rpm_esq > 50 ) {
        analogWrite(PINO_PWM, pwd_control_esq_rev--);
        delay(100);
      }
    }
    else if (dir_esq == 1) {
      if ( rpm_esq < 50) {
        analogWrite(PINO_PWM, pwd_control_esq++);
        delay(100);
      }
      else if (rpm_esq > 50) {
        analogWrite(PINO_PWM, pwd_control_esq--);
        delay(100);
      }
    }
  }
}


void FrenteDir() {
  for (int i = 0; i <= PWM_DIR; i++) {
    if (pwd_control_dir >= PWM_DIR) {
      break;
    }
    else {
      analogWrite(PINO_PWM_DIR, pwd_control_dir++);
    }
    delay(75);

  }

  t[2].enabled = false;

}

void FrenteEsq() {
  for (int i = 0; i <= PWM_ESQ; i++) {
    if (pwd_control_esq >= PWM_ESQ) {
      break;
    }
    else {
      analogWrite(PINO_PWM, pwd_control_esq++);
    }

    delay(1);
  }

  t[3].enabled = false;
}
