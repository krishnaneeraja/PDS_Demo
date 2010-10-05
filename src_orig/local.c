#include <header.h>

int
Local_test (void)
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


  strcpy (menu.title, "Local");
  strcpy (menu.menu[0], "RS232");
  strcpy (menu.menu[1], "USB_B");

  while (1)
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);

      switch (opt)
	{

	case CANCEL:
	  return 0;

	case ENTER:

	case 1:
	case 2:

	  switch (selItem + 1)
	    {
	    case 1:
	      Download_images_rs232 ();
	      break;

	    case 2:
	      Download_images_usbb ();
	      break;
	    }


	}
    }

  return 0;

}



int
Download_images_rs232 (void)
{
  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;

  menu.start = 0;
  menu.maxEntries = 4;
  maxEntries = menu.maxEntries;

  strcpy (menu.title, "Types of Images");

  strcpy (menu.menu[0], "Linux Kernel ");
  strcpy (menu.menu[1], "Ramdisk ");
  strcpy (menu.menu[2], "Application ");
  strcpy (menu.menu[3], "JFFS2");

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
	      download_kernel_rs232 ();
	      break;

	    case 2:

	      download_ramdisk_rs232 ();
	      break;

	    case 3:

	      download_application_rs232 ();
	      break;

	    case 4:

	      jffs2_upload_download_rs232 ();
	      break;

	    }
	}
    }
  return 0;

}


int
download_kernel_rs232 (void)
{
  lk_dispclr ();
  lk_disptext (3, 4, "Send File From", 0);
  lk_disptext (4, 7, "PC Side", 0);

  lk_disptext (1, 3, "Kernel Download", 1);
  printf ("\nNew Kernel downloading ...\n");
  system ("/home/devkermit -i -r -a /mnt/tmp/kernel.img");
  printf ("\nNew Kernel downloaded from RS232\n");

  lk_dispclr ();
  lk_disptext (2, 2, "Fusing Kernel ...", 0);
  printf ("\nWriting Kernel To Flash\n");
  system ("/home/downloadkernel");

  lk_dispclr ();
  printf ("\nDownload Comppleated\n");

  lk_disptext (2, 1, "Download Compleated", 0);
  lk_disptext (3, 2, "Reboot The System", 1);
  remove ("/mnt/tmp/kernel.img");
  lk_getkey ();
  return 0;
}

int
download_ramdisk_rs232 (void)
{
  lk_dispclr ();
  lk_disptext (3, 4, "Send File From", 0);
  lk_disptext (4, 7, "PC Side", 0);

  lk_disptext (1, 3, "Ramdisk Download", 1);
  printf ("\nNew Ramdisk Downloading ...\n");
  system ("/home/devkermit -i -r -a  /mnt/tmp/ramdisk.img");
  printf ("\nNew Ramdisk downloaded from RS232\n");


  lk_dispclr ();
  lk_disptext (2, 1, "Fusing Ramdisk ...", 0);
  printf ("\nWriting Ramdisk To Flash\n");
  system ("/home/downloadramdisk");

  lk_dispclr ();
  printf ("\nDownload Comppleated\n");
  lk_disptext (2, 1, "Download Compleated", 0);
  lk_disptext (3, 2, "Reboot The System", 1);
  remove ("/mnt/tmp/ramdisk.img");
  lk_getkey ();
  return 0;

}

int
download_application_rs232 (void)
{
  lk_dispclr ();
  lk_disptext (3, 4, "Send File From", 0);
  lk_disptext (4, 7, "PC Side", 0);

  lk_disptext (1, 1, "Application Download", 1);
  printf ("\nNew Application downloading ...\n");
  system ("/home/devkermit -ira /mnt/tmp/app.img");
  printf ("\nNew Application downloaded from usb-b\n");


  lk_dispclr ();
  lk_disptext (2, 1, "Fusing Application ...", 0);
  printf ("\nWriting Application To Flash\n");
  system ("/home/downloadapplication");

  lk_dispclr ();
  printf ("\nDownload Comppleated\n");
  lk_disptext (2, 1, "Download Compleated", 0);
  lk_disptext (3, 2, "Reboot The System", 1);
  remove ("/mnt/tmp/app.img");
  lk_getkey ();
  return 0;
}


int
jffs2_upload_download_rs232 (void)
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
	      jffs2_upload_rs232 ();
	      break;
	    case 2:
	      jffs2_download_rs232 ();
	      break;
	    }
	}
    }
  return 0;
}


