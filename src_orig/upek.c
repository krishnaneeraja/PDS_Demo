#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "header.h"
#include "libgen.h"
#include "pwd.h"

#include "config.h"
#include "libthinkfinger.h"

#define MODE_UNDEFINED 0
#define MODE_ACQUIRE   1
#define MODE_VERIFY    2
#define MAX_USER       32

#define MAX_PATH       256


#define  ACQ   1		// Acquire finger print for default user
#define  VER   2		// Verifying the Default  User
#define  ACQ_USER 3		// Acquire for the specified user
#define  VER_USER 4		// Verify for the specified user


#define DEFAULT_BIR_PATH "/mnt/jffs2/test.bir"
#define BIR_EXTENSION    ".bir"
#define ISO_EXTENSION    ".iso-fmr"

typedef struct
{
  int mode;
  char bir[MAX_PATH];
  int verbose;
  int swipe_success;
  int swipe_failed;
} s_tfdata;

extern int v_setValue (char *display, char *dbKey, char validationKey,
		       char *validationValue, char *defaultVal);
extern char *getVal (char *);

typedef enum
{
  IN = 1,
  OUT = 0
} IN_OUT;

int insertUpdate (char *username, IN_OUT * inOut);

extern void *upDown (void *arg);
IN_OUT inOut;
extern FILE *fp;
extern char *configFile;
extern char *getName (char *username);
extern int deleteUser (char *username);

char username[100] = "";
int
upek_menu (void)
{

  fprintf (fp, "Reached here...4");
  fflush (fp);

  system ("mount -t usbdevfs usbdevfs /proc/bus/usb/");

  fprintf (fp, "Reached here...5");
  fflush (fp);

  while (1)
    {
      if (finger (VER_USER) == 2)
	return 1;
    }
}

static void
print_status (int swipe_success, int swiped_required, int swipe_failed)
{
  char buffer[128];
  char name[128];

  lk_dispclr ();
  lk_disptext (1, 1, "Please swipe finger", 1);
  sprintf (name, "%s", getName (username));
  lk_disptext (3, 0, name, 0);
  sprintf (buffer, "Required : %i", swiped_required - swipe_success);
  lk_disptext (4, 0, buffer, 0);
  //sprintf(buffer,"Success  : %i",swipe_success);
  //lk_disptext(4,0,buffer,0);
  //sprintf(buffer,"Failed   : %i",swipe_failed);
  //lk_disptext(5,0,buffer,0);

/*	sprintf (buffer,"Please swipe your finger (successful swipes %i/%i, failed swipes: %i)...",
	swipe_success, swiped_required, swipe_failed);
	lk_disptext(3,0,buffer,0);*/
}

