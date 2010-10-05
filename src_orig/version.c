#include <header.h>

extern unsigned int ramdisk_type;


int
versioninfo ()
{

  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 0, ret;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;
  menu.start = 0;
  menu.maxEntries = 4;
  maxEntries = menu.maxEntries;

  strcpy (menu.title, "Version Menu");

  strcpy (menu.menu[0], "Check Kernel ver");
  strcpy (menu.menu[1], "Check Ramdisk ver");
  strcpy (menu.menu[2], "Check Appl ver");
  strcpy (menu.menu[3], "Boot-loader ver");

  while (1)

    {
      lk_dispclr ();
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
	      lk_disptext (2, 3, "KERNEL VERSION", 0);
	      lk_disptext (4, 8, "2.4", 0);
	      break;

	    case 2:
	      lk_dispclr ();
	      Ramdisk_image_version ();
	      break;
	    case 3:
	      lk_dispclr ();
	      lk_disptext (2, 0, "APPLICATION VERSION", 0);
	      lk_disptext (4, 9, "4.0", 0);
	      break;

	    case 4:
	      lk_dispclr ();
	      lk_disptext (2, 0, "BOOTLOADER VERSION", 0);
	      lk_disptext (4, 8, "1.0.0", 0);
	      break;
	    }
	}
      lk_getkey ();
    }
}

int
Ramdisk_image_version (void)
{
  int ret = 0;
  lk_dispclr ();
  lk_disptext (2, 3, "RAMDISK VERSION", 0);

  switch (ramdisk_type)
    {
    case RALINKWLAN:
      lk_disptext (3, 5, "RALINK 1.0", 0);
      break;

    case ETHERNET:
      lk_disptext (4, 5, "ETHERNET 1.0", 0);
      break;

    case CNETWLAN:
      lk_disptext (4, 5, "CNET 1.0", 0);
      break;

    case CDMA:
      lk_disptext (4, 5, "CDMA 1.0", 0);
      break;

    case GSM_GPRS:
      lk_disptext (4, 5, "GSM 1.0", 0);
      break;

    case PSTN:
      lk_disptext (4, 5, "PSTN 1.0", 0);
      break;
    case SERIAL:
      lk_disptext (4, 5, "SERIAL 1.0", 0);
      break;
    default:
      lk_disptext (4, 1, "COMMON RAMDISK 1.0", 0);


    }
  return SUCCESS;
}
