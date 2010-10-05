#include <header.h>

int
Setup_menu (void)
{
  int opt = 0, ret;
  MENU_T menu;
  char event;

  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;
  struct tm curt;

  memset (&curt, 0, sizeof (struct tm));
  menu.start = 0;
  menu.maxEntries = 3;
  maxEntries = menu.maxEntries;

  strcpy (menu.title, "Setup");

  strcpy (menu.menu[0], "Set Time");
  strcpy (menu.menu[1], "Set Date");
  strcpy (menu.menu[2], "Time Format");


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
	case 6:
	  switch (selItem + 1)
	    {

	    case 1:

	      lk_getrtc (&curt);
	      time_setting (&curt);
	      lk_setrtc (&curt);
	      break;

	    case 2:
	      lk_getrtc (&curt);
	      Date_setting (&curt);
	      lk_setrtc (&curt);
	      break;


	    case 3:
	      notimplemented ();
	      break;

	    }
	default:
	  break;
	}

    }
}



int
Date_setting (struct tm *curt)
{
  unsigned char i, len = 0;
  char str[100] = "";
  int rval, flag = 0, key = 0;

  while (1)
    {
      memset (str, 0, 100);
      flag = 0;
      lk_dispclr ();
      lk_disptext (2, 0, "Enter Date DDMMYY", 0);
      rval = lk_getnumeric (4, 0, str, 6, str, 4);
      len = strlen (str);
      if (rval < 0)
	return -1;

      if (len != 6)
	flag = 1;

      for (i = 0; i < len; i++)
	{
	  if (str[i] == '.')
	    flag = 1;
	}

      if (flag == 0)
	{
	  if (chk_date (str) < 0)
	    flag = 1;
	}

      if (flag == 1)
	{
	  lk_dispclr ();
	  lk_disptext (3, 4, "TRY AGAIN", 1);
	  printf ("ENTER CORRECT DATA\n");
	  if (CANCEL == lk_getkey ())
	    return -1;
	}
      else
	break;
    }
  if (flag == 0)
    {
      curt->tm_mday = (str[0] - 0x30) * 10 + (str[1] - 0x30);
      curt->tm_mon = (str[2] - 0x30) * 10 + (str[3] - 0x30) - 1;
      curt->tm_year = (str[4] - 0x30) * 10 + (str[5] - 0x30) + 100;
      return 0;
    }
  else
    return -1;
}

int
time_setting (struct tm *curt)
{
  char str[100];
  int rval;
  memset (str, 0, 100);
  lk_dispclr ();
  while (1)
    {
      strcpy (str, "");
      lk_dispclr ();
      lk_disptext (2, 0, "Enter time HHMMSS", 0);
      lk_disptext (5, 0, "enter in 24hrs format", 0);
      rval = lk_getnumeric (4, 0, str, 6, str, 4);
      if (rval < 0)
	return 0;
      if (chk_time (str) == 0)
	break;
      else
	{
	  lk_dispclr ();
	  lk_disptext (3, 4, "TRY AGAIN", 1);
	  printf ("ENTER CORRECT DATA\n");
	  lk_getkey ();
	}

    }
  curt->tm_hour = (str[0] - 0x30) * 10 + (str[1] - 0x30);
  curt->tm_min = (str[2] - 0x30) * 10 + (str[3] - 0x30);
  curt->tm_sec = (str[4] - 0x30) * 10 + (str[5] - 0x30);

//      printf("%d   %d  %d\n",curt->tm_hour,curt->tm_min,curt->tm_sec);

  return 0;
}


int
TimeFormat (const struct tm *curt)
{
  int value;
  struct tm settime;
  memcpy (&settime, curt, sizeof (struct tm));
  lk_dispclr ();
  lk_disptext (2, 4, "DISPLAY FORMAT", 0);
  lk_disptext (3, 5, "0. 12 Hours", 0);
  lk_disptext (4, 5, "1. 24 Hours", 0);
  value = lk_getkey ();


//      printf("TimeFormat %d   %d  %d\n",curt->tm_hour,curt->tm_min,curt->tm_sec);

  lk_dispclr ();

  if (value == 0 && settime.tm_hour > 12)
    {
      settime.tm_hour = settime.tm_hour - 12;
      lk_setrtc (&settime);
    }
  else if (value == 1 && settime.tm_hour < 12)
    {
      settime.tm_hour = settime.tm_hour + 12;
      lk_setrtc (&settime);
    }
  return 0;

}




int
chk_time (char *tim)
{
  char str[3];
  str[0] = tim[0];
  str[1] = tim[1];
  str[2] = 0;
  if (atoi (str) < 0 || atoi (str) > 23)
    return -1;
  str[0] = tim[2];
  str[1] = tim[3];
  str[2] = 0;
  if (atoi (str) < 0 || atoi (str) > 59)
    return -1;
  str[0] = tim[4];
  str[1] = tim[5];
  str[2] = 0;
  if (atoi (str) < 0 || atoi (str) > 59)
    return -1;
  return 0;
}

int
chk_date (char *dt)
{
  char str[3];
  int epos_date = 0, epos_month = 0, epos_year = 0;

  str[0] = dt[0];
  str[1] = dt[1];
  str[2] = 0x00;

  epos_date = atoi (str);


  str[0] = dt[2];
  str[1] = dt[3];
  str[2] = 0x00;

  epos_month = atoi (str);


  str[0] = dt[4];
  str[1] = dt[5];
  str[2] = 0x00;

  epos_year = 2000 + atoi (str);


  if (epos_month == 0 || epos_date == 0 || epos_year < 2008)
    return ERROR;

  else if (epos_month == 1 || epos_month == 3 || epos_month == 5
	   || epos_month == 7 || epos_month == 8 || epos_date == 10
	   || epos_month == 12)
    {

      if (epos_date > 31)
	return ERROR;
    }



  else if (epos_month == 4 || epos_month == 6 || epos_month == 9
	   || epos_month == 11)
    {

      if (epos_date > 30)
	return ERROR;
    }

  else if (epos_month == 2)

    {
      if (!(epos_year % 400) || (epos_year % 100 != 0 && epos_year % 4 == 0))
	{
	  if (epos_date > 29)
	    return ERROR;
	}

      else if (epos_date > 28)
	return ERROR;
    }


  else
    return 0;
}