static void
callback (libthinkfinger_state state, void *data)
{
  char *str;
  s_tfdata *tfdata = (s_tfdata *) data;
  IN_OUT inOut = IN;
  int rc;
  char msg[100];

  if (tfdata->verbose == true)
    {
      str = "unknown";

      if (state == TF_STATE_SWIPE_0)
	str = "TF_STATE_SWIPE_0";
      else if (state == TF_STATE_SWIPE_1)
	str = "TF_STATE_SWIPE_1";
      else if (state == TF_STATE_SWIPE_2)
	str = "TF_STATE_SWIPE_2";
      else if (state == TF_STATE_SWIPE_SUCCESS)
	str = "TF_STATE_SWIPE_SUCCESS";
      else if (state == TF_STATE_SWIPE_FAILED)
	str = "TF_STATE_SWIPE_FAILED";
      else if (state == TF_STATE_ENROLL_SUCCESS)
	str = "TF_STATE_ENROLL_SUCCESS";
      else if (state == TF_STATE_ACQUIRE_SUCCESS)
	str = "TF_STATE_ACQUIRE_SUCCESS";
      else if (state == TF_STATE_ACQUIRE_FAILED)
	str = "TF_STATE_ACQUIRE_FAILED";
      else if (state == TF_STATE_VERIFY_SUCCESS)
	str = "TF_STATE_VERIFY_SUCCESS";
      else if (state == TF_STATE_VERIFY_FAILED)
	str = "TF_STATE_VERIFY_FAILED";
      else if (state == TF_STATE_COMM_FAILED)
	str = "TF_STATE_COMM_FAILED";
      else if (state == TF_STATE_INITIAL)
	str = "TF_STATE_INITIAL";

      printf ("tf-tool: %s (0x%02x)\n", str, state);
    }

  if (tfdata->mode == MODE_ACQUIRE)
    {
      printf ("\nIn callback\n");
      switch (state)
	{
	case TF_STATE_ACQUIRE_SUCCESS:
	  updateEnroll (username);

	  lk_dispclr ();
	  lk_disptext (3, 0, "Enroll Success", 0);
	  lk_getkey ();
	  break;
	case TF_STATE_ACQUIRE_FAILED:
	  lk_dispclr ();
	  lk_disptext (3, 0, "Enroll Failed", 0);
	  lk_getkey ();
	  break;
	case TF_STATE_ENROLL_SUCCESS:

	  //print_status (tfdata->swipe_success, 3, tfdata->swipe_failed);
	  //printf (" done.\nStoring data (%s)...", tfdata->bir);
	  //fflush (stdout);
	  lk_dispclr ();
	  lk_disptext (3, 0, "Storing...Data", 0);
	  //lk_getkey();
	  break;
	case TF_STATE_SWIPE_FAILED:
	  print_status (tfdata->swipe_success, 3, ++tfdata->swipe_failed);
	  //lk_dispclr();
	  //lk_disptext(3,0,"Swipe Failed",0);
	  //usleep(50000);
	  break;
	case TF_STATE_SWIPE_SUCCESS:
	  print_status (++tfdata->swipe_success, 3, tfdata->swipe_failed);
	  //lk_dispclr();
	  //lk_disptext(3,0,"Swipe Success",0);
	  //usleep(50000);
	  break;
	case TF_STATE_SWIPE_0:
	  print_status (tfdata->swipe_success, 3, tfdata->swipe_failed);
	  break;
	case TF_STATE_SWIPE_1:
	  print_status (++tfdata->swipe_success, 3, tfdata->swipe_failed);
	  break;
	case TF_STATE_SWIPE_2:
	  usleep (50000);
	  print_status (++tfdata->swipe_success, 3, tfdata->swipe_failed);
	  break;
	default:
	  break;
	}
    }
  else if (tfdata->mode == MODE_VERIFY)
    {
      switch (state)
	{
	case TF_STATE_VERIFY_SUCCESS:
	  rc = insertUpdate (username, &inOut);

	  if (rc == 1 && inOut == IN)
	    str = "IN";
	  else
	    str = "OUT";

	  sprintf (msg, "User: %s => %s", username, str);
	  lk_dispclr ();
	  lk_disptext (1, 0, "Verification Success", 1);
	  lk_disptext (3, 0, msg, 1);
	  lk_getkey ();
	  break;
	case TF_STATE_VERIFY_FAILED:
	  lk_dispclr ();
	  lk_disptext (3, 0, "Verification Failed", 1);
	  lk_getkey ();
	  break;
	case TF_STATE_ENROLL_SUCCESS:
	  //print_status (tfdata->swipe_success, 1, tfdata->swipe_failed);
	  // lk_dispclr();
	  //lk_disptext(3,0,"Enrolment Success",0); 
	  //lk_getkey();
	  break;
	case TF_STATE_SWIPE_FAILED:
	  lk_dispclr ();
	  //lk_disptext(3,0,"Swipe Failed",0);
	  print_status (tfdata->swipe_success, 1, ++tfdata->swipe_failed);
	  break;
	case TF_STATE_SWIPE_SUCCESS:
	  print_status (++tfdata->swipe_success, 1, tfdata->swipe_failed);
	  // lk_dispclr();
	  // lk_disptext(3,0,"Swipe Success",0);
	  break;
	case TF_STATE_SWIPE_0:
	  print_status (tfdata->swipe_success, 1, tfdata->swipe_failed);
	  break;
	default:
	  break;
	}
    }
}

