/*
 * get_http.c-v0.9:
 *   get_http is a simple libcurl-based http client that will display headers
 *   and/or body from a web server.
 *
 * Requirements:
 *   libcurl - http://curl.haxx.se/
 *
 * Compiling:
 *   gcc -Wall -O2 `curl-config --cflags` get_http.c -o get_http `curl-config --libs`
 *     
 *
 * Usage:
 *   see get_http -h
 *
 *   # displays headers only, follows redirects
 *   get_http -Hf http://www.gozer.org/
 *
 *   # execute a request with a specific user-agent id or string (see help)
 *   get_http -a 0 msn.com
 *   get_http -a "Dancin' Jackson 2.0!" https://have.bigger.for-you.com
 *
 *   # run a request on a protected site
 *   get_http -u user:pass http://staff.example.com/
 *
 * (c) 2003-2006 Mike Hokenson <logan at dct dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <regex.h>

#define _VERSION "0.9"
#define _HOME    "http://www.gozer.org/programs/c/"

#ifndef isdigit
#define isdigit(c) (((c) >= '0') && ((c) <= '9'))
#endif

#define _N_ELEMENTS(s) (sizeof(s) / sizeof((s)[0]))

#define NUMBER "0123456789"
#define UNUSED(x) x = x


typedef struct {
  const char *key;
  const char *val;
} UserAgent;

size_t header_callback(char *ptr, size_t msize, size_t nmemb, void *stream);
void print(const char *s, size_t len);
void lookup(const char *key);

  bool html = false;
  bool start;
void header_accept_ranges(const char *s);
void header_age(const char *s);
void header_allow(const char *s);
void header_cache_control(const char *s);
void header_cache_control_callback(const char *s, regmatch_t pmatch[]);
void header_connection(const char *s);
void header_content_encoding(const char *s);
void header_content_encoding_callback(const char *s, regmatch_t pmatch[]);
void header_content_language(const char *s);
void header_content_length(const char *s);
void header_content_disposition(const char *s);
void header_content_location(const char *s);
void header_content_md5(const char *s);
void header_content_range(const char *s);
void header_content_type(const char *s);
void header_date(const char *s);
void header_etag(const char *s);
void header_expires(const char *s);
void header_last_modified(const char *s);
void header_location(const char *s);
void header_pragma(const char *s);
void header_retry_after(const char *s);
void header_server(const char *s);
void header_trailer(const char *s);
void header_transfer_encoding(const char *s);
void header_transfer_encoding_callback(const char *s, regmatch_t pmatch[]);
void header_upgrade(const char *s);
void header_vary(const char *s);
void header_via(const char *s);
void header_set_cookie(const char *s);

struct header_entry {
  char name[40];
  void (*handler)(const char *s);
  int count;
  char *missing;
} header_table[] = {
  { "Accept-Ranges", header_accept_ranges, 0, 0 },
  { "Age", header_age, 0, 0 },
  { "Allow", header_allow, 0, 0 },
  { "Cache-Control", header_cache_control, 0, 0 },
  { "Connection", header_connection, 0, 0 },
  { "Content-Encoding", header_content_encoding, 0, 0 },
  { "Content-Language", header_content_language, 0, "missingcontlang" },
  { "Content-Disposition", header_content_disposition, 0, 0 },
  { "Content-Length", header_content_length, 0, 0 },
  { "Content-Location", header_content_location, 0, 0 },
  { "Content-MD5", header_content_md5, 0, 0 },
  { "Content-Range", header_content_range, 0, 0 },
  { "Content-Type", header_content_type, 0, "missingcontenttype" },
  { "Date", header_date, 0, "missingdate" },
  { "ETag", header_etag, 0, 0 },
  { "Expires", header_expires, 0, 0 },
  { "Last-Modified", header_last_modified, 0, "missinglastmod" },
  { "Location", header_location, 0, 0 },
  { "Pragma", header_pragma, 0, 0 },
  { "Retry-After", header_retry_after, 0, 0 },
  { "Server", header_server, 0, 0 },
  { "Set-Cookie", header_set_cookie, 0, 0 },
  { "Trailer", header_trailer, 0, 0 },
  { "Transfer-Encoding", header_transfer_encoding, 0, 0 },
  { "Upgrade", header_upgrade, 0, 0 },
  { "Vary", header_vary, 0, 0 },
  { "Via", header_via, 0, 0 }
};

UserAgent useragent_map[] = {
  {
    "Internet Explorer 6.0 (Windows XP)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)"
  },
  {
    "Internet Explorer 5.1 (Mac OS)",
    "Mozilla/4.0 (compatible; MSIE 5.15; Mac_PowerPC)"
  },
  {
    "Mozilla Firefox 1.5.0.6 (Windows XP)",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.0.6) Gecko/20060728 Firefox/1.5.0.6"
  },
  {
    "Mozilla Firefox 1.5.0.6 (Linux)",
    "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.0.6) Gecko/20060728 Firefox/1.5.0.6"
  },
  {
    "Mozilla 1.7.13 (Windows XP)",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.13) Gecko/20060414",
  },
  {
    "Mozilla 1.7.13 (Linux)",
    "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.7.13) Gecko/20060417"
  },
  {
    "Opera 9.01 (Windows XP)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; en) Opera 9.01"
  },
  {
    "Opera 9.01 (Linux)",
    "Mozilla/4.0 (compatible; MSIE 6.0; X11; Linux i686; en) Opera 9.01"
  },
  {
    "Opera 8.52 (Windows XP)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; en) Opera 8.52"
  },
  {
    "Opera 8.52 (Linux)",
    "Mozilla/4.0 (compatible; MSIE 6.0; X11; Linux i686; en) Opera 8.52"
  },
  {
    "Safari 2.0.3 (Mac OS X 10.4.5)",
    "Mozilla/5.0 (Macintosh; U; PPC Mac OS X; en) AppleWebKit/418 (KHTML, like Gecko) Safari/417.9.3"
  },
  {
    "Safari 1.0 (Mac OS X)",
    "Mozilla/5.0 (Macintosh; U; PPC Mac OS X; en-us) AppleWebKit/85 (KHTML, like Gecko) Safari/85"
  },
  {
    "Konqueror 3.4 (Linux)",
    "Mozilla/5.0 (compatible; Konqueror/3.4; Linux) KHTML/3.4.0 (like Gecko)"
  },
  {
    "Netscape 4.8 (Windows XP)",
    "Mozilla/4.8 [en] (Windows NT 5.1; U)"
  },
  {
    "Netscape 4.8 (Linux)",
    "Mozilla/4.8 [en] (X11; U; Linux 2.6.17 i686)"
  },
  {
    "OmniWeb 4.2.1 (Mac OS)",
    "Mozilla/4.5 (compatible; OmniWeb/4.2.1-v435.9; Mac_PowerPC)"
  },
  {
    "WebTV",
    "Mozilla/4.0 WebTV/2.6 (compatible; MSIE 4.0)"
  },
  {
    "Googlebot",
    "Googlebot/2.1 (+http://www.googlebot.com/bot.html)"
  },
  {
    "MSNBot",
    "msnbot/0.1 (+http://search.msn.com/msnbot.htm)"
  },
  {
    NULL,
    NULL
  }
};


struct message_entry {
  const char key[20];
  const char *value;
} message_table[] = {
  { "1xx", "A response status code in the range 100 - 199 indicates a "
           "'provisional response'." },
  { "2xx", "A response status code in the range 200 - 299 indicates that "
           "the request was successful." },
  { "3xx", "A response status code in the range 300 - 399 indicates that "
           "the client should redirect to a new URL." },
  { "4xx", "A response status code in the range 400 - 499 indicates that "
           "the request could not be fulfilled due to client error." },
  { "5xx", "A response status code in the range 500 - 599 indicates that "
           "an error occurred on the server." },
  { "asctime", "Warning: This date is in the obsolete asctime() format. "
               "Consider using the RFC 1123 format instead." },
  { "badage", "Error: The Age header must be one number." },
  { "badallow", "Error: The Allow header must be a comma-separated list of "
                "HTTP methods." },
  { "badcachecont", "Error: The Cache-Control header must be a "
                    "comma-separated list of directives." },
  { "badconnection", "Warning: The only value of the Connection header "
                     "defined by HTTP/1.1 is \"close\"." },
  { "badcontenc", "Error: The Content-Encoding header must be a "
                  "comma-separated list of encodings." },
  { "badcontenttype", "Error: The Content-Type header must be of the form "
                      "'type/subtype (; optional parameters)'." },
  { "badcontlang", "Error: The Content-Language header must be a "
                   "comma-separated list of language tags." },
  { "badcontlen", "Error: The Content-Length header must be a number." },
  { "badcontloc", "Error: The Content-Location header must be an absolute "
                  "or relative URI." },
  { "badcontmd5", "Error: The Content-MD5 header must be a base64 encoded "
                  "MD5 sum." },
  { "baddate", "Error: Failed to parse this date. Dates should be in the RFC "
               "1123 format." },
  { "badetag", "Error: The ETag header must be a quoted string (optionally "
               "preceded by \"W/\" for a weak tag)." },
  { "badlocation", "Error: The Location header must be an absolute URI. "
                   "Relative URIs are not permitted." },
  { "badpragma", "Error: The Pragma header must be a comma-separated list of "
                 "directives." },
  { "badserver", "Error: The Server header must be a space-separated list of "
                 "products of the form Name/optional-version and comments "
                 "in ()." },
  { "badstatus", "Warning: The response status code is outside the standard "
                 "range 100 - 599." },
  { "badstatusline", "Error: Failed to parse the response Status-Line. The "
                     "status line must be of the form 'HTTP/n.n <3-digit "
                     "status> <reason phrase>'." },
  { "badtrailer", "Error: The Trailer header must be a comma-separated list "
                  "of header names." },
  { "badtransenc", "Error: The Transfer-Encoding header must be a "
                   "comma-separated of encodings." },
  { "badupgrade", "Error: The Upgrade header must be a comma-separated list "
                  "of product identifiers." },
  { "badvary", "Error: The Vary header must be a comma-separated list "
                  "of header names, or \"*\"." },
  { "contentrange", "Warning: The Content-Range header should not be returned "
                    "by the server for this request." },
  { "cookiebaddate", "Error: The expires date must be in the form "
                     "\"Wdy, DD-Mon-YYYY HH:MM:SS GMT\"." },
  { "cookiebadnameval", "Error: A Set-Cookie header must start with "
                        "name=value, each excluding semi-colon, comma and "
                        "white space." },
  { "cookiebadpath", "Error: The path does not start with \"/\"." },
  { "cookiepastdate", "Warning: The expires date is in the past. The cookie "
                      "will be deleted by browsers." },
  { "cookieunknownfield", "Warning: This is not a standard Set-Cookie "
                          "field." },
  { "endofheaders", "End of headers." },
  { "futurehttp", "Warning: I only understand HTTP/1.1. Check for a newer "
                  "version of this tool." },
  { "futurelastmod", "Error: The specified Last-Modified date-time is in "
                     "the future." },
  { "headertoolong", "Warning: Header too long: ignored." },
  { "missingcolon", "Error: Headers must be of the form 'Name: value'." },
  { "missingcontenttype", "Warning: No Content-Type header was present. The "
                          "client will have to guess the media type or ask "
                          "the user. Adding a Content-Type header is strongly "
                          "recommended." },
  { "missingcontlang", "Consider adding a Content-Language header if "
                       "applicable for this document." },
  { "missingdate", "Warning: No Date header was present. A Date header must "
                   "be present, unless the server does not have a clock, or "
                   "the response is 100, 101, or 500 - 599." },
  { "missinglastmod", "No Last-Modified header was present. The "
                      "HTTP/1.1 specification states that this header should "
                      "be sent whenever feasible." },
  { "nocharset", "Warning: No character set is specified in the Content-Type. "
                 "Clients may assume the default of ISO-8859-1. Consider "
                 "appending '; charset=...'." },
  { "nonstandard", "Warning: I don't know anything about this header. Is it "
                   "a standard HTTP response header?" },
  { "notcrlf", "Error: This header line does not end in CR LF. HTTP requires "
               "that all header lines end with CR LF." },
  { "ok", "OK." },
  { "oldhttp", "Warning: This version of HTTP is obsolete. Consider upgrading "
               "to HTTP/1.1." },
  { "rfc1036", "Warning: This date is in the obsolete RFC 1036 format. "
               "Consider using the RFC 1123 format instead." },
  { "ugly", "This URL appears to contain implementation-specific parts such "
            "as an extension or a query string. This may make the URL liable "
            "to change when the implementation is changed, resulting in "
            "broken links. Consider using URL rewriting or equivalent to "
            "implement a future-proof URL space. See "
            "http://www.w3.org/Provider/Style/URI for more information." },
  { "unknowncachecont", "Warning: This Cache-Control directive is "
                        "non-standard and will have limited support." },
  { "unknowncontenc", "Warning: This is not a standard Content-Encoding." },
  { "unknownrange", "Warning: This range unit is not a standard HTTP/1.1 "
                    "range." },
  { "unknowntransenc", "Warning: This is not a standard Transfer-Encoding." },
  { "via", "This header was added by a proxy, cache or gateway." },
  { "wrongdate", "Warning: The server date-time differs from this system's "
                 "date-time by more than 10 seconds. Check that both the "
                 "system clocks are correct." },
  { "xheader", "This is an extension header. I don't know how to check it." }
};

void check_header(const char *name, const char *value);
void check_status_line(const char *s);
const char *skip_lws(const char *s);

void show_help(void)
{
  int i;

  fprintf(stderr, "Usage: get_http [OPTS] [URL1 URL2 ...]\n");

  fprintf(stderr, "  OPTS:\n");
  fprintf(stderr, "    -a [id/string]  set user-agent by id or string\n");
  fprintf(stderr, "    -b              display response body only\n");
  fprintf(stderr, "    -f              follow Location: redirects\n");
  fprintf(stderr, "    -h              display help\n");
  fprintf(stderr, "    -H              display response headers only\n");
  fprintf(stderr, "    -r [referrer]   send HTTP referrer\n");
  fprintf(stderr, "    -t [timeout]    set overall connection timeout\n");
  fprintf(stderr, "    -T              enable transaction timer\n");
  fprintf(stderr, "    -u [user:pass]  set authentication credentials\n");
  fprintf(stderr, "    -v              make verbose (libcurl)\n");
  fprintf(stderr, "    -V              display version\n\n");

  fprintf(stderr, "  Builtin agents:\n");

  for (i = 0; useragent_map[i].key; i++)
    fprintf(stderr, "    %2d: %s\n", i, useragent_map[i].key);
}

int str_is_digit(const char *str, int *val)
{
  const char *p;

  if (!str)
      return 0;

  for (p = str; *p; p++)
    if (!isdigit(*p))
      return 0;

  if (val)
    *val = atoi(str);

  return 1;
}

int main(int argc, char **argv)
{
  int option, timer = 0;
  
  CURL *curl;

  char error[CURL_ERROR_SIZE];

  if (!(curl = curl_easy_init())) {
      fprintf(stderr, "Unable to initialize curl\n");
      exit(1);
  }

  curl_easy_setopt(curl, CURLOPT_HEADER, 1);
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
  curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, 1);

  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);

  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);

  while ((option = getopt(argc, argv, "a:bfhHr:t:Tu:vV")) != EOF) {
    switch (option) {
      case 'a':
        {
          const char *agent = NULL;

          int id;

          if (str_is_digit(optarg, &id)) {
            if (id >= 0 && id < _N_ELEMENTS(useragent_map))
              agent = useragent_map[id].val;
          } else
            agent = optarg;

          if (agent)
            curl_easy_setopt(curl, CURLOPT_USERAGENT, agent);
        }

        break;
      case 'b':
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
        break;
      case 'f':
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        break;
      case 'h':
        show_help();
        exit(0);
      case 'H':
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        break;
      case 'r':
        curl_easy_setopt(curl, CURLOPT_REFERER, optarg);
        break;
      case 't':
        {
          int timeout;

          if (!str_is_digit(optarg, &timeout)) {
            show_help();
            exit(1);
          }

          curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        }
        break;
      case 'T':
        timer = 1;
        break;
      case 'u':
        curl_easy_setopt(curl, CURLOPT_USERPWD, optarg);
        break;
      case 'v':
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        break;
      case 'V':
        fprintf(stderr, "get_http/%s (%s)\n", _VERSION, _HOME);
        exit(0);
      default:
        show_help();
        exit(1);
    }
  }

  if (optind >= argc) {
    show_help();
    exit(1);
  }

  while (optind < argc) {
    const char *uri = argv[optind++];

    error[0] = '\0';

    curl_easy_setopt(curl, CURLOPT_URL, uri);

    if (curl_easy_perform(curl) == CURLE_OK) {
      if (timer) {
        double s;

        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &s);

        fprintf(stdout, "Time: %0.3fs\n\n", s);
      }
    } else
      fprintf(stderr, "%s: %s\n\n", uri, (error[0]) ? error : "Unknown Error");
  }

  curl_easy_cleanup(curl);

  exit(0);
}

/**
 * Callback for received header data.
 */
