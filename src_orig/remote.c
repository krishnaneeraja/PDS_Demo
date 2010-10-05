#include <header.h>

extern char ramdisk_type;
extern char ethtype[30];

int
Remote_test ()
{

  switch (ramdisk_type)
    {

    case ETHERNET:
      strcpy (ethtype, "eth0");
      ethernet_remote ();
      break;

    case RALINKWLAN:
      strcpy (ethtype, "rausb0");
      ethernet_remote ();
      break;

    case CNETWLAN:
      strcpy (ethtype, "wlan0");
      ethernet_remote ();
      break;


    case GSM_GPRS:
      gsm_gprs_remote ();
      break;

    case CDMA:
      cdma_remote ();
      break;

    case PSTN:
      pstn_remote ();
      break;

    default:
      select_communication_module ();
      return -1;


    }


  return SUCCESS;


}


int
ethernet_remote (void)
{
  if (set_ip () < 0)
    return ERROR;

  if (set_gateway () < 0)
    return ERROR;

  if (ftpdown_ke_ra_ap () < 0)
    return ERROR;

}

int
ftpdown_ke_ra_ap (void)
{
  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;

  menu.start = 0;
  menu.maxEntries = 3;
  maxEntries = menu.maxEntries;

  strcpy (menu.title, "Types of Images");

  strcpy (menu.menu[0], "Linux Kernel ");
  strcpy (menu.menu[1], "Ramdisk ");
  strcpy (menu.menu[2], "Application ");

  while (1)
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);
      switch (opt)
	{
	case CANCEL:
	  return -1;
	case ENTER:
	  switch (selItem + 1)
	    {
	    case 1:
	      Dwnd_ke_ra_ap (KERNEL);
	      break;
	    case 2:

	      Dwnd_ke_ra_ap (RAMDISK);
	      break;
	    case 3:
	      Dwnd_ke_ra_ap (APPLICATION);
	      break;
	    }

	}
    }

  return SUCCESS;
}

