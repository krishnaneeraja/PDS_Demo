#ifndef PRINTER_H
#define PRINTER_H

/*Done by T.V.R */
#define PRN_ERR -1
#define PRN_LEN_ERR -2
#define PRN_OFF     0
#define PRN_ON      1
#define PRN_BUSY    2
#define PRN_FAIL    0 
#define PRN_SUCCESS 1

//This function opens the printer device 
// returns SUCCESS if the device is opend successfully, returns BUSY if the device is already opend
int prn_open();  

//This fucntion closes the printer device 
//returns 0 if the device closed successfully, returns PRN_ERR if the device is not opend.
int prn_close(); 

//This fucntion advance the paper 
// no.of scanlines is to be passed as parameter ( 1 text line is equal to 17 scan lines)
//return PRN_ERR if the device not opend else returns the response from the ioctl function
int prn_paper_feed(int scanlines);

//This fucntion prints the text 
//pointer to the buffer from which the text to be printed, length of the text and font to be used to be 
//passed as arguments
//max lines of text can be 25 (42 charecters per line if the font is 1 and 21 charecters if the font is 2)
//presently there are two fonts are supported (1 and 2)
// returns SUCCESS if the fucntion successfull, returns PRN_ERR if the device not opend, returns PRN_LEN_ERR if the length is more 
int prn_write_text(unsigned char *text2, int len, int font); 

// 
int prn_write_bmp(unsigned char *, int); // bitmap
						//returns no. of characters written or PRN_ERR if error occurs
void prn_abort(); // printing aborts

#endif
