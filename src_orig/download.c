#include <header.h>

int
Download_menu (void)
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

  strcpy (menu.title, "Download Menu");
  strcpy (menu.menu[0], "Local");
  strcpy (menu.menu[1], "Remote ");

  while (1)
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);

      switch (opt)

	{
	case CANCEL:
	  return;

	case ENTER:

	  switch (selItem + 1)
	    {

	    case 1:

	      Local_test ();
	      break;

	    case 2:
	      Remote_test ();
	      break;
	    }
	default:
	  break;
	}

    }
  return 0;
}
