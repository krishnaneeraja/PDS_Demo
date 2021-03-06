#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "header.h"
#include <stdbool.h>

#include "config.h"

extern int v_setValue (char *display, char *dbKey, char validationKey,
		       char *validationValue, char *defaultVal);
extern char *getVal (char *);
extern FILE *fp;
extern char *configFile;
extern char username[100];

#define  ACQ   1		// Acquire finger print for default user
#define  VER   2		// Verifying the Default  User
#define  ACQ_USER 3		// Acquire for the specified user
#define  VER_USER 4		// Verify for the specified user


int
admin_menu (void)
{
  int opt = 0, ret;
  MENU_T menu;
  char event;
  int selItem = 0, j;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  char password[16];

  short scroll = 0;
  menu.start = 0;
  menu.maxEntries = 6;
  maxEntries = menu.maxEntries;

  lk_dispclr ();

  for (j = 0; j < 15; j++)
    password[j] = '\0';

  lk_disptext (2, 5, "Password", 0);
  if (lk_getalpha (4, 0, password, 15, password, 1) < 0)
    return ERROR;

  if (strcmp (password, getVal ("admin_pwd")) != 0)
    return;

  lk_dispclr ();

  strcpy (menu.title, "Admin Menu ");
  strcpy (menu.menu[0], "Acquire User");
  strcpy (menu.menu[1], "Delete User");
  strcpy (menu.menu[2], "Synchronise");
  strcpy (menu.menu[3], "Configuration");
  strcpy (menu.menu[4], "Bash");
  strcpy (menu.menu[5], "Back to Main Menu");

  while (1)
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);

      switch (opt)
	{
	case CANCEL:
	  return 0;
	  break;
	case ENTER:

	  switch (selItem + 1)
	    {
	    case 1:
	      finger (ACQ_USER);
	      break;
	    case 2:
	      delete ();
	      break;
	    case 3:
	      upDown ((void *) 0);
	      break;
	    case 4:
	      configuration ();
	      break;
	    case 5:
	      return 2;
	    case 6:
	      return;
	    }

	default:
	  break;
	}
    }
  return SUCCESS;
}

int
bash ()
{
  lk_close ();			// closing pinpad
  system ("init 0");
}

int
delete ()
{
  char buffer[128];
  char name[128];

  lk_dispclr ();
  lk_disptext (1, 1, "Enter Employee Id", 1);

  if (lk_getalpha (4, 1, username, 15, username, 4) < 0)
    return ERROR;

  lk_dispclr ();
  lk_disptext (1, 1, "Confirm delete user?", 0);

  sprintf (buffer, "Emp Id %s: %s", username, getName (username));
  if (v_prompt (buffer) == 0)
    if (deleteUser (username) == 1)
      {
	lk_dispclr ();
	lk_disptext (2, 1, "Deleted user successfully", 1);
	lk_getkey ();
      }
}

int
v_prompt (char *msg)
{
  char sprompt[2];
  char prompt;
  char temp[4][100];
  int ikey;
  int pMetReading;

  lk_disptext (2, 1, msg, 0);

  sprintf (temp[3], "X-CANCEL    ENTER-OK");
  lk_disptext (4, 0, temp[3], 1);

  ikey = lk_getkey ();

  if (ikey == ENTER)
    {
      return 0;
    }
  else if (ikey == CANCEL)
    {
      return 1;
    }

  return 1;
}

