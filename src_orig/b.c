#include <stdio.h>

void *syncThread (void *arg);

extern FILE *fp;
void *upDown (void *arg);

void *
syncThread (void *arg)
{
  int sleepTime = 300, temp = 0;
  while (1)
    {
      fprintf (fp, "About to synchronise\n");
      upDown (arg);

      temp = 60;
      if (temp > 40)
	sleepTime = temp;

      sleep (sleepTime);
    }
}

void *
upDown (void *arg)
{
  fprintf (fp, "Called Function");
  fflush (stdout);
}
