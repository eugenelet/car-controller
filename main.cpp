#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "GPIOClass.h"
#include "getChar.cpp"
#include "receiver.cpp"
#include "transmit.cpp"
#include <pthread.h>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

/*************************************
        OP CODE
**************************************/

#define CONTROL 0x01
#define PICTURE 0x02
#define VIDEO   0x03

/*************************************
        FUNCTION
**************************************/


/**************************
        CONTROL
***************************/
#define IDLE    0x00
#define FORWARD 0x01
#define BACK    0x02
#define LEFT    0x03
#define RIGHT   0x04


/**************************
        VIDEO
***************************/
#define START   0x00
#define VALID   0x01
#define REQUEST 0x02
#define STOP	0x03
#define SENT    0x04



static char* rcv_PORT="2022";
static char* snd_PORT="2023";
/*const unsigned char FORWARD=0x0100;
const unsigned char BACK=0x0200;
const unsigned char LEFT=0x0400;
const unsigned char RIGHT=0x0800;
const unsigned char CONTROL=0x0001;
const unsigned char PICTURE=0x0010;
const unsigned char VIDEO=0x0011;
*/
static char* IP_ADDR = "127.0.0.1";
//static char* IP_ADDR = "192.168.31.241";
/*


GPIOClass* frontLeft1; 
GPIOClass* frontLeft2;
GPIOClass* frontRight1;
GPIOClass* frontRight2;
GPIOClass* rearLeft1;
GPIOClass* rearLeft2;
GPIOClass* rearRight1; 
GPIOClass* rearRight2;

void idle(){
	frontLeft1->setval_gpio("1"); 
	frontLeft2->setval_gpio("1"); 
	frontRight1->setval_gpio("1");
	frontRight2->setval_gpio("1");
	rearLeft1->setval_gpio("1"); 
	rearLeft2->setval_gpio("1"); 
	rearRight1->setval_gpio("1");
	rearRight2->setval_gpio("1");
}

void forward(){
	frontLeft1->setval_gpio("0"); 
	frontLeft2->setval_gpio("1"); 
	frontRight1->setval_gpio("0");
	frontRight2->setval_gpio("1");
	rearLeft1->setval_gpio("0"); 
	rearLeft2->setval_gpio("1"); 
	rearRight1->setval_gpio("0");
	rearRight2->setval_gpio("1");
}

void back(){
	frontLeft1->setval_gpio("1"); 
	frontLeft2->setval_gpio("0"); 
	frontRight1->setval_gpio("1");
	frontRight2->setval_gpio("0");
	rearLeft1->setval_gpio("1"); 
	rearLeft2->setval_gpio("0"); 
	rearRight1->setval_gpio("1");
	rearRight2->setval_gpio("0");
}

void left(){
	frontLeft1->setval_gpio("1"); 
	frontLeft2->setval_gpio("0"); 
	frontRight1->setval_gpio("0");
	frontRight2->setval_gpio("1");
	rearLeft1->setval_gpio("1"); 
	rearLeft2->setval_gpio("0"); 
	rearRight1->setval_gpio("0");
	rearRight2->setval_gpio("1");
}

void right(){
	frontLeft1->setval_gpio("0"); 
	frontLeft2->setval_gpio("1"); 
	frontRight1->setval_gpio("1");
	frontRight2->setval_gpio("0");
	rearLeft1->setval_gpio("0"); 
	rearLeft2->setval_gpio("1"); 
	rearRight1->setval_gpio("1");
	rearRight2->setval_gpio("0");
}
*/

int quit_check(pthread_mutex_t *mutex){
	switch(pthread_mutex_trylock(mutex)){
		case 0:{
			pthread_mutex_unlock(mutex);
			return 1;
		}
		case EBUSY:{
			return 0;
		}

	}
	return 1;
}

void *video_do(void *mtx_check){
	unsigned char output[1024];
	pthread_mutex_t *mtx = ((pthread_mutex_t *) mtx_check);
	VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return NULL;

	while( !quit_check(mtx) ){
	//	cout << "SENDING VIDEO..." <<endl;
		Mat video_stream;
		cap >> video_stream;
		int size = video_stream.total() * video_stream.elemSize();
		unsigned char* raw_video = video_stream.data;
		int data_count = 0;
/*  ===========================================
 *		Send first Packet
 *  ===========================================
*/
		output[0] = VIDEO;
		output[1] = START;
		output[2] = (unsigned char) (video_stream.rows & 0x000000ff); // LSB of Row
		output[3] = (unsigned char) ((video_stream.rows >> 2) & 0x000000ff); //MSB of Row
		output[4] = (unsigned char) ((video_stream.cols) & 0x000000ff);// LSB of Cols
		output[5] = (unsigned char) ((video_stream.cols >> 2) & 0x000000ff); // MSB of Cols
		cout << "Depth: " << video_stream.type() << endl;
		for(data_count=0 ; (data_count < size) && (data_count < 1018) ; data_count++){
			output[data_count + 6] = raw_video[data_count];
		}
		transmit(IP_ADDR, snd_PORT, output);
		while(data_count < size){
			cout << "Data Count: " << data_count << endl;
			output[0] = VIDEO;
			output[1] = VALID;
			for(int i = 0 ; (data_count < size) && (i < 1022) ; i++, data_count++){
				output[i + 2] = raw_video[data_count];
			}
			if(data_count == size){
				output[0] = VIDEO;
				output[1] = SENT;
				transmit(IP_ADDR, snd_PORT, output);
				break; //quit while loop for next image capture;
			}
			else{
				transmit(IP_ADDR, snd_PORT, output);
			}
			
		}


	}
	
	return NULL;

}

