/*****************************************************************************************************
 *            pppcomm.c
 *
 *
 *  
 *  Comments:
 *  1. ppp_init_deinit to ifconfig up and down in preparation for PPP
 *  2. Open ppp connection
 *  3. Close ppp connection
 *  4. Post/Get messages http and https using curl library
 *  5. Functions with (int) return type return 1 if successful and 0 if not
 ******************************************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/io.h>

//#include"/usr/include/curl/types.h"
//#include"/usr/include/curl/curl.h"
//#include"/usr/include/curl/easy.h"

//#include"curlver.h"
#include"multi.h"
#include"curl.h"
#include"easy.h"

#include <sys/ioctl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/time.h>

#include "sdlc.h"
#include "V91printer.h"

#define ENTER 13
#define CANCEL 0x17

#define TIMEOUT 15

char *Userfile = "/mnt/jffs2/dat";
char Errfile[CURL_ERROR_SIZE];
static char *shtml = { "http://124.125.131.54/" };
int GET_file_clear (char *http_url, char *filename, char *data);
/*unsigned char CashPurchase[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x91,0x02,0x00,0x00,0x00,0x3F,0xFC,0x08,0x33,0x33,0x33,0x33,0x43,0x10,0x12,0x34,0x56,0x78,0x90,0x12,0x34,
 0x56,0xA9,0x04,0x11,0x11,0xF8,0x05,0x00,0x00,0x1F,0x70,0x40,0x12,0x3F,0xE0,0x80,0x00,0x10,0x00,0x0F,0x90,
0xE1,0x41,0x63,0x42,0x52,0x40,0x92,0x00,0xAA,0x02,0x01,0xA9,0x04,0x11,0x11,0xF8,0x05,0x00,0x00,0x1F,0x70, 
0x40,0x12,0x3F,0xE0,0x80,0x00,0x10,0x00,0x0F,0x90,0xE1,0x41,0x63,0x42,0x52,0x40,0x92,0x00,0xAA,0x02,0x01,
0xA9,0x04,0x11,0x11,0xF8,0x05,0x00,0x00,0x1F,0x70,0x40,0x12,0x3F,0xE0,0x80,0x00,0x10,0x00,0x0F,0x90,
0xE1,0x41,0x63,0x42,0x52,0x40,0x92,0x00,0xAA,0x02,0x01,0xA9,0x04,0x11,0x11,0xF8,0x05,0x00,0x00,0x1F,
0x70,0x40,0x12,0x3F,0xE0,0x80,0x00,0x10,0x00,0x0F,0x90,0xE1,0x41,0x63,0x42,0x52,0x40,0x92,0x00,0xAA,
0x02,0x01,0xA9,0x04,0x11,0x11,0xF8,0x05,0x00,0x00,0x1F,0x70,0x40,0x12,0x3F,0xE0,0x80,0x00,0x10,0x00,
0x0F,0x90, 0xE1,0x41,0x63,0x42,0x52,0x40,0x92,0x00,0xAA,0x02,0x01};*/


char CashPurchase[] =
  { "000000000000000000000091020000003FFC083333333343101234567890123456"
"A9041111F80500001F7040123FE08000100.00F90E14163424092007AA0201"
"A9041111F80500001F7040123FE08000100.00F90E14163424092007AA0201"
"A9041111F80500001F7040123FE08000100.00F90E14163424092007AA0201"
"A9041111F80500001F7040123FE08000100.00F90E14163424092007AA0201"
"A9041111F80500001F7040123FE08000100.00F90E14163424092007AA0201"
};

struct personalization
{
  unsigned char term_id[9];
  unsigned char Mer_id[9];
  float discount;
  unsigned char phno[14];
  unsigned char Mer_code[9];
  unsigned char date[8];
  unsigned char pw[6];
  unsigned char ServerIP[17];
  unsigned char Hdr[21];
  unsigned char FHdr[21];	//[47];
  unsigned char Hdr2[21];
  unsigned char FHdr2[21];
  unsigned char GPRSDomain[30];
} termPer;
struct personalization1
{
  unsigned char term_id[9];
  unsigned char Mer_id[9];
  float discount;
  unsigned char phno[14];
  unsigned char Mer_code[9];
  unsigned char date[8];
  unsigned char pw[6];
  unsigned char ServerIP[17];
  unsigned char Hdr[21];
  unsigned char FHdr[21];	//[47];
  unsigned char Hdr2[21];
  unsigned char FHdr2[21];
  unsigned char GPRSDomain[30];
} termPer1;
unsigned char *PersFile;
unsigned char *PersFile1;

