#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
//#include <wiringPi.h>
#include <pthread.h>
//#include <softPwm.h>
#include <signal.h>
#include <wiringSerial.h>
//#include <fstream>
#include "/usr/include/stdint.h"

#define _pin_Atv 7
#define _pin_Acel_Dir 24
#define _pin_Acel_Esq 26
#define _pin_Freio 5
#define _pin_Rev_Esq 4
#define _pin_Rev_Dir 6

#define Atv 1
#define freia 0
#define frt_esq 0
#define frt_dir 0

bool parado=true;
bool dir_frente=false;
bool dir_tras=false;
bool dir_direita=false;
bool dir_esquerda=false;

int ace_ini_esq_frt=32;
int ace_ini_dir_frt=35;
int ace_fim_esq_frt=73;
int ace_fim_dir_frt=43;
int ace_ini_esq_trz=30;
int ace_ini_dir_trz=35;
int ace_fim_esq_trz=39;
int ace_fim_dir_trz=40;
int ace_esq;
int ace_dir;


//pthread_t ace_es, ace_di, g_e, g_d;

#define _pin_pul 27
#define _pin_dir 28
#define _pin_en 29

int ini_socket();
bool giro_dir_max=false;
bool giro_esq_max=false;
/*void ativar(){
    digitalWrite(_pin_Atv, 1);
    usleep(200000);
}
void desativar(){
    digitalWrite(_pin_Atv, 0);
    usleep(200000);
}
void frear(){
    digitalWrite(_pin_Freio, 0);
    usleep(1000000);
    parado=true;
}
void libera(){
    digitalWrite(_pin_Freio, 1);
    usleep(400000);
}
void f_dir(){
    digitalWrite(_pin_Rev_Dir, 0);
    usleep(200000);
}
void t_dir(){
    digitalWrite(_pin_Rev_Dir, 1);
    usleep(200000);
}
void f_esq(){
    digitalWrite(_pin_Rev_Esq, 0);
    usleep(200000);
}
void t_esq(){
    digitalWrite(_pin_Rev_Esq, 1);
    usleep(200000);
}
void * gira_esq(void *arg){
            digitalWrite(_pin_en, 0);
        usleep(10000);
        digitalWrite(_pin_dir, 0);
        usleep(10000);
        for(int i=0; i<100; i++){
            digitalWrite(_pin_pul, 0);
            usleep(8000);
            digitalWrite(_pin_pul, 1);
            usleep(8000);
        }
        pthread_exit(NULL);
}
void * gira_dir(void *arg){
            digitalWrite(_pin_en, 0);
        usleep(10000);
        digitalWrite(_pin_dir, 1);
        usleep(10000);
        for(int i=0; i<100; i++){
            digitalWrite(_pin_pul, 0);
            usleep(8000);
            digitalWrite(_pin_pul, 1);
            usleep(8000);
        }
        pthread_exit(NULL);
}
void * acelera_esq(void *arg){
    parado=false;
    if(dir_frente || dir_direita){
        ace_esq=ace_ini_esq_frt;
        for(int i=ace_ini_esq_frt; i<ace_fim_esq_frt; i++){
            softPwmWrite(_pin_Acel_Esq, ace_esq++);
            usleep(15000);
        }
    }else{
        ace_esq=ace_ini_esq_trz;
        for(int i=ace_ini_esq_trz; i<ace_fim_esq_trz; i++){
            softPwmWrite(_pin_Acel_Esq, ace_esq++);
        }
    }
    pthread_exit(NULL);
}
void * acelera_dir(void *arg){
    parado=false;
    if(dir_frente || dir_esquerda){
        ace_dir=ace_ini_dir_frt;
        for(int i=ace_ini_dir_frt; i<ace_fim_dir_frt; i++){
            softPwmWrite(_pin_Acel_Dir, ace_dir++);
            usleep(15000);
        }
    }else{
        ace_dir=ace_ini_dir_trz;
        for(int i=ace_ini_dir_trz; i<ace_fim_dir_trz; i++){
            softPwmWrite(_pin_Acel_Dir, ace_dir++);
        }
    }
    pthread_exit(NULL);
}
void frente(){
    dir_frente=true;
    f_dir();
    f_esq();
    pthread_create(&ace_es, NULL, acelera_esq, NULL);
    pthread_create(&ace_di, NULL, acelera_dir, NULL);
}
void tras(){
    dir_tras=true;
    t_dir();
    t_esq();
    pthread_create(&ace_es, NULL, acelera_esq, NULL);
    pthread_create(&ace_di, NULL, acelera_dir, NULL);
}
void direita(){
    dir_direita=true;
    t_dir();
    f_esq();
    pthread_create(&ace_es, NULL, acelera_esq, NULL);
    pthread_create(&ace_di, NULL, acelera_dir, NULL);
}
void esquerda(){
    dir_esquerda=true;
    f_dir();
    t_esq();
    pthread_create(&ace_es, NULL, acelera_esq, NULL);
    pthread_create(&ace_di, NULL, acelera_dir, NULL);
}
void parar(){
    pthread_cancel(ace_es);
    pthread_cancel(ace_di);
    softPwmWrite(_pin_Acel_Dir, 0);
    softPwmWrite(_pin_Acel_Esq, 0);
    dir_direita=false;
    dir_esquerda=false;
    dir_frente=false;
    dir_tras=false;
    usleep(300000);
    //frear();
    libera();
}
void init(){
    wiringPiSetup();
    pinMode(_pin_Atv, OUTPUT);
    pinMode(_pin_Acel_Dir, OUTPUT);
    pinMode(_pin_Acel_Esq, OUTPUT);
    pinMode(_pin_Freio, OUTPUT);
    pinMode(_pin_Rev_Dir, OUTPUT);
    pinMode(_pin_Rev_Esq, OUTPUT);
    pinMode(_pin_pul, OUTPUT);
    pinMode(_pin_dir, OUTPUT);
    pinMode(_pin_en, OUTPUT);
    digitalWrite(_pin_en, 1);
    ativar();
    libera();
    f_dir();
    f_esq();
    softPwmCreate(_pin_Acel_Dir, 0, 100);
    softPwmCreate(_pin_Acel_Esq, 0, 100);
    usleep(1000000);
}*/
int main(){

    init();
    //ini_socket();
    int i;
    while(1){
        scanf("%d", &i);
        if(i == 8){
            if(!parado)
                parar();
            frente();
        }else if(i==4){
            if(!parado)
                parar();
            esquerda();
        }else if(i==6){
            if(!parado)
                 parar();
            direita();
        }else if(i==2){
            if(!parado)
                parar();
            tras();
        }else if(i==7){
            pthread_create(&g_e, NULL, gira_esq, NULL);
        }else if(i==9){
            pthread_create(&g_d, NULL, gira_dir, NULL);
        }else if(i==1){
            if(!parado)
                parar();
            t_esq();
            for(int i=0; i<60; i++){
                printf("%d\n",i);
               // softPwmWrite(_pin_Acel_Esq, i);
                usleep(1000000);
            }
        }else if(i==3){
            if(!parado)
                parar();
            t_dir();
            for(int i=0; i<60; i++){
                printf("%d\n", i);
               //s softPwmWrite(_pin_Acel_Dir, i);
                usleep(1000000);
            }
        }
        else{
            parar();
        }
    }
}
/*int ini_socket(){
    struct sockaddr_in server;
    struct sockaddr_in client;
    int cliente;
    char buffer_in[4], buffer_out[1024];
    socklen_t addr_size;

    int sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock==-1){
        return 1;
    }
    memset(&server, '\0', sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(8000);
    server.sin_addr.s_addr=inet_addr("192.168.0.6");
    int bd=bind(sock, (struct sockaddr*)&server, sizeof(server));
    if(bd<0){
        return 2;
    }

    listen(sock, 1);

    while(1){

        cliente = accept(sock, (struct sockaddr*)&client, &addr_size);
        usleep(10000);
        memset(buffer_out, '\0', sizeof(buffer_out));
        memset(buffer_in, '\0', sizeof(buffer_in));
        if((recv(cliente, buffer_in, 4, 0))>0){
            printf("%s\n", buffer_in);
        }
        if(strcmp(buffer_in,"1UP")==0){
            if(!parado){
                parar();
            }
            frente();
        }else if(strcmp(buffer_in,"0UP")==0){
            parar();
        }else if(strcmp(buffer_in,"1LF")==0){
            if(!parado){
                parar();
            }
            esquerda();
        }else if(strcmp(buffer_in,"0LF")==0){
            parar();
        }else if(strcmp(buffer_in,"1RG")==0){
            if(!parado){
                parar();
            }
            direita();
        }else if(strcmp(buffer_in,"0RG")==0){
            parar();
        }else if(strcmp(buffer_in,"1DW")==0){
            if(!parado){
                parar();
            }
            tras();
        }else if(strcmp(buffer_in,"0DW")==0){
            parar();
        }else if(strcmp(buffer_in,"90L")==0){
            if(!giro_esq_max){
                pthread_create(&g_e, NULL, gira_esq, NULL);
                giro_esq_max=true;
            }
        }else if(strcmp(buffer_in,"90C")==0){
            if(giro_dir_max){
                pthread_create(&g_e, NULL, gira_esq, NULL);
                giro_dir_max=false;
            }else if(giro_esq_max){
                pthread_create(&g_d, NULL, gira_dir, NULL);
                giro_esq_max=false;
            }
        }else if(strcmp(buffer_in,"90R")==0){
            if(!giro_dir_max){
                pthread_create(&g_d, NULL, gira_dir, NULL);
                giro_dir_max=true;
            }
        }
        close(cliente);
    }
    close(sock);
}*/
