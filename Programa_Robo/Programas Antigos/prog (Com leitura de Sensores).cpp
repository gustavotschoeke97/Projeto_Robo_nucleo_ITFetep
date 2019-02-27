#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <wiringPi.h>
#include <pthread.h>
#include <softPwm.h>
#include <signal.h>
#include <wiringSerial.h>
#include <fstream>
#include "/usr/include/stdint.h"

#define Atv 1
#define Lib 1
#define Frt 0
#define _pin_Atv 4
#define _pin_Ace_dir 26
#define _pin_Fre_dir 27
#define _pin_Rev_dir 28
#define _pin_Ace_esq 24
#define _pin_Fre_esq 22
#define _pin_Rev_esq 23

/*..........var distancias..........*/
pthread_t dist;
volatile int dist_dir, dist_esq;
int sd;
bool thread_dist=false;
/*...............................*/

/*.............var acelerar......*/
#define ace_ini_dir 0
#define ace_ini_esq 0
int ace_fim_dir_frt = 98;
int ace_fim_esq_frt = 81;
int ace_fim_dir_esq = 99;
int ace_fim_esq_esq = 53;
int ace_fim_dir_dir = 97;
int ace_fim_esq_dir = 85;
int ace_fim_dir_trz = 99;
int ace_fim_esq_trz = 52;
int ace_normal_esq=80;
int ace_normal_dir=90;
int ace_fim_dir;
int ace_fim_esq;
#define ace_ini 0
#define ace_fim 100
int const_ace_dir = 6;
int const_ace_esq = 1;
#define time_ace_ini 100000    //useconds
float const_ace = 0.05;        
int aceleracao_dir;
int aceleracao_esq;
pthread_t ac;
volatile bool thread_ace = false;
volatile bool parado = true;
/*...............................*/

/*............ thread emergencia*/
pthread_t em, aj;
int dist_e[3];
int dist_d[3];
bool thread_emer;
int dist_emer = 1000;
bool emer=false;
volatile bool thread_aj=false;
/*...............................*/
/*metodos..................*/
int init();
void ativar(int com);
void parar();
void frente();
void acel();
void direita();
void esquerda();
void traz();
void * acelerar(void *arg);
/*..........................*/

/*var movi*/
volatile bool m_frente=false;
volatile bool ajuste=false;
volatile bool m_traz=false;
volatile bool m_esq=false;
volatile bool m_dir=false;
volatile bool re=false;

volatile int batt_m;
volatile float batt_c;

void * auxi(void *arg){
    int i;
    float k;
    while(1){
        scanf("%d", &i);
        if(i==1){
            acel();
        }else if(i==0){
            parar();
            m_frente=false;
            m_traz=false;
            m_esq=false;
            m_dir=false;
        }else if(i==5){
            parar();
            scanf("%f", &k);
            const_ace=k;
        }else if(i==6){
            parar();
            scanf("%d", &i);
            const_ace_dir=i;
        }else if(i==7){
            parar();
            scanf("%d", &i);
            const_ace_esq=i;
        }else if(i==8){
            printf("Ac Fin: Dir %d Esq %d\nConstantes Roda: Dir %d Esq %.d\nconst acele tmp %.2f\n", ace_fim_dir, ace_fim_esq, const_ace_dir, const_ace_esq, const_ace);
        }else if(i==9){
            esquerda();
        }
        else if(i==10){
            direita();
        }else if(i==11){
            traz();
        }else if(i==12){
            frente();
        }
    }
    pthread_exit(NULL);
}

