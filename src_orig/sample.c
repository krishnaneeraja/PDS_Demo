#include<header.h>

int
main (void)
{

  int i, j, l, k;
  char msg[100] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`1234567890-=\\[];'./,~!@#$%^&*()_+|}{:\">?<";
  char str[21];

  lk_open ();

  l = 0;
  k = strlen (msg);

  while (1)
    {
      for (i = 0; i < 21; i++)
	str[i] = msg[i + l];
      str[21] = 0x00;

      lk_dispclr ();
      lk_disptext (2, 0, str, 0);
      //lk_disphlight(2);
      usleep (300000);
      l++;

      if (l == k)
	l = 0;
    }
  lk_close ();


}

99
