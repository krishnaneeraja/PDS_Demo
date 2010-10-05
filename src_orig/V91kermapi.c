#include <V91kermapi.h>
#include <sys/types.h>
#include <signal.h>
#define NULL 0
/*** up load a text file from epos to pc*********/
int
send_text_file (const char *filename)
{
  pid_t child_pid;
  int status;
  child_pid = fork ();
  if (child_pid != 0)
    wait (&status);
  else if (child_pid == 0)
    {
      execlp ("/home/devkermit", "devkermit", "-is", filename);
      abort ();
    }
  return status;
}

/*******  upload a image file from epos to pc*********/

int
send_image_file (const char *filename)
{
  char *arg_list_Tx[] = {
    "devkermit",
    "-is",
    "/mnt/jffs2/output.txt",
    NULL
  };

  pid_t child_pid;
  int status;
  chmod ("/mnt/jffs2", 0755);
  child_pid = fork ();
  if (child_pid != 0)
    wait (&status);
  else if (child_pid == 0)
    {
      execvp ("/home/devkermit", arg_list_Tx);
      abort ();
    }
  return status;
}


/******** download a file from pc to epos********/

int
recv_text_file ()
{
  pid_t child_pid;
  int status;
  chmod ("/mnt/jffs2", 0755);
  child_pid = fork ();
  if (child_pid != 0)
    wait (&status);
  else if (child_pid == 0)
    {
      execlp ("/home/devkermit", "devkermit", "-ir");
      abort ();
    }
  return status;
}


/******** download a image file from pc to epos*******/

int
recv_image_file (const char *path)
{
  char *arg_list_Rx[] = {
    "devkermit",
    "-ir",
    "-w",
    "-a",
    "/mnt/jffs2/output.txt",
    NULL
  };
  pid_t child_pid;
  int status;
  char buff[30];
  chmod ("/mnt/jffs2", 0755);
  child_pid = fork ();
  if (child_pid != 0)
    wait (&status);
  else if (child_pid == 0)
    {
      execvp ("/home/devkermit", arg_list_Rx);
      abort ();
    }
  sprintf (buff, "mv /mnt/jffs2/output.txt %s", path);
  system (buff);
  return status;
}
