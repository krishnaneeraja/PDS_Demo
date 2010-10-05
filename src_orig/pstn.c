#include <header.h>

typedef struct
{
  char phnum[32];
  char command[32];
  char dialno[32];
  char username[32];
  char password[32];
  char timeout[8];
} PSTN_SET;



char pstn_ip[50] = "";
extern char str5[20], str6[20];

int
get_pstnip ()
{
  int fd;
  struct ifreq ifr;
  fd = socket (AF_INET, SOCK_DGRAM, 0);
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy (ifr.ifr_name, "ppp0", IFNAMSIZ - 1);
  ioctl (fd, SIOCGIFADDR, &ifr);
  close (fd);
  sprintf (pstn_ip, "%d.%d.%d.%d", (unsigned char) ifr.ifr_addr.sa_data[2],
	   (unsigned char) ifr.ifr_addr.sa_data[3],
	   (unsigned char) ifr.ifr_addr.sa_data[4],
	   (unsigned char) ifr.ifr_addr.sa_data[5],
	   (unsigned char) ifr.ifr_addr.sa_data[6]);
  return 0;
}


int
comm_modem_test ()
{
  int opt = 0, ret;
  MENU_T menu;
  char event;


  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;

  menu.start = 0;
  menu.maxEntries = 4;
  maxEntries = menu.maxEntries;

  strcpy (menu.title, "PSTN Menu");

  strcpy (menu.menu[0], "Dial Mode");
  strcpy (menu.menu[1], "PSTN SDLC");
  strcpy (menu.menu[2], "Async-Normal");
  strcpy (menu.menu[3], "PPP Settings");
  while (1)
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);

      switch (opt)
	{
	case CANCEL:
	  return;
	case ENTER:
	case 1:
	case 2:
	case 3:
	case 4:
	  switch (selItem + 1)
	    {

	    case 1:
	      lk_dispclr ();
	      dialmode ();
	      break;
	    case 2:
	      lk_dispclr ();
	      pstn_sdlc_test ();
	      break;

	    case 3:
	      lk_dispclr ();
	      async_normal ();
	      break;
	    case 4:
	      lk_dispclr ();
	      ppp_test_pstn ();

	      break;
	    }
	default:
	  break;
	}
    }
}



int
dialmode ()
{
  int opt = 0;
  MENU_T menu;
  char event;


  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;


  menu.start = 0;
  menu.maxEntries = 2;
  maxEntries = menu.maxEntries;

  strcpy (menu.title, "Dial Menu");

  strcpy (menu.menu[0], "Pulse Test");
  strcpy (menu.menu[1], "Tone Test");
  while (1)
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);

      switch (opt)
	{
	case CANCEL:
	  return;
	case ENTER:
	case 1:
	case 2:
	  switch (selItem + 1)
	    {

	    case 1:
	      lk_dispclr ();
	      pulse_test ();
	      break;
	    case 2:
	      lk_dispclr ();
	      tone_test ();
	      break;
	    }
	default:
	  break;
	}
    }
}



int
pulse_test ()
{
  int res;
  unsigned char response[256] = "";
  static char phnum[32], dialnum[64];
  char *expect[] =
    { "CONNECT", "NO CARRIER", "NO DIALTONE", "LINE BUSY", "ERROR",
"DIALING OK", NULL };

  res = ModemOpen ("/dev/ttyS0");
  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      return ERROR;
    }

  setmodemparams (2400);
  memset (response, 0, 256);
  //   memset(phnum,0,32);
  //   memset(dialnum,0,64);
  lk_disptext (2, 3, "Enter Phone no. ", 0);
  res = lk_getnumeric (4, 0, phnum, 14, phnum, 4);

  if (res <= 0)
    {
      lk_dispclr ();
      lk_disptext (3, 0, "Phone Number Not Entered", 0);
      Modemclose ();
      return -1;
    }

  lk_dispclr ();
  lk_disptext (3, 4, "DIALING ....", 1);
  sprintf (dialnum, "ATDP%s;\r\n", phnum);
  res = Test_modem_response_without_signal (dialnum, response, expect, 60);
  fprintf (stderr, "The value of res=%d\n", res);

  lk_dispclr ();
  if (res != -2)
    lk_disptext (3, 3, expect[res], 1);
  else
    lk_disptext (3, 4, "DIALING OK", 1);
  lk_getkey ();
  Modemclose ();
  return 0;



}


