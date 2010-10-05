#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include <sys/signal.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include <sys/ioctl.h>
#include<string.h>
#include<fcntl.h>
#include<termios.h>
#include<V91printer.h>
#include<V91magswipe.h>
#include<V91pinpad.h>
#include <errno.h>
#include <V91sdlc.h>
#include <sys/types.h>
#include<sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<sys/time.h>
#include<time.h>
#include <net/if.h>
#include <dirent.h>
//#include <rfid.h>
#include <pthread.h>


#include "type.h"
#include "V91ppp.h"
#include "scr_errors.h"
#include "soundapp.h"

#define MaxCol 21
                                                                                                                            
#define F1 17
#define F2 18
#define F3 19
#define F4 20
                                                                                                                            
#define ENTER 13
#define CANCEL 23
 
#define RES32 4
#define UprOffset 1
#define MAX_MENU_ENTRIES 200
#define MAX_ENTRY_LENGTH 30
#define MAX_FILE_PATH 500
#define FALSE 0
#define TRUE  1

#define USER_VER_SUCCESS 1


#define KERNEL      1
#define RAMDISK     2
#define APPLICATION 3
                                                                                                                             
#define ETHERNET    1
#define RALINKWLAN  2
#define CNETWLAN    3
#define CDMA        4
#define GSM_GPRS    5
#define PSTN        6
#define SERIAL      7
#define COMBO	    8 		

#define  SUCCESS   0
#define  ERROR    -1

#define LINESIZE 5000
#define MAX_THUMBPRINT_SIZE 2000

/******************* scroll menu structure*********************/

typedef struct
{
    short start;
    short maxEntries;
    char selectedIndex;
    char title[44];
    char menu[MAX_MENU_ENTRIES][MAX_ENTRY_LENGTH];

}MENU_T;

typedef int BOOL;

char *toUpper (char *strToConvert);
char *toLower (char *strToConvert);


/*********************main_menu functions**********************/

int ramdisk_chk(void);                // Checking for ramdisk Type

int scroll_menu(MENU_T *,int *,int);  // Scroll menu function


int Display_test(void);

int Keypad_test(void);

int Printer_test(void);

int Comm_port_settings(void);

int MagHead_test(void);

int Ifd_test(void);

int Battery_test(void);

int Comm_test(void);

int USB_test(void);

int Download_menu(void);

int versioninfo(void);

int Setup_menu(void);

int jffs2_menu(void);

int get_machineid(void);

FILE *fp;
/**************** Time functios ******************/
void inttime(void);
void outtime(void);
void main_init_time(void);
void main_outt_time(void);
/***************************Data base Functions***************/
int enter_database(char[],int,int,char[],char[],char[],int);
int get_database(void);


typedef struct resultset_struct
{
	int rows;
	int cols;
	char ***recordset;
} resultset;
extern resultset get_result (const char *fmt, ...);

/*********************** lcd functions ************************/

int Display_test(void);               // lcd main menu
             
int backlight_test(void);             // Back light test
                        
int contrast_test(void);              // other oprtons

int increment_contrast(void);

int contrast_id(void);

                               


/************************ Printer Punctions ****************************/

int printer_test(void);       // printing test

int paperstatus_test(void);   // paper status test

int motor_test(void);         // motor moving test

int font_test(void);          // font test

/***************************  serial.c  ************************************/

/***************************  main_menu.c  ************************************/
int getUserName();
int getPasswd();
FILE *fp;
char userName[20];
char password[20];
char office_id[20];
char *toUpper(char *);
void *syncThread (void *arg);

/***************************  main_menu.c  ************************************/

/***************************  retail.c  ************************************/
typedef struct
{
	char company_name[100];
	char address_line_1[100];
	char address_line_2[100];
	char address_line_3[100];
	char city[100];
	char state[100];
	char country[100];
	char postcode[20];
	char telephone[100];
	char mobile[100];
	char fax[100];
	char e_mail[100];
} ADDRESS;

