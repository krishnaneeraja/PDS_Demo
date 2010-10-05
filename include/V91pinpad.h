
#define BUFFER_OVERFLOW  -5
#define LK_SUCCESS 0
#define LK_TIMEOUT -1
#define LK_ON 1
#define LK_OFF 0
#define LK_ERROR   -2;
#define LK_WAIT    1;
#define LK_P_TIMEOUT -3

#define KBD_EVENT 1
#define MCR_EVENT 2
#include <time.h>

/* int lk_open():it opens the  serial port  for communicating pinpad and   return 	“1 ” on succes  or -1 on failure . 
And also sets a value to the file descriptor.  */
int lk_open();
/* int lk_close(): it closes the opened  serial port by using the file descrptor */
void lk_close();
                                                                                                 
//*******************************************
/* lk_getkey() :: This  function gets  the key from  pinpad buffer. */
int lk_getkey();


/* lk_getkey_wait() :: This  function clear the key buffer and wait untill new key is pressed. It returns the key */
int lk_getkey_wait();
 
/* 
int lk_getnumeric(unsigned char lineno , unsigned char column, unsigned char msg_data *,
		 unsigned  char Maxdigit , char * dest , char row ) 
	
	lineno   : specifies the lineno in the range of 1to 5 ( '0'  Zero line reserved)
	column :specifies the column number in the range of 0 to 62.
	msg_data: it holds  message  that is to be printed  at lineno and column ,
		prior to the user input from the pinpad.
	Maxdigit :number of digits to be taken  from the  pinpad. This is specified by the	application.
	dest: it holds the  output that is numeric keys.(out put parameter).
	row : a line at which input to be taken from the pinpad. 

	   this function takes number of digits of  Maxdigit onto  the   “dest”  at specified lineno 		column number  and row from the pinpad, and return number read digits.


 */
int lk_getnumeric(unsigned char , unsigned char , unsigned char* , unsigned char, char *,char);
/*
 
int lk_getalpha(unsigned char lineno , unsigned char column  ,unsigned char * msg_data, 
		unsigned char MAXkey,char  *dest, char  row ) 

	this function similar to lk_getmnumaeric() (i.e. Above function ) except it reads alpha keys also and numeric keys from pinpad.
 */
int lk_getalpha(unsigned char , unsigned char , unsigned char* , unsigned char, char *,char);
/*


this function is similar to lk_getnumeric()  
*/
int lk_getip(unsigned char lineno, unsigned  char column, unsigned  char* msg_data, unsigned char MAXkey,char *dest,char row);
/* 

	
int lk_dispbutton(char *b1,char *b2,char *b3,char *b4 )

	b1 : button  one    	b2 : button  two
	b3 : button three                b4 : button four.
	this function displays the four buttons b1, b2, b3, b4.  
*/
int lk_dispbutton(char* b1,char *b2,char *b3,char *b4);
/*
  
int lk_getpassword(unsigned char lineno, unsigned char column , unsigned char * msg_data,unsigned char MAXdigit , char * dest ,char  row )

	lineno   : specifies the lineno in the range of 1to 5 ( '0'  Zero line reserved)
	column :specifies the column number in the range of 0 to 62.
	msg_data: it holds  message  that is to be printed  at lineno and column ,
		prior to the user input from the pinpad.
	Maxdigit :number of digits to be taken  from the  pinpad. This is specified by the application.
	dest: it holds the  output that is numeric keys.(out put parameter).
	row : a line at which input to be taken from the pinpad. 

	It takes the password entry from the user through pinpad of Number of digits  	MAXdigit and copies onto the dest .And returns a number of byte read from the pinpad   	to the calling function.
*/
int lk_getpassword(unsigned char, unsigned char, unsigned char*,unsigned char,char *,char);
/*


int lk_getamount( unsigned char lineno , unsigned char column , unsigned char * msg_data ,unsigned char  MAXdigit , unsigned char  deci_pos ,float * amt ,char  row,  char pfx) 

	lineno   : specifies the lineno in the range of 1to 5 ( '0'  Zero line reserved)
	column :specifies the column number in the range of 0 to 62.
	msg_data: it holds  message  that is to be printed  at lineno and column ,
		prior to the user input from the pinpad.
	Maxdigit :number of digits to be taken  from the  pinpad. This is specified by the application.
	deci_pos :it specifies the decimal point position from where to start. 
	amt: it holds the  amount  .(out put parameter).
	row : a line at which input to be taken from the pinpad.	
	pfx : specified prefix for the amount will be displayed.  (ex dollors , Rs etc)

	this function  reads the amount from the pinad 
	
	and return “ 0” on success.

*/
//int lk_getamount(unsigned char, unsigned char, unsigned char*, unsigned char,unsigned char,float *,char,char);
int lk_getamount(unsigned char, unsigned char, unsigned char*, unsigned char,unsigned char,double *,char,char);
/* 
	
int lk_led(unssigned char LEDno, unsigned char T)
	
	This function  lets the led glow
	LEDno : led number which led to be glow 
	T          :  Type of glowing(Green ,Red, Yellow)

	and it returns the number of bits send onto the LCD.
*/
int lk_led(unsigned char,unsigned char);
/*
int  lk_dispbmp(unsigned char x1 , unsigned char y1, unsigned char x2 , unsigned char y2,
		unsigned char* data  )
	
		x -coordinate points are in the range of  '0' to  '127'.
		y -coordinate points are in the range of  '0' to '62'.	
		x1,y1   one pair of points ( top left  points ).
		x2, y2  second pair points (bottom right points).
		data : holds 	the bmp image which  is to be diplayed on the LCD
	this function displays the bmp image on LCD and returns the number of bytes  send onto the LCD.
 */
