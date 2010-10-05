

int modem_open(void) ;
//2
int modem_close (void) ;
//3
char * modem_config (char *) ;
//4
int  modem_dial (const char *) ;
//5
int modem_tx_data (unsigned char * , int) ;
//6
int modem_rx_data(unsigned char *,int) ;
//7
int modem_disconnect (void) ;
