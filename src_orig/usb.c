#include <header.h>

extern char str5[20], str6[20];
static int usba = 1;
extern int tr;
static int usbb = 1;
int ret;

int
usb_detect (void)
{
  int opt = 0, usbaret = 0;

  usbaret = usba_test ();

  if (usbaret == ERROR)
    {
      lk_dispclr ();
      lk_disptext (2, 2, "USB-A Information", 0);
      lk_disptext (3, 4, "Not Available", 0);
      lk_getkey ();
    }
  return SUCCESS;
}


int
usba_test (void)
{
  char ven[20] = "", ven1[20] = "", prod[20] = "", prod1[20], chr[80] =
    "", flag = 0, i, j;
  char product1[100] = "", product2[100] = "";
  int value;
  FILE *fp;

  fp = fopen ("/proc/bus/usb/devices", "r");

  if (fp == NULL)
    return ERROR;


  while (fscanf (fp, " %s", chr) != EOF)
    {
      if (strstr (chr, "Vendor"))
	{
	  if ((strcmp (chr, "Vendor=0000")))
	    {
	      if (flag == 1)
		strcpy (ven1, chr);
	      strcpy (ven, chr);
	      flag++;
	    }
	}

      else
	{
	  if (strstr (chr, "ProdID"))
	    if ((strcmp (chr, "ProdID=0000")))
	      {
		if (flag == 2)
		  {
		    strcpy (prod1, chr);

		    for (i = 0, j = 0; i < 4; i++)
		      {
			fgets (product2, 80, fp);
			if (strstr (product2, "Product"))
			  {
			    j = 1;
			    break;
			  }
		      }

		    if (j == 0)
		      strcpy (product2, "Device Name Not found");

		    else
		      {
			for (i = 0; product2[i] != '='; i++);
			for (j = 0; product2[i + j + 1] != '\0'; j++)
			  product2[j] = product2[i + j + 1];
			product2[j - 1] = '\0';
		      }

		  }

		else
		  {
		    strcpy (prod, chr);

		    for (i = 0, j = 0; i < 4; i++)
		      {
			fgets (product1, 80, fp);
			if (strstr (product1, "Product"))
			  {
			    j = 1;
			    break;
			  }
		      }

		    if (j == 0)
		      strcpy (product1, "Device Name Not found");

		    else
		      {
			for (i = 0; product1[i] != '='; i++);
			for (j = 0; product1[i + j + 1] != '\0'; j++)
			  product1[j] = product1[i + j + 1];
			product1[j - 1] = '\0';
		      }
		  }

	      }
	}
    }

  fclose (fp);

  lk_dispclr ();
  lk_disptext (2, 3, "USB_A TEST", 0);
  lk_disptext (3, 1, "0. USB_A1", 0);
  lk_disptext (4, 1, "1. USB_A2", 0);
  value = lk_getkey ();

  if (value == 0)
    {
      if (flag == 0)
	{
	  fprintf (stderr, "NO USB DEVICE FOUND");
	  lk_dispclr ();
	  lk_disptext (3, 2, "NO USB DEVICE FOUND", 0);
	  lk_getkey ();
	  return 0;
	}


      else if (flag == 1 || flag == 2)
	{
	  lk_dispclr ();
	  lk_disptext (1, 0, product1, 0);
	  lk_disptext (3, 2, ven, 0);
	  lk_disptext (4, 2, prod, 0);
	  lk_getkey ();
	  return 0;
	}
    }


  else if (value == 1)
    {
      if (flag == 1 || flag == 0)
	{
	  fprintf (stderr, "NO USB DEVICE FOUND");
	  lk_dispclr ();
	  lk_disptext (3, 2, "NO USB DEVICE FOUND", 0);
	  lk_getkey ();
	  return 0;
	}


      else if (flag == 2)
	{
	  lk_dispclr ();
	  lk_disptext (1, 0, product2, 0);
	  lk_disptext (3, 2, ven1, 0);
	  lk_disptext (4, 2, prod1, 0);
	  lk_getkey ();
	  return 0;
	}

    }
  return 0;
}
