#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEBUG 1
#define DEBUG_E 0
#define DEBUG_V 0
#define CMD 0
#define DEBUG_TEMPLATE 1

#define SC2_vID	0x170E
#define SC2_pID	0x3010

#define OFF	0
#define ON	1
#define STAT	2

#define EXIT	'x'
#define VERIFY	'v'
#define ENROLL	'e'
#define CAPTURE	'c'
#define DETAILS 'd'

#define OK 0
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 4
#define MOVE_RIGHT 8
#define NO_FINGER 255


#define TIMEOUT -2

/*typedef struct TLV {
	unsigned char Tag;
	int Length;
	unsigned char Data[];
}TLV_PACKET;

typedef struct Bio_Oper{
	char Cmd;
	char Param;
	short Length;
	TLV_PACKET init;
}PROTOCOL_PKT;
*/

/*
Routines
*/

int io_init();
int io_write(void *buff,int len);
int io_read(void *buff,int len);
int io_read_intr(void *buff,int len);
int io_cleanup();

int Device_Init();
void Device_Close();
void Print_Response(unsigned char *,unsigned char *,int );
void Grab_Image();
void Read_Image();
int Capture_Fingerprint_Image(int Timeout);
void Get_Dev_Response(unsigned char *,int ,void *,int *);
void Device_Functions();
int Enroll_Fingerprint_Image(unsigned char *,int Timeout);
int Finger_Placement_Guide(int Timeout);
int Detect_Finger();
int Sensor_Power_Control(unsigned char );
int Verify_Fingerprint_Image(unsigned char *,int Timeout);
void Read_Fingerprint_Image_Buffer();
void Compress_RawImg_WSQ();
void Compress_RawImg_WSQ2();
void Decompress_WSQ_Raw(unsigned char *,int);
void Read_WSQ_Buffer();
/*
Grab 360x256 sensor and read it
*/
#ifdef	GRAB_IMAGE
	unsigned char Grab_Img[]={	0x63,0x00,0x00,0x18,		//cmd
                                	0x68,0x01,0x00,0x00,		//Rows
                                	0x00,0x00,0x00,0x00,		//Row offset
                                	0x01,0x00,0x00,0x00,		//Row Delta
                                	0x00,0x01,0x00,0x00,		//Cols
                                	0x00,0x00,0x00,0x00,		//Col offset
                                	0x01,0x00,0x00,0x00};		//Col Delta
                                                                                                                             
        unsigned char Read_Img_Data[]={ 0x63,0x01,0x00,0x08,		//cmd
                                        0xE8,0x03,0x00,0x00,		//Length
                                        0x00,0x00,0x00,0x00		//Offset
                                        };

#endif
/*
Enroll guide
*/
#ifdef ENROLL_IMAGE
        unsigned char Init[]={		0x70,0x00,0x00,0x02,		//TLV protocol
					0x28,0x00};			//cmd

        unsigned char Capture[]={       0x66,0x00,0x00,0x08,		//ANSI378 protocol format
                                	0x40,0x06,			//cmd
                                	0xD6,0x04,0xF4,0x01,0x00,0x00	//Timeout Tag
                         };
	
#endif
/*
Capture guide
*/
#ifdef CAPTURE_IMAGE
        unsigned char Cap_Frpt_Img[]={  0x61,0x00,0x00,0x1c,		//cmd
                                	0x10,0x27,0x00,0x00,		//timeout
                                	0x68,0x01,0x00,0x00,		//Rows
		                      	0x00,0x00,0x00,0x00,		//Row Offset
                                	0x01,0x00,0x00,0x00,		//Row Delta
                                	0x00,0x01,0x00,0x00,		//Cols
                                	0x00,0x00,0x00,0x00,		//Col Offset
                                	0x01,0x00,0x00,0x00};		//Col Delta
                                                                                                                             
        unsigned char Read_Frpt_Img[]={ 0x61,0x01,0x00,0x00,		//cmd
                                        0xE8,0x03,0x00,0x00,		//Length
                                        0x00,0x00,0x00,0x00};		//Offset

	unsigned char Cap_Frpt_Img_Fast[]={	0x61,0x00,0x00,0x1c,	//cmd
						0x68,0x01,0x00,0x00,	//Rows
	                                        0x00,0x00,0x00,0x00,	//Row Offset
        	                                0x01,0x00,0x00,0x00,	//Row Delta
                	                        0x00,0x01,0x00,0x00,	//Cols
                        	                0x00,0x00,0x00,0x00,	//Col Offset
                                	        0x01,0x00,0x00,0x00	//Col Delta
					};
	unsigned char Compr_RawImg_Wsq[]={	0x61,0x10,0x00,0x01,	//cmd
						0x00			//BRI 0 - 0.75 ,4 - 2.25
					};
	
	unsigned char Compr_RawImg_Wsq2[]={	0x61,0x20,0x00,0x0D,		//cmd	
						0x68,0x01,0x00,0x00,		//Width
						0x00,0x01,0x00,0x00,		//Height
						0x00,0x01,0x00,0x00,		//PPI
						0x04,				//BRI
					};	

	unsigned char Decomp[]={	0x61,0x21,0x00,0x00,		//cmd
					0x00,0x00,0x00,0x00		//WSQ size, we can give in appl
				};

	unsigned char R_WBuff[]={	0x61,0x11,0x00,0x00,		//cmd
					0xE8,0x03,0x00,0x00,		//Length
                                        0x00,0x00,0x00,0x00		//Offset
				};	
#endif

