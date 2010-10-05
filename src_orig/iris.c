/*
	This is demo application for the SC2_Sample works for the ePOS.It shows some interactive messages on LCD.
	It uses the library libSC2_ANSI378.a for APIs used here

	Enroll_Fingerprint_Image()
	Verify_Fingerprint_Image() 
	and for device initialization libusb_epos.a library is used.

*/

#include <header.h>

int
smart_core ()
{
  unsigned char choice;
  unsigned char BIR_tag[1024];
  int ret = 0, i = 0, Timeout = 20;
//      lk_open();
  lk_dispclr ();
  lk_disptext (3, 2, "Initializing...SC2", 1);

  ret = Device_Init ();
  if (ret < 0)
    {
      printf ("Device Initialization Failed\n");
      lk_dispclr ();
      lk_disptext (3, 2, "Failed", 1);
      return -1;
    }
  else if (ret == 2)
    {
      printf ("Power Control Failed\n");
      lk_dispclr ();
      lk_disptext (3, 2, "Failed", 1);
      return -1;
    }

  printf ("Device is Ready\n");
  lk_dispclr ();
  lk_disptext (3, 2, "Success", 1);

  do
    {
      lk_dispclr ();
      lk_disptext (1, 2, "1.Enroll", 0);
      lk_disptext (2, 2, "2.Verify", 0);
      lk_disptext (3, 2, "3.Exit", 0);

      lk_disptext (5, 0, "Select Your Option", 0);
      choice = lk_getkey ();
      printf ("\n");

      switch (choice)
	{

	case 1:
	  lk_dispclr ();
	  lk_disptext (2, 2, "Ready to Enroll..!", 0);
	  lk_disptext (3, 2, "Put Finger...!", 0);	///
	  ret = Enroll_Fingerprint_Image (BIR_tag, Timeout);
#if SHOW_TEMPLATE
	  printf ("TEMPLATE data of length::%d\n", ret - 4);
	  for (i = 4; i < ret; i++)
	    printf ("%02x ", BIR_tag[i]);
	  puts ("");
#endif
	  lk_dispclr ();
	  if (ret > 0)
	    lk_disptext (4, 2, "Enroll successful", 0);
	  else
	    lk_disptext (4, 2, "Enroll Unsuccessful", 0);
	  sleep (2);
	  break;
	case 2:
	  ret = 0;
	  lk_dispclr ();
	  lk_disptext (2, 2, "Ready to Verify..!", 0);
	  lk_disptext (3, 2, "Put Finger...!", 0);	///
	  ret = Verify_Fingerprint_Image (BIR_tag, Timeout);
	  lk_dispclr ();
	  if (ret == 1)
	    lk_disptext (3, 4, "MATCHED", 1);
	  else if (ret == 0)
	    lk_disptext (3, 2, "NOT MATCHED", 1);
	  else
	    lk_disptext (3, 0, "Verification Failed", 1);

	  sleep (2);
	  break;

	case 3:
	  lk_dispclr ();
	  lk_disptext (3, 2, "Exiting....!!", 1);
	  sleep (1);
	  break;
	default:
	  lk_dispclr ();
	  lk_disptext (3, 2, "Invalid Choice", 1);
	  break;
	}

    }
  while (choice != 3);

  Device_Close ();
//      lk_close();

  return 0;
}
