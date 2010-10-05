#include <header.h>

int
get_machineid (void)
{
  int key;

  while (1)
    {
      lk_dispclr ();
      lk_disptext (1, 0, "Machine ID MENU", 0);

      lk_disptext (2, 2, "1.Hardware ID", 0);
      lk_disptext (3, 2, "2.Terminal ID", 0);

      lk_disptext (5, 0, "Enter Your Option", 0);

      key = lk_getkey ();

      if (key == 1)
	hwid ();
      else if (key == 2)
	pinpadid ();
      else if (key == CANCEL)
	break;
    }

  return 0;
}


int
pinpadid (void)
{
  int key = 0;
  char machineid[20] = "";

  key = lk_getpinpadid (machineid);

  machineid[10] = '\0';

  if (key == -1)		// if machine is not available
    {
      fprintf (stderr, "\nMACHINE ID NOT AVAILABLE\n");
      lk_dispclr ();
      lk_disptext (2, 6, "MACHINE ID", 0);
      lk_disptext (3, 4, "NOT AVAILABLE", 0);
      lk_getkey ();
      return -1;
    }

  fprintf (stderr, "\nPINPAD ID  = %s\n", machineid);

  lk_dispclr ();
  lk_disptext (1, 5, "MACHINE ID", 1);
  lk_disptext (3, 5, machineid, 1);
  lk_getkey ();			// waiting for key

  return SUCCESS;

}

int
hwid ()
{
  unsigned char str[10], arr[48];
  int result;
  lk_dispclr ();

  lk_disptext (3, 3, "Reading HWID", 1);
  readotp (str);

  lk_dispclr ();
  //for(result=0;result<8;result++)
  //printf(" %c- ",str[result]);

  // for(result=0;result<8;result++)
  // printf(" %x- ",str[result]);

  str[8] = '\0';
  //lk_disptext(3,6,str,0); // if u want in ascii format display
  //lk_disptext(4,4,"ASCII FORMAT",0);

  sprintf (arr, "%x%x%x%x%x%x%x%x", str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]);	// 
  lk_disptext (3, 3, arr, 0);	// if u want in hex format  display
  lk_disptext (4, 4, "HEX FORMAT", 0);


  if (str[0] == 0xff && str[1] == 0xff && str[2] == 0xff && str[3] == 0xff
      && str[4] == 0xff && str[5] == 0xff && str[6] == 0xff && str[7] == 0xff)
    lk_disptext (1, 4, "NO Hardware ID", 1);
  else
    lk_disptext (1, 5, "Hardware ID", 1);
  lk_disptext (5, 0, "Press Any Key to Exit", 0);
  lk_getkey ();
  return 0;
}