size_t header_callback(char *ptr, size_t msize, size_t nmemb, void *stream)
{
  const size_t size = msize * nmemb;
  char s[400], *name, *value;

  UNUSED(stream);

  printf(html ? "<li><code>" : "* ");
  print(ptr, size);
  printf(html ? "</code><ul>" : "\n");

  if (size < 2 || ptr[size - 2] != 13 || ptr[size - 1] != 10) {
    lookup("notcrlf");
    if (html)
      printf("</ul></li>\n");
    return size;
  }
  if (sizeof s <= size) {
    lookup("headertoolong");
    if (html)
      printf("</ul></li>\n");
    return size;
  }
  strncpy(s, ptr, size);
  s[size - 2] = 0;

  name = s;
  value = strchr(s, ':');

  if (s[0] == 0) {
    /* empty header indicates end of headers */
    lookup("endofheaders");
    if (html)
      printf("</ul></li>\n");
    return 0;

  } else if (start) {
    /* Status-Line [6.1] */
    check_status_line(s);
    start = false;

  } else if (!value) {
    lookup("missingcolon");

  } else {
    *value = 0;
    value++;

    check_header(name, skip_lws(value));
  }

  if (html)
    printf("</ul></li>\n");
  return size;
}

/**
 * Print a string which contains control characters.
 */