int
configuration ()
{
  int c;
  int breakFlag = 1;

  int opt = 0, ret;

  MENU_T *menu, menu1;
  char event;
  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  char cmd[50];

  short scroll = 0;

  fprintf (fp, "Begun configuration\n");
  fflush (fp);

  if (read_config (configFile) == 1)
    {
      lk_dispclr ();
      lk_disptext (1, 1, "Error reading config", 1);
      lk_disptext (4, 1, "Press any key to exit", 1);
      lk_getkey ();
      exit;
    }

  menu = &menu1;

  while (breakFlag)
    {
      lk_dispclr ();
      read_config (configFile);

      strcpy (menu->title, "Configuration");
      strcpy (menu->menu[0], "Server App URL");
      strcpy (menu->menu[1], "Admin User Id");
      strcpy (menu->menu[2], "Admin Password");
      strcpy (menu->menu[3], "Server User");
      strcpy (menu->menu[4], "Server Pwd");
      strcpy (menu->menu[5], "Log file size");
      strcpy (menu->menu[6], "File Path");
      strcpy (menu->menu[7], "Communication Mode");
      strcpy (menu->menu[8], "Communication Type");
      strcpy (menu->menu[9], "Ethernet config");
      strcpy (menu->menu[10], "GPRS Setup");
      strcpy (menu->menu[11], "Sync Interval");
      strcpy (menu->menu[12], "Reset configuration");
      strcpy (menu->menu[13], "Back to Main Menu");

      menu->start = 0;
      menu->maxEntries = 14;

      opt = scroll_menu (menu, &selItem, acceptKbdEvents);

      switch (opt)
	{
	case CANCEL:
	  return 1;

	case ENTER:

	case 1:
	  switch (selItem + 1)
	    {
	    case 1:
	      v_setValue ("Server App URL", "remote_url", 'L', "45",
			  getVal ("remote_url"));
	      break;
	    case 2:
	      v_setValue ("Admin User Id", "admin_user", 'L', "5",
			  getVal ("admin_user"));
	      break;
	    case 3:
	      v_setValue ("Admin Password", "admin_pwd", 'L', "4",
			  getVal ("admin_pwd"));
	      break;
	    case 4:
	      v_setValue ("Server User", "server_user", 'L', "20",
			  getVal ("server_user"));
	      break;
	    case 5:
	      v_setValue ("Server Password", "server_pwd", 'L', "15",
			  getVal ("server_pwd"));
	      break;
	    case 6:
	      v_setValue ("Log file size", "logFileSize", 'L', "20",
			  getVal ("logFileSize"));
	      break;
	    case 7:
	      v_setValue ("Default File Path", "FilePath", 'L', "20",
			  getVal ("FilePath"));
	      break;
	    case 8:
	      v_setValue ("Communication Mode", "commMode", 'L', "20",
			  getVal ("commMode"));
		break;
	    case 9:
	      v_setValue ("Communication Type", "commType", 'L', "20",
			  getVal ("commType"));
		break;
	    case 10:
	      ethernet_setup ();
	      break;
	    case 11:
	      v_setValue ("GPRS Domain", "gprs_domain", 'L', "10",
			  getVal ("gprs_domain"));
	      break;
	    case 12:
	      v_setValue ("Sync Interval", "sleepTime", 'L', "5",
			  getVal ("sleepTime"));
	      break;
	    case 13:
	      sprintf (cmd, "rm -rf %s/%s", getVal ("FilePath"),
		       "config.txt");
	      system (cmd);
	      lk_dispclr ();
	      lk_disptext (1, 1, "Configuration erased", 1);
	      lk_disptext (3, 1, "Reboot device to reload", 1);
	      lk_disptext (5, 1, "factory settings.", 1);
	      lk_getkey ();
	      break;
	    case 14:
	      return;

	    }
	}
    }

  fprintf (fp, "End configuration\n");
  fflush (fp);
}

int
v_setValue (char *display, char *dbKey, char validationKey,
	    char *validationValue, char *defaultVal)
{
  int i = 0;
  char sql[5000];
  int rc = 0;
  bool validation = FALSE;
  char value[50];
  char temp[50];

  fprintf (fp, "Begun v_setValue:%s\n", display);

  lk_dispclr ();

  sprintf (temp, "Set %s", display);
  lk_disptext (0, 0, temp, 0);

  strcpy (value, defaultVal);

  fprintf (fp, "This is the default value: %s\n", defaultVal);
  fflush (fp);

  if (validationKey == 'L')
    lk_getalpha (3, 0, value, atoi (validationValue), value, 3);
  else
    lk_getalpha (3, 0, value, 10, value, 3);

  if (strlen (value) == 0)
    return;

  fprintf (fp, "Calling setVal with key=%s and value=%s\n", dbKey, value);
  fflush (fp);

  setVal (dbKey, value);
  write_config (configFile);

  fprintf (fp, "Write config with file=%s\n", configFile);
  fflush (fp);

  fprintf (fp, "Ended v_setValue\n");

}