int pppdial_gprs (char *dialno);
/***
int main()
{
  int i;
  FILE *out_fd;
  char buff[100]="";
   if (lk_open() < 0) 
  {
     printf("Port error\n");
     exit(1);
  }

  if (prn_open() < 0) 
  {
     printf("Prn error\n");
     lk_dispclr();	
     lk_disptext(2, 1, "Printer Opening Err", 0);	
     exit(1);
  }
	
/****
  if (mscr_open() < 0) 
  {
     printf("mcr error\n");
     lk_dispclr();	
     lk_disptext(2, 1, "MCR Opening Err", 0);	
     exit(1);
  }
*** /
   //ppp_options("12383989","13rtruy");
  lk_dispclr();	
  lk_disptext(2, 1, "GPRS testing", 0);	
   sleep(4);
   ppp_dial_update("*99***1#","airtelgprs.com");
   pppdial_gprs("*99***1#");
  while(1)
   {
     lk_dispclr();	
     lk_disptext(2, 1, "opening PPP", 0);
    if(!ppp_open())
       {
          lk_dispclr();	
          lk_disptext(2, 1, "Try again", 0);
          lk_disptext(3, 1, "Press any key", 0);
          lk_getkey();
        }
     else break;
    }  	
   //GET_mesg_clear("http:\\www.yahoo.com");
   memcpy(CashPurchase,shtml,22);
  while(1)
  {
   GET_file_clear(CashPurchase, Userfile);
   out_fd = fopen (Userfile, "rt");
   fread(buff,100,1,out_fd);
   fclose(out_fd);
   
   lk_dispclr();	
   lk_disptext(1, 1, buff, 0);
   lk_getkey();
   lk_dispclr();
   lk_disptext(3, 1, "Enter -send again", 0);
   lk_disptext(4, 1, "Canc - to cancel", 0);
   while(1)
   {
       i = lk_getkey();
             if ((i == ENTER) || (i == CANCEL))	   
             break;
   }	
   if (i == ENTER) continue;
   if (i == CANCEL) break;
   }
   ppp_close();
   lk_dispclr();	
   lk_disptext(2, 1, "Test over", 0);
   sleep(3);	
   exit(1);//while(1);

 }
****/
/*
int connect_To_GPRS()
{
   ppp_dial_update("*99***1#","airtelgprs.com");
   pppdial_gprs("*99***1#");
   if(!ppp_open()) return 0;

}
*/

int
connect_To_GPRS ()
{
  int fd, pos;

  printf ("Begun connect_To_GPRS");

  fd = open (PersFile, O_RDONLY, 0751);
  if (fd > 0)
    {
      pos = lseek (fd, 0, SEEK_SET);
      read (fd, &termPer, sizeof (termPer));
    }
  close (fd);

  //ppp_dial_update("*99***1#",termPer.GPRSDomain);
  ppp_dial_update ("*99***1#", getVal ("gprs_domain"));
  //ppp_dial_update("*99***1#","wapsouth.cellone.in");
  pppdial_gprs ("*99***1#");

  printf ("End of connect_To_GPRS");

  if (!ppp_open ())
    return 0;
  else
    return 1;

}

int
connect_To_GPRS1 ()
{
  int fd, pos;
  fd = open (PersFile1, O_RDONLY, 0751);
  if (fd > 0)
    {
      pos = lseek (fd, 0, SEEK_SET);
      read (fd, &termPer1, sizeof (termPer1));
    }
  close (fd);

  ppp_dial_update ("*99***1#", "internet");
  //ppp_dial_update("*99***1#","airtelgprs.com");
  //ppp_dial_update("*99***1#","wapsouth.cellone.in");
  pppdial_gprs ("*99***1#");
  if (!ppp_open ())
    return 0;
  else
    return 1;

}

int
Disconnect_GPRS ()
{
  ppp_close ();

}

int
pppdial_gprs (char *dialno)
{
  char str[1000];
  FILE *rfp;

  sprintf (str,
	   "#!/bin/sh\nTELEPHONE=\"%s\" \nexport TELEPHONE \nrm -f /var/run/ppp.link \nexec /usr/sbin/pppd    \\\n        debug /dev/ttyS0  \\\n        9600 0.0.0.0:0.0.0.0  \\\n        connect /etc/ppp/ppp-on-dialer",
	   dialno);

  rfp = fopen ("/etc/ppp/dialout", "w");
  if (rfp < 0)
    return -1;

  fprintf (rfp, "%s", str);
  fclose (rfp);
  return 0;
}

int
ppp_options (char *userid, char *password)
{
  char str[1000];
  FILE *rfp;

  sprintf (str,
	   "-detach\nlock\nasyncmap 0\ncrtscts\ndefaultroute\nmodem\nmru 552\nmtu 552\nname %s\npassword %s",
	   userid, password);

  rfp = fopen ("/etc/ppp/options", "w");
  if (rfp < 0)
    return -1;

  fprintf (rfp, "%s", str);
  fclose (rfp);
  return 0;
}