void print(const char *s, size_t len)
{
  size_t i;
  for (i = 0; i != len; i++) {
    if (html && s[i] == '<')
      printf("&lt;");
    else if (html && s[i] == '>')
      printf("&gt;");
    else if (html && s[i] == '&')
      printf("&amp;");
    else if (31 < s[i] && s[i] < 127)
      putchar(s[i]);
    else {
      if (html)
        printf("<span class='cc'>");
      printf("[%.2x]", s[i]);
      if (html)
        printf("</span>");
    }
  }
}


/**
 * Look up and output the string referenced by a key.
 */
void lookup(const char *key)
{
  const char *s, *spc;
  int x;
  struct message_entry *message;

  message = bsearch(key, message_table,
      sizeof message_table / sizeof message_table[0],
      sizeof message_table[0],
      (int (*)(const void *, const void *)) strcasecmp);
  if (message)
    s = message->value;
  else
    s = key;

  if (html) {
    if (strncmp(s, "Warning:", 8) == 0)
      printf("<li class='warning'>");
    else if (strncmp(s, "Error:", 6) == 0)
      printf("<li class='error'>");
    else if (strncmp(s, "OK", 2) == 0)
      printf("<li class='ok'>");
    else
      printf("<li>");
    for (; *s; s++) {
      if (strncmp(s, "http://", 7) == 0) {
        spc = strchr(s, ' ');
        printf("<a href='%.*s'>%.*s</a>", spc - s, s, spc - s, s);
        s = spc;
      }
      switch (*s) {
        case '<': printf("&lt;"); break;
        case '>': printf("&gt;"); break;
        case '&': printf("&amp;"); break;
        default: printf("%c", *s); break;
      }
    }
    printf("</li>\n");

  } else {
    printf("    ");
    x = 4;
    while (*s) {
      spc = strchr(s, ' ');
      if (!spc)
        spc = s + strlen(s);
      if (75 < x + (spc - s)) {
        printf("\n    ");
        x = 4;
      }
      x += spc - s + 1;
      printf("%.*s ", spc - s, s);
      if (*spc)
        s = spc + 1;
      else
        s = spc;
    }
    printf("\n\n");
  }
}