int lk_dispbmp(unsigned char,unsigned char,unsigned char, unsigned char,unsigned char *);

/*	
	
int lk_disptext(unsigned char lineno, unsigned char column,unsigned char * data, char font )
	
	this function displays the sting of data at specified line and column in a given font.
	lineno  :  line number where to display the given data.
	column : column number where to display the given data.
	font :  data is displaying in a given font style.
	and it  returns the number of bytes transferred onto LCD.

*/
int lk_disptext(unsigned char ,unsigned char, unsigned char* ,char );
/*
	
int lk_dispicon(unsigned char x1,unsigned char y1, unsigned char x2 ,unsigned char y2, char ID,
		 char intensity)
	x1,y1: is one set of pair points
	x2,y2: is one set of pair points.
	intensity : intensity of the ID.
	ID : is to bedisplayed in a given intensity.
	and it  returns the number of bytes transferred onto LCD.

*/	
int lk_dispicon(unsigned char, unsigned char , unsigned char , unsigned char ,char , char);
/*
	
int lk_dispinit()
	this function initializes the LCD display and return the  status of the LCD.
*/
int lk_dispinit();
/* 
	

int lk_dispclr()
	this function clears the LCD display and  return  the status of the LCD.

*/
int lk_dispclr();
/*

int lk_powerdown(void)
		
	     this function power off the LCD and returns the status of the LCD.
 
*/
int lk_powerdown();
/*
int lk_ dispbklight(unsigned char  status )
	
	this function enable the background light in LCD of type status and return the status of 		the LCD. */
int lk_dispbklight(unsigned char);
/*
	
int  lk_disphlight(unsigned char lineno)
	
	this function  highlight the given lineno line and returns the status of the LCD.

 */
int lk_disphlight(unsigned char);
/* 
	
int  lk_disphlight(unsigned char lineno)
	
	this function  highlight the given lineno line and returns the status of the LCD.

*/ 
int lk_displineclr(unsigned char);
/* 
int lk_displineclr( unsigned char  lineno)
	this function clears given lineno line and returns the status of the LCD.

	
*/
int lk_buzzer();
/*
	
int lk_buzzer(unsigned char repeatcount ,char * data)
	this function gives the buzzer sound  of repeat count times,  and it returns the number of 	bytes send onto the LCD  display.

	 
*/
//*****************************************

