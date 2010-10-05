#include <header.h>

typedef struct
{
  char dialno[32];
  char username[32];
  char password[32];
  char timeout[8];
} CDMA_SET;

char cdma_ip[50] = "";

int
cdma_test ()
{
  int opt = 0;
  MENU_T menu;
  char event;


  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;


  char str[1000];
  FILE *rfp;
  char uname[20], psw[20], dialno[20];
  int ret;
  static int cdma = 1;
  memset (uname, 0, 20);
  memset (psw, 0, 20);
  memset (dialno, 0, 20);


  menu.start = 0;
  menu.maxEntries = 4;
  maxEntries = menu.maxEntries;

  strcpy (menu.title, "CDMA Menu");

  strcpy (menu.menu[0], "CDMA Test");
  strcpy (menu.menu[1], "RUIM Status");
  strcpy (menu.menu[2], "CDMA PPP Test");
  strcpy (menu.menu[3], "RUIM PPP Test");

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
	      cdma_testing ();
	      break;
	    case 2:
	      check_uim ();
	      break;
	    case 3:
	      ppp_test_cdma (1);
	      break;
	    case 4:
	      ppp_test_cdma (3);
	      break;
	    }
	default:
	  break;
	}
    }
}

	/**************************/

int
cdma_testing ()
{
  int opt = 0;
  MENU_T menu;
  char event;


  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;

  char str[1000];
  FILE *rfp;
  char uname[20], psw[20], dialno[20];
  memset (uname, 0, 20);
  memset (psw, 0, 20);
  memset (dialno, 0, 20);


  menu.start = 0;
  menu.maxEntries = 4;
  maxEntries = menu.maxEntries;

  strcpy (menu.title, "CDMA Test Menu");
  strcpy (menu.menu[0], "Check Model");
  strcpy (menu.menu[1], "Get ESN Number");
  strcpy (menu.menu[2], "Check NW Reg.");
  strcpy (menu.menu[3], "Check sig strength");

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
	      check_model ();
	      break;
	    case 2:
	      get_ESN ();
	      break;
	    case 3:
	      check_network_reg ();
	      break;
	    case 4:
	      check_signal_strength ();
	      break;
	    }
	default:
	  break;
	}
    }
}

	  /********************/


int
check_model ()
{
  int res, i;
  char response[256];
  char *expect[32] = { "OK", "ERROR", "READY", NULL };

  res = ModemOpen ("/dev/ttyS0");
  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      return ERROR;
    }

  setmodemparams (115200);
  //setmodemparams(9600);
  memset (response, 0, 256);
  res =
    Test_modem_response_without_signal ("ATI0I3\r\n", response, expect, 10);
  fprintf (stderr, "The value of res=%d\n", res);

  printf ("Response of Check Model =%s\n", response);
  for (i = 0; i < strlen (response); i++)
    {
      if (response[i] < 0x20)
	response[i] = 0x20;
      else if (response[i] == 'O' && response[i + 1] == 'K')
	{
	  response[i] = '\0';
	  break;
	}
    }
  Modemclose ();
  lk_dispclr ();
  lk_disptext (1, 0, response + 6, 0);
  lk_getkey ();
  return 0;
}

						/************************/
int
get_ESN ()
{
  int res, i;
  char response[256], flag = 0;
  char *expect[32] = { "OK", "ERROR", "READY", NULL };
  char *tmp;
  res = ModemOpen ("/dev/ttyS0");
  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      return ERROR;
    }

  setmodemparams (115200);
  //setmodemparams(9600);
  memset (response, 0, 256);
  res =
    Test_modem_response_without_signal ("AT+CGSN\r\n", response, expect, 10);
  fprintf (stderr, "The value of res=%d\n", res);
  printf ("Response of ESN = %s\n", response);
  tmp = strstr (response, ":");
  for (i = 2; i < strlen (tmp); i++)
    {
      if (tmp[i] < 0x20)
	break;
    }
  tmp[i] = '\0';
  Modemclose ();

  lk_dispclr ();
  lk_disptext (1, 4, "ESN NUMBER", 0);
  lk_disptext (3, 2, tmp + 2, 0);

  lk_getkey ();
  return SUCCESS;
}

						/********************/