/**
 * Skip optional LWS (linear white space) [2.2]
 */
const char *skip_lws(const char *s)
{
  if (s[0] == 13 && s[1] == 10 && (s[2] == ' ' || s[2] == '\t'))
    s += 2;
  while (*s == ' ' || *s == '\t')
    s++;
  return s;
}

/**
 * Check the syntax and content of a header.
 */
void check_header(const char *name, const char *value)
{
  struct header_entry *header;

  header = bsearch(name, header_table,
      sizeof header_table / sizeof header_table[0],
      sizeof header_table[0],
      (int (*)(const void *, const void *)) strcasecmp);

  if (header) {
    header->count++;
    header->handler(value);
  } else if ((name[0] == 'X' || name[0] == 'x') && name[1] == '-') {
    lookup("xheader");
  } else {
    lookup("nonstandard");
  }
}



/* Header-specific validation. */
void header_accept_ranges(const char *s)
{
  if (strcmp(s, "bytes") == 0)
    lookup("ok");
  else if (strcmp(s, "none") == 0)
    lookup("ok");
  else
    lookup("unknownrange");
}

void header_age(const char *s)
{
  if (s[0] == 0 || strspn(s, NUMBER) != strlen(s))
    lookup("badage");
  else
    lookup("ok");
}

void header_allow(const char *s)
{
  if (parse_list(s, &re_token, 0, UINT_MAX, 0))
    lookup("ok");
  else
    lookup("badallow");
}

