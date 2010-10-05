#include<header.h>

int
Battery_test (void)
{
  FILE *fp;
  int len, i, ret;
  float val, cnt;
  char str[5], bat1[4], message[40] = "";
  static int bat = 1;

  fp = fopen ("/proc/battery", "r");
  fread (str, 4, 1, fp);

  bat1[0] = str[0];
  bat1[1] = str[1];
  bat1[2] = str[2];
  bat1[3] = str[3];
  bat1[4] = '\0';

  i = atoi (bat1);
  cnt = (float) 8.4 / 1024;
  val = i * cnt;

  sprintf (message, "VOLTAGE = %0.2fV ", val);
  fprintf (stdout, message);
  lk_dispclr ();
  lk_disptext (2, 4, "BATTERY TEST", 0);
  lk_disptext (4, 2, message, 1);
  lk_getkey ();
  return SUCCESS;
}
