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
#define PWM_ESQ          70
#define PWM_DIR_REV      83
#define PWM_ESQ_REV      45

char read_value;
String wheel;
int  dir_esq, dir_dir, pwd_control_esq, pwd_control_esq_rev, pwd_control_dir, pwd_control_dir_rev;
int  rpm_esq, rpm_dir;
bool freio       = false;
bool lib_control = false;
Thread        t1 = Thread();
Thread        t2 = Thread();


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
  t1.onRun(controleRPM);
  t1.setInterval(50);
  t2.onRun(controleRPM);
  t2.setInterval(50);
}

void loop() {
  
  if (lib_control == true) {
    if (wheel == "D") {
      t1.run();
    }
    else {
      t2.run();
    }
  }
  
  if (Serial.available() > 0) {
    read_value = char(Serial.read());
    //Serial.println(read_value);
  }
  switch (read_value) {
    case '1': // girar para frente
      pwd_control_esq     = PWM_ESQ;        
      pwd_control_dir     = PWM_DIR;       
      freio = false;
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      delay(300);
      digitalWrite(PINO_REVERSO, LOW);
      digitalWrite(PINO_REVERSO_DIR, LOW);
      delay(300);
      analogWrite(PINO_PWM, pwd_control_esq);
      analogWrite(PINO_PWM_DIR, pwd_control_dir);
      lib_control = true;
      read_value = 0;
      break;
      
    case '2': // girar para tras
      pwd_control_esq_rev = PWM_ESQ_REV;  
      pwd_control_dir_rev = PWM_DIR_REV; 
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
      delay(500);
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      lib_control = false;
      
      break;
      
     case '4':// virar esquerda
      freio = true;
      read_value = Movimenta(freio);
      digitalWrite(PINO_FREIO, 1);
      digitalWrite(PINO_FREIO_DIR, 1);
      analogWrite(PINO_PWM,pwd_control_esq); 
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
        delay(100);
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