/*tatic void usage (char *name)
{
	printf ("Usage: %s %s\n", basename (name), usage_string);
}*/

static int
user_sanity_check (const char *user)
{
  size_t len = strlen (user);
  return strstr (user, "../") || user[0] == '-' || user[len - 1] == '/';
}

static int
user_exists (const char *login, unsigned char mode)
{
#define ACQUIRE 0
#define VERIFY  1
  int retval = false;
  unsigned char filepath[256];
  struct stat buf;
/*
	struct passwd *p  = getpwnam (login);
	if (p == NULL)
		retval = false;
	else
		retval = true;
*/
  snprintf (filepath, MAX_PATH - 1, "%s/%s%s", PAM_BIRDIR, login,
	    BIR_EXTENSION);
  if (mode == ACQUIRE)
    {
      if (stat (filepath, &buf) < 0)
	retval = true;
      else
	retval = false;
    }
  else if (mode == VERIFY)
    {
      if (stat (filepath, &buf) < 0)
	retval = false;
      else
	retval = true;
    }


  return retval;
}

static void
raise_error (libthinkfinger_init_status init_status)
{
  const char *msg;

  switch (init_status)
    {
    case TF_INIT_NO_MEMORY:
      msg = "Not enough memory.";
      break;
    case TF_INIT_USB_DEVICE_NOT_FOUND:
      msg = "USB device not found.";
      break;
    case TF_INIT_USB_OPEN_FAILED:
      msg = "Could not open USB device.";
      break;
    case TF_INIT_USB_CLAIM_FAILED:
      msg = "Could not claim USB device.";
      break;
    case TF_INIT_USB_HELLO_FAILED:
      msg = "Sending HELLO failed.";
      break;
    case TF_INIT_UNDEFINED:
      msg = "Undefined error.";
      break;
    default:
      msg = "Unknown error.";
    }

  printf ("%s\n", msg);

  return;
}

static int
acquire (const s_tfdata * tfdata)
{
  libthinkfinger *tf;
  libthinkfinger_init_status init_status;
  libthinkfinger_result tf_result;
  int retval = -1;

  lk_dispclr ();
  lk_disptext (3, 3, "Initializing...", 1);
  printf ("Initializing...");
  fflush (stdout);
  tf = libthinkfinger_new (&init_status);
  if (init_status != TF_INIT_SUCCESS)
    {
      raise_error (init_status);
      goto out;
    }
  printf (" done.\n");

  if (libthinkfinger_set_file (tf, tfdata->bir) < 0)
    goto out;
  if (libthinkfinger_set_callback (tf, callback, (void *) tfdata) < 0)
    goto out;

//      usleep(20*1000);        

  tf_result = libthinkfinger_acquire (tf);
  switch (tf_result)
    {
    case TF_RESULT_ACQUIRE_SUCCESS:
      retval = 0;
      break;
    case TF_RESULT_ACQUIRE_FAILED:
      retval = -1;
      break;
    case TF_RESULT_OPEN_FAILED:
      retval = -1;
      printf ("Could not open '%s'.\n", tfdata->bir);
      break;
    case TF_RESULT_SIGINT:
      retval = -1;
      printf ("Interrupted\n.");
      break;
    case TF_RESULT_USB_ERROR:
      printf ("Could not acquire fingerprint (USB error).\n");
      retval = -1;
      break;
    case TF_RESULT_COMM_FAILED:
      printf
	("Could not acquire fingerprint (communication with fingerprint reader failed).\n");
      retval = -1;
      break;
    default:
      printf ("Undefined error occured (0x%02x).\n", tf_result);
      retval = -1;
      break;
    }

  libthinkfinger_free (tf);
out:
  return retval;
}

