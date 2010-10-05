#include<header.h>

int
Voice_Call ()
{

  unsigned char Phone_No[20] = "", Buff[20], ret;
  int res;


  res = sim_status_test ();
  if (res == -1)
    {
      fprintf (stderr, "Simcard not Inserted\n");
      lk_dispclr ();
      lk_disptext (2, 8, "SIMCARD", 1);
      lk_disptext (4, 5, "NOT PRESENT", 1);
      lk_getkey ();
      return -1;
    }

label:
  lk_dispclr ();
  lk_disptext (2, 1, "1.Wavecom", 0);
  lk_disptext (3, 1, "2.Simcom", 0);
  lk_disptext (5, 1, "Enter your option", 0);
  res = lk_getkey ();		// waiting for a key

  if (res == CANCEL)
    return -1;


  lk_dispclr ();
  lk_disptext (2, 2, "Enter Phone Number", 0);
  lk_getalpha (4, 0, Phone_No, 15, Phone_No, 1);
  sprintf (Buff, "ATDT%s;\r\n", Phone_No);

  if (res == 1)
    ret = Get_Response_wavecom (Buff);

  else if (res == 2)
    ret = Get_Response_simcom (Buff);
  else
    goto label;			// other than 1,2 and CANCEL keys


  return 0;

}

int
Get_Response_wavecom (unsigned char *Dial_Buff)
{
  int res;
  char response[256];
  char *expect[32] = { "OK", "NO DIALTONE", "NO CARRIER", NULL };

  res = ModemOpen ("/dev/ttyS0");
  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      return ERROR;

    }

  setmodemparams (9600);
  memset (response, 0, 256);

  res = Test_modem_response_without_signal (Dial_Buff, response, expect, 10);

  fprintf (stdout, "%s Res=%s\n", Dial_Buff, expect[res]);

  fprintf (stdout, "Response of Voice Test = %s\n", response);

  if (res == 0)
    {
      lk_dispclr ();
      lk_disptext (2, 2, "Dialing...", 1);
      lk_disptext (5, 2, "press any key", 0);
      lk_getkey ();
    }
  else if (res == 1)
    {
      lk_dispclr ();
      lk_disptext (2, 2, "NO DIALTONE", 1);
      lk_disptext (5, 2, "press any key", 0);
      lk_getkey ();
    }

  else if (res == 2)
    {
      lk_dispclr ();
      lk_disptext (2, 2, "NO CARRIER", 1);
      lk_disptext (5, 2, "press any key", 0);
      lk_getkey ();
    }


  else
    {
      res = lk_getkey ();

      if (res == CANCEL)
	{
	  Test_modem_response_without_signal ("ATH\r\n", response, expect,
					      10);
	  Modemclose ();
	  return 0;
	}

      Modemclose ();
      return 0;
    }
}



int
Get_Response_simcom (unsigned char *Dial_Buff)
{
  int res;
  char response[256];
  char *expect[32] = { "OK", "NO DIALTONE", "NO CARRIER", NULL };

  res = ModemOpen ("/dev/ttyS0");
  if (res < 0)
    {
      fprintf (stderr, "Modem Open failed\n");
      exit (-1);
    }
  setmodemparams (9600);
  memset (response, 0, 256);

  res =
    Test_modem_response_without_signal ("AT+CLVL=99\r\n", response, expect,
					30);
  fprintf (stderr, "AT+CLVL=99 Res=%s\n", expect[res]);
  if (res != 0)
    {
      printf ("AT+CLVL failed\n");
      return -1;
    }
  sleep (1);

  memset (response, 0, 256);
  res =
    Test_modem_response_without_signal ("AT+CMIC=1,10\r\n", response, expect,
					30);
  fprintf (stderr, "AT+CMIC=1,10 Res=%s\n", expect[res]);
  if (res != 0)
    {
      printf ("AT+CMIC failed\n");
      return -1;
    }

  sleep (1);

  memset (response, 0, 256);
  res = Test_modem_response_without_signal ("AT&W\r\n", response, expect, 30);

  memset (response, 0, 256);

  res = Test_modem_response_without_signal (Dial_Buff, response, expect, 10);
  fprintf (stderr, "%s Res=%s\n", Dial_Buff, expect[res]);
  sleep (1);

  printf ("Response of Voice Test = %s\n", response);

  if (res == 0)
    {
      lk_dispclr ();
      lk_disptext (2, 2, "Dialing...", 1);
      lk_disptext (5, 2, "press any key", 0);
      lk_getkey ();
    }
  else if (res == 1)
    {
      lk_dispclr ();
      lk_disptext (2, 2, "NO DIALTONE", 1);
      lk_disptext (5, 2, "press any key", 0);
      lk_getkey ();
    }
  else if (res == 2)
    {
      lk_dispclr ();
      lk_disptext (2, 2, "NO CARRIER", 1);
      lk_disptext (5, 2, "press any key", 0);
    }
  else
    {
      res = lk_getkey ();

      if (res == CANCEL)
	{
	  Test_modem_response_without_signal ("ATH\r\n", response, expect,
					      10);
	  Modemclose ();
	  return 0;
	}

    }
  Test_modem_response_without_signal ("ATH\r\n", response, expect, 10);
  Modemclose ();

  return 0;
}
