#include "header.h"

/********************************** global varibles********************************************/

int ramdisk_type;

int initialize_db ();

void select1 (char *sql);
void parseFiles ();
int main_menu();


/***********************************************************************************************
                        MAIN MENU FOR RETAIL

***********************************************************************************************/
int
main (void)
{
	struct tm intim;
	int Ret_val;
	double amount;
	unsigned char str[100] = "";
	pthread_t syncT;
	pthread_attr_t attr;
	int newprio;
	struct sched_param param;
	int ret;

  
//fp = fopen (logFile, "w+");
  fp = stdout;

  fprintf(fp,"Begun main()");
  fflush(fp);

  lk_open ();
  prn_open ();
  mscr_open ();
  lk_dispinit ();  
  
  lk_dispclr();
  
  strcpy(DATABASE,"/mnt/jffs2/fps.sqlite");
  
  initialize_db ();

  open_sqlite (DATABASE);

  strcpy(CONFIG_FILE, get_config("CONFIG_FILE",tmpbuf,"0"));	//Need OFFICE ID From config file before init_globals  
  read_config (CONFIG_FILE);
  strcpy(OFFICE_ID,getVal("office_id")); // Get from config file 
  strcpy(SESSION_ID,"");
  
  fprintf(fp,"office_id:",OFFICE_ID);
  fflush(fp);
  
  init_globals();
  
	strcpy(IP,getVal("remote_url"));
	strcpy(shop_id,getVal("shop"));
	strcpy(terminal,getVal("terminal"));
	strcpy(tempFile,"/mnt/jffs2/dat");
	strcpy(svr_username,getVal("server_user"));
	strcpy(svr_password,getVal("server_pwd"));
  
	setAuthType();
  
  if(strcmp(toUpper(getVal("show_splash")),"TRUE")==0)
  {
	  lk_disptext (1, 0, "        PDS ", 1);
	  lk_disptext (3, 0, "     Govt of AP", 0);
	  lk_disptext (4, 0, "    Version:0.9", 0);
	  lk_disptext (5, 0, "  Enter to continue", 0);
	  lk_getkey ();
	  lk_dispclr ();
  }

	if(strcmp(toUpper(getVal("commMode")),"ETHERNET") == 0)
		ip_setup ();

	strcpy(background,getVal("background"));
	
	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
	newprio = 1;
	param.sched_priority = newprio;
	pthread_attr_setschedparam(&attr,&param);
	
	if(strcmp(background,"true")==0){
		fprintf(fp,"Starting a background thread");
		fflush(fp);
	
		if (pthread_create (&syncT, &attr, syncThread, NULL) != 0)
			fprintf (stderr, "Error creating the thread");
	}

  while(1){
	  ret = login();
	  
	  printFlag = FALSE;
	  
	  if(ret==USER)
		  main_menu();
	  else if(ret == ADMIN)
		  admin_menu();
	  else{
		  lk_dispclr();
		  lk_disptext (2, 0, "Invalid credentials", 0);
		  lk_getkey ();
		  lk_dispclr ();
	  }
  }

  prn_close ();			// closing printer
  lk_close ();			// closing pinpad
  mscr_close ();		// closing magnetic swipe
  close_sqlite ();

  fprintf(fp,"Ended main()");
  fflush(fp);
  
  return SUCCESS;
}

int setAuthType()
{
//	int (*read_card)(CARD *card, MEMBER *member);
//	int read_card_file(CARD *card, MEMBER *member);
//
//	int (*auth_card)(CARD *card, MEMBER *member, char *thumbprint);
//	int auth_card_file(CARD *card, MEMBER *member, char *thumbprint);
//
//	int readThumb(char *thumbprint);
//	int readThumb_file(char *thumbprint);
//
//	int (*getCardDetails)(CARD *card, char *line);
//	int getCardDetails_file(CARD *card, char *line);
//
//	int (*getMemberDetails)(MEMBER *memberCurr, int i, char *line);
//	int getMemberDetails_file(MEMBER *memberCurr, int i, char *line);

	if(strcmp(getVal("authMode"),"file")==0)
	{
		read_card = &read_card_file;
		readThumb = &readThumb_file;
		auth_card = &auth_card_file;
	}
	else if(strcmp(getVal("authMode"),"rfid")==0)
	{
		read_card = &read_card_rfid;
		readThumb = &readThumb_rfid;
		auth_card = &auth_card_rfid;
	}
	else if(strcmp(getVal("authMode"),"smart")==0)
	{
		read_card = &read_card_smart;
		readThumb = &readThumb_smart;
		auth_card = &auth_card_smart;
	}
	else if(strcmp(getVal("authMode"),"contact")==0)
	{
		read_card = &read_card_contact;
		readThumb = &readThumb_contact;
		auth_card = &auth_card_contact;
	}
}

int getPending()
{
	char sql[100];
	resultset table;
	int count;

	sprintf(sql,"SELECT count(*) FROM ORDER_HEADER WHERE SYNC_STATUS=0");

	table = get_result(sql);
	fprintf(fp,"After executing query:%s\n",sql);
	fprintf(fp,"No. of rows:%d\n",table.rows);

	fflush(fp);	
	
	if(table.rows > 0 && strcmp(table.recordset[0][0], "") != 0)
	{
			count = atoi(table.recordset[0][0]); 
	}
	
	free_result(table);
	
	return count;
}