int
ppp_dial_update (char *dialno, char *gprsdomain)
{
  char str[1000];
  FILE *rfp;


  sprintf (str,
	   "#!/bin/sh\n/usr/sbin/chat -v      \\\n        TIMEOUT         3    \\\n        ABORT           \'\\nBUSY\\r\'      \\\n        ABORT           \'\\nNO ANSWER\\r\' \\\n        ABORT          \'\\nRINGING\\r\\n\\r\\nRINGING\\r\'    \\\n        ECHO            ON      \\\n        \'\'               AT     \\\n        \'OK-+++\\C-OK'   ATH0    \\\n        \'\'              AT+CSQ      \\\n        OK              AT+CGATT=1 \\\n        OK              \'AT+CGDCONT=1,\"IP\",\"%s\"\' \\\n       TIMEOUT         15      \\\n        OK              ATDT\'%s\'  \\\n        CONNECT         \'\'",
	   gprsdomain, dialno);

  rfp = fopen ("/etc/ppp/ppp-on-dialer", "w");
  if (rfp < 0)
    {
      lk_dispclr ();
      lk_disptext (2, 1, "ppp-on-dialer  ", 0);
      lk_disptext (3, 1, "opening error  ", 0);
      sleep (3);
      return 0;
    }

  printf ("str = %s\n", str);

  fprintf (rfp, "%s", str);
  fclose (rfp);
  return 0;
}

int
ppp_open ()
{
  int fd9, i;
  unsigned char buffer[30];
  //err_printf ("PPP Connecting\n");
  printf ("PPP Connecting\n");
  system ("rm -f /var/run/ppp.link");
  system ("/etc/ppp/dialout&");
  sleep (3);
  //sleep (5);  commented by cvp on 28/09/07
  i = 0;
  while (i < 10)		//old val 35
    {
      fd9 = open ("/var/run/ppp.link", O_RDONLY);
      //err_printf ("%d\n", fd9);
      printf ("%d\n", fd9);
      if (fd9 > 0)
	{
	  close (fd9);
	  break;
	}
      sleep (1);
      i++;
    }
  if (i >= 10)			//old val 35
    {
      /*lk_dispclr();
         lk_disptext(1,2,"PPP Connection failed",0);
         sleep(3); */
      ppp_close ();
      return 0;
    }
  // gett_ip(buffer);
  //ppp_close();
  /* lk_dispclr();
     lk_disptext(1,2,"PPP SUCCEESS ",0);
     //lk_disptext(3,0,"WITH IP ",0);
     // lk_disptext(3,8,buffer,0);
     sleep(3);
     // err_printf ("PPP Connected \n");

     lk_disptext(2,2,"PPP Connected ",0);
     printf ("PPP Connected \n"); */
  return 1;
}

//returns 1 if ppp instantiated successfully else return 0
int
ppp_close ()
{
  char p_id[8];
  char str[50] = "";
  int fd9;
  fd9 = open ("/var/run/ppp0.pid", O_RDONLY);

  if (fd9)
    {
      read (fd9, p_id, 4);
      p_id[4] = '\0';
      sprintf (str, "kill %s", p_id);
      printf ("\n%s\n", str);
      system (str);
      close (fd9);
      //system("rm /var/run/ppp0.pid");
    }

  //err_printf ("PPP Closed. \n");
  printf ("PPP Closed. \n");
  sleep (2);
  return 1;
}

int
GET_mesg_clear (char *http_url)
{
  CURL *curl;
  CURLcode res;
  int result = 0;

  curl = curl_easy_init ();

  if (curl)
    {




      curl_easy_setopt (curl, CURLOPT_URL, http_url);
      curl_easy_setopt (curl, CURLOPT_FAILONERROR, 1);
      curl_easy_setopt (curl, CURLOPT_CONNECTTIMEOUT, 5);
      curl_easy_setopt (curl, CURLOPT_TIMEOUT, 5);
      curl_easy_setopt (curl, CURLOPT_NOSIGNAL, 1);

      //err_printf("Posting message...\n");
      printf ("Posting message...\n");
      lk_dispclr ();
      lk_disptext (1, 2, "Posting message... ", 0);
      res = curl_easy_perform (curl);	//post away!
      if (res == 0)
	{
	  lk_dispclr ();
	  lk_disptext (1, 2, "message posted succ", 0);
	  sleep (3);
	  result = 1;
	}
      else
	{
	  //message was not successfully posted to server
	  lk_dispclr ();
	  lk_disptext (1, 2, "message post failed", 0);
	  sleep (3);
	  result = 0;
	}
    }

  //there must be a corresponding curl_easy_cleanup() to curl_easy_init()
  curl_easy_cleanup (curl);

  return result;
}