void header_cache_control(const char *s)
{
  if (parse_list(s, &re_token_value, 1, UINT_MAX,
      header_cache_control_callback))
    lookup("ok");
  else
    lookup("badcachecont");
}

char cache_control_list[][20] = {
  "max-age", "max-stale", "min-fresh", "must-revalidate",
  "no-cache", "no-store", "no-transform", "only-if-cached",
  "private", "proxy-revalidate", "public", "s-maxage"
};

void header_cache_control_callback(const char *s, regmatch_t pmatch[])
{
  size_t len = pmatch[1].rm_eo - pmatch[1].rm_so;
  char name[20];
  char *dir;

  if (19 < len) {
    lookup("unknowncachecont");
    return;
  }

  strncpy(name, s + pmatch[1].rm_so, len);
  name[len] = 0;

  dir = bsearch(name, cache_control_list,
      sizeof cache_control_list / sizeof cache_control_list[0],
      sizeof cache_control_list[0],
      (int (*)(const void *, const void *)) strcasecmp);

  if (!dir) {
    if (html)
      printf("<li class='warning'>");
    printf("    Cache-Control directive '");
    print(name, strlen(name));
    printf("':\n");
    if (html)
      printf("</li>\n");
    lookup("unknowncachecont");
  }
}

void header_connection(const char *s)
{
  if (strcmp(s, "close") == 0)
    lookup("ok");
  else
    lookup("badconnection");
}

