#ifndef _SDLC_APP_
#define _SDLC_APP_

int sdlc_open(char *phonnum);
int sdlc_transmit(unsigned char *buf,int buflen);
int sdlc_rx(unsigned char *buf,int len);
int sdlc_close(void);

#endif
