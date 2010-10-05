#include<header.h>

int
usb_test (void)
{
  int key;

  while (1)
    {
      lk_dispclr ();
      lk_disptext (1, 0, "USB TEST MENU", 0);

      lk_disptext (2, 4, "1.Devices", 0);
      lk_disptext (3, 4, "2.Barcode", 0);
      lk_disptext (4, 4, "3.Finger Print", 0);
      lk_disptext (5, 4, "4.Mass Storage", 0);

      key = lk_getkey ();

      if (key == 1)
	usb_detect ();

      else if (key == 2)
	scan_barcode ();

      else if (key == 3)
	finger_test ();

      else if (key == 4)
	mass_storage ();

      else if (key == CANCEL)
	break;
    }

  return SUCCESS;

}


int
finger_test (void)
{
  int key = 0;

  while (1)
    {
      lk_dispclr ();
      lk_disptext (1, 0, "FINGER PRINT MENU", 0);

      lk_disptext (3, 4, "1.UPEK", 0);
      lk_disptext (4, 4, "2.IRIS", 0);
      lk_disptext (5, 0, "Enter Your Option", 0);

      key = lk_getkey ();

      if (key == 1)
	upek_menu ();
      else if (key == 2)
	smart_core ();

      else if (key == CANCEL)
	break;
    }

  return 0;
}