int
Dwnd_ke_ra_ap (int img_type)
{

  char load[100] = "", str1[20] = "", str2[20] = "", str3[20] = "", str4[20] =
    "";
  char ip[20] = "192.168.1.179", usrname[20] = "linkwell", passwd[20] =
    "9848279229", fname[20] = "kernel.img";
  lk_dispclr ();
  lk_disptext (2, 2, "Enter user name", 0);
  if (lk_getalpha (4, 0, usrname, 13, usrname, 1) < 0)
    return ERROR;

  strncpy (str1, usrname, 13);
  lk_dispclr ();
  lk_disptext (2, 2, "Enter password", 0);
  if (lk_getalpha (4, 0, passwd, 13, passwd, 1) < 0)
    return ERROR;

  strncpy (str2, passwd, 13);
  lk_dispclr ();
  lk_disptext (2, 2, "Enter Ftp IP", 0);
  if (lk_getnumeric (4, 0, ip, 13, ip, 4) < 0)
    return ERROR;

  strncpy (str3, ip, 13);

  remove ("/mnt/tmp/app.img");
  remove ("/mnt/tmp/ramdisk.img");
  remove ("/mnt/tmp/kernel.img");


  lk_dispclr ();

  if (img_type == KERNEL)
    {
      lk_disptext (1, 7, "KERNEL", 0);
      lk_disptext (2, 2, "Enter file name", 0);
      if (lk_getalpha (4, 1, fname, 13, fname, 1) < 0)
	return ERROR;

      strncpy (str4, fname, 13);
      sprintf (load,
	       "ftpget -u %s -p %s %s /mnt/tmp/kernel.img %s > status 2>&1",
	       str1, str2, str3, str4);
      // printf("\n%s\n",load);
      lk_dispclr ();
      lk_disptext (2, 0, "Downloading Remote Kernel ...", 0);
      system (load);

      if (checkftp_status () < 0)
	{
	  remove ("status");
	  return ERROR;
	}
      lk_dispclr ();
      lk_disptext (2, 0, "Fusing Kernel to Flash ...", 0);
      system ("/home/downloadkernel");
      lk_dispclr ();
      printf ("\nDownload Comppleated\n");
      lk_disptext (3, 1, "Download Compleated", 0);

    }

  else if (img_type == RAMDISK)
    {
      strcpy (fname, "ramdisk.img");
      lk_disptext (1, 7, "RAMDISK", 0);
      lk_disptext (2, 2, "Enter file name", 0);
      lk_getalpha (4, 1, fname, 13, fname, 1);
      strncpy (str4, fname, 13);
      sprintf (load,
	       "ftpget -u %s -p %s %s /mnt/tmp/ramdisk.img %s > status 2>&1",
	       str1, str2, str3, str4);

      lk_dispclr ();
      lk_disptext (2, 0, "Downloading Remote Ramdisk ...", 0);
      system (load);

      if (checkftp_status () < 0)
	{
	  remove ("status");
	  return ERROR;
	}

      lk_dispclr ();
      lk_disptext (2, 0, "Fusing Ramdisk to Flash...", 0);
      system ("/home/downloadramdisk");
      lk_dispclr ();
      printf ("\nDownload Comppleated\n");
      lk_disptext (3, 1, "Download Compleated", 0);

    }

  else if (img_type == APPLICATION)
    {
      strcpy (fname, "app.img");
      lk_disptext (1, 5, "APPLICATION", 0);
      lk_disptext (2, 2, "Enter file name", 0);
      lk_getalpha (4, 1, fname, 13, fname, 1);
      strncpy (str4, fname, 13);
      sprintf (load,
	       "ftpget -u %s -p %s %s /mnt/tmp/app.img %s > status 2>&1",
	       str1, str2, str3, str4);

      lk_dispclr ();
      lk_disptext (2, 0, "Downloading Remote Application ...", 0);

      system (load);
      printf ("%s", load);

      if (checkftp_status () < 0)
	{
	  remove ("status");
	  return ERROR;
	}


      lk_dispclr ();
      lk_disptext (2, 0, "Fusing Application to Flash...", 0);
      system ("/home/downloadapplication");
      lk_dispclr ();
      printf ("\nDownload Comppleated\n");
      lk_disptext (3, 1, "Download Compleated", 0);
    }

  return SUCCESS;

}

int
checkftp_status (void)
{
  char buff[100] = "";

  FILE *fp;

  fp = fopen ("status", "r");

  fgets (buff, 80, fp);		// Checking for Ramdisk type


  lk_dispclr ();
  lk_disptext (2, 8, "ERROR", 0);
  if ((strstr (buff, "Failed to open")) != NULL)
    {
      lk_disptext (4, 3, "Failed to open", 0);
      fprintf (stderr, "File open error\n");
      lk_getkey ();
      fclose (fp);
      return ERROR;
    }
  else if ((strstr (buff, "incorrect")) != NULL)
    {

      lk_disptext (4, 3, "Login incorrect", 0);
      fprintf (stderr, "\nLogin incorrect\n");
      lk_getkey ();
      fclose (fp);
      return ERROR;
    }

  else if ((strstr (buff, "unreachable")) != NULL)
    {
      lk_disptext (4, 1, "Network Unreachable", 0);
      fprintf (stderr, "\nNetwork Unreachable\n");
      lk_getkey ();
      fclose (fp);
      return ERROR;
    }


  else if ((strstr (buff, "route")) != NULL)
    {
      lk_disptext (4, 3, "No route to host", 0);
      fprintf (stderr, "\nNo route to host\n");
      lk_getkey ();
      fclose (fp);
      return ERROR;
    }

  else if ((strstr (buff, "refused")) != NULL)
    {
      lk_disptext (4, 3, "Unable to Connect", 0);
      fprintf (stderr,
	       "\nUnable to connect to remote host Connection refused\n");
      lk_getkey ();
      fclose (fp);
      return ERROR;
    }



  fclose (fp);
  return SUCCESS;
}
