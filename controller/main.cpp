#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "getChar.cpp"
#include "transmit.cpp"
#include "receiver.cpp"
#include <pthread.h>
#include <vector>
#include "opencv2/opencv.hpp"
/*************************************
		OP CODE
**************************************/

#define CONTROL	0x01
#define PICTURE	0x02
#define	VIDEO	0x03

/*************************************
		FUNCTION
**************************************/


/**************************
		CONTROL
***************************/
#define IDLE	0x00
#define FORWARD	0x01
#define BACK	0x02
#define LEFT	0x03
#define RIGHT	0x04


/**************************
		VIDEO
***************************/
#define START   0x00
#define VALID   0x01
#define REQUEST 0x02
#define STOP    0x03
#define SENT    0x04

using namespace std;
using namespace cv;
static char* snd_PORT="2022";
static char* rcv_PORT="2023";
//static char* IP_ADDR="192.168.31.241";
static char* IP_ADDR="127.0.0.1";

unsigned char* receivedPacket;
bool receive_flag = false;
bool getch_flag = false;
bool requestVid_flag = false;
char keyIn;
vector<unsigned char> video_buffer;
int image_size= 0;
Mat test;
int test_flag = 0;
/*
void* receive_thread(void* data){
	while(1){
		receivedPacket = receiver();//TEST
		receive_flag = true;
	}
	pthread_exit(NULL);
}*/

void* getch_thread(void* data){
	while(1){
		keyIn = getch();
		getch_flag = true;
	}
	pthread_exit(NULL);
}

void* video_thread(void* data){//receive video
	int size_counter;
//	namedWindow("video",1);
	int rows, cols;
	while(1){
	//	if(test_flag) break;
		receivedPacket = receiver();//TEST
		receive_flag = true;
		if(receivedPacket[0] == VIDEO){
	//		cout << "VIDEO RECEIVED... " <<endl;
			switch(receivedPacket[1]){
				case START:{
	//				video_buffer.clear();
					rows = (int) (receivedPacket[3] << 2) | (receivedPacket[2]);
					cols = (int) (receivedPacket[5] << 2) | (receivedPacket[4]);
					image_size = rows * cols * 3;
	//				cout << "Rows: " << rows << endl;
	//				cout << "Columns: " << cols << endl;
	//				cout << "Packet 4: " << receivedPacket[4]  << endl;
	//				cout << "Packet 5: " << receivedPacket[5]  << endl;
	//				cout << "Image Size: " << image_size << endl;
					for(size_counter = 0; (size_counter < image_size) && (size_counter < 1018) ; size_counter++){//1024-6=1018{
						video_buffer.push_back(receivedPacket[size_counter + 6]);
					}
					break;
				}
				case VALID:{
					for(int i = 0; (size_counter < image_size) && (i < 1022) ; i++ , size_counter++){//1024-2=1022{
						video_buffer.push_back(receivedPacket[i + 2]);
					}
//					cout << "size counter: " << size_counter << endl;
					break;
				}
				case SENT:{
					for(int i = 0; (size_counter < image_size) && (i < 1022) ; i++ , size_counter++){//1024-2=1022{
						video_buffer.push_back(receivedPacket[i + 2]);
					}
					cout << "size counter: " << size_counter << endl;
					cout << "Buffer Size: " << video_buffer.size() << endl;
	//				if(video_buffer.size() < image_size){ break;}
					unsigned char* whole_vid = &video_buffer[0];
					Mat vid_Mat(rows, cols, 16, whole_vid);
					test = vid_Mat;
					test_flag = 1;
		//			imshow("video", vid_Mat);
					video_buffer.clear();
					break;
				}
			}

		}

	}
	pthread_exit(NULL);
}

void* display_thread(void* data){

}

int main (void)
{
	unsigned char output[1024];
	pthread_t  rcv_t, getch_t, video_t;
	receiver_init(rcv_PORT);//TEST
//	pthread_create(&rcv_t, NULL, receive_thread, (void*)&keyIn);
	pthread_create(&getch_t, NULL, getch_thread, (void*)&keyIn);
	pthread_create(&video_t, NULL, video_thread, (void*)&keyIn);


	namedWindow("edges",1);//TEST

    while(1)
    {
	
		if(test_flag){
//			test_flag = 0;
			imshow("edges", test);
			break;
		}
		if(receive_flag){
			//cout << hex << receivedPacket[0];//TEST
			receive_flag = false;
		}
		if(getch_flag){
			getch_flag = false;
			switch(keyIn){
				case 'a':{
					cout <<"left"<<endl;
					output[0] = CONTROL;
					output[1] = LEFT;
					transmit(IP_ADDR, snd_PORT, output);
					break;
				}
				case 's':{
					cout <<"back"<<endl;
					output[0] = CONTROL;
					output[1] = BACK;
					transmit(IP_ADDR, snd_PORT, output);
					break;
				}
				case 'd':{
					cout <<"right"<<endl;
					output[0] = CONTROL;
					output[1] = RIGHT;
					transmit(IP_ADDR, snd_PORT, output);
					break;
				}
				case 'w':{
					cout <<"forward"<<endl;
					output[0] = CONTROL;
					output[1] = FORWARD;
					transmit(IP_ADDR, snd_PORT, output);
					break;
				}
				case 'v':{
					requestVid_flag = ~requestVid_flag;
					if(requestVid_flag){
						cout <<"VIDEO ON"<<endl;
						output[0] = VIDEO;
						output[1] = REQUEST;
						transmit(IP_ADDR, snd_PORT, output);
					}
					else{
						cout <<"VIDEO OFF"<<endl;
						output[0] = VIDEO;
						output[1] = STOP;
						transmit(IP_ADDR, snd_PORT, output);
					}
					break;
				}
				default:{
					cout <<"idle"<<endl;
					output[0] =  CONTROL;
					output[1] = IDLE;
					transmit(IP_ADDR, snd_PORT, output);
					break;
				}
    	
			}
		}
	}
	waitKey(0);
    return 0;
}