int main_menu()
{
        MENU_T menu;
        int opt=0;
        int selItem  = 0;
        int acceptKbdEvents=0;

        //lk_getkey();  //Commented by Neeraja 08/06/2010 for 2 times pressed after entered password field.
        lk_dispclr(); 

        fprintf(fp,"Begun main_menu()");
        fflush(fp);
        
        menu.start                      =       0;
        menu.maxEntries                 =       4;
 
        strcpy(menu.title,"Main Menu");                                                                                     
        strcpy(menu.menu[0],"Retail Sale");
        strcpy(menu.menu[1],"Reprint");
        strcpy(menu.menu[3],"Logout");

	while(1){
			sprintf(menu.menu[2],"Update Pending (%d)",getPending());
          
        	lk_dispclr();
               	opt = scroll_menu(&menu,&selItem,acceptKbdEvents);
		switch(opt){
                  
                   	case ENTER:

                		switch (selItem+1){
                        		case 1: retail_sale(); break; 	//retail.c
                        		
                        		case 2: reprint();
                        				break;
                        				
                        		case 3: updatePendingBatch();
                        				break;
                        				
                        		case 4: logout();
                        				return SUCCESS; 
                        				break;                        
										
                         		default:   break;						
	  
                  		} 
         	}
	}
}

int logout()
{
//Send logout request to server	
}

int
initialize_db ()
{
  int rc = 1;
  struct stat buf;
  int i = 0;
  FILE *fp;

  fp = stdout;

  fprintf (fp, "Begun initialize_db\n");

  fprintf (fp, "1...Initializing database from path...%s\n", DATABASE);
  i = stat (DATABASE, &buf);
  fprintf (fp, "2...Initializing database...:%d\n", i);

  if ((i < 0) || (buf.st_size == 0))
    {
      fprintf (fp, "Initializing database...\n");
      rc = open_sqlite (DATABASE);
      if (rc == 0)
	{
	  fprintf (fp, "Create database failure!\n");
	}
      close_sqlite ();
    }

  fprintf (fp, "Ended initialize_db\n");

  return rc;
}


int admin_menu()
{
	int breakFlag=1;

    MENU_T *menu, _menu;
    int opt=0;
    int selItem  = 0;
    int acceptKbdEvents=0;
    
	fprintf(fp,"Begun v_show_admin_menu\n");
	fflush(fp);

	menu = &_menu;
	
    menu->start          =       0;
    menu->maxEntries     =       14;

	while(breakFlag)
	{	
		lk_dispclr();
	
		strcpy(menu->title,"Admin Panel");                                                                                     
		strcpy(menu->menu[0],"Terminal ID");
		strcpy(menu->menu[1],"Date & Time");
		strcpy(menu->menu[2],"GPRS Domain");
		strcpy(menu->menu[3],"Server IP");
		strcpy(menu->menu[4],"Shop Id");
		strcpy(menu->menu[5],"User");
		strcpy(menu->menu[6],"Password");
		strcpy(menu->menu[7],"Download DB");
		strcpy(menu->menu[8],"Authentication Type");
		strcpy(menu->menu[9],"Validate Bio");
		strcpy(menu->menu[10],"Auto Sync");
		strcpy(menu->menu[11],"Auto Update");
		strcpy(menu->menu[12],"Bash");
		strcpy(menu->menu[13],"Exit");

		opt = scroll_menu(menu,&selItem,acceptKbdEvents);

		switch(opt)
		{		  
		   case CANCEL:
		               return 1;

		   case ENTER:

		   case 1: 
			switch(selItem+1)
			{
			//			int setString(char *defaultVal, char *display, char *var, int length)
				case 1:	setString(getVal("terminal"), "Terminal Id", terminal, 10, TRUE);	
						setVal("terminal",terminal);
						write_config(CONFIG_FILE);
						break;
				case 2:	Setup_menu();  //Date time
						break;
				case 3:	setString(getVal("gprs_domain"), "GPRS Domain", gprs_domain, 20, TRUE);	
						setVal("gprs_domain",gprs_domain);
						write_config(CONFIG_FILE);
						break;
				case 4:	setString(getVal("remote_url"), "Remote URL", url, 255, TRUE);
						setVal("remote_url",url);
						write_config(CONFIG_FILE);
						break;
				case 5:	setString(getVal("shop"), "Shop Id", shop_id, 255, TRUE);
						setVal("shop_id",shop_id);
						write_config(CONFIG_FILE);
						break;
				case 6:	setString(getVal("server_user"), "Server User", svr_username, 20, TRUE);
						setVal("server_user",svr_username);
						write_config(CONFIG_FILE);
						break;
				case 7:	setString(getVal("server_pwd"), "Server Pwd", svr_password, 20, TRUE);
						setVal("server_pwd",svr_password);
						write_config(CONFIG_FILE);
						break;
				case 8:	//ethernet();
						//downloadDB();
						break;
				case 9:setString(getVal("authMode"), "Authentication Type", authMode, 20, TRUE);
						setVal("authMode",authMode);
						write_config(CONFIG_FILE);
						break;
				case 10:setString(getVal("authBio"), "Validate Bio", authBio, 20, TRUE);
						setVal("authBio",authBio);
						write_config(CONFIG_FILE);
						break;
				case 11:setString(getVal("background"), "Auto Sync", background, 20, TRUE);
						setVal("background",background);
						write_config(CONFIG_FILE);
						break;
				case 12:setString(getVal("autoUpdate"), "Auto Update", autoUpdate, 20, TRUE);
						setVal("autoUpdate",autoUpdate);
						write_config(CONFIG_FILE);
						break;
                case 13:lk_dispclr();

		                        lk_disptext(2,5,"Bash Prompt",1);
		                        prn_close();  // closing printer
		                        lk_close();   // closing pinpad
		                        sleep(2);
		                        exit(0);
		                        break;

				case 14:breakFlag=0;
						break;
			}
		}
	}

	fprintf(fp,"End v_show_admin_menu\n");
	fflush(fp);
}