static int
verify (const s_tfdata * tfdata)
{
  libthinkfinger *tf;
  libthinkfinger_init_status init_status;
  libthinkfinger_result tf_result;
  int retval = -1;


  printf ("Initializing...");
  lk_dispclr ();
  lk_disptext (3, 3, "Initializing...", 1);

  tf = libthinkfinger_new (&init_status);
  if (init_status != TF_INIT_SUCCESS)
    {
      raise_error (init_status);
      goto out;
    }
  printf (" done.\n");

  if (libthinkfinger_set_file (tf, tfdata->bir) < 0)
    goto out;
  if (libthinkfinger_set_callback (tf, callback, (void *) tfdata) < 0)
    goto out;

//      usleep(20*1000);

  tf_result = libthinkfinger_verify (tf);
  switch (tf_result)
    {
    case TF_RESULT_VERIFY_SUCCESS:
      retval = 0;
      break;
    case TF_RESULT_VERIFY_FAILED:
      retval = -1;
      break;
    case TF_RESULT_OPEN_FAILED:
      retval = -1;
      printf ("Could not open '%s'.\n", tfdata->bir);
      break;
    case TF_RESULT_SIGINT:
      retval = -1;
      printf ("Interrupted\n.");
      break;
    case TF_RESULT_USB_ERROR:
      printf ("Could not verify fingerprint (USB error).\n");
      retval = -1;
      break;
    case TF_RESULT_COMM_FAILED:
      printf
	("Could not verify fingerprint (communication with fingerprint reader failed).\n");
      retval = -1;
      break;
    default:
      printf ("Undefined error occured (0x%02x).\n", tf_result);

      retval = -1;
      break;
    }
  libthinkfinger_free (tf);
out:
  return retval;


}

