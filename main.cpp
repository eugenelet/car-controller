#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "GPIOClass.h"
#include "getChar.cpp"
#include "receiver.cpp"
#include "transmit.cpp"
using namespace std;

/*************************************
        OP CODE
**************************************/

#define CONTROL 0x1000
#define PICTURE 0x2000
#define VIDEO   0x4000

/*************************************
        FUNCTION
**************************************/


/**************************
        CONTROL
***************************/
#define FORWARD 0x0100
#define BACK    0x0200
#define LEFT    0x0400
#define RIGHT   0x0800


/**************************
        VIDEO
***************************/
#define SENDING 0x0010
#define VALID   0x0020
#define REQUEST 0x0040
#define SENT    0x0080




static char* rcv_PORT="2022";
static char* snd_PORT="2023";
/*const unsigned short FORWARD=0x0100;
const unsigned short BACK=0x0200;
const unsigned short LEFT=0x0400;
const unsigned short RIGHT=0x0800;
const unsigned short CONTROL=0x0001;
const unsigned short PICTURE=0x0010;
const unsigned short VIDEO=0x0011;
*/
static char* IP_ADDR = "127.0.0.1";
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
int main (void)
{
	char keyIn;
	unsigned short* receivedPacket;
	unsigned short opcode;
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
		if((receivedPacket[0] & CONTROL)){
			opcode = CONTROL;	
		}		
		else if((receivedPacket[0] & PICTURE)){
			opcode = PICTURE;	
		}		
		else if((receivedPacket[0] & VIDEO)){
			opcode = VIDEO;	
		}		
		else{opcode = NULL;}

		switch(opcode){
			case CONTROL:{
					if((receivedPacket[0] & FORWARD)){
		//				forward();
						cout << "FORWARD" << endl;
						unsigned short output[512];///TEST
						output[0] = 0x1010;//TEST
						transmit(IP_ADDR, snd_PORT, output);//TEST
					}
					else if((receivedPacket[0] & BACK)){
		//				back();
						cout << "back" << endl;
					}
					else if((receivedPacket[0] & LEFT)){
		//				left();
						cout << "LEFT" << endl;
					}
					else if((receivedPacket[0] & RIGHT)){
		//				right();
						cout << "RIGHT" << endl;
					}
					else {}//idle();}
					
					break;	
				}
			case PICTURE:{
				cout << "PICTURE" << endl;
				break;
				//idle();
			}
			case VIDEO:{
				cout << "VIDEO" << endl;
				break;
				//idle();
			}

			default:{}// idle();



		}
	}
    return 0;
}
