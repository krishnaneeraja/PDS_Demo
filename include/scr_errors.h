
#define NO_ERR  	0
#define WRITE_ERROR  	1
#define COMOPEN_ERROR  	2 
#define COMPORT_CLOSED 	3
#define READER_MUTE 	4
#define TIME_0VER	5
#define ERR_LRC		6
#define CARD1_INSERTED  7
#define CARD1_EXTRACTED 8
#define CARD2_INSERTED  9 
#define CARD2_EXTRACTED 10
#define CARD_PRESENT    11
#define CARD_ABSENT     12
int SCR_Checkup_CardPresence();
int SCR_Open();
int SCR_Close();
int SCR_PowerUP5();
int SCR_Powerdown();
int SCR_GetSN(unsigned char *Response,unsigned int *RLen);
int SCR_GetSN8(unsigned char *Response,unsigned int *RLen);
unsigned char SCR_SendCommand(unsigned char *CAPDU_Data,unsigned char CLen,unsigned char *RAPDU_Data,unsigned int *RLen );
int Select_card(unsigned char C1);
int Insert_Card();