typedef struct
{
	char office_id[10]; 
	char order_id[10];
	char card_id[20];
	char member[61];
	int member_slno;
	char card_type[20];
	char order_date[20];
	char order_type[2];
	float amount;
	float vat;
	float discount;
	char payment_mode[2];
	char status[2];
	char remarks[100];
	char user_id[10];
	char reconcile_status[2];
	char sync_status[2];
} ORDER_HEADER;

typedef struct cartITEM
{
	char product_id[20]; //Product id
	char unique_code[20]; //Bar code
	char product_name[100];
	char allotment_type[10];
	float price_upper_limit;
	float price_lower_limit;
	float quantity;
	float actual_price;
	float vat;
	int uom;
	float max_quantity;
	float availed;
	long scheme_id;
	char warranty_start_date[11];
	char warranty_end_date[11];
	char prod_code[20]; //IMEI
	struct cartITEM *next;
} CART_ITEM;

ORDER_HEADER *order_header;
CART_ITEM *cart;

typedef struct
{
	char card_id[16]; 
	char district[3]; 
	char mandal[3];
	char fpshop[4];
	char no_of_members[3];
	char family_head[61];
	char card_type[4];
	char surname[61];	
} CARD;

CARD *card;

typedef struct MemberLL
{
	char slno[3]; 
	char member[61]; 
	char age[4];
	char relation[3];
	char fp_lif[513];
	char fp_rif[513];
	char fp_ltf[1025];
	int sz_lif;
	int sz_rif;
	int sz_ltf;
	struct MemberLL *next;
} MEMBER;

typedef struct DistLL
{
	char product_id[11];
	char name[101];
	char card_type[3];
	char allotment_type[3];
	char quantity[11];
	char price[9];
	char uom[3];
	char max_quantity[11];
	char scheme_id[5];
	char availed[9];
	struct DistLL *next;
} DISTRIBUTION;

char IP[100];
char url[1000];
char terminal[100];
char svr_username[100];
char svr_password[100];
char tempFile[100];
char gprs_domain[100];
char authMode[50];
char authBio[50];
char shop_id[10];
char autoUpdate[20];

int retail_sale();
int add_item(CARD *card, CART_ITEM *cart_item, DISTRIBUTION *distribution);
BOOL is_item_available();
void get_item_details();
int mark_item_blocked(char *unique_code);
int clear_cart();
int remove_item(char *unique_code);
int show_cart();
int payment();
int print_quote();
int bill();
int edit_cart();
int order_menu();
int print_bill();
int item_menu(CARD *card, DISTRIBUTION *distribution);
int insert_orders_in_process(CART_ITEM *cart_item);
int begin_order(CARD *card);
int set_order_id();
int print_address(char *type);
int print_order(char *type);
int print_footer(char *type);
int authenticate(CARD *card, MEMBER *member, char *thumbprint);
int display_menu(CARD *card, resultset ptable, DISTRIBUTION *distribution);

int (*read_card)(CARD *card, MEMBER *member);
int read_card_file(CARD *card, MEMBER *member);
int read_card_smart(CARD *card, MEMBER *member);
int read_card_rfid(CARD *card, MEMBER *member);
int read_card_contact(CARD *card, MEMBER *member);

int (*auth_card)(CARD *card, MEMBER *member, char *thumbprint);
int auth_card_file(CARD *card, MEMBER *member, char *thumbprint);
int auth_card_smart(CARD *card, MEMBER *member, char *thumbprint);
int auth_card_rfid(CARD *card, MEMBER *member, char *thumbprint);
int auth_card_contact(CARD *card, MEMBER *member, char *thumbprint);

int (*readThumb)(char *thumbprint);
int readThumb_file(char *thumbprint);
int readThumb_smart(char *thumbprint);
int readThumb_rfid(char *thumbprint);
int readThumb_contact(char *thumbprint);

int (*getCardDetails)(CARD *card, char *line);
int getCardDetails_file(CARD *card, char *line);
int getCardDetails_smart(CARD *card, char *line);
int getCardDetails_rfid(CARD *card, char *line);
int getCardDetails_contact(CARD *card, char *line);

