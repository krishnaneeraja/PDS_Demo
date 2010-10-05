#include "header.h"

#include "curl.h"
#include "types.h"
#include "easy.h"

extern FILE *fp;
int upload (char *uploadFileName, char *url, char *date,
	    char *responseFileName);

#ifdef MAIN
int
main (int argc, char **argv)
{
  read_config ("./config.txt");
  upload (argv[1], argv[2], argv[3], argv[4]);
}
#endif

int
upload (char *uploadFileName, char *url, char *action, char *responseFileName)
{
  CURL *curl;
  CURLcode res;
  FILE *out_fd = (FILE *) 0;
  int fd;

  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  struct curl_slist *headerlist = NULL;
  static const char buf[] = "Expect:";
  char header[100];
  char response[1000];

  curl_global_init (CURL_GLOBAL_ALL);

  printf ("This is the file %s being uploaded to %s for action: %s\n",
	  uploadFileName, url, action);
  printf ("Response file: %s\n", responseFileName);

  printf ("Server user: %s, pwd=%s\n", getVal ("server_user"),
	  getVal ("server_pwd"));

  /* Fill in the file upload field */
  curl_formadd (&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "FileUpload1",
		CURLFORM_FILE, uploadFileName, CURLFORM_END);

  /* Fill in the filename field */
  curl_formadd (&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "userid",
		CURLFORM_COPYCONTENTS, getVal ("server_user"), CURLFORM_END);

  /* Fill in the filename field */
  curl_formadd (&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "password",
		CURLFORM_COPYCONTENTS, getVal ("server_pwd"), CURLFORM_END);

  /* Fill in the submit field too, even if this is rarely needed */
  curl_formadd (&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "BtnUpload",
		CURLFORM_COPYCONTENTS, "Upload", CURLFORM_END);

  curl = curl_easy_init ();
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */
  headerlist = curl_slist_append (headerlist, buf);
  
  sprintf(header,"Content-Type: text/xml; charset=utf-8");  
  headerlist = curl_slist_append(headerlist, header); 
  
  sprintf(header,"SOAPAction:");
  headerlist = curl_slist_append(headerlist, header); 
  
  if (curl)
    {
      /* what URL that receives this POST */
      curl_easy_setopt (curl, CURLOPT_URL, url);
      out_fd = fopen (responseFileName, "wrt");	//open for read and write  
      curl_easy_setopt (curl, CURLOPT_FILE, out_fd);

//    if ( (argc == 2) && (!strcmp(argv[1], "noexpectheader")) )
      /* only disable 100-continue header if explicitly requested */
      curl_easy_setopt (curl, CURLOPT_HTTPHEADER, headerlist);
      curl_easy_setopt (curl, CURLOPT_HTTPPOST, formpost);
      res = curl_easy_perform (curl);

      fclose (out_fd);

      /* always cleanup */
      curl_easy_cleanup (curl);

      /* then cleanup the formpost chain */
      curl_formfree (formpost);
      /* free slist */
      curl_slist_free_all (headerlist);
    }
  return 0;
}