/*
void lk_lcdtest()
 this function  calls this function 
int lk_dispbmp(unsigned char,unsigned char,unsigned char, unsigned char,unsigned char *);
  with possible data i.e, a set of '00' or  '10' or  '01' or '11'
 */ 
void lk_lcdtest();

/*int lk_mgetpassword(unsigned char lineno, unsigned char column, unsigned char *title,unsigned char* msg_data,unsigned char *prifix,unsigned char MAXdigit,char *dest); 
	lineno  : line number where to display the given data.
        column  : column number where to display the given data.
	msg_data:it holds  message  that is to be printed  at lineno and column ,
        	        prior to the user input from the pinpad.
 	
      	title:  holds the character to be displayed of our interset like login or some  relevant name to the activity 
	prefix : this is for prefix
	Maxdigit :number of digits to be taken  from the  pinpad. This is specified by the application.
        dest: it holds the  output that is numeric keys.(out put parameter).
                                                                                                                             
        It takes the password entry from the user through pinpad of Number of digits    MAXdigit and copies onto the dest .
	 	And returns a number of byte read from the pinpad    to the calling function.

	*/
int lk_mgetpassword(unsigned char lineno, unsigned char column, unsigned char *title,unsigned char* msg_data,unsigned char *prifix,unsigned char MAXdigit,char *dest);
/*
int lk_mgetnumeric(unsigned char lineno, unsigned char column, unsigned char * title,unsigned char* msg_data,unsigned char * prefix, unsigned char MAXdigit, char *dest);
	int lk_getnumeric(unsigned char lineno , unsigned char column, unsigned char msg_data *,
                 unsigned  char Maxdigit , char * dest , char row )
                                                                                                                             
        lineno   : specifies the lineno in the range of 1to 5 ( '0'  Zero line reserved)
        column :specifies the column number in the range of 0 to 62.
        msg_data: it holds  message  that is to be printed  at lineno and column ,
                prior to the user input from the pinpad.
        Maxdigit :number of digits to be taken  from the  pinpad. This is specified by the      application.
        dest: it holds the  output that is numeric keys.(out put parameter).
        row : a line at which input to be taken from the pinpad.
                                                                                                                             
           this function takes number of digits of  Maxdigit onto  the   “dest”  at specified lineno            column number  and row from the pinpad, and return number read digits.
                                                                                                                             
	 
*/
int lk_mgetnumeric(unsigned char lineno, unsigned char column, unsigned char * title,unsigned char* msg_data,unsigned char * prefix, unsigned char MAXdigit, char *dest);
/*
int lk_mgetalpha(unsigned char lineno, unsigned  char column, unsigned char *title,unsigned  char* msg_data, unsigned char * prefix,unsigned char MAXkey,char *dest);
	this function similar to the lk_mgetpassword(.......) and it reads the alpha and  numeric keys from the pinapad.

*/
int lk_mgetalpha(unsigned char lineno, unsigned  char column, unsigned char *title,unsigned  char* msg_data, unsigned char * prefix,unsigned char MAXkey,char *dest);
/*
	 lineno   : specifies the lineno in the range of 1to 5 ( '0'  Zero line reserved)
        column :specifies the column number in the range of 0 to 62.
        msg_data: it holds  message  that is to be printed  at lineno and column ,
                prior to the user input from the pinpad.
        Maxdigit :number of digits to be taken  from the  pinpad. This is specified by the application.
        deci_pos :it specifies the decimal point position from where to start.
        amt: it holds the  amount  .(out put parameter).
        row : a line at which input to be taken from the pinpad.
        pfx : specified prefix for the amount will be displayed.  (ex dollors , Rs etc)
                                                                                                                             
        this function  reads the amount from the pinad
                                                                                                                             
        and return “ 0” on success.


 */
//int lk_mgetamount(unsigned char lineno, unsigned  char column, unsigned char *title,unsigned  char* msg_data, unsigned char * prefix,unsigned char MAXkey,unsigned char deci_pos,char *ff);
int lk_mgetamount(unsigned char lineno, unsigned  char column, unsigned char *title,unsigned  char* msg_data, unsigned char * prefix,unsigned char MAXkey,unsigned char deci_pos,double *ff);

