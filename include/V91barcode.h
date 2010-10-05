#ifndef __LK__USB__BAR__CODE__
#define __LK__USB__BAR__CODE__
int bar_code_open(void);
int bar_code_close (void);
int bar_code_read (unsigned char *rxbuf,int len);
#endif