int (*getMemberDetails)(CARD *card, MEMBER *memberCurr, int i, char *line);
int getMemberDetails_file(CARD *card, MEMBER *memberCurr, int i, char *line);
int getMemberDetails_smart(CARD *card, MEMBER *memberCurr, int i, char *line);
int getMemberDetails_rfid(CARD *card, MEMBER *memberCurr, int i, char *line);
int getMemberDetails_contact(CARD *card, MEMBER *memberCurr, int i, char *line);

char *getCardTypeName(char *card_type, char *card_type_name);
int initCartItem(CART_ITEM *cart_item);
int display_member_menu(CARD *card, MEMBER *member);
int show_thumb_menu(MEMBER *memberCurr);

/***************************  retail.c  ************************************/

/***************************  auth_rfid.c  ************************************/
#define CARD_DOES_NOT_EXIST -1
int rfid_setup(CARD *card);

/***************************  auth_rfid.c  ************************************/

/***************************  auth_smart.c  ************************************/
int Ifd_setup();
/***************************  auth_smart.c  ************************************/

int convertTOUPEK(char *inFile, char *outFile);

/***************************  login.c  ************************************/
BOOL userVerifStatus;
#define USER_DOES_NOT_EXIST -1
#define USER 1
#define ADMIN 2
/***************************  login.c  ************************************/

/***************************  soap.c  ************************************/
typedef struct 
{
    long MsgNo;
    int LoginID;
    char Password[100];
    char DateTime[25];
    int VendorID;
    int TerminalID;
    char SwVersion[10];	
} LoginRequest;

typedef struct 
{
    char response[100];	
} LoginResponse;


typedef struct _Commodity
{
	char CommodityCode[20];
	float Quantity;
	float Price;
	struct _Commodity *next;
} Commodity;


typedef struct
{
	long MsgNo;
	int TransactionNo;
	char CardNo[50];
	char MemberId[100];
	Commodity *commodity;
	float TotalPrice;
	int SchemeCode;
	char SessionId[100];
	char DateTime[25];
} TransactionRequest;

typedef struct 
{
    char response[100];	
} TransactionResponse;

typedef struct
{
	long MsgNo;
	char sessionID[100];
	char DateTime[50];
} LogoutRequest;

typedef struct 
{
    char response[100];	
} LogoutResponse;

int svrLogin(LoginRequest *);
int postTxn(ORDER_HEADER *oh,  CART_ITEM *cart);
int svrLogout(LogoutRequest *);

int invokeSoap(char *requestBuf,char *url,char *action, char *responseBuf);	

/***************************  soap.c  ************************************/

/***************************  synchronise.c  ************************************/
int update_order(ORDER_HEADER *oh,char status);
int populateRequest(TransactionRequest *tr, ORDER_HEADER *oh, CART_ITEM *cart);	

int populateOrder(ORDER_HEADER *oh, CART_ITEM *cart, int i, resultset table);
int insert_remote_bill_details(ORDER_HEADER *oh, CART_ITEM *cart);
int LoadDataFromFile2();

/***************************  synchronise.c  ************************************/

/***************************  globals.c  ************************************/
int ret;
BOOL printFlag;
char background[10];
char SESSION_ID[100];

char DATABASE[100];
char LOG_FILE[100];
char CONFIG_FILE[100];
char CURRENCY[10];
char OFFICE_ID[10];
ADDRESS OFF_ADDRESS;
char USER_NAME[50];
char TILL[50];
char tmpbuf[100];

void init_globals();
/***************************  globals.c  ************************************/


/***************************  reader.c  ************************************/
char *getVal(char *);
/***************************  reader.c  ************************************/
		
/***************************  exch_rate.c  ************************************/
int exch_rate();
/***************************  exch_rate.c  ************************************/

/***************************  download.c  ************************************/
int download();
/***************************  download.c  ************************************/

/***************************  utils.c  ************************************/
int setString(char* defaultVal, char *display, char *var, int length, BOOL clrFlag);
char *trim(char *val);
/***************************  utils.c  ************************************/

/***************************  db_config.c  ************************************/
char *get_config(char *key, char *value, char *office_id);
char *get_prop(char *key, char *value, char *office_id);
/***************************  utils.c  ************************************/
