#include <header.h>

int
scan_barcode ()
{
  int ret = 0, key = 0;
  char buf[256];

  ret = bar_code_open ();
  if (ret < 0)
    {
      fprintf (stderr, "Unable to open Barcode device\n");
      lk_dispclr ();
      lk_disptext (2, 2, "Unable to Open device", 0);
      lk_disptext (5, 0, "Press Any Key to Exit", 0);
      lk_getkey ();
      return -1;
    }

  lk_dispclr ();
  lk_disptext (2, 2, "Waiting ", 0);
  lk_disptext (3, 2, "       for", 0);
  lk_disptext (4, 2, "          Barcode", 0);


  while (1)
    {
      memset (buf, 0, sizeof (buf));
      ret = bar_code_read (buf, sizeof (buf));
      if (ret > 0)
	{
	  fprintf (stdout, "barcode ::%s\n", buf);
	  buf[strlen (buf) - 1] = 0x00;
	  lk_dispclr ();
	  lk_disptext (2, 2, "Barcode", 1);
	  lk_disptext (4, 0, buf, 0);
	  lk_disptext (5, 0, "Press Any Key to Exit", 0);
	  lk_getkey ();
	  break;
	}
      else
	key = lk_getkey_wait ();
      if (key == CANCEL)
	break;
    }

  bar_code_close ();
  return 0;
}
