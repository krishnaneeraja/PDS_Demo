#include <header.h>

static int kp = 1;
int parse (int[], char *[]);
int
Keypad_test (void)
{
  int event, ret, le, pe;
  char text[40];
  int arr[25] =
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1,
0 };
  char *list[25] =
    { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "11", "12",
"ENTER", "14", "#/00", "16", "F1", "F2", "F3", "F4", "21", "22", "23", "CLEAR" };

begin:
  lk_dispclr ();
  lk_disptext (2, 2, "KEYPAD TEST MODE", 0);
  while (1)
    {
      event = lk_getkey ();	// waiting for a key
      lk_dispclr ();
      lk_disptext (2, 2, "KEYPAD TEST MODE", 0);
      switch (event)
	{
	case 17:
	  lk_disptext (3, 4, "KEY = F1", 1);
	  arr[17] = 1;
	  break;

	case 18:
	  lk_disptext (3, 4, "KEY = F2", 1);
	  arr[18] = 1;
	  break;
	case 19:
	  lk_disptext (3, 4, "KEY = F3", 1);
	  arr[19] = 1;
	  break;
	case 20:
	  lk_disptext (3, 4, "KEY = F4", 1);
	  arr[20] = 1;
	  break;
	case 24:
	  lk_disptext (3, 4, "KEY = CLEAR", 1);
	  arr[24] = 1;
	  break;
	case 10:
	  lk_disptext (3, 4, "KEY = *", 1);
	  arr[10] = 1;
	  break;
	case 15:
	  lk_disptext (3, 4, "KEY = #/00", 1);
	  arr[15] = 1;
	  break;
	case 13:
	  lk_disptext (3, 4, "KEY = ENTER", 1);
	  arr[13] = 1;
	  break;
	case CANCEL:
	  lk_disptext (3, 4, "KEY = CANCEL", 1);
	  le = parse (arr, list);
	  if (le < 0)
	    {
	      lk_dispclr ();
	      lk_disptext (3, 3, "Back To Test", 1);
	      sleep (1);
	      lk_dispclr ();
	      goto begin;
	    }
	  else
	    {
	      lk_disptext (5, 2, "Back to Main Menu", 0);
	      sleep (2);
	      return 0;
	    }
	  break;
	default:
	  sprintf (text, "KEY = %02x", event);
	  arr[event] = 1;
	  lk_disptext (3, 4, text, 1);
	  break;

	}
    }
  return 0;
}


int
parse (int abc[25], char *pre[25])
{
  int i;
  int flag = 1;
  char buff[100];
  char one[10];
  strcpy (buff, "");
  for (i = 0; i < 25; i++)
    {
      if (abc[i] == 0)
	{
	  //              printf("\n %s : KEY NOT PRESSED\n",pre[i]);
	  sprintf (one, "%s ", pre[i]);
	  strcat (buff, one);
	  flag = 0;
	}
    }
  if (flag == 1)
    {
      return 1;
    }
  else
    {
      lk_dispclr ();
      lk_disptext (1, 1, buff, 0);
      lk_disptext (4, 3, "Not Pressed", 1);
      lk_buzzer (4);
      //      lk_disptext(5,0,"Back to test",0);
      sleep (4);
      lk_dispclr ();
      return -1;
    }
}
