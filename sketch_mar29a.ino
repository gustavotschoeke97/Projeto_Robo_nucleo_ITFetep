#define PINO_PWM     3      //pino de aceleração
#define PINO_FREIO   4
#define PINO_REVERSO 5      // pino inverte rotação
#define TEMPO        50  //tempo (ms) em que o motor gira
#define PINO_FREIO_DIR 7
#define PINO_REVERSO_DIR 8
#define PINO_PWM_DIR 9
char read_value;
bool freio = false;
void setup() {
  Serial.begin(9600);
  pinMode(PINO_PWM    , OUTPUT);
  pinMode(PINO_REVERSO, OUTPUT);
  pinMode(PINO_FREIO  , OUTPUT);
  pinMode(PINO_PWM_DIR    , OUTPUT);
  pinMode(PINO_REVERSO_DIR, OUTPUT);
  pinMode(PINO_FREIO_DIR  , OUTPUT);
}

void loop() {
  //Aumento de velocidade (0%..100%)
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
      digitalWrite(PINO_REVERSO, HIGH);
      delay(100);
      Movimenta(freio);
      break;
    case '3':
      freio = true;
      Movimenta(freio);
      digitalWrite(PINO_REVERSO_DIR, 1);
      digitalWrite(PINO_FREIO, 0);
      break;
  }
}

void Movimenta(bool freio) {
  float duty_cicle = 0;
  if (freio == true) {
    for (int i = 50; i > 0; i--) {  
      analogWrite(PINO_PWM, i);
      analogWrite(PINO_PWM_DIR, i);
      delay(TEMPO);
      if (i > 0 ) {
        duty_cicle = (i / 256) * 100;
        Serial.print("Valor do PWM: ");
        Serial.println(duty_cicle,DEC);
      }
    }
  }
  else {
      for (int i = 0; i < 80; i++) {      //acelera para frente
        analogWrite(PINO_PWM, i);
        analogWrite(PINO_PWM_DIR, i);
        delay(TEMPO);
        if (i > 0 ) {
           duty_cicle = (i/256);
           duty_cicle = duty_cicle * 100;
           Serial.print("Valor do PWM: ");
           Serial.println(duty_cicle,DEC);
        }
      }   
      read_value=0;
  }
}
    

