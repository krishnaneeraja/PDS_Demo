/********************************************************************
 *                   V91Modem.c
 * Oct 13 2006 
 * 
 *
 *******************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>


int Modemfd;
sig_atomic_t Modem_timeout_flag = 0;

static void modem_timeout_handler (int signum, siginfo_t * sinfo, void *ctxt);

/*-----------------------------Open Modem Interface--------------------------*/
/*
  @portname : name of the device to be opened like /dev/ttyS0 or /dev/modem etc.

  Returns : 0 if device is opened successfully.
          :-1 if it fails to open the interface.
*/
int
ModemOpen (char *portname)
{
  Modemfd = open (portname, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (Modemfd < 0)
    {
      fprintf (stderr, "Cannot the Serial Port with name %s\n", portname);
      return -1;
    }
  return 0;
}

/*---------------------------Set Baudrate and other modem Parameters---------------------------------------------*/
/*
  @baudrate : baudrate at which data is to be transferred.
  
  No return value.
  
  Ignores parity errors only.
*/
void
setmodemparretail (int baudrate)	/* setup serial port */
{
  struct termios newtio;
  bzero (&newtio, sizeof (newtio));
  newtio.c_cflag = CS8 | CLOCAL | CREAD;

  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;
  newtio.c_lflag = 0;
  newtio.c_cc[VTIME] = 0;
  newtio.c_cc[VMIN] = 0;
  switch (baudrate)
    {
    case 300:
      baudrate = B300;
      break;
    case 1200:
      baudrate = B1200;
      break;
    case 2400:
      baudrate = B2400;
      break;
    case 9600:
      baudrate = B9600;
      break;
    case 19200:
      baudrate = B19200;
      break;
    case 38400:
      baudrate = B38400;
      break;
#ifdef B57600
    case 57600:
      baudrate = B57600;
      break;
#endif
#ifdef B115200
    case 115200:
      baudrate = B115200;
      break;
#endif
#ifdef B230400
    case 230400:
      baudrate = B230400;
      break;
#endif
    default:
      baudrate = B9600;
    }
  cfsetispeed (&newtio, baudrate);
  cfsetospeed (&newtio, baudrate);
  tcsetattr (Modemfd, TCSANOW, &newtio);
}

/*--------------------Read data---------------------------------------------------*/
/*
  @buffer : response empty buffer

  @no_of_bytes :number of bytes to be read.
  
  @timeout:time before which the no_of_bytes must be read.
  
  Returns : 0 if no data is available or timeout occurs.
          :>0 if data received.

  The value in buffer is a string if tot_bytes_read is greater than zero.
  
  In this function we actually take a signal handler for the SIGALRM signal for the timeout specified,
  and try to read the no_of_bytes specified before timeout occurs.The previous state of the signal handler 
  is restored before leaving the function.
  
*/
int
read_from_modem (char *buffer, int no_of_bytes, int timeout)
{
  struct sigaction oldact, newact;
  int count, tot_bytes_read = 0;

  /*Set the timeout using the signal SIGALRM */
  newact.sa_sigaction = modem_timeout_handler;
  newact.sa_flags = SA_SIGINFO;
  sigemptyset (&newact.sa_mask);
  sigfillset (&newact.sa_mask);
  sigaction (SIGALRM, &newact, &oldact);
  alarm (timeout);
  Modem_timeout_flag = 0;

  do
    {
      count = read (Modemfd, buffer + tot_bytes_read, no_of_bytes);
      if (count > 0)
	{
	  tot_bytes_read += count;
	  no_of_bytes -= count;
	}
    }
  while (no_of_bytes > 0 && Modem_timeout_flag == 0);

  if (!Modem_timeout_flag)
    alarm (0);
  if (tot_bytes_read > 0)
    buffer[tot_bytes_read] = '\0';
  sigaction (SIGALRM, &oldact, &newact);
  return tot_bytes_read;
}

/*-----------------Write Data-------------------------------------------------------------*/
/*
  @buffer : data to be written
  
  @no_of_bytes : length of data to be sent.
  
  If the application defines SINGLE_BYTE_WRITE one byte write is followed else all the data is written at once.
  
  Returns : 0 on Success
          :-1 if it fails.
*/
int
write_to_modem (char *buffer, int no_of_bytes)
{
#ifdef SINGLE_BYTE_WRITE
  int index;
  for (index = 0; index < no_of_bytes; index++)
    {
      if (write (Modemfd, buffer + index, 1) < 1)
	return -1;
    }
#else
  if (write (Modemfd, buffer, no_of_bytes) < 0)
    return -1;
#endif
  return 0;
}

/*-------------------Execute the modem command-----------------------------------------------------------*/
/*
  @command : command to be executed

  @response : buffer to store the response received.

  @timeout: time before which the response is expected for the command after writing it to the modem.
  
  Returns : -1 on failure.
          : -2 on timeout
          : 0 on success
  
  The function after writing the command to the modem waits for the response which features two "\r\n" 
  combinations.on receiving these the function returns or if timeout occurs the function returns.

NOTE:The command sent must feature only \r at the end and not "\r\n".
*/
int
Exec_Modem_command (char *command, char *response, int timeout)
{
  int tot_bytes_read = 0, count = 0;
  unsigned char modemflag = 0, ch;
  struct sigaction oldact, newact;

  if (write_to_modem (command, strlen (command)) < 0)
    {
      fprintf (stderr, "Writing Command to the Modem Failed\n");
      return -1;
    }

  newact.sa_sigaction = modem_timeout_handler;
  newact.sa_flags = SA_SIGINFO;
  sigemptyset (&newact.sa_mask);
  sigfillset (&newact.sa_mask);
  sigaction (SIGALRM, &newact, &oldact);

  Modem_timeout_flag = 0;
  alarm (timeout);

  do
    {
      count = read (Modemfd, &ch, 1);
      if (count > 0)
	{
	  response[tot_bytes_read] = ch;
	  if (ch == 0x0a && response[tot_bytes_read - 1] == 0x0d)
	    {
	      modemflag++;
	    }
	  tot_bytes_read += count;
	  response[tot_bytes_read] = '\0';
#ifdef DEBUG
	  fprintf (stderr, "%c ", ch);
#endif
	}
    }
  while (modemflag < 2 && Modem_timeout_flag == 0);
  alarm (0);
  sigaction (SIGALRM, &oldact, &newact);
  if (modemflag == 2)
    return 0;
  else
    return -2;
}

/*--------------------------Execute and check response----------------------*/
/*
  @command : command to be exceuted

  @response :response received

  @expect : an array of strings of expected responses 

  @timeout : time before which the response is expected.

  Returns : -1 on fialure
          : -2 if timeout.
          : array index of the expect string which matches the response.  

    This is same as the above function,but here the user can send a list of the responses he is expecting
    which will be checked.
   NOTE: check the response buffer even if timeout occurs and the function returns.
*/
int
Test_modem_response (char *command, char *response, char *expect[],
		     int timeout)
{
  int tot_bytes_read = 0, count = 0, index = 0;
  unsigned char ch;
  struct sigaction oldact, newact;

  if (write_to_modem (command, strlen (command)) < 0)
    {
      fprintf (stderr, "Writing Command to the Modem Failed\n");
      return -1;
    }

  newact.sa_sigaction = modem_timeout_handler;
  newact.sa_flags = SA_SIGINFO;
  sigemptyset (&newact.sa_mask);
  sigfillset (&newact.sa_mask);
  sigaction (SIGALRM, &newact, &oldact);

  Modem_timeout_flag = 0;
  alarm (timeout);

  do
    {

      count = read (Modemfd, &ch, 1);
      if (count > 0)
	{
	  response[tot_bytes_read] = ch;
	  tot_bytes_read += count;
	  response[tot_bytes_read] = '\0';
	}
      for (index = 0; expect[index] != NULL; index++)
	{
	  if (strstr (response, expect[index]))
	    {
	      alarm (0);
	      sigaction (SIGALRM, &oldact, &newact);
	      return index;
	    }
	}
    }
  while (Modem_timeout_flag == 0);
  alarm (0);
  sigaction (SIGALRM, &oldact, &newact);
  return -2;
}

/*--------Close the device--------------------------------------------------------------------------*/
int
Modemclose (void)
{
  return close (Modemfd);
}

/*------------------------Timeout Handler-----------------------------------------------------------*/
void
modem_timeout_handler (int signum, siginfo_t * sinfo, void *ctxt)
{
  Modem_timeout_flag = 1;
}

/*----------------Execute the given command -----------------------------------------------------------------*/
/*
 @command : command to be executed

  @response : buffer to store the response received.

  @timeout: time before which the response is expected for the command after writing it to the modem.
  
  Returns : -1 on failure.
          : -2 on timeout 
          :  0 on success
  
  
  This is same as the above function Exec_Modem_command but it uses the select system call to 
  evaluate the timeout instead of the signal handler for SIGALARM signal.

  This function uses the logic that select system call updates the timeval structure with the time 
  remaining when it returns.so we wiat until the timeout occurs or the required response is received.
*/
int
exec_modem_comm_without_signal (char *command, char *response, int timeout)
{
  struct timeval tv;
  int result, count, tot_bytes_read = 0, bytes_avlbl = 0, index = 0;
  unsigned char cntr = 0;
  fd_set rfds;

  FD_ZERO (&rfds);
  FD_SET (Modemfd, &rfds);
  tv.tv_sec = timeout;
  tv.tv_usec = 0;

  if (write_to_modem (command, strlen (command)) < 0)
    {
      fprintf (stderr, "Writing Command to the Modem Failed\n");
      return -1;
    }

  do
    {
      result = select (Modemfd + 1, &rfds, NULL, NULL, &tv);
      if (result > 0)
	{
	  ioctl (Modemfd, FIONREAD, &bytes_avlbl);
	  if (bytes_avlbl > 0)
	    count = read (Modemfd, (response + tot_bytes_read), bytes_avlbl);
	  if (count > 0)
	    {
	      tot_bytes_read += count;
	      response[tot_bytes_read] = '\0';
	    }
	  for (; ((index < strlen (response)) && (strlen (response) > 1));
	       index++)
	    if (response[index] == 0x0a && response[index - 1] == 0x0d)
	      cntr++;
	}
    }
  while (tv.tv_sec > 0 && cntr < 2);
  if (cntr == 2)
    return 0;
  else
    return -2;
}

/*---------------------Execute the command and check----------------------------------------------------------*/
/*
  @command : command to be exceuted
  
  @response :response received
  
  @expect : an array of strings of expected responses 
  
  @timeout : time before which the response is expected.

  Returns : -1 on fialure
          : -2 if timeout.
          : array index of the expect string which matches the response.  

  This is same as the above function Test_modem_response but the timeout is not identified using
  a signal handler for the SIGALRM signal but by using the select system call.
*/
int
Test_modem_response_without_signal (char *command, char *response,
				    char *expect[], int timeout)
{
  struct timeval tv;
  int result, count, tot_bytes_read = 0, bytes_avlbl = 0;

  fd_set rfds;

  FD_ZERO (&rfds);
  FD_SET (Modemfd, &rfds);
  tv.tv_sec = timeout;
  tv.tv_usec = 0;
  tcflush (Modemfd, TCIOFLUSH);
  if (write_to_modem (command, strlen (command)) < 0)
    {
      fprintf (stderr, "Writing Command to the Modem Failed\n");
      return -1;
    }

  do
    {
      int i = 0;
      result = select (Modemfd + 1, &rfds, NULL, NULL, &tv);
      if (result > 0)
	{
	  ioctl (Modemfd, FIONREAD, &bytes_avlbl);
	  if (bytes_avlbl > 0)
	    count = read (Modemfd, (response + tot_bytes_read), bytes_avlbl);
	  if (count > 0)
	    {
	      tot_bytes_read += count;
	      response[tot_bytes_read] = '\0';
	    }
	}
      for (i = 0; expect[i] != NULL; i++)
	if (strstr (response, expect[i]))
	  return i;
    }
  while (tv.tv_sec > 0);
  return -2;
}

/*----------------------------------------------------------------------------*/
/*
  @buffer : response empty buffer
  
  @no_of_bytes :number of bytes to be read.
  
  @timeout:time before which the no_of_bytes must be read.
  
  Returns : 0 if no data is available or timeout occurs.
          : >0 if data received.
  
  This is same as the above function read_from_modem but the timeout is not identified using
  a signal handler for the SIGALRM signal but by using the select system call.
  
*/
int
read_from_modem_without_signal (char *buffer, int no_of_bytes, int timeout)
{
  struct timeval tv;
  int result, count, tot_bytes_read = 0, bytes_avlbl = 0;

  fd_set rfds;
  FD_ZERO (&rfds);
  FD_SET (Modemfd, &rfds);
  tv.tv_sec = timeout;
  tv.tv_usec = 0;

  do
    {
      result = select (Modemfd + 1, &rfds, NULL, NULL, &tv);
      if (result > 0)
	{
	  ioctl (Modemfd, FIONREAD, &bytes_avlbl);
	  if (bytes_avlbl > 0)
	    count = read (Modemfd, (buffer + tot_bytes_read), bytes_avlbl);
	  if (count > 0)
	    {
	      tot_bytes_read += count;
	      no_of_bytes -= count;
	    }
	}
    }
  while (tv.tv_sec > 0 && no_of_bytes > 0);
  buffer[tot_bytes_read] = '\0';
  return tot_bytes_read;
}

/*-------------------------------------------------------------------------------------------*/
