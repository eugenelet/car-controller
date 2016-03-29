#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "getChar.cpp"
#include "transmit.cpp"
#include "receiver.cpp"
#include <pthread.h>

/*************************************
		OP CODE
**************************************/

#define CONTROL	0x0001
#define PICTURE	0x0010
#define	VIDEO	0x0011

/*************************************
		FUNCTION
**************************************/


/**************************
		CONTROL
***************************/
#define FORWARD	0x0100
#define BACK	0x0200
#define LEFT	0x0400
#define RIGHT	0x0800


/**************************
		VIDEO
***************************/
#define SENDING 0x0000
#define REQUEST 0x0001
#define STOP	0x0010
#define SENT	0x1111

using namespace std;

static char* snd_PORT="2022";
static char* rcv_PORT="2023";
//static char* IP_ADDR="192.168.1.31";
static char* IP_ADDR="127.0.0.1";

unsigned short* receivedPacket;
bool receive_flag = false;
bool getch_flag = false;
bool requestVid_flag = false;
char keyIn;

void* receive_thread(void* data){
	while(1){
		receivedPacket = receiver();//TEST
		receive_flag = true;
	}
	pthread_exit(NULL);
}

void* getch_thread(void* data){
	while(1){
		keyIn = getch();
		getch_flag = true;
	}
	pthread_exit(NULL);
}

void* video_thread(void* data){
	while(1){
		if(receviedPacket[0] & (VIDEO | START) ){
			
		}
	}
	pthread_exit(NULL);
}

int main (void)
{
	unsigned short output[512];
	pthread_t  p_thread;
	receiver_init(rcv_PORT);//TEST
	pthread_create(&p_thread, NULL, receive_thread, (void*)&keyIn);
	pthread_create(&p_thread, NULL, getch_thread, (void*)&keyIn);

    while(1)
    {
		if(receive_flag){
			cout << hex << receivedPacket[0];//TEST
			receive_flag = false;
		}
		if(getch_flag){
			getch_flag = false;
			switch(keyIn){
				case 'a':{
					cout <<"left"<<endl;
					output[0] = CONTROL | LEFT;
					transmit(IP_ADDR, snd_PORT, output);
					break;
				}
				case 's':{
					cout <<"back"<<endl;
					output[0] = CONTROL | BACK;
					transmit(IP_ADDR, snd_PORT, output);
					break;
				}
				case 'd':{
					cout <<"right"<<endl;
					output[0] = CONTROL | RIGHT;
					transmit(IP_ADDR, snd_PORT, output);
					break;
				}
				case 'w':{
					cout <<"forward"<<endl;
					output[0] = CONTROL | FORWARD;
					transmit(IP_ADDR, snd_PORT, output);
					break;
				}
				case 'v':{
					requestVid_flag ~= requestVid_flag;
					if(requestVid_flag){
						cout <<"VIDEO ON"<<endl;
						output[0] = VIDEO | REQUEST;
						transmit(IP_ADDR, snd_PORT, output);
					}
					else{
						cout <<"VIDEO OFF"<<endl;
						output[0] = VIDEO | STOP;
						transmit(IP_ADDR, snd_PORT, output);
					}
					break;
				}
				default:{
					cout <<"idle"<<endl;
					output[0] = (unsigned short) CONTROL;
					transmit(IP_ADDR, snd_PORT, output);
					break;
				}
    	
			}
		}
	}
    return 0;
}