int
check_network_reg ()
{
  int res, i;
  char response[256];
  char *expect[32] = { "OK", "ERROR", "READY", NULL };
  char *tmp;
  res = ModemOpen ("/dev/ttyS0");

  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      return ERROR;
    }

  setmodemparams (115200);
  //setmodemparams(9600);
  memset (response, 0, 256);
  res =
    Test_modem_response_without_signal ("AT+CREG?\r\n", response, expect, 10);
  fprintf (stderr, "The value of res=%d\n", res);
  printf ("Response = %s \n", response);
  tmp = strstr (response, ":");

  if (tmp == NULL)
    {
      Modemclose ();
      return ERROR;
    }

  if (tmp[2] == ',' && tmp[3] == '0' || tmp[3] == '2' || tmp[3] == '3')
    {
      lk_dispclr ();
      lk_disptext (2, 2, "NW NOT REGISTER", 0);
      lk_getkey ();

      Modemclose ();
      return ERROR;
    }

  if (tmp[2] == ',' && tmp[3] == '1')
    {

      lk_dispclr ();
      lk_disptext (2, 2, "NW REGISTERED", 0);
      lk_getkey ();
    }
  Modemclose ();
  return 0;
}

							/***********************/

int
check_signal_strength ()
{
  int res, i, result;
  char response[256];
  char *expect[32] = { "OK", "ERROR", "READY", NULL };
  char *c, num[7];

  res = ModemOpen ("/dev/ttyS0");
  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      return ERROR;
    }
  setmodemparams (115200);
  //setmodemparams(9600);
  memset (response, 0, 256);
  res =
    Test_modem_response_without_signal ("AT+CSQ?\r\n", response, expect, 10);
  fprintf (stderr, "The value of res=%d\n", res);

  c = strstr (response, ":");

  if (c != NULL)
    {
      for (i = 0; c[i] != ','; i++)
	{
	}

      num[0] = c[i - 2];
      num[1] = c[i - 1];
      num[2] = '\0';
      result = atoi (num);

      if (result >= 0 && result <= 31)
	{
	  lk_dispclr ();
	  lk_disptext (1, 2, "NW SIGNAL STRENGTH", 0);
	  lk_disptext (3, 6, num, 0);
	  lk_disptext (3, 9, "db", 0);
	  lk_getkey ();
	}
      else
	{
	  lk_dispclr ();
	  lk_disptext (3, 0, "BAD SIGNAL STRENGTH", 0);
	  lk_getkey ();
	}
    }

  Modemclose ();
  return 0;


}


int
check_uim ()
{
  int res;
  char response[256];
  char *expect[32] = { "READY", "ERROR", "OK", NULL };

  res = ModemOpen ("/dev/ttyS0");
  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      return ERROR;
    }
  setmodemparams (115200);
  memset (response, 0, 256);
  res =
    Test_modem_response_without_signal ("AT+CPIN?\r\n", response, expect, 10);
  fprintf (stderr, "The value of res=%d\n", res);
  printf ("Response = %s\n", response);

  if (res == 0)
    {
      lk_dispclr ();
      lk_disptext (2, 4, "UIM PRESENT", 0);
      lk_getkey ();
    }

  else
    {
      lk_dispclr ();
      lk_disptext (2, 3, "UIM NOT PRESENT", 0);
      lk_getkey ();
    }
  Modemclose ();
  return 0;
}

int
CDMA_AT_commands ()
{
  int res;
  char response[256];
  char *expect[32] = { "READY", "ERROR", "OK", NULL };

  res = ModemOpen ("/dev/ttyS0");
  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      return ERROR;
    }
  setmodemparams (115200);
  memset (response, 0, 256);
  res =
    Test_modem_response_without_signal ("AT+CRM=1\r\n", response, expect, 10);
  fprintf (stderr, "The value of res=%d\n", res);
  printf ("Response = %s\n", response);

  if (res == 2)
    {
      lk_dispclr ();
      lk_disptext (2, 4, "COMMAND VALID", 0);
      lk_getkey ();
    }

  else
    {
      lk_dispclr ();
      lk_disptext (2, 3, "COMMAND NOT VALID", 0);
      lk_getkey ();
    }
  Modemclose ();
  return 0;
}

int
get_cdmaip ()
{
  int fd;
  struct ifreq ifr;
  fd = socket (AF_INET, SOCK_DGRAM, 0);
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy (ifr.ifr_name, "ppp0", IFNAMSIZ - 1);
  ioctl (fd, SIOCGIFADDR, &ifr);
  close (fd);
  sprintf (cdma_ip, "%d.%d.%d.%d", (unsigned char) ifr.ifr_addr.sa_data[2],
	   (unsigned char) ifr.ifr_addr.sa_data[3],
	   (unsigned char) ifr.ifr_addr.sa_data[4],
	   (unsigned char) ifr.ifr_addr.sa_data[5],
	   (unsigned char) ifr.ifr_addr.sa_data[6]);
  return 0;
}