void header_content_encoding(const char *s)
{
  if (parse_list(s, &re_token, 1, UINT_MAX,
      header_content_encoding_callback))
    lookup("ok");
  else
    lookup("badcontenc");
}

char content_coding_list[][20] = {
  "compress", "deflate", "gzip", "identity"
};

void header_content_encoding_callback(const char *s, regmatch_t pmatch[])
{
  size_t len = pmatch[1].rm_eo - pmatch[1].rm_so;
  char name[20];
  char *dir;

  if (19 < len) {
    lookup("unknowncontenc");
    return;
  }

  strncpy(name, s + pmatch[1].rm_so, len);
  name[len] = 0;

  dir = bsearch(name, content_coding_list,
      sizeof content_coding_list / sizeof content_coding_list[0],
      sizeof content_coding_list[0],
      (int (*)(const void *, const void *)) strcasecmp);
  if (!dir) {
    if (html)
      printf("<li class='warning'>");
    printf("    Content-Encoding '%s':\n", name);
    if (html)
      printf("</li>\n");
    lookup("unknowncontenc");
  }
}

void header_content_language(const char *s)
{
  if (parse_list(s, &re_token, 1, UINT_MAX, 0))
    lookup("ok");
  else
    lookup("badcontlang");
}

void header_content_length(const char *s)
{
  if (s[0] == 0 || strspn(s, NUMBER) != strlen(s))
    lookup("badcontlen");
  else
    lookup("ok");
}

