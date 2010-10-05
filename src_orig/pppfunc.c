#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include <sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include <signal.h>
#include "V91ppp.h"


extern char **environ;
extern int errno;
int
ppp_pstn_update_settings (PPP_PSTN_SETTINGS * settings)
{
  FILE *setfp;
  setenv ("TELEPHONE", settings->dialnum, 1);
  setenv ("ACCOUNT", settings->account, 1);
  setenv ("PASSWORD", settings->password, 1);
  setenv ("STATIC_IP", settings->localip, 1);
  setenv ("REMOTE_IP", settings->remoteip, 1);
  setenv ("NETMASK", settings->netmask, 1);
  setenv ("MODEM", "/dev/ttyS0", 1);
  setenv ("SPEED", "2400", 1);
  if (settings->auth == USE_PAP_AUTH)
    setenv ("USE_PAP", "1", 1);
  else if (settings->auth == USE_CHAP_AUTH)
    setenv ("USE_CHAP", "1", 1);
  setfp = fopen ("/etc/ppp/settings", "w");
  if (setfp == NULL)
    {
      fprintf (stderr, "Failed to Open file\n");
    }
  else
    {
      fwrite (settings, sizeof (PPP_PSTN_SETTINGS), 1, setfp);
      fclose (setfp);
    }
  return 0;
}


int
ppp_open_pstn (void)
{
  int conres, childstatus;
  struct stat buf;
  pid_t child_pid;

  printf ("PPP starting\n");
  child_pid = fork ();
  if (child_pid > 0)
    {
      sleep (3);
      do
	{
	  errno = 0;
	  conres = stat ("/var/run/ppp.link", &buf);
	  childstatus = stat ("/var/run/ppp.delink", &buf);
	}
      while (conres < 0 && childstatus < 0);

      if (conres == 0)
	{
	  printf ("PPP Connected \n");
	  return child_pid;
	}
      else if (childstatus == 0)
	return -1;
    }
  else if (child_pid == 0)
    {
      char *argv[] = { "dialout", NULL };
      execve ("/etc/ppp/dialout", argv, environ);
    }
  else
    return -1;
}

//to terminate the ppp connection
void
ppp_close_pstn (void)
{
  struct stat buf;
  int res = 0, cntr = 0;
  system ("sh /etc/ppp/ppp-close");
  do
    {
      res = stat ("/var/run/ppp.link", &buf);
      sleep (1);
      cntr++;
    }
  while (res > 0 && cntr < 20);
  return;
}
