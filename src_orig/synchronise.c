void *syncThread (void *arg);

void *
syncThread (void *arg)
{
  int sleepTime = 300, temp = 0;
  while (1)
    {
      sleep (sleepTime);

      fprintf (fp, "About to synchronise\n");
      fflush (fp);

      upDown (arg);

      temp = (atoi) (getVal ("sleepTime"));
      if (temp > 300)
	sleepTime = temp;

      fprintf (fp, "Sleeping now for Sleeptime:%d\n", sleepTime);
      fflush (fp);

    }
}