void * emergencia(void *arg){
    thread_emer=true;
    int contdir=0;
    int contesq=0;
    for(;;){
        contdir=0;
        contesq=0;
        if(m_frente){
            if(dist_dir<dist_emer){
                while(dist_dir<dist_emer){
                    if(contdir<3){
                        dist_d[contdir]=dist_dir;
                        contdir++;
                        usleep(10000);
                    }else{
                        emer=true;
                        contdir=0;
                    }
                }
            }else{
                ajuste=false;
                emer=false;
                contdir=0;
            }
            if(dist_esq<dist_emer){
                while(dist_esq<dist_emer){
                    if(contesq<3){
                        dist_e[contesq]=dist_esq;
                        contesq++;
                        usleep(10000);
                    }else{
                        emer=true;
                        contesq=0;
                    }
                }
            }else{
                ajuste=false;
                emer=false;
                contdir=0;
            }
            
        }

    }
    thread_emer=false;
    pthread_exit(NULL);
}
void * acelerar(void *arg){
    thread_ace=true;

    if(m_frente){
        ace_fim_dir=ace_fim_dir_frt;
        ace_fim_esq=ace_fim_esq_frt;
    }else if(m_traz){
        ace_fim_dir=ace_fim_dir_trz;
        ace_fim_esq=ace_fim_esq_trz;
    }else if(m_esq){
        ace_fim_dir=ace_fim_dir_esq;
        ace_fim_esq=ace_fim_esq_esq;
    }else if(m_dir){
        ace_fim_dir=ace_fim_dir_dir;
        ace_fim_esq=ace_fim_esq_dir;
    }

    int ace_dir = ace_ini_dir;
    int ace_esq = ace_ini_esq;

    /*for(int i=ace_ini; i<=ace_fim; i++){
        if((ace_esq=i*const_ace_esq)<=ace_fim_esq){
            softPwmWrite(_pin_Ace_esq, ace_esq);
        }else{
            ace_esq=ace_fim_esq;
            softPwmWrite(_pin_Ace_esq, ace_esq);
        }
        if((ace_dir=i*const_ace_dir)<=ace_fim_dir){
            softPwmWrite(_pin_Ace_dir, ace_dir);
        }else{
            ace_dir=ace_fim_dir;
            softPwmWrite(_pin_Ace_dir, ace_dir);
        }
        usleep(i/const_ace);
    }
    usleep(time_ace_ini);*/
    
    for(;;){
        if(m_frente){
            if(emer){
                if(!parado){
                    if(dist_dir < 1000 && dist_esq < 1000){
                        softPwmWrite(_pin_Ace_dir, 0);
                        softPwmWrite(_pin_Ace_esq, 0);
                        usleep(50000);
                        digitalWrite(_pin_Fre_dir, !Lib);
                        digitalWrite(_pin_Fre_esq, !Lib);
                        sleep(1);
                        digitalWrite(_pin_Rev_dir, 0);
                        digitalWrite(_pin_Rev_esq, 0);
                        usleep(100000);
                        digitalWrite(_pin_Fre_dir, Lib);
                        digitalWrite(_pin_Fre_esq, Lib);
                        usleep(50000);
                        parado=true;
                        ajuste=true;
                        usleep(1000);
                    }else if(dist_dir < 400 || dist_esq < 400){
                        softPwmWrite(_pin_Ace_dir, 0);
                        softPwmWrite(_pin_Ace_esq, 0);
                        usleep(50000);
                        digitalWrite(_pin_Fre_dir, !Lib);
                        digitalWrite(_pin_Fre_esq, !Lib);
                        sleep(1);
                        digitalWrite(_pin_Rev_dir, 0);
                        digitalWrite(_pin_Rev_esq, 0);
                        usleep(100000);
                        digitalWrite(_pin_Fre_dir, Lib);
                        digitalWrite(_pin_Fre_esq, Lib);
                        usleep(50000);
                        parado=true;
                        ajuste=true;
                        usleep(1000);
                    }
                }else{
                    if(ajuste && dist_dir<700 && dist_esq < 700){
                        if(dist_dir < 700 && dist_esq <700 && parado){
                            digitalWrite(_pin_Rev_dir, 1);
                            digitalWrite(_pin_Rev_esq, 1);
                            usleep(100000);
                            m_traz=true;
                            while(dist_dir < 750 && dist_esq < 750){
                                if(parado){
                                    for(int i=ace_ini; i<=ace_fim; i++){
                                        if((ace_esq=i*const_ace_esq)<=ace_fim_esq_trz){
                                            softPwmWrite(_pin_Ace_esq, ace_esq);
                                        }else{
                                            ace_esq=ace_fim_esq_trz;
                                            softPwmWrite(_pin_Ace_esq, ace_esq);
                                        }
                                        if((ace_dir=i*const_ace_dir)<=ace_fim_dir_trz){
                                            softPwmWrite(_pin_Ace_dir, ace_dir);
                                        }else{
                                            ace_dir=ace_fim_dir_trz;
                                            softPwmWrite(_pin_Ace_dir, ace_dir);
                                        }
                                        usleep((i*10)/const_ace);
                                    }
                                    parado=false;
                                }
                            }
                            usleep(10000);
                            softPwmWrite(_pin_Ace_dir, 0);
                            softPwmWrite(_pin_Ace_esq, 0);
                            usleep(100000);
                            digitalWrite(_pin_Fre_dir, !Lib);
                            digitalWrite(_pin_Fre_esq, !Lib);
                            sleep(1);
                            digitalWrite(_pin_Rev_dir, 0);
                            digitalWrite(_pin_Rev_esq, 0);
                            usleep(100000);
                            digitalWrite(_pin_Fre_dir, Lib);
                            digitalWrite(_pin_Fre_esq, Lib);
                            usleep(50000);
                            m_traz=false;
                            parado=true;
                            re=false;
                        }
                    }else if(ajuste && dist_dir>dist_esq){
                        while(emer){
                            if(parado==true){
                                for(int i=ace_ini; i<=ace_fim; i++){
                                    if((ace_esq=i*const_ace_esq)<=ace_fim_esq){
                                        softPwmWrite(_pin_Ace_esq, ace_esq);
                                    }else{
                                        ace_esq=ace_fim_esq;
                                        softPwmWrite(_pin_Ace_esq, ace_esq);
                                    }
                                    usleep((i*10)/const_ace);
                                }
                            }
                            parado=false;
                        }
                        softPwmWrite(_pin_Ace_esq, ace_esq=0);
                        parado=true;
                    }else if(ajuste && dist_dir<dist_esq){
                        while(emer){
                            if(parado==true){
                                for(int i=ace_ini; i<=ace_fim; i++){
                                    if((ace_dir=i*const_ace_dir)<=ace_fim_dir){
                                        softPwmWrite(_pin_Ace_dir, ace_dir);
                                    }else{
                                        ace_dir=ace_fim_dir;
                                        softPwmWrite(_pin_Ace_dir, ace_dir);
                                    }
                                    usleep((i*10)/const_ace);
                                }
                            }
                            parado=false;
                        }
                        softPwmWrite(_pin_Ace_dir, ace_dir=0);
                        parado=true;
                    }
                }                
            }else{
                if(parado){
                    for(int i=ace_ini; i<=ace_fim; i++){
                        if((ace_esq=i*const_ace_esq)<=ace_fim_esq){
                            softPwmWrite(_pin_Ace_esq, ace_esq);
                        }else{
                            ace_esq=ace_fim_esq;
                            softPwmWrite(_pin_Ace_esq, ace_esq);
                        }
                        if((ace_dir=i*const_ace_dir)<=ace_fim_dir){
                            softPwmWrite(_pin_Ace_dir, ace_dir);
                        }else{
                            ace_dir=ace_fim_dir;
                            softPwmWrite(_pin_Ace_dir, ace_dir);
                        }
                        if(emer){
                            break;
                        }
                        usleep(i*10/const_ace);
                        parado=false;
                    }
                    usleep(10000);
                    ace_esq=ace_normal_esq;
                    ace_dir=ace_normal_dir;
                    softPwmWrite(_pin_Ace_esq, ace_esq);
                    softPwmWrite(_pin_Ace_dir, ace_dir);
                    parado=false;
                }else{
                    if(dist_esq>1000 && dist_esq < 1300 && dist_dir>1000 && ace_dir>10){
                        softPwmWrite(_pin_Ace_dir, ace_dir--);
                        usleep(10000);
                    }else if(dist_esq > 1200){
                        ace_dir=ace_fim_dir;
                        softPwmWrite(_pin_Ace_dir, ace_dir);
                    }
                    if(dist_dir>1000 && dist_dir < 1300 && dist_esq>1000 && ace_esq>10){
                        softPwmWrite(_pin_Ace_esq, ace_esq--);
                        usleep(10000);
                    }else if(dist_dir > 1200){
                        ace_esq=ace_fim_esq;
                        softPwmWrite(_pin_Ace_esq, ace_esq);
                    }
                    parado=false;
                }
            }
        }else{
            if(parado){
                for(int i=ace_ini; i<=ace_fim; i++){
                    if((ace_esq=i*const_ace_esq)<=ace_fim_esq){
                        softPwmWrite(_pin_Ace_esq, ace_esq);
                    }else{
                        ace_esq=ace_fim_esq;
                        softPwmWrite(_pin_Ace_esq, ace_esq);
                    }
                    if((ace_dir=i*const_ace_dir)<=ace_fim_dir){
                        softPwmWrite(_pin_Ace_dir, ace_dir);
                    }else{
                        ace_dir=ace_fim_dir;
                        softPwmWrite(_pin_Ace_dir, ace_dir);
                    }
                    usleep((i+10)/const_ace);
                    parado=false;
                }
            }
        }
    }
    thread_ace=false;
    pthread_exit(NULL);
}
void * distancias(void *arg){
    thread_dist=true;
    for(;;){
        fflush(stdout);
        usleep(1000);
        serialPutchar(sd, 'e');
        int len=0;
        if((len=serialDataAvail(sd))>0){
            char r[5];
            for(int u=0; u<=4; u++){
                r[u]=serialGetchar(sd);
            }
            serialFlush(sd);
            dist_esq=abs(atoi(r));
        }
        usleep(50000);
        fflush(stdout);
        usleep(1000);
        serialPutchar(sd, 'd');
        len=0;
        if((len=serialDataAvail(sd))>0){
            char r[5];
            for(int u=0; u<=4; u++){
                r[u]=serialGetchar(sd);
            }
            serialFlush(sd);
            dist_dir=abs(atoi(r));
        }
        usleep(5000);
        fflush(stdout);
        usleep(1000);
        serialPutchar(sd, 'm');
        len=0;
        if((len=serialDataAvail(sd))>0){
            char r[5];
            for(int u=0; u<=4; u++){
                r[u]=serialGetchar(sd);
            }
            serialFlush(sd);
           batt_m=abs(atoi(r));
        }
        usleep(5000);
        fflush(stdout);
        usleep(1000);
        serialPutchar(sd, 'c');
        len=0;
        if((len=serialDataAvail(sd))>0){
            char r[5];
            for(int u=0; u<=4; u++){
                r[u]=serialGetchar(sd);
            }
            serialFlush(sd);
           batt_c=atof(r);
        }

    
    usleep(50000);
    //printf("Dir %d Esq %d\n", dist_dir, dist_esq);
    }
    thread_dist=false;
    pthread_exit(NULL);
}
int main(void){
    init();
    pthread_create(&dist, NULL, distancias, NULL);
    pthread_create(&em, NULL, emergencia, NULL);
    pthread_t au;
    pthread_create(&au, NULL, auxi, NULL);
    while(1){

    }
    return 0;
}
int ini_socket(){
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
    server.sin_port=htons(5000);
    server.sin_addr.s_addr=inet_addr("192.168.0.106");
    int bd=bind(sock, (struct sockaddr*)&server, sizeof(server));
    if(bd<0){
        return 2;
    }

    listen(sock, 1);
    
    while(1){
        
        cliente = accept(sock, (struct sockaddr*)&client, &addr_size);
        ativar(Atv);
        sleep(1);
        memset(buffer_out, '\0', sizeof(buffer_out));
        usleep(1000);
        sprintf(buffer_out, "%d\n%f\n", batt_m, batt_c);
        send(cliente, buffer_out, sizeof(buffer_out),0);
        usleep(10000);
        memset(buffer_out, '\0', sizeof(buffer_out));
        memset(buffer_in, '\0', sizeof(buffer_in));
        if((recv(cliente, buffer_in, 4, 0))>0){
            printf("%s\n", buffer_in);
        }
        close(cliente);
    }
    close(sock);
}
int init(){
    wiringPiSetup();
    sd=serialOpen("/dev/ttyAMA0", 115200);
    pinMode(_pin_Atv, OUTPUT);
    ativar(Atv);
    usleep(100000);
    pinMode(_pin_Fre_dir, OUTPUT);
    pinMode(_pin_Rev_dir, OUTPUT);
    pinMode(_pin_Fre_esq, OUTPUT);
    pinMode(_pin_Rev_esq, OUTPUT);
    digitalWrite(_pin_Fre_dir, Lib);
    digitalWrite(_pin_Fre_esq, Lib);
    digitalWrite(_pin_Rev_dir, Frt);
    digitalWrite(_pin_Rev_esq, Frt);
    usleep(100000);
    softPwmCreate(_pin_Ace_dir, 0, 100);
    softPwmCreate(_pin_Ace_esq, 0, 100);
    usleep(100000);
    return 0;
}
void ativar(int com){
    digitalWrite(_pin_Atv, com);
}
void frente(){
        digitalWrite(_pin_Rev_dir, 0);
        digitalWrite(_pin_Rev_esq, 0);
        usleep(20000);
        m_frente=true;
        acel();
}
void acel(){
    if(!thread_ace){
        pthread_create(&ac, NULL, acelerar, NULL);
    }

}
void parar(){
    int c;
    if(thread_ace){
        for(int i=0; i<=100; i++){
            c=pthread_cancel(ac);
            if(c==0){
                i=101;
                thread_ace=false;
                usleep(1000);
            }
            else{
                usleep(500);
            }
        }
        thread_ace=false;
    }
    softPwmWrite(_pin_Ace_dir, 0);
    softPwmWrite(_pin_Ace_esq, 0);
    usleep(50000);
    digitalWrite(_pin_Fre_dir, !Lib);
    digitalWrite(_pin_Fre_esq, !Lib);
    sleep(1);
    digitalWrite(_pin_Rev_dir, 0);
    digitalWrite(_pin_Rev_esq, 0);
    usleep(10000);
    digitalWrite(_pin_Fre_dir, Lib);
    digitalWrite(_pin_Fre_esq, Lib);
    usleep(100000);
    parado=true;
    m_frente=false;
    m_traz=false;
    m_dir=false;
    m_esq=false;
}
void esquerda(){
    digitalWrite(_pin_Rev_dir, 0);
    digitalWrite(_pin_Rev_esq, 1);
    usleep(20000);
    m_esq=true;
    acel();
}
void direita(){
        digitalWrite(_pin_Rev_dir, 1);
        digitalWrite(_pin_Rev_esq, 0);
        usleep(20000);
        m_dir=true;
        acel();

}
void traz(){
        digitalWrite(_pin_Rev_dir, 1);
        digitalWrite(_pin_Rev_esq, 1);
        usleep(20000);
        m_traz=true;
        acel();
}