int main (void)
{
	char keyIn;
	unsigned char* receivedPacket;
	unsigned char opcode;
	pthread_t video_thread;
	pthread_mutex_t video_mutex;

/*
    frontLeft1 = new GPIOClass("6"); //create new GPIO object to be attached to  GPIO4
    frontLeft2 = new GPIOClass("13"); //create new GPIO object to be attached to  GPIO4
    frontRight1 = new GPIOClass("19"); //create new GPIO object to be attached to  GPIO17
    frontRight2 = new GPIOClass("26"); //create new GPIO object to be attached to  GPIO17
    rearLeft1 = new GPIOClass("12"); //create new GPIO object to be attached to  GPIO4
    rearLeft2 = new GPIOClass("16"); //create new GPIO object to be attached to  GPIO4
    rearRight1 = new GPIOClass("20"); //create new GPIO object to be attached to  GPIO17
    rearRight2 = new GPIOClass("21"); //create new GPIO object to be attached to  GPIO17

    frontLeft1->export_gpio(); //export GPIO4
    frontLeft2->export_gpio(); //export GPIO4
    frontRight1->export_gpio(); //export GPIO4
    frontRight2->export_gpio(); //export GPIO4
    rearLeft1->export_gpio(); //export GPIO4
    rearLeft2->export_gpio(); //export GPIO4
    rearRight1->export_gpio(); //export GPIO4
    rearRight2->export_gpio(); //export GPIO4


    frontLeft1->setdir_gpio("out");
    frontLeft2->setdir_gpio("out");
    frontRight1->setdir_gpio("out");
    frontRight2->setdir_gpio("out");
    rearLeft1->setdir_gpio("out");
    rearLeft2->setdir_gpio("out");
    rearRight1->setdir_gpio("out");
    rearRight2->setdir_gpio("out");
   */ 
	//idle();
	receiver_init(rcv_PORT);
    while(1)
    {
		receivedPacket = receiver();
		//cout << receivedPacket[0]<<endl;		
		if((receivedPacket[0] == CONTROL)){
			opcode = CONTROL;	
		}		
		else if((receivedPacket[0] == PICTURE)){
			opcode = PICTURE;	
		}		
		else if((receivedPacket[0] == VIDEO)){
			opcode = VIDEO;	
		}		
		else{opcode = NULL;}

		switch(opcode){
			case CONTROL:{
					if((receivedPacket[1] == FORWARD)){
		//				forward();
						cout << "FORWARD" << endl;
						unsigned char output[1024];///TEST
						output[0] = 0x10;//TEST
						transmit(IP_ADDR, snd_PORT, output);//TEST
					}
					else if((receivedPacket[1] == BACK)){
		//				back();
						cout << "back" << endl;
					}
					else if((receivedPacket[1] == LEFT)){
		//				left();
						cout << "LEFT" << endl;
					}
					else if((receivedPacket[1] == RIGHT)){
		//				right();
						cout << "RIGHT" << endl;
					}
					else {
						cout << "IDLE" << endl;
						//idle();
					}
					
					break;	
				}
			case PICTURE:{
				cout << "PICTURE" << endl;
				break;
				//idle();
			}
			case VIDEO:{
				cout << "VIDEO" << endl;
				switch(receivedPacket[1]){
					case REQUEST:{
						cout << "VIDEO REQUEST... " << endl;
						pthread_mutex_init(&video_mutex, NULL);
						pthread_mutex_lock(&video_mutex);
						pthread_create(&video_thread, NULL, video_do, &video_mutex);
						break;
					}
					case STOP:{
						cout << "VIDEO STOP... " << endl;
						pthread_mutex_unlock(&video_mutex);
						pthread_join(video_thread, NULL);
						break;
					}
					default: {break;}
				}
				break;
				//idle();
			}

			default:{}// idle();



		}
	}
    return 0;
}
