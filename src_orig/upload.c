/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: postit2.c,v 1.5 2007-07-12 21:11:10 danf Exp $
 *
 * Example code that uploads a file name 'foo' to a remote script that accepts
 * "HTML form based" (as described in RFC1738) uploads using HTTP POST.
 *
 * The imaginary form we'll fill in looks like:
 *
 * <form method="post" enctype="multipart/form-data" action="examplepost.cgi">
 * Enter file: <input type="file" name="sendfile" size="40">
 * Enter file name: <input type="text" name="filename" size="30">
 * <input type="submit" value="send" name="submit">
 * </form>
 *
 * This exact source code has not been verified to work.
 */

#include <stdio.h>
#include <string.h>

#include "curl.h"
#include "types.h"
#include "easy.h"

int
main (int argc, char *argv[])
{
  CURL *curl;
  CURLcode res;

  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  struct curl_slist *headerlist = NULL;
  static const char buf[] = "Expect:";

  curl_global_init (CURL_GLOBAL_ALL);

  printf ("This is the file: %s\n", argv[1]);

  /* Fill in the file upload field */
  curl_formadd (&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "FileUpload1",
		CURLFORM_FILE, argv[1], CURLFORM_END);

  /* Fill in the filename field */
  curl_formadd (&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "userid",
		CURLFORM_COPYCONTENTS, "3", CURLFORM_END);

  /* Fill in the filename field */
  curl_formadd (&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "password",
		CURLFORM_COPYCONTENTS, "padma", CURLFORM_END);

  /* Fill in the filename field */
  curl_formadd (&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "date",
		CURLFORM_COPYCONTENTS, "20100117", CURLFORM_END);


  /* Fill in the submit field too, even if this is rarely needed */
  curl_formadd (&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "BtnUpload",
		CURLFORM_COPYCONTENTS, "Upload", CURLFORM_END);

  curl = curl_easy_init ();
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */
  headerlist = curl_slist_append (headerlist, buf);
  if (curl)
    {
      /* what URL that receives this POST */
      curl_easy_setopt (curl, CURLOPT_URL,
			"http://192.168.1.25/AMS/uploadfile1.aspx");
      curl_easy_setopt (curl, CURLOPT_HTTPHEADER, headerlist);
      curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L);
      curl_easy_setopt (curl, CURLOPT_POST, 1L);
      curl_easy_setopt (curl, CURLOPT_CONNECTTIMEOUT, 8);
      curl_easy_setopt (curl, CURLOPT_HTTPPOST, formpost);
      res = curl_easy_perform (curl);

      /* always cleanup */
      curl_easy_cleanup (curl);

      /* then cleanup the formpost chain */
      curl_formfree (formpost);
      /* free slist */
      curl_slist_free_all (headerlist);
    }
  return 0;
}