int
finger (int option)
{
  int i;
  int retval = 0;
  s_tfdata tfdata;
  int path_len = 0;
  const char *user;
  char bir_file_path[200];
  char iso_file_path[200];

  tfdata.mode = MODE_UNDEFINED;
  tfdata.verbose = false;
  tfdata.swipe_success = 0;
  tfdata.swipe_failed = 0;


  switch (option)
    {
    case ACQ:
      snprintf (tfdata.bir, MAX_PATH - 1, "%s", DEFAULT_BIR_PATH);
      tfdata.mode = MODE_ACQUIRE;
      retval = acquire (&tfdata);

      if (retval == -1)
	{
	  remove ("/mnt/jffs2/test.bir");
	  lk_dispclr ();
	  lk_disptext (1, 4, "ERROR", 1);
	  lk_disptext (3, 2, "Unable To", 0);
	  lk_disptext (4, 0, "Match Finger", 0);
	  lk_getkey ();
	}

      break;

    case VER:
      if (access (DEFAULT_BIR_PATH, R_OK) != 0)
	{
	  lk_dispclr ();
	  lk_disptext (1, 4, "ERROR", 1);
	  lk_disptext (3, 2, "User not exist or", 0);
	  lk_disptext (4, 0, "File Accessing Error", 0);
	  lk_getkey ();
	  return ERROR;
	}

      snprintf (tfdata.bir, MAX_PATH - 1, "%s", DEFAULT_BIR_PATH);
      tfdata.mode = MODE_VERIFY;
      retval = verify (&tfdata);
      break;

    case ACQ_USER:
      lk_dispclr ();
      lk_disptext (2, 5, "Enter Name", 0);
      if (lk_getalpha (4, 1, "", 15, username, 4) < 0)
	return ERROR;

      user = username;
      if (strlen (user) > MAX_USER)
	{
	  printf ("User name \"%s\" is too long (maximum %i chars).\n", user,
		  MAX_USER);
	  return ERROR;
	}
      retval = checkUserInDB (user);
      if (retval == 0)
	{
	  lk_dispclr ();
	  lk_disptext (1, 4, "ERROR", 1);
	  lk_disptext (3, 1, "User does not exist!", 1);
	  lk_getkey ();
	  return ERROR;
	}
      else if (retval == 2)
	{
	  lk_dispclr ();
	  lk_disptext (1, 4, "ERROR", 1);
	  lk_disptext (3, 1, "User already registered!", 1);
	  lk_getkey ();
	  return ERROR;
	}
      if (user_sanity_check (user) || user_exists (user, 0) == false)
	{
	  // printf ("The user \"%s\" already exists.\n", user);
	  lk_dispclr ();
	  lk_disptext (1, 4, "ERROR", 1);
	  lk_disptext (3, 1, "User fingerprint already exists!", 1);
	  lk_getkey ();
	  return ERROR;
	}
      if (access (PAM_BIRDIR, R_OK | W_OK | X_OK) != 0)
	{
	  perror ("Could not access " PAM_BIRDIR);
	  return ERROR;
	}

      path_len =
	strlen (PAM_BIRDIR) + strlen ("/") + strlen (user) +
	strlen (BIR_EXTENSION);
      if (path_len > MAX_PATH - 1)
	{
	  printf ("Path \"%s/%s%s\" is too long (maximum %i chars).\n",
		  PAM_BIRDIR, user, BIR_EXTENSION, MAX_PATH - 1);
	  return ERROR;
	}
      snprintf (tfdata.bir, MAX_PATH - 1, "%s/%s%s", PAM_BIRDIR, user,
		BIR_EXTENSION);
      tfdata.mode = MODE_ACQUIRE;
      retval = acquire (&tfdata);

      if (retval == -1)
	{
	  remove (tfdata.bir);
	  lk_dispclr ();
	  lk_disptext (1, 4, "ERROR", 1);
	  lk_disptext (3, 2, "Unable To", 0);
	  lk_disptext (4, 0, "Match Finger", 0);
	  lk_getkey ();

	}
      break;

    case VER_USER:
      lk_dispclr ();
      lk_disptext (2, 0, "Enter Employee Id:", 0);
      if (lk_getalpha (4, 1, "", 15, username, 4) < 0)
	return ERROR;

      if (strcmp (username, getVal ("admin_user")) == 0)
	{
	  if (admin_menu () == 2)
	    return 2;
	  else
	    return 0;
	}
      user = username;
      printf ("Path= \"%s/%s%s\".\n", PAM_BIRDIR, user, BIR_EXTENSION);

      if (strlen (user) > MAX_USER)
	{
	  printf ("User name \"%s\" is too long (maximum %i chars).\n", user,
		  MAX_USER);
	  return ERROR;
	}
      if (user_sanity_check (user) || user_exists (user, 1) == false)
	{
	  printf ("The user \"%s\" does not exist.\n", user);
	  lk_dispclr ();
	  lk_disptext (1, 4, "ERROR", 1);
	  lk_disptext (3, 2, "User does not exist!", 1);
	  lk_getkey ();
	  return ERROR;
	}

      if (access (PAM_BIRDIR, R_OK | W_OK | X_OK) != 0)
	{
	  perror ("Could not access " PAM_BIRDIR);
	  retval = -1;
	  return retval;
	}

	sprintf(bir_file_path,"%s/%s%s",PAM_BIRDIR,user,BIR_EXTENSION);
	
      if (access (bir_file_path, R_OK | W_OK | X_OK) != 0)
	{
		sprintf(iso_file_path,"%s/%s%s",PAM_BIRDIR,user,ISO_EXTENSION);
		if (access (iso_file_path, R_OK | W_OK | X_OK) != 0) //If ISO file not found
		{
			perror ("Could not access " PAM_BIRDIR);
			retval = -1;
		}
		else
		{
			convert(iso_file_path,bir_file_path);
		}
	  return retval;
	}

      path_len =
	strlen (PAM_BIRDIR) + strlen ("/") + strlen (user) +
	strlen (BIR_EXTENSION);
      if (path_len > MAX_PATH - 1)
	{
	  printf ("Path \"%s/%s%s\" is too long (maximum %i chars).\n",
		  PAM_BIRDIR, user, BIR_EXTENSION, MAX_PATH - 1);
	  return ERROR;
	}
      snprintf (tfdata.bir, MAX_PATH - 1, "%s/%s%s", PAM_BIRDIR, user,
		BIR_EXTENSION);
      tfdata.mode = MODE_VERIFY;
      retval = verify (&tfdata);
      break;
    }
  return retval;

}