int
tone_test ()
{
  int res;
  char response[256];
  char *expect[] =
    { "CONNECT", "NO CARRIER", "NO DIALTONE", "BUSY", "ERROR", "DIALING OK",
NULL };
  static char phnum[32], dialnum[64];

  res = ModemOpen ("/dev/ttyS0");
  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      return ERROR;
    }
  setmodemparams (2400);
  memset (response, 0, 256);
  //  memset(phnum,0,32);
  //  memset(dialnum,0,64);
  lk_disptext (2, 4, "Enter phone no. ", 0);
  res = lk_getnumeric (4, 0, phnum, 14, phnum, 4);
  if (res <= 0)
    {
      lk_dispclr ();
      lk_disptext (3, 0, "Phone Number Not Entered", 0);
      return -1;
      Modemclose ();
    }
  sprintf (dialnum, "ATDT%s;\r\n", phnum);
  lk_dispclr ();
  lk_disptext (3, 4, "DIALING ....", 1);
  res = Test_modem_response_without_signal (dialnum, response, expect, 60);
  fprintf (stderr, "The value of res=%d\n", res);
  lk_dispclr ();
  if (res != -2)
    lk_disptext (3, 3, expect[res], 1);
  else
    lk_disptext (3, 4, "DIALING OK", 1);
  lk_getkey ();
  Modemclose ();
  return 0;

}



int
pstn_sdlc_test ()
{
  int opt = 0;
  MENU_T menu;
  char event;


  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;

  PSTN_SET pstn_settings;

  menu.start = 0;
  menu.maxEntries = 3;
  maxEntries = menu.maxEntries;


  strcpy (menu.title, "PSTN SDLC Menu");

  strcpy (menu.menu[0], "Enter AT Commands");
  strcpy (menu.menu[1], "Enter PSTN Dial No.");
  strcpy (menu.menu[2], "Sync Connect Test");

  while (1)
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);
      switch (opt)
	{
	case CANCEL:
	  return;
	case ENTER:
	case 1:
	case 2:
	case 3:
	  switch (selItem + 1)
	    {

	    case 1:
	      lk_dispclr ();
	      pstn_AT_command (&pstn_settings);
	      break;
	    case 2:
	      lk_dispclr ();
	      enter_pstn_dialno (&pstn_settings);
	      break;

	    case 3:
	      lk_dispclr ();
	      sync_connect_test (&pstn_settings);
	      break;
	    }
	default:
	  break;
	}
    }
}


int
pstn_AT_command (PSTN_SET * settings)
{
  int opt;
  strcpy (settings->command, "AT+?");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter AT COMMAND", 0);
  opt = lk_getalpha (4, 0, settings->command, 19, settings->command, 2);
  if (opt < 0)
    return -1;
  else
    return 0;
}

int
enter_pstn_dialno (PSTN_SET * settings)
{
  int opt;
  strcpy (settings->phnum, "\0");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter PHONE NO.", 0);
  opt = lk_getalpha (4, 0, settings->phnum, 19, settings->phnum, 2);
  if (opt < 0)
    return -1;
  else
    return 0;

}


int
sync_connect_test (PSTN_SET * settings)
{
  int i = 0, response = 0, len = 0, key = 0;
  int k = 0;
  /* unsigned char data[]={2,0,48,32,5,128,46,192,
     0,4,0,0,0,0,0,0,
     0,3,114,0,0,2,0,0,33,
     0,34,0,51,71,97,115,144,
     1,1,0,16,209,1,34,1,
     17,67,135,128,128,52,49,53,
     49,49,49,49,49,48,50,49,
     50,52,49,53,49,49,49,48,
     48,57,57,57,57,57,57,57,
     57,49,49,49,49,49,49,49,
     49,32,32,32,32,32,32,32,
     0,6,48,48,48,48,48,50
     };
     unsigned char datarx[256]; */
  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;

  menu.start = 0;
  menu.maxEntries = 2;
  maxEntries = menu.maxEntries;

  strcpy (menu.title, "Dial Menu");

  strcpy (menu.menu[0], "Pulse Test");
  strcpy (menu.menu[1], "Tone Test");
  while (1)
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);

      switch (opt)
	{
	case CANCEL:
	  return;
	case ENTER:
	case 1:
	case 2:
	  switch (selItem + 1)
	    {

	    case 1:
	      lk_dispclr ();
	      lk_disptext (2, 4, "Opening Link", 0);
	      lk_disptext (2, 2, "Please Wait ....", 0);
	      // response=sdlc_open(settings->phnum,O_PULSE);
	      //dial to the given number using pulse dialing  
	      printf ("response=%d\n", response);
	      if (response == 1)
		{
		  lk_dispclr ();
		  lk_disptext (1, 2, "SDLC Conn. Success", 0);
		  lk_disptext (3, 0, "Press any key", 0);
		  lk_getkey ();
		  lk_dispclr ();
		  lk_disptext (1, 2, "Closing Sdlc Link", 0);
		  //response=sdlc_close();

		}

	      break;
	    case 2:
	      lk_dispclr ();
	      lk_disptext (2, 4, "Opening Link", 0);
	      lk_disptext (2, 2, "Please Wait ....", 0);
	      //response=sdlc_open(settings->phnum,O_TONE);
	      printf ("response=%d\n", response);
	      if (response == 1)
		{
		  lk_dispclr ();
		  lk_disptext (3, 3, "SDLC Conn. Success", 0);
		  lk_disptext (5, 5, "Press Any Key", 0);
		  lk_getkey ();
		  lk_dispclr ();
		  lk_disptext (3, 2, "Closing SDLC Link", 0);
		  //response=sdlc_close();
		  lk_dispclr ();
		  lk_disptext (1, 3, "Reboot System", 1);
		  lk_getkey ();
		}

	      //dial to the given number using tone dialing
	      break;
	    }
	default:
	  break;
	}
    }

  return 0;
}