void header_content_disposition(const char *s)
{
    lookup("ok");
}


void header_content_location(const char *s)
{
  if (strchr(s, ' '))
    lookup("badcontloc");
  else
    lookup("ok");
}

void header_content_md5(const char *s)
{
  if (strlen(s) != 24)
    lookup("badcontmd5");
  else
    lookup("ok");
}

void header_content_range(const char *s)
{
  UNUSED(s);
  lookup("contentrange");
}

void header_content_type(const char *s)
{
  bool charset = false;
  char *type, *subtype;
  unsigned int i;
  int r;
  regmatch_t pmatch[30];

  r = regexec(&re_content_type, s, 30, pmatch, 0);
  if (r) {
    lookup("badcontenttype");
    return;
  }

  type = strndup(s + pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);
  subtype = strndup(s + pmatch[2].rm_so, pmatch[2].rm_eo - pmatch[2].rm_so);

  /* parameters */
  for (i = 3; i != 30 && pmatch[i].rm_so != -1; i += 3) {
    char *attrib, *value;

    attrib = strndup(s + pmatch[i + 1].rm_so,
        pmatch[i + 1].rm_eo - pmatch[i + 1].rm_so);
    value = strndup(s + pmatch[i + 2].rm_so,
        pmatch[i + 2].rm_eo - pmatch[i + 2].rm_so);

    if (strcasecmp(attrib, "charset") == 0)
      charset = true;
  }

  if (strcasecmp(type, "text") == 0 && !charset)
    lookup("nocharset");
  else
    lookup("ok");
}

void header_date(const char *s)
{
  double diff;
  time_t time0, time1;
  struct tm tm;

  time0 = time(0);
  if (!parse_date(s, &tm))
    return;
  time1 = mktime_from_utc(&tm);

  diff = difftime(time0, time1);
  if (10 < fabs(diff))
    lookup("wrongdate");
  else
    lookup("ok");
}

void header_etag(const char *s)
{
  int r;
  r = regexec(&re_etag, s, 0, 0, 0);
  if (r)
    lookup("badetag");
  else
    lookup("ok");
}

void header_expires(const char *s)
{
  struct tm tm;
  if (parse_date(s, &tm))
    lookup("ok");
}

void header_last_modified(const char *s)
{
  double diff;
  time_t time0, time1;
  struct tm tm;

  time0 = time(0);
  if (!parse_date(s, &tm))
    return;
  time1 = mktime_from_utc(&tm);

  diff = difftime(time1, time0);
  if (10 < diff)
    lookup("futurelastmod");
  else
    lookup("ok");
}

void header_location(const char *s)
{
  int r;
  r = regexec(&re_absolute_uri, s, 0, 0, 0);
  if (r)
    lookup("badlocation");
  else
    lookup("ok");
}

void header_pragma(const char *s)
{
  if (parse_list(s, &re_token_value, 1, UINT_MAX, 0))
    lookup("ok");
  else
    lookup("badpragma");
}

void header_retry_after(const char *s)
{
  struct tm tm;

  if (s[0] != 0 && strspn(s, NUMBER) == strlen(s)) {
    lookup("ok");
    return;
  }

  if (!parse_date(s, &tm))
    return;

  lookup("ok");
}

void header_server(const char *s)
{
  int r;
  r = regexec(&re_server, s, 0, 0, 0);
  if (r)
    lookup("badserver");
  else
    lookup("ok");
}

