#include <header.h>

/*int microsd_domount(int flag)
{
	char cmd[50];

	memset(cmd,0,50);

	if(flag == 0)
		strcpy(cmd,"/home/microsd.conf mount");
	else
		strcpy(cmd,"/home/microsd.conf umount");
	
	return  system(cmd);
}*/


int
sdcard (void)
{
  int ret = 0, key = 0;
  FILE *fp;
  char str[100] = "", flag = 0;


  lk_dispclr ();

  lk_disptext (0, 0, "SD Card", 1);
  lk_disptext (2, 3, "1.Mount Card", 0);
  lk_disptext (3, 3, "2.Unmount Card", 0);
  lk_disptext (4, 3, "3.Data transfer", 0);


  key = lk_getkey ();

  fp = fopen ("/etc/mtab", "r");

  if (fp == NULL)
    fprintf (stderr, "File open Error\n");

  while ((fgets (str, 80, fp)) != NULL)
    {
      if ((strstr (str, "/mnt/microsd")) != NULL)
	flag = 1;
    }

  fclose (fp);


  if (key == 1 && flag == 1)
    {
      lk_dispclr ();
      lk_disptext (2, 2, "Already Mounted", 1);
      lk_disptext (5, 0, "Press Any Key to Exit", 0);
      lk_getkey ();
      return 0;
    }

  else if (key == 1 && flag == 0)
    {
      ret = system ("mount -t vfat /dev/microsd /mnt/microsd");
      if (ret == 0)
	{
	  lk_dispclr ();
	  fprintf (stdout, "SD Card mounted\n");
	  lk_disptext (2, 2, "MOUNT SUCCESS", 1);
	  lk_disptext (5, 0, "Press Any Key to Exit", 0);
	  lk_getkey ();
	  return 0;
	}
      else
	{
	  lk_dispclr ();
	  fprintf (stderr, "SD Card mounting Failed \n");
	  lk_disptext (2, 2, "MOUNT FAILED", 1);
	  lk_disptext (5, 0, "Press Any Key to Exit", 0);
	  lk_getkey ();
	  return -1;
	}
    }

  else if (key == 3 && flag == 0)
    {
      lk_dispclr ();
      lk_disptext (2, 2, "SD Card ", 0);
      lk_disptext (3, 3, "      Not Mounted", 0);
      lk_disptext (5, 0, "Press Any Key to Exit", 0);
      lk_getkey ();
      return -1;
    }

  else if (key == 3 && flag == 1)

    sd_upload_download_rs232 ();


  else if (key == 2 && flag == 0)
    {
      lk_dispclr ();
      lk_disptext (2, 2, "SD Card", 0);
      lk_disptext (3, 3, "      Not Mounted", 0);
      lk_disptext (5, 0, "Press Any Key to Exit", 0);
      lk_getkey ();
      return -1;
    }

  else if (key == 2 && flag == 1)
    {

      lk_dispclr ();
      lk_disptext (2, 2, "Unmounting Card..", 0);
      ret = system ("umount /mnt/microsd/");
      if (ret == 0)
	lk_disptext (3, 2, "Unmout Successfull", 0);
      else
	lk_disptext (3, 2, "Unmounting  Failed....", 0);
      lk_disptext (5, 0, "Press Any Key to Exit", 0);
      lk_getkey ();
      return -1;
    }

  return 0;
}

int
sd_upload_download_rs232 (void)
{
  char upload_file[50] = "", down_file[50] = "";
  char upload_com[100] = "", down_com[100] = "";

  FILE *fp;
  char str[30] = "";
  char arr[80][30];
  int i = 0, j = 0, ch;

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
  strcpy (menu.title, "Types of Images");
  strcpy (menu.menu[0], "UPLOAD");
  strcpy (menu.menu[1], "DOWNLOAD");

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
	      sd_upload_rs232 ();
	      break;
	    case 2:
	      sd_download_rs232 ();
	      break;
	    }
	}
    }

  return 0;

}


int
sd_upload_rs232 (void)
{
  struct dirent *entry;
  char upload_file[80] = "";
  char upload_com[100] = "";
  char arr[80][80];
  int i = 0, j = 0, ch = 0;

  DIR *dir = opendir ("/mnt/microsd/");
  if (!dir)
    {
      perror ("unable to open microsd directory\n");
      return -1;
    }

  while ((entry = readdir (dir)) != NULL)
    {
      if (entry->d_name[0] != '.')
	{
	  strcpy (arr[i], entry->d_name);
	  i++;
	}
    }

  if (closedir (dir) == -1)
    perror ("closedir");
  if (!i)
    {
      lk_dispclr ();
      lk_disptext (1, 4, "ERROR", 1);
      lk_disptext (3, 4, "FILES NOT FOUND", 0);
      lk_disptext (4, 4, "IN DISK", 0);
      lk_getkey ();
      return -1;
    }

  while (1)
    {
      lk_dispclr ();
      lk_disptext (1, 7, "UPLOAD", 1);
      lk_disptext (3, 4, "Select a File", 0);
      lk_disptext (4, 0, arr[j], 0);
      lk_disptext (5, 0, "F2->up       F3->down", 0);
      lk_disphlight (4);
      ch = lk_getkey ();

      if (ch == CANCEL)
	return -1;
      else if (ch == ENTER)
	{
	  strcpy (upload_file, arr[j]);
	  break;
	}
      else if (ch == 19)
	{
	  j++;
	  if (j == i)
	    j = 0;
	}
      else if (ch == 18)
	{
	  j--;
	  if (j == -1)
	    j = i - 1;
	}
      else
	continue;
    }

  lk_dispclr ();
  lk_disptext (3, 3, "Uploading File ..", 1);
  sprintf (upload_com, "/home/devkermit -is /mnt/microsd/%s", upload_file);
  system (upload_com);

  lk_dispclr ();
  lk_disptext (3, 4, "File Uploaded", 1);
  lk_disptext (5, 4, "Press Any Key", 0);
  lk_getkey ();
  return 0;
}

int
sd_download_rs232 (void)
{
  chdir ("/mnt/microsd/");
  lk_dispclr ();
  lk_disptext (3, 4, "Send File From", 0);
  lk_disptext (4, 7, "PC Side", 0);
  system ("/home/devkermit -ir ");
  lk_dispclr ();
  lk_disptext (3, 4, "File Downloaded", 0);
  lk_disptext (5, 4, "Press Any Key", 0);
  lk_getkey ();
  chdir ("/");
  return 0;
}