int
async_normal ()
{
  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;

  PSTN_SET pstn_settings;
  menu.start = 0;
  menu.maxEntries = 3;
  maxEntries = menu.maxEntries;
  strcpy (menu.title, "ASYNC NORMAL Menu");

  strcpy (menu.menu[0], "Enter AT Commands");
  strcpy (menu.menu[1], "Enter Dial No.");
  strcpy (menu.menu[2], "Async Connect Test");

  while (1)
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);
      switch (opt)
	{
	case CANCEL:
	  return;
	case ENTER:
	case 1:
	case 2:
	case 3:
	  switch (selItem + 1)
	    {

	    case 1:
	      lk_dispclr ();
	      async_AT_commands (&pstn_settings);
	      break;
	    case 2:
	      lk_dispclr ();
	      async_dialno (&pstn_settings);
	      break;

	    case 3:
	      lk_dispclr ();
	      async_connect_test (&pstn_settings);
	      break;
	    }
	default:
	  break;
	}
    }
}


int
async_AT_commands (PSTN_SET * settings)
{
  int opt;
  strcpy (settings->command, "AT+?");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter AT COMMAND", 0);
  opt = lk_getalpha (4, 0, settings->command, 19, settings->command, 2);
  if (opt < 0)
    return -1;
  else
    return 0;

}

int
async_dialno (PSTN_SET * settings)
{
  int opt;
  strcpy (settings->phnum, "\0");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter PHONE NO.", 0);
  opt = lk_getalpha (4, 0, settings->phnum, 19, settings->phnum, 2);
  if (opt < 0)
    return -1;
  else
    return 0;


}

int
async_connect_test (PSTN_SET * settings)
{
  int res;
  char response[256];
  char *expect[] =
    { "CONNECT", "NO CARRIER", "NO DIALTONE", "BUSY", "ERROR", "OK", NULL };
  static char phnum[32], dialnum[64] = "ATDT";

  res = ModemOpen ("/dev/ttyS0");
  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      return ERROR;
    }
  setmodemparams (2400);
  //sprintf(settings->phnum,"%s%s;\r\n",settings->command,settings->phnum);
  res = Test_modem_response_without_signal ("AT\r\n", response, expect, 10);
  fprintf (stderr, "The value of res=%d\n", res);
  printf ("Response = %s \n", response);

  strcat (dialnum, settings->phnum);
  strcat (dialnum, "\r\n");
  lk_dispclr ();
  lk_disptext (3, 4, "DIALING ....", 1);
  res = Test_modem_response_without_signal (dialnum, response, expect, 60);
  fprintf (stderr, "The value of res=%d\n", res);
  printf ("Response = %s \n", response);

  if (res == 0)
    {
      lk_dispclr ();
      lk_disptext (2, 3, "Modem Connected", 0);
      lk_getkey ();
    }
  else
    {
      lk_dispclr ();
      lk_disptext (2, 3, "ERROR", 0);
      lk_getkey ();
    }
  Modemclose ();
  return 0;
}