int
enter_pppdialno (CDMA_SET * settings)
{
  int opt;
  strcpy (settings->dialno, "#777");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter PHONE NO.", 0);
  opt = lk_getalpha (4, 0, settings->dialno, 19, settings->dialno, 2);
  if (opt < 0)
    return -1;
  else
    return 0;

}


int
enter_ppp_username (CDMA_SET * settings)
{

  int opt;
  strcpy (settings->username, "internet");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter User name", 0);
  opt = lk_getalpha (4, 0, settings->username, 30, settings->username, 2);
  if (opt < 0)
    return -1;
  else


    return 0;
}

int
enter_ppp_password (CDMA_SET * settings)
{
  int opt;
  strcpy (settings->password, "internet");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter Password", 0);
  opt = lk_getalpha (4, 0, settings->password, 19, settings->password, 2);
  if (opt < 0)
    return -1;
  else
    return 0;
}



int
enter_ppp_timeout (CDMA_SET * settings)
{
  int opt;
  strcpy (settings->timeout, "10");
  lk_dispclr ();
  lk_disptext (2, 2, "Enter TIMEOUT", 0);
  opt = lk_getalpha (4, 0, settings->timeout, 19, settings->timeout, 2);
  if (opt < 0)
    return -1;
  else
    return 0;
}



int
ppp_testing (CDMA_SET * settings, int arg)
{
  char buffer[64];

  pppdial_cdma (settings->dialno);

  if (arg == 3)
    ppp_options_ruim (settings->username, settings->password);
  else
    ppp_options (settings->username, settings->password);

  ppp_on_dial (settings->dialno);

  lk_dispclr ();
  lk_disptext (2, 2, "Connecting PPP", 0);
  lk_disptext (3, 3, "pls wait...", 1);
  if (ppp_open () < 0)
    {
      lk_dispclr ();
      lk_disptext (2, 0, "ISP Connection", 0);
      lk_disptext (3, 0, "Failed", 0);
      lk_disptext (5, 0, "Press any key", 0);
    }
  else
    {
      if (arg == 2)
	{
	  ftpdown_ke_ra_ap ();
	  ppp_close ();
	  return 0;
	}

      get_cdmaip ();
      fprintf (stderr, "before closing");


      ppp_close ();
      fprintf (stderr, "After closing");

      lk_dispclr ();
      lk_disptext (2, 2, "PPP CONN. SUCCESS", 0);
      lk_disptext (3, 0, "WITH", 0);
      lk_disptext (3, 6, cdma_ip, 0);
      lk_disptext (5, 3, "Press any key", 0);
    }
  lk_getkey ();
  return 0;

}

int
ppp_test_cdma (int arg)
{
  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;

  CDMA_SET cdma_settings;

  menu.start = 0;
  menu.maxEntries = 5;
  maxEntries = menu.maxEntries;
  if (arg == 3)
    strcpy (menu.title, "UIM PPP Menu");
  else
    strcpy (menu.title, "CDMA PPP Menu");

  strcpy (menu.menu[0], "Enter PPP Dial No.");
  strcpy (menu.menu[1], "Enter User name");
  strcpy (menu.menu[2], "Enter Password");
  strcpy (menu.menu[3], "Enter Timeout");
  strcpy (menu.menu[4], "PPP Test");
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
	case 5:
	  switch (selItem + 1)
	    {
	    case 1:
	      enter_pppdialno (&cdma_settings);
	      break;
	    case 2:
	      enter_ppp_username (&cdma_settings);
	      break;
	    case 3:
	      enter_ppp_password (&cdma_settings);
	      break;
	    case 4:
	      enter_ppp_timeout (&cdma_settings);
	      break;
	    case 5:
	      ppp_testing (&cdma_settings, arg);
	      break;
	    }
	default:
	  break;
	}
    }
}


int
cdma_remote ()
{
  CDMA_SET cdma_settings;

  enter_pppdialno (&cdma_settings);
  enter_ppp_username (&cdma_settings);
  enter_ppp_password (&cdma_settings);
  enter_ppp_timeout (&cdma_settings);
  ppp_testing (&cdma_settings, 2);
}
