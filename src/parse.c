#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTOKENS       256
#define MAXLINE         1024	/* fgets buff */
#define MINLEN          3	/* skip lines shorter as */

char **split (char *string, char *delim);
int (*doWork) (char **tokens) = NULL;

#ifdef MAIN
int myWork (char **tokens);
int hisWork (char **tokens);

int
main (int argc, char **argv)
{
  printf ("Calling parseCSV\n");
  parseCSV ("download_20100108.csv", &hisWork);
  parseCSV ("download_20100108.csv", &myWork);
}
#endif

int
parseCSV (char *filename, int (*doWork) (char **))
{
//      char *delim = ".,:;`'\"+-_(){}[]<>*&^%$#@!?~/|\\= \t\r\n1234567890";
  char *delim = ",\r\n";
  char **tokens = NULL;
  char line[MAXLINE];
  int i = 0, lcount = 0;
  FILE *file = fopen (filename, "r");

  while (fgets (line, MAXLINE, file) != NULL)
    {
      printf ("This is the line:%s\n", line);

      lcount++;

      if (strlen (line) < MINLEN)
	continue;

      printf ("== line: %d\n", lcount);

      tokens = split (line, delim);

      (*doWork) (tokens);

      for (i = 0; tokens[i] != NULL; i++)
	free (tokens[i]);

      free (tokens);
    }
  return 0;
}

#ifdef MAIN
int
myWork (char **tokens)
{
  int i;
  for (i = 0; tokens[i] != NULL; i++)
    printf ("My Work:%02d: %s\n", i, tokens[i]);
}

int
hisWork (char **tokens)
{
  int i;
  for (i = 0; tokens[i] != NULL; i++)
    printf ("His Work:%02d: %s\n", i, tokens[i]);
}
#endif

/* split string into tokens, return token array */
char **
split (char *string, char *delim)
{
  char **tokens = NULL;
  char *working = NULL;
  char *token = NULL;
  int idx = 0;

  tokens = malloc (sizeof (char *) * MAXTOKENS);
  if (tokens == NULL)
    return NULL;
  working = malloc (sizeof (char) * strlen (string) + 1);
  if (working == NULL)
    return NULL;

  /* to make sure, copy string to a safe place */
  strcpy (working, string);
  for (idx = 0; idx < MAXTOKENS; idx++)
    tokens[idx] = NULL;

  token = strtok (working, delim);
  idx = 0;

  /* always keep the last entry NULL termindated */
  while ((idx < (MAXTOKENS - 1)) && (token != NULL))
    {
      tokens[idx] = malloc (sizeof (char) * strlen (token) + 1);
      if (tokens[idx] != NULL)
	{
	  strcpy (tokens[idx], token);
	  idx++;
	  token = strtok (NULL, delim);
	}
    }

  free (working);
  return tokens;
}
