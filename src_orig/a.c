#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

extern void *syncThread (void *arg);
FILE *fp;

int
main (void)
{
  struct tm intim;
  int Ret_val;
  double amount;
  unsigned char str[100] = "";
  pthread_t syncT;

  fp = stdout;

  if (pthread_create (&syncT, NULL, syncThread, NULL) != 0)
    fprintf (stderr, "Error creating the thread");

  while (1)
    {
      sleep (10);
      fprintf (fp, "In main thread\n");
    }
  return 0;
}
