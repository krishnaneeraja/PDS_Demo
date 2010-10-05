#ifndef __V91__KBD__
#define __V91__KBD__
/*
Opens the keyboard device,this is to be called prior to using any functions of the USB keyboard.
returns 0 on success and -1 on failure
*/
int kbd_open(void);
/*
Close the keyboard device while exiting your application.returns 0.
*/ 
int kbd_close (void);
/*
Gets a character from the keyboard and its ascii value is returned.
*/
int lk_getchar (void);
/*
Get numeric entries only from the keyboard.
Parameters:
 lineno:if title is passed lineno denotes the line where the title is displayed and msg_data is displayed in the next line,if title is not passed msg_data is displayed on the lineno specified.
 column:with title it is the column number of where the title should start,otherwise it is the column number from where msg_data starts.
 title:title that is displayed and highlighted such as ur company name etc.
 msg_data:user relevant message such as "Enter Acc. No."
 MAXdigit:Maximum number of digits that should be read.
 dest:destination buffer where the entered number is to be stored.
 row:line number where the entry is taken.
returns number of digits entered on success and -1 on failure or cancellation.
*/
       
int lk_keyb_numeric_entry(unsigned char lineno, unsigned char column,unsigned char *title, unsigned char* msg_data, unsigned char MAXdigit, char *dest,char row);
/*
Get alphanumeric entries from keyboard.
parameters:
lineno:if title is passed lineno denotes the line where the title is displayed and msg_data is displayed in the next line,if title is not passed msg_data is displayed on the lineno specified.
 column:with title it is the column number of where the title should start,otherwise it is the column number from where msg_data starts.
 title:title that is displayed and highlighted such as ur company name etc.
 msg_data:user relevant message such as "Enter Acc. No."
 MAXkey:Maximum number of alpha numeric characters that should be read.
 dest:destination buffer where the entered number is to be stored.
 row:line number where the entry is taken.
returns number of digits entered on success and -1 on failure or cancellation.
*/
int lk_keyb_alpha_entry(unsigned char lineno, unsigned  char column, unsigned char *title,unsigned  char* msg_data, unsigned char MAXkey,char *dest,char
row);
/*
Get Password entries from keyboard.
parameters:
lineno:if title is passed lineno denotes the line where the title is displayed and msg_data is displayed in the next line,if title is not passed msg_data is displayed on the lineno specified.
 column:with title it is the column number of where the title should start,otherwise it is the column number from where msg_data starts.
 title:title that is displayed and highlighted such as ur company name etc.
 msg_data:user relevant message such as "Enter Acc. No."
 MAXdigit:Maximum number of digits that should be read.
 dest:destination buffer where the entered number is to be stored.
 row:line number where the entry is taken.
 returns number of digits entered on success and -1 on failure or cancellation.
*/
int lk_keyb_passwd_entry(unsigned char lineno, unsigned char column, unsigned char*title,unsigned char* msg_data,unsigned char MAXdigit,char *dest,char row);
/*
Get amount entry.
Parameters:
lineno:if title is passed lineno denotes the line where the title is displayed and msg_data is displayed in the next line,if title is not passed msg_data is displayed on the lineno specified.
 column:with title it is the column number of where the title should start,otherwise it is the column number from where msg_data starts.
 title:title that is displayed and highlighted such as ur company name etc.
 msg_data:user relevant message such as "Enter Acc. No."
 MAXdigit:Maximum number of digits that should be read.
 amt:amount entered.
 row:line number where the entry is taken.
 pfx:string indicating monetary prefix such as "Rs" etc.
returns number of digits entered on success and -1 on failure or cancellation.
*/
int lk_keyb_amount_entry(unsigned char lineno,unsigned char column,unsigned char *title,unsigned  char* msg_data,unsigned char MAXdigit,float *amt,char row,unsigned char *pfx);
#endif




