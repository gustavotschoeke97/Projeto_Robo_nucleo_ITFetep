#include <unistd.h>
#include <time.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <softPwm.h>

#define atv 7

int main(void){
	wiringPiSetup();
	pinMode(atv, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(26, OUTPUT);
	softPwmCreate(24, 0, 100);
	digitalWrite(atv, 1);
	usleep(100000);
	digitalWrite(5, 1);
	usleep(10000);
	softPwmWrite(24, 40);
	for(;;);
}