/* 
this function is similar to lk_mgetnumeric()  
	
*/ 
int lk_mgetip(unsigned char lineno, unsigned  char column, unsigned char *title,unsigned  char* msg_data, unsigned char * prefix,unsigned char MAXkey,char *dest);

/*This funtion waits for events from keyboard and magswie card reader and returns the event any one of the events can be selected independently or both can be selected 
parameter to be passed to select the events if opt = 0 (both events) 1(keyboard) 2(magswipe)*/
int poll_event(int opt);

//This functios get the time of the system 
//pointer to (struct tm) to be passed in which time returned.
// return value is ) for success and -1 for error
int lk_getrtc(struct tm *tt);


//This functios set the time of the system 
//pointer to (struct tm) to be passed with the time to be set
// return value is ) for success and -1 for error
int lk_setrtc(struct tm *tt);

/*
        Used to map the keys to particular characters.
        input:
                lang_code       language code (it should be already downloaded language code and should not be 0x00 and 0xFF)                keymap_tab      (key Number1, font codes1, unit separator, key Number2, font codes2, unit separator,…..)
                                                                                                                             
                                Key Number1, Key Number2…   -  1 byte( 0 – 9 )
                                Font codes1,Font codes2….   -  index of the font characters given in the font table
                                                               should be in the range of 0x80 to0xff.
                                Unit separator              -  1 byte (0x1F)
                table_len       size of keymap table ;
*/

int lk_keymap_table(unsigned char lang_code,unsigned char *keymap_tab,int table_size);

/*
        Used to list the font tables downloaded into the pinpad
        Gets  the list of language codes into " Lang_list " buffer.
                                                                                                                             
        Returns
                success         no.of languages supported
                failure         negative integer
*/

int lk_list_lang(unsigned char *Lang_list);

/*
   Used to install the font table specified by the user. The character height should be 5 pixels width and 7 pixels height.
        input:
                lang_code       -  language code , 1 byte (should not be 0x00 and 0xff)
                font_data       -  hex data corresponding to the characters in the given font table
                font_size       -  font data size
                                                                                                                             
*/
int lk_download_fonts(unsigned char lang_code,unsigned char *font_data,int font_size);



/*
  this function allow you to on and off lcd backlight 

	on_off values
	
          0x00 ( disable the Back light of LCD)  
	  0x01 ( enable the  Back light of LCD)

*/



int lk_bklight_onoff(unsigned char on_off);

/* This function will get float value from user 

 get float  of 15 digits including "." and floating value

 returns 0 on success and -1 and -2 on failure	
*/

int lk_getfloat_value (double *ptr,char *str);


/*   this function power on the usb supply  

	on returns -1 on error 
	and o on success                   */

int lk_usbpoweron(void);


/*   this function power off the usb supply
                                                                                                                                                                               
        on returns -1 on error
        and o on success                   */
                                                                                                                                                                               
	int lk_usbpoweroff(void);


/*     enabling and disabling backlight of lcd   */

/*  	argument    1   ON 
		    0   OFF     */





int lk_bklight_onoff(unsigned char on_off);



/*
Function: lk_getrfidkey()
IN Parameters: unsigned char index
Out: unsigned char *key;

index: index of the key
key: Key vales stored in Pinpad in the form of Null terminated string.
*/

int lk_getrfidkey(unsigned char index,unsigned char *key);


/*
Function: lk_setrfidkey()
IN Parameters: unsigned char index    //index of the key
unsigned char *key;	      //8-bytes key to be stored    
                                                                                                                                                                                
index: index of the key
key: Key vales stored in Pinpad in the form of Null terminated string.
*/

int lk_setrfidkey(unsigned char index,unsigned char *key);

/*
Function:  lk_setrfidmasterkey()
IN Parameters: 
unsigned char *key;              //8-bytes key to be stored as a primary key;

*/

int lk_setrfidmasterkey( unsigned char *key);

