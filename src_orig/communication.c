#include <header.h>

extern char ramdisk_type;
extern char ethtype[30];

int
Comm_test (void)		// communication test
{


  switch (ramdisk_type)
    {

    case ETHERNET:		// ethernet module
      strcpy (ethtype, "eth0");
      ethernet_setup ();
      break;

    case RALINKWLAN:		// ralink wireless module
      strcpy (ethtype, "rausb0");
      ethernet_setup ();
      break;

    case CNETWLAN:		// CNET wireless module             
      strcpy (ethtype, "wlan0");
      ethernet_setup ();
      break;

    case CDMA:			// CDMA module
      cdma_test ();
      break;

    case GSM_GPRS:		//  GSM/GPRS module
      gsm_gprs_test ();
      break;

    case PSTN:			// PSTN module
      comm_modem_test ();
      break;

    case COMBO:
      combo_test ();
      break;

    case SERIAL:
      serial_test ();
      break;
    default:			// Communication detection error

      select_communication_module ();
      return -1;

    }

  return 0;
}


int
select_communication_module (void)
{
  int key, j = 0;
  char str[8][20] = {
    "ETHERNET",
    "RALINKWLAN",
    "CNETWLAN",
    "CDMA",
    "GSM-GPRS",
    "PSTN",
    "SERIAL",
    "COMBO"
  };



/*
     lk_dispclr();
     fprintf(stderr,"Communication Module Detection Error\n");
     lk_disptext(2,1,"COMMUNICATION MODULE",0);
     lk_disptext(3,3,"DETECTION ERROR",0);
     lk_getkey();*/


  while (1)
    {
      lk_dispclr ();
      lk_disptext (2, 0, "Do You want to select", 0);
      lk_disptext (3, 4, "Communication", 0);
      lk_disptext (5, 0, "ENTER->YES      X->NO", 0);
      key = lk_getkey ();

      if (key == CANCEL)
	return -1;

      else if (key == ENTER)
	{

	  for (;;)
	    {
	      lk_dispclr ();
	      lk_disptext (2, 7, "SELECT", 0);
	      lk_disptext (3, 0, "COMMUNICATION MODULE", 0);
	      lk_disptext (4, 0, str[j], 0);
	      lk_disptext (5, 0, "ENTER->YES      X->NO", 0);
	      lk_disphlight (4);
	      key = lk_getkey ();

	      if (key == CANCEL)
		return -1;

	      else if (key == ENTER)
		{
		  ramdisk_type = j + 1;
		  break;
		}

	      else if (key == 19)
		{
		  j++;
		  if (j == 8)
		    j = 0;
		}

	      else if (key == 18)
		{
		  j--;
		  if (j == -1)
		    j = 7;
		}

	    }

	}

      break;
    }
  return 0;
}

int
combo_test (void)
{

  MENU_T menu;
  char buf[100];
  int opt = 0, ret;
  int selItem = 0;
  int acceptKbdEvents;
  int maxEntries = 0;
  short scroll = 0;



  menu.start = 0;
  menu.maxEntries = 7;
  maxEntries = menu.maxEntries;

  strcpy (menu.title, "Communication");
  strcpy (menu.menu[0], "ETHERNET");
  strcpy (menu.menu[1], "WIFI-RALINK");
  strcpy (menu.menu[2], "WIFI-CNET");
  strcpy (menu.menu[3], "CDMA");
  strcpy (menu.menu[4], "GSM/GPRS");
  strcpy (menu.menu[5], "PSTN");
  strcpy (menu.menu[6], "SERIAL");

  while (1)
    {

      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);

      switch (opt)
	{
	case CANCEL:
	  return 0;

	case ENTER:

	  switch (selItem + 1)

	    {
	    case ETHERNET:	// ethernet module
	      strcpy (ethtype, "eth0");
	      ethernet_setup ();
	      break;

	    case RALINKWLAN:	// ralink wireless module
	      strcpy (ethtype, "rausb0");
	      ethernet_setup ();
	      break;

	    case CNETWLAN:	// CNET wireless module
	      strcpy (ethtype, "wlan0");
	      ethernet_setup ();
	      break;

	    case CDMA:		// CDMA module
	      cdma_test ();
	      break;

	    case GSM_GPRS:	//  GSM/GPRS module
	      gsm_gprs_test ();
	      break;

	    case PSTN:		// PSTN module
	      comm_modem_test ();
	      break;

	    case SERIAL:
	      serial_test ();
	      break;

	    }
	default:
	  break;
	}
    }

}