int
jffs2_upload_rs232 (void)
{
  struct dirent *entry;
  char upload_file[80] = "";
  char upload_com[100] = "";
  char arr[80][80];
  int i = 0, j = 0, ch = 0;

  DIR *dir = opendir ("/mnt/jffs2/");
  if (!dir)
    {
      perror ("unable to open jffs2 directory\n");
      return -1;
    }

  while ((entry = readdir (dir)) != NULL)
    {
      // printf("%s\n", entry->d_name);
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
      lk_disptext (4, 4, "IN JFFS2", 0);
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

      else if (ch == 13)
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
  sprintf (upload_com, "/home/devkermit -is /mnt/jffs2/%s", upload_file);
  system (upload_com);


  lk_dispclr ();
  lk_disptext (3, 4, "File Uploaded", 1);
  lk_disptext (5, 4, "Press Any Key", 0);
  lk_getkey ();
  return 0;
}

int
jffs2_download_rs232 (void)
{
  chdir ("/mnt/jffs2/");
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

int
Download_images_usbb (void)
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
	  return;
	case ENTER:
	case 1:
	case 2:
	case 3:
	  switch (selItem + 1)
	    {

	    case 1:
	      download_kernel_usbb ();
	      break;

	    case 2:
	      download_ramdisk_usbb ();
	      break;

	    case 3:
	      download_application_usbb ();
	      break;
	    }
	}
    }
  return 0;
}

int
download_kernel_usbb (void)
{
  lk_dispclr ();
  lk_disptext (3, 4, "Send File From", 0);
  lk_disptext (4, 7, "PC Side", 0);

  lk_disptext (1, 3, "Kernel Download", 1);
  printf ("\nNew Kernel downloading ...\n");
  system ("usb_hnxappl receive /mnt/tmp/kernel.img");
  printf ("\nNew Kernel downloaded from usb-b\n");

  lk_dispclr ();
  lk_disptext (2, 2, "Fusing Kernel to Flash ...", 0);
  printf ("\nWriting Kernel To Flash\n");
  system ("/home/downloadkernel");

  lk_dispclr ();
  printf ("\nDownload Comppleated\n");

  lk_disptext (2, 1, "Download Compleated", 0);
  lk_disptext (3, 2, "Reboot The System", 1);
  remove ("/mnt/tmp/kernel.img");
  lk_getkey ();
  return 0;
}

int
download_ramdisk_usbb (void)
{
  lk_dispclr ();
  lk_disptext (3, 4, "Send File From", 0);
  lk_disptext (4, 7, "PC Side", 0);

  lk_disptext (1, 3, "Ramdisk Download", 1);
  printf ("\nNew Ramdisk downloading ...\n");
  system ("usb_hnxappl receive /mnt/tmp/ramdisk.img");
  printf ("\nNew Ramdisk downloaded from usb-b\n");

  lk_dispclr ();
  lk_disptext (2, 1, "Fusing Ramdisk to Flash ...", 0);
  printf ("\nWriting Ramdisk To Flash\n");
  system ("/home/downloadramdisk");

  lk_dispclr ();
  printf ("\nDownload Comppleated\n");
  lk_disptext (2, 1, "Download Compleated", 0);
  lk_disptext (3, 2, "Reboot The System", 1);
  remove ("/mnt/tmp/ramdisk.img");
  lk_getkey ();
  return 0;

}

int
download_application_usbb (void)
{
  lk_dispclr ();
  lk_disptext (3, 4, "Send File From", 0);
  lk_disptext (4, 7, "PC Side", 0);

  lk_disptext (1, 1, "Application Download", 1);
  printf ("\nNew Application downloading ...\n");
  system ("usb_hnxappl receive /mnt/tmp/app.img");
  printf ("\nNew Application downloaded from usb-b\n");

  lk_dispclr ();
  lk_disptext (2, 1, "Fusing Application to Flash ...", 0);
  printf ("\nWriting Application To Flash\n");
  system ("/home/downloadapplication");

  lk_dispclr ();
  printf ("\nDownload Comppleated\n");
  lk_disptext (2, 1, "Download Compleated", 0);
  lk_disptext (3, 2, "Reboot The System", 1);
  remove ("/mnt/tmp/app.img");
  lk_getkey ();
  return 0;
}
