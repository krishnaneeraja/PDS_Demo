#include<stdlib.h>
#include<math.h>
#include<string.h>

#define NO_ERR  1  //everything is ok
static unsigned char READ_VERSION[] = {0xAA};
static unsigned char POWER_UP[] = {0x6E};
static unsigned char POWER_DOWN[] = {0x4D};
//static unsigned char PSE[]={0x00,0xA4,0x04,0x00,0x0E,0x31,0x50,0x41,0x59,0x2E,0x53,0x59,0x53,0x2E,0x44,0x44,0x46,0x30,0x31,0x00};

int SCR_Open();   //intialization  port parameters and returns NO_ERR 
int SCR_Close();   //closing the port return 1 on success 0 on failure
int SCR_Checkup_CardPresence();   //return NO_ERR on Insert the Card 
int  SCR_SendCommand( unsigned char *pszCmd, unsigned char ucLgcmd ,unsigned char* szRep, unsigned int *respLen);
	  // API for sending a command and response 
int SCR_Send_num_mask(unsigned char *rxnbuf);
int SCR_set_serial_baud_rate (unsigned char par);
int SCR_set_serial_time_out(unsigned char par);
int SCR_negotiate(unsigned char pro,unsigned char fd);
int SCR_select_card (unsigned char par);
int SCR_idle_mode (unsigned char par);
int SCR_power_up_three (unsigned char *rxnbuf,unsigned int *rlen);
int SCR_power_up_five (unsigned char *rxnbuf,unsigned int *rlen);
int SCR_power_off ();
int SCR_serial_number (unsigned char *rxnbuf);
