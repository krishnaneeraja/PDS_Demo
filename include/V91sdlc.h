#ifndef _SDLC_APP_
#define _SDLC_APP_
/* Do Not change these Values */
#define O_PULSE 1
#define O_TONE 0

int sdlc_open(char *phonnum,unsigned char mode);
int sdlc_transmit(unsigned char *buf,int buflen);
int sdlc_rx(unsigned char *buf,int len);
int sdlc_close(void);
#endif
