#include "header.h"

struct tm time_date;

int Setup_menu()
{
	MENU_T menu;
        int opt=0,ret=0;
        int selItem  = 0;
        int acceptKbdEvents = TRUE;
        int maxEntries = 0;
                                                                                                                             
        menu.start                      =       0;
        menu.maxEntries                =        4;
        maxEntries                      =       menu.maxEntries;
        strcpy(menu.title, "Setup ");
        strcpy(menu.menu[0], "Date");
        strcpy(menu.menu[1], "Time");
	strcpy(menu.menu[2], "Save and Exit");
        strcpy(menu.menu[3], "Poweroff");
	while(1){
		opt = scroll_menu(&menu,&selItem,acceptKbdEvents);
                switch(opt){
                        case CANCEL:
                                return 0;
                        case ENTER:
                                case 1: case 2:
                                switch (selItem+1){
                                        case 1:
						ret=Date();
						break;
					case 2:
						ret=Clock();
						break;
					case 3:
						if(ret>=6){	//valid time and date values
        						lk_setrtc(&time_date) ;
							lk_dispclr();
							lk_disptext(2,0,"Date and Time are set",1);
							sleep(1);
							return 0;
						}
						else{
							lk_dispclr();
                                                        lk_disptext(2,0,"Invalid Date or Time",1);
                                                        lk_disptext(4,0,"Enter Again",0);
							sleep(1);
						}

						break;
                                           
                                        case 4:	
						lk_dispclr();
						lk_disptext(2,0,"Shutting down system",1);			
						system ("init 0");
						sleep(4);
 						break;
 

				}
			default:
				break;
		}
	}
	
}
int Date()
{
	int ret=0;
	unsigned char ptr[10],wday[1];

	memset(ptr,0,10);
	lk_dispclr() ;
	lk_disptext (0x00,0x01,"Set the weekday",0x00) ;

        ret = setNumeric("", "Set the weekday", wday, 1) ;

        if (ret == -2 ){
                return ret;
        }
        
        ret = setNumeric("", "Set the date", ptr, 8) ;
        if (ret == -2 ){
        	return ret;
        }

        time_date.tm_mday=(ptr[0]-0x30)*10+(ptr[1]-0x30);
        time_date.tm_mon=(ptr[2]-0x30)*10+(ptr[3]-0x30);
        time_date.tm_year=((ptr[4]-0x30)*1000+(ptr[5]-0x30)*100+(ptr[6]-0x30)*10+(ptr[7]-0x30));
	time_date.tm_wday=wday[0];

	return ret;
}
int Clock()
{
	int ret=0;
	unsigned char ptr[10];
	
	ret = setNumeric("", "Set the time", ptr, 6) ;
        if (ret == -2 ){
                return ret;
        }
        time_date.tm_hour=(ptr[0]-0x30)*10+(ptr[1]-0x30);
        time_date.tm_min=(ptr[2]-0x30)*10+(ptr[3]-0x30);
        time_date.tm_sec=(ptr[4]-0x30)*10+(ptr[5]-0x30);

	return ret;
}

