#include<header.h>

int
serial_test (void)
{
  struct termios my_termios, old_termios;
  int i, l, n, count = 0, ret = 0;

  unsigned char send[1024] = "", recv[1024] = "";

  static int fd;

  memset (&my_termios, 0, sizeof (struct termios));
  memset (&old_termios, 0, sizeof (struct termios));

  fd = open ("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0)
    {
      fprintf (stderr, "Unable to open port \n Please check \n");
      return -1;
    }

  tcgetattr (fd, &old_termios);
  tcflush (fd, TCIFLUSH);
  my_termios.c_cflag = B115200 | CS8 | CREAD;
  my_termios.c_cc[VMIN] = 0;
  my_termios.c_cc[VTIME] = 0;
  tcsetattr (fd, TCSANOW, &my_termios);
  while (1)
    {
      lk_dispclr ();
      lk_disptext (2, 4, "SERIAL TEST", 0);
      lk_disptext (3, 2, "1. Write Data", 0);
      lk_disptext (4, 2, "2. Read Data", 0);

      ret = lk_getkey ();

      if (ret == CANCEL)
	break;
      else if (ret == 1)
	{
	  lk_dispclr ();
	  lk_disptext (2, 2, "Enter Data to send", 0);
	  if (lk_getalpha (4, 1, send, 15, send, 4) < 0)
	    continue;
	  l = strlen (send);
	  n = write (fd, send, l);

	  lk_dispclr ();

	  if (n != l)

	    lk_disptext (2, 2, "Write Error", 1);

	  else

	    lk_disptext (2, 2, "Write Success", 0);

	  lk_disptext (5, 0, "Press Any Key to Exit", 0);

	  lk_getkey ();


	}

      else if (ret == 2)

	{

	  lk_dispclr ();
	  lk_disptext (2, 2, "Ready To Receive", 0);

	  while (1)
	    {
	      sleep (2);
	      memset (recv, 0x00, 1024);
	      l = read (fd, &recv, 1024);
	      recv[l - 1] = 0x00;
	      if (l > 0)
		{
		  lk_dispclr ();
		  lk_disptext (1, 0, "Received Data is: ", 0);
		  lk_disptext (2, 0, recv, 0);
		  sleep (2);
		  lk_disptext (5, 0, "Press Any Key to Exit", 0);
		  lk_getkey ();
		  break;
		}
	      else if (count > 10)
		{
		  lk_disptext (3, 1, "Time out", 1);
		  sleep (2);
		  count = 0;
		  break;
		}
	      count++;
	    }

	}
    }

  tcsetattr (fd, TCSANOW, &old_termios);
  close (fd);
  return 0;
}