int
ppp_test_pstn ()
{
  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;

  PPP_PSTN_SETTINGS settings;



  menu.start = 0;
  menu.maxEntries = 5;
  maxEntries = menu.maxEntries;
  strcpy (menu.title, "PSTN PPP Menu");
  strcpy (menu.menu[0], "Enter PPP Dial No.");
  strcpy (menu.menu[1], "Enter User name");
  strcpy (menu.menu[2], "Enter Password");
  strcpy (menu.menu[3], "Enter Timeout");
  strcpy (menu.menu[4], "PPP Test");
  while (1)
    {
      char timeout[6];
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);
      switch (opt)
	{
	case CANCEL:
	  return;
	case ENTER:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	  switch (selItem + 1)
	    {
	    case 1:
	      strcpy (settings.dialnum, "\0");
	      lk_dispclr ();
	      lk_disptext (2, 2, "Enter PHONE NO.", 0);
	      opt =
		lk_getalpha (4, 0, settings.dialnum, 19, settings.dialnum, 2);
	      if (opt < 0)
		return -1;
	      break;
	    case 2:
	      lk_dispclr ();
	      strcpy (settings.account, "\0");
	      lk_disptext (2, 2, "Enter User Name", 0);
	      opt =
		lk_getalpha (4, 0, settings.account, 30, settings.account, 2);
	      if (opt < 0)
		return -1;
	      break;
	    case 3:
	      lk_dispclr ();
	      strcpy (settings.password, "\0");
	      lk_dispclr ();
	      lk_disptext (2, 2, "Enter Password", 0);
	      opt =
		lk_getalpha (4, 0, settings.password, 19, settings.password,
			     2);
	      if (opt < 0)
		return -1;

	      break;
	    case 4:
	      strcpy (timeout, "50");
	      lk_dispclr ();
	      lk_disptext (2, 2, "Enter TIMEOUT", 0);
	      opt = lk_getalpha (4, 0, timeout, 19, timeout, 2);
	      if (opt < 0)
		return -1;
	      break;
	    case 5:
	      lk_dispclr ();
	      lk_disptext (2, 0, "Connecting To PPP ... ", 0);
	      lk_disptext (3, 4, "Please Wait", 0);

	      settings.auth = USE_PAP_AUTH;
	      strcpy (settings.localip, "0.0.0.0");
	      strcpy (settings.remoteip, "0.0.0.0");
	      strcpy (settings.netmask, "255.255.255.0");
	      ppp_pstn_update_settings (&settings);
	      opt = ppp_open_pstn ();
	      if (opt < 0)
		{
		  lk_dispclr ();
		  lk_disptext (2, 0, "ISP Connection", 0);
		  lk_disptext (3, 0, "Failed", 0);
		  lk_disptext (5, 0, "Press any key", 0);
		  lk_getkey ();
		  return -1;
		}
	      else
		{
		  char buffer[64];
		  get_pstnip ();
		  ppp_close_pstn ();
		  lk_dispclr ();
		  lk_disptext (2, 2, "PPP CONN. SUCCESS", 0);
		  lk_disptext (3, 0, "WITH", 0);
		  lk_disptext (3, 6, pstn_ip, 0);
		  lk_disptext (5, 3, "Press any key", 0);
		  lk_getkey ();
		  return 0;
		}
	      break;
	    }
	default:
	  break;
	}
    }
}



int
ppp_pstn_testing (PSTN_SET * settings)
{
  char buffer[64];
  pppondial_pstn_a ("9172233");
  ppp_options_pstn_a ("visiontek", "ippco123");
  ppp_pstn_a ("9172233", "visiontek", "ippco123", "0.0.0.0", "0.0.0.0",
	      "255.255.255.0");


  //  pppdial_pstn(settings->dialno);
  //  ppp_options(settings->username,settings->password);
  //  ppp_on_dial(settings->dialno);
  lk_dispclr ();
  lk_disptext (2, 0, "Connecting PPP", 0);
  lk_disptext (3, 0, "pls wait...", 0);
  if (ppp_open () < 0)
    {
      lk_dispclr ();
      lk_disptext (2, 0, "ISP Connection", 0);
      lk_disptext (3, 0, "Failed", 0);
      lk_disptext (5, 0, "Press any key", 0);
    }
  else
    {
      get_pstnip ();
      ppp_close ();
      lk_dispclr ();
      lk_disptext (2, 2, "PPP CONN. SUCCESS", 0);
      lk_disptext (3, 0, "WITH", 0);
      lk_disptext (3, 6, pstn_ip, 0);
      lk_disptext (5, 3, "Press any key", 0);
    }
  lk_getkey ();
  return 0;
}