void header_trailer(const char *s)
{
  if (parse_list(s, &re_token, 1, UINT_MAX, 0))
    lookup("ok");
  else
    lookup("badtrailer");
}

void header_transfer_encoding(const char *s)
{
  if (parse_list(s, &re_transfer_coding, 1, UINT_MAX,
      header_transfer_encoding_callback))
    lookup("ok");
  else
    lookup("badtransenc");
}

char transfer_coding_list[][20] = {
  "chunked", "compress", "deflate", "gzip", "identity"
};

void header_transfer_encoding_callback(const char *s, regmatch_t pmatch[])
{
  size_t len = pmatch[1].rm_eo - pmatch[1].rm_so;
  char name[20];
  char *dir;

  if (19 < len) {
    lookup("unknowntransenc");
    return;
  }

  strncpy(name, s + pmatch[1].rm_so, len);
  name[len] = 0;

  dir = bsearch(name, transfer_coding_list,
      sizeof transfer_coding_list / sizeof transfer_coding_list[0],
      sizeof transfer_coding_list[0],
      (int (*)(const void *, const void *)) strcasecmp);
  if (!dir) {
    if (html)
      printf("<li class='warning'>");
    printf("    Transfer-Encoding '%s':\n", name);
    if (html)
      printf("</li>\n");
    lookup("unknowntransenc");
  }
}

void header_upgrade(const char *s)
{
  int r;
  r = regexec(&re_upgrade, s, 0, 0, 0);
  if (r)
    lookup("badupgrade");
  else
    lookup("ok");
}

void header_vary(const char *s)
{
  if (strcmp(s, "*") == 0 || parse_list(s, &re_token, 1, UINT_MAX, 0))
    lookup("ok");
  else
    lookup("badvary");
}

void header_via(const char *s)
{
  UNUSED(s);
  lookup("via");
}

/* http://wp.netscape.com/newsref/std/cookie_spec.html */
void header_set_cookie(const char *s)
{
  bool ok = true;
  int r;
  const char *semi = strchr(s, ';');
  const char *s2;
  struct tm tm;
  double diff;
  time_t time0, time1;
  regmatch_t pmatch[20];

  if (semi)
    s2 = strndup(s, semi - s);
  else
    s2 = s;

  r = regexec(&re_cookie_nameval, s2, 0, 0, 0);
  if (r) {
    lookup("cookiebadnameval");
    ok = false;
  }

  if (!semi)
    return;

  s = skip_lws(semi + 1);

  while (*s) {
    semi = strchr(s, ';');
    if (semi)
      s2 = strndup(s, semi - s);
    else
      s2 = s;

    if (strncasecmp(s2, "expires=", 8) == 0) {
      s2 += 8;
      r = regexec(&re_cookie_expires, s2, 20, pmatch, 0);
      if (r == 0) {
        tm.tm_mday = atoi(s2 + pmatch[2].rm_so);
        tm.tm_mon = month(s2 + pmatch[3].rm_so);
        tm.tm_year = atoi(s2 + pmatch[4].rm_so) - 1900;
        tm.tm_hour = atoi(s2 + pmatch[5].rm_so);
        tm.tm_min = atoi(s2 + pmatch[6].rm_so);
        tm.tm_sec = atoi(s2 + pmatch[7].rm_so);

        time0 = time(0);
        time1 = mktime_from_utc(&tm);

        diff = difftime(time0, time1);
        if (10 < diff) {
          lookup("cookiepastdate");
          ok = false;
        }
      } else {
        lookup("cookiebaddate");
        ok = false;
      }
    } else if (strncasecmp(s2, "domain=", 7) == 0) {
    } else if (strncasecmp(s2, "path=", 5) == 0) {
      if (s2[5] != '/') {
        lookup("cookiebadpath");
        ok = false;
      }
    } else if (strcasecmp(s, "secure") == 0) {
    } else {
      if (html)
        printf("<li class='warning'>");
      printf("    Set-Cookie field '%s':\n", s2);
      if (html)
        printf("</li>\n");
      lookup("cookieunknownfield");
      ok = false;
    }

    if (semi)
      s = skip_lws(semi + 1);
    else
      break;
  }

  if (ok)
    lookup("ok");
}