int
GET_mesg_ssl (char *https_url)
{
  CURL *curl;
  CURLcode res;
  int result = 0;

  curl = curl_easy_init ();

  if (curl)
    {
      curl_easy_setopt (curl, CURLOPT_URL, https_url);

      /*
       * If you want to connect to a site who isn't using a certificate that is
       * signed by one of the certs in the CA bundle you have, you can skip the
       * verification of the server's certificate. This makes the connection
       * A LOT LESS SECURE.
       *
       * If you have a CA cert for the server stored someplace else than in the
       * default bundle, then the CURLOPT_CAPATH option might come handy for
       * you.
       */
      curl_easy_setopt (curl, CURLOPT_SSL_VERIFYPEER, 0);

      /*
       * If the site you're connecting to uses a different host name that what
       * they have mentioned in their server certificate's commonName (or
       * subjectAltName) fields, libcurl will refuse to connect. You can skip
       * this check, but this will make the connection less secure.
       */
      curl_easy_setopt (curl, CURLOPT_SSL_VERIFYHOST, 0);

      //err_printf("Posting message...\n");
      printf ("Posting message...\n");
      res = curl_easy_perform (curl);	//post away!
      if (res == 0)
	result = 1;
      else
	//message was not successfully posted to server
	result = 0;
    }

  //there must be a corresponding curl_easy_cleanup() to curl_easy_init()
  curl_easy_cleanup (curl);

  return result;

}

//the http_url could also be a file!
int
GET_file_clear (char *http_url, char *filename, char *data)
{
  CURL *curl;
  CURLcode res;
  FILE *out_fd = (FILE *) 0;
  int result = 0;
  int fd;
  memset (Errfile, 0, sizeof (Errfile));
  curl = curl_easy_init ();
  if (curl)
    {
      //err_printf("Downloading %s file...\n", filename);
      printf ("Downloading %s file...\n", filename);
//              lk_dispclr();
//              lk_disptext(2,0,"Downloading... ",1);
      out_fd = fopen (filename, "wrt");	//open for read and write      
      curl_easy_setopt (curl, CURLOPT_FILE, out_fd);
      // curl_easy_setopt(curl, CURLOPT_FILE, fd);
      //curl_easy_setopt(curl, CURLOPT_HEADER, 0);
      curl_easy_setopt (curl, CURLOPT_CONNECTTIMEOUT, 120);	//old 70
      curl_easy_setopt (curl, CURLOPT_TIMEOUT, 1000);	// old 50
      if (strcmp (data, "") != 0)
	curl_easy_setopt (curl, CURLOPT_POSTFIELDS, data);
      curl_easy_setopt (curl, CURLOPT_URL, http_url);
      printf ("Setting SKIP_PEER_VERIFICATION");
      curl_easy_setopt (curl, CURLOPT_SSL_VERIFYPEER, 0L);
      printf ("Setting SKIP_HOSTNAME_VERIFICATION");
      curl_easy_setopt (curl, CURLOPT_SSL_VERIFYHOST, 0L);
      curl_easy_setopt (curl, CURLOPT_USERPWD, "Pilot:Pilot123");
      curl_easy_setopt (curl, CURLOPT_ERRORBUFFER, Errfile);

      res = curl_easy_perform (curl);	//post away!
      //printf("the result is %d\n",res);


      if (res == 0)
	result = 1;
      else
	{
	  //message was not successfully posted to server
	  //printf("the Error is %s\n",Errfile);   

	  //                       lk_dispclr();
	  //                     lk_disptext(2,2,Errfile,0); 
	  sleep (3);
	  result = 0;

	}
      fclose (out_fd);
      //   close(fd);
    }
  //there must be a corresponding curl_easy_cleanup() to curl_easy_init()
  curl_easy_cleanup (curl);

  return result;
}

//the https_url could also be a file!
int
GET_file_ssl (char *https_url, char *filename)
{
  CURL *curl;
  CURLcode res;
  FILE *out_fd = (FILE *) 0;
  int result = 0;

  curl = curl_easy_init ();
  if (curl)
    {
      //err_printf("Downloading %s file...\n", filename);
      printf ("Downloading %s file...\n", filename);
      out_fd = fopen (filename, "w");	//open for read and write                        
      curl_easy_setopt (curl, CURLOPT_FILE, out_fd);
      curl_easy_setopt (curl, CURLOPT_HEADER, 0);
      curl_easy_setopt (curl, CURLOPT_URL, https_url);
      curl_easy_setopt (curl, CURLOPT_SSL_VERIFYPEER, 0);
      curl_easy_setopt (curl, CURLOPT_SSL_VERIFYHOST, 0);

      res = curl_easy_perform (curl);	//post away!
      if (res == 0)
	result = 1;
      else
	//message was not successfully posted to server
	result = 0;
      fclose (out_fd);
    }
  //there must be a corresponding curl_easy_cleanup() to curl_easy_init()
  curl_easy_cleanup (curl);

  return result;
}