int
pppondial_pstn_a (char *dialno)
{
  char str[1000];
  FILE *rfp;

  sprintf (str,
	   "#!/bin/sh\n/usr/sbin/chat -v      \\\n        TIMEOUT         3     \\\n        ABORT           \'\\nBUSY\\r\'      \\\n        ABORT           \'\\nNO ANSWER\\r\' \\\n        ABORT          \'\\nRINGING\\r\\n\\r\\nRINGING\\r\'      \\\n        \'\'              AT       \\\n        ECHO            ON     \\\n        OK             ATDT\'%s\'  \\\n        TIMEOUT         75     \\\n         CONNECT         \'\'",
	   dialno);
  rfp = fopen ("/etc/ppp/ppp-on-dialer", "w");

  if (rfp < 0)
    return -1;
  fprintf (rfp, "%s", str);
  fclose (rfp);
#ifdef TEST
  printf ("%s", str);
  printf ("\n");
#endif
  return 0;
}

int
ppp_options_pstn_a (char *userid, char *password)
{
  char str[1000];
  FILE *rfp;

  sprintf (str,
	   "-detach\nlock\nasyncmap 20a0000\nescape 0x10\nnocrtscts\ndefaultroute\nnomagic\nnopcomp\nnoaccomp\nmodem\nmru 1500\nmtu 1500\nrecord   /mnt/jffs2/log\n name %s\npassword %s\n",
	   userid, password);

//record /mnt/jffs2/glog
  rfp = fopen ("/etc/ppp/options", "w");
  if (rfp < 0)
    return -1;
  fprintf (rfp, "%s", str);
  fclose (rfp);
#ifdef TEST
  printf ("%s", str);
  printf ("\n");
#endif

  return 0;
}

int
ppp_pstn_a (char *dialno, char *userid, char *password, char *localip,
	    char *remoteip, char *netmask)
{
  char str[1000];
  FILE *rfp;

  sprintf (str,
	   "#!/bin/sh\nTELEPHONE=%s \nACCOUNT=%s \nPASSWORD=%s \nLOCAL_IP=%s \nREMOTE_IP=%s \nNETMASK=%s \nexport TELEPHONE ACCOUNT PASSWORD \nDIALER_SCRIPT=/etc/ppp/ppp-on-dialer \nexec /usr/sbin/pppd  debug /dev/ttyS0 2400  \\\n        $LOCAL_IP:$REMOTE_IP \\\n        connect $DIALER_SCRIPT ",
	   dialno, userid, password, localip, remoteip, netmask);

  rfp = fopen ("/etc/ppp/dialout", "w");
  if (rfp < 0)
    return -1;
  fprintf (rfp, "%s", str);
  fclose (rfp);
#ifdef TEST
  printf ("%s", str);
  printf ("\n");
#endif
  return 0;
}



int
pstn_remote ()
{
  char event;
  int opt = 0;
  PPP_PSTN_SETTINGS settings;
  char timeout[6];

  strcpy (settings.dialnum, "\0");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter PHONE NO.", 0);
  opt = lk_getalpha (4, 0, settings.dialnum, 19, settings.dialnum, 2);
  if (opt < 0)
    return -1;

  lk_dispclr ();
  strcpy (settings.account, "\0");
  lk_disptext (2, 2, "Enter User Name", 0);
  opt = lk_getalpha (4, 0, settings.account, 30, settings.account, 2);
  if (opt < 0)
    return -1;

  lk_dispclr ();
  strcpy (settings.password, "\0");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter Password", 0);
  opt = lk_getalpha (4, 0, settings.password, 19, settings.password, 2);
  if (opt < 0)
    return -1;

  strcpy (timeout, "50");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter TIMEOUT", 0);
  opt = lk_getalpha (4, 0, timeout, 19, timeout, 2);
  if (opt < 0)
    return -1;

  lk_dispclr ();
  lk_disptext (2, 0, "Connecting To PPP ... ", 0);
  lk_disptext (3, 4, "Please Wait", 0);
  settings.auth = USE_PAP_AUTH;
  strcpy (settings.localip, "0.0.0.0");
  strcpy (settings.remoteip, "0.0.0.0");
  strcpy (settings.netmask, "255.255.255.0");
  ppp_pstn_update_settings (&settings);
  opt = ppp_open_pstn ();
  if (opt < 0)
    {
      lk_dispclr ();
      lk_disptext (2, 0, "ISP Connection", 0);
      lk_disptext (3, 0, "Failed", 0);
      lk_disptext (5, 0, "Press any key", 0);
      lk_getkey ();
      return -1;
    }

  else
    {
      ftpdown_ke_ra_ap ();
      ppp_close_pstn ();
    }
}
