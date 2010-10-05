#include<header.h>
union
{
  unsigned int temp;
  struct
  {
    unsigned char lsb;		//datalen lower byte.
    unsigned char msb;		//datalen higher byte.
  } uc;
} ui;

int
Ifd_test ()
{
  int ret = 0, res = 0, count = 0, brflag;

  unsigned char Wrtbuf[100], temp_buf[100];

  unsigned char cmd[][15] = {
    {0x00, 0x20, 0x00, 0x00, 0x05, 0x55, 0x55, 0x55, 0x55, 0x55},	//verify
    {0x00, 0xDA, 0x00, 0x00},	//write
    {0x00, 0x21, 0x00, 0x00, 0x05, 0x55, 0x55, 0x55, 0x55, 0x55},	//change password
    {0x00, 0xDC, 0x00, 0x01, 0x05, 0x05, 0x04, 0x03, 0x02, 0x01},	//update/
    {0x00, 0xCA, 0x00, 0x01, 0x42},	//read
    {0x00, 0x22, 0x00, 0x00, 0x02},	//read records
    {0x00, 0x9C, 0x00, 0x00, 0x00}	//format
  };

  unsigned char temp[5];

  ret = SCR_Open ();
  if (ret < 0)
    {
      lk_dispclr ();
      lk_disptext (2, 0, "IFD Open Failed ", 0);
      lk_disptext (3, 0, "Check the Connection", 0);
      lk_disptext (4, 0, "Reboot ", 0);
      fprintf (stderr, "SCR Open Error\n");
      lk_getkey ();


    }

start:

  lk_dispclr ();
  lk_disptext (1, 0, "IFD Menu", 0);
  lk_disptext (2, 2, "1.External IFD CARD", 0);
  lk_disptext (3, 2, "2.Internal SAM CARD", 0);

  lk_disptext (5, 0, "Enter Your Option", 0);

  while (1)
    {
      res = lk_getkey ();

      if (res == 1)
	{
	  Select_card (0x01);
	  break;
	}

      else if (res == 2)
	{
	  Select_card (0x02);
	  break;
	}

      else if (res == CANCEL)
	return ERROR;
    }

  ret = SCR_PowerUP5 ();
  if (ret)
    {
      lk_dispclr ();
      lk_disptext (2, 0, "IFD PowerUP Failed", 0);
      lk_disptext (3, 0, "Check the Connection", 0);
      lk_disptext (4, 0, "Reboot the System", 0);
      lk_getkey ();
      fprintf (stderr, "SCR Open Error\n");
      return ERROR;

    }
  brflag = 0;
  while (1)
    {

      ret = SCR_Checkup_CardPresence ();
      if (ret == 12)
	{
	  lk_dispclr ();
	  lk_disptext (2, 0, "CARD NOT PRESENT", 0);
	  lk_disptext (3, 0, "  INSERT CARD  ", 0);
	  brflag = 1;
	  sleep (2);
	  break;
	}


      lk_dispclr ();
      lk_disptext (1, 2, "1.Write Record", 0);
      lk_disptext (2, 2, "2.Read Record", 0);
      lk_disptext (3, 2, "3.No.of Records", 0);
      lk_disptext (4, 2, "4.Format Card", 0);
      lk_disptext (5, 0, "Enter Your Option", 0);

      while (1)
	{

	  res = lk_getkey ();

	  if (res == CANCEL)
	    {
	      brflag = 1;
	      goto start;
	    }

	  else if (res > 0 && res < 5)
	    break;
	}


      switch (res)
	{

	case 0x01:
	  lk_dispclr ();
	  memset (temp_buf, 0, 100);
	  memset (Wrtbuf, 0, 100);
	  count = lk_mgetalpha (2, 0, "Enter The Data", "", "", 50, temp_buf);
	  if (count < 0)
	    {
	      lk_dispclr ();
	      lk_disptext (2, 0, "Operation Cancel", 0);
	      sleep (2);
	      break;
	    }
	  if (count > 0)
	    {

	      temp_buf[count] = '\0';
	      memcpy (Wrtbuf, &cmd[1][0], 4);
	      Wrtbuf[4] = count;
	      memcpy (&Wrtbuf[5], temp_buf, count);
	      Wrtbuf[count + 5] = '\0';
	    }

	  lk_dispclr ();
	  SendCommand_Smartcard (cmd[0], 10);
	  SendCommand_Smartcard (Wrtbuf, (count + 5));
	  break;
	case 0x02:

	  lk_dispclr ();
	  memset (temp, 0, 5);
	  res = lk_mgetnumeric (2, 0, "Enter Record No.", "", "", 3, temp);
	  if (res <= 0)
	    break;
	  temp[res] = '\0';
	  ui.temp = atoi (temp);
	  cmd[4][2] = ui.uc.msb;
	  cmd[4][3] = ui.uc.lsb;
	  if (SendCommand_Smartcard (cmd[4], 5) == ERROR)
	    {

	      lk_dispclr ();
	      lk_disptext (2, 0, "Record Not Present", 1);
	      sleep (2);

	    }
	  break;

	case 0x03:
	  SendCommand_Smartcard (cmd[5], 5);
	  break;
	case 0x04:
	  lk_dispclr ();
	  lk_disptext (2, 3, "Formatting .. ", 1);
	  SendCommand_Smartcard (cmd[6], 5);
	  break;
	default:
	  break;
	}			//switch

    }				//while
  if (brflag == 1)
    goto start;
  SCR_Powerdown ();
  SCR_Close ();

  return SUCCESS;
}				//main

int
SendCommand_Smartcard (unsigned char *commd, int clen)
{
  int rlen = 0, rec, i;
  unsigned char res[100], temp[40];

  memset (res, 0, 100);
  SCR_SendCommand (commd, clen, res, &rlen);

  if (rlen > 0 && res[rlen - 2] == 0x90 && res[rlen - 1] == 0x00)
    {

      lk_dispclr ();
      lk_disptext (2, 2, "Operation Success", 1);
      sleep (1);

      if (commd[1] == 0x22)
	{
	  ui.uc.msb = res[0];
	  ui.uc.lsb = res[1];
	  rec = ui.temp;
	  sprintf (temp, "No of Records - > %d", rec);
	  lk_dispclr ();
	  lk_disptext (2, 0, temp, 0);
	  lk_getkey ();
	}
      if (commd[1] == 0xCA)
	{

	  for (i = 0; i < rlen, res[i] != 0xff; i++)
	    {
	      sprintf (temp + i, "%c", res[i]);
	    }
	  lk_dispclr ();
	  lk_disptext (1, 0, "Record Data :", 0);
	  lk_disptext (2, 0, temp, 0);
	  lk_getkey ();
	}

      return SUCCESS;

    }

  else
    {
      lk_dispclr ();
      lk_disptext (2, 2, "Operation Failed", 1);
      sleep (1);
      return ERROR;
    }
}
