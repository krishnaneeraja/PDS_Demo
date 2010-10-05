#include <stdio.h>
#include <string.h>

char name[20][30], value[20][70];
int keys = 0;

char *getVal (char *);
void setVal (char *, char *);
extern FILE *fp;

int
read_config (char *configFile)
{
  FILE *file = fopen (configFile, "r");
  int i = 0, rc = 0, index = 0;
  char line[500], temp[100], *ptr;

  keys = 0;
  if (file == NULL)
    {
      write_empty (configFile);
      file = fopen (configFile, "r");
    }

  if (file != NULL)
    {
      while (fgets (line, sizeof line, file) != NULL)
	{
	  ptr = strchr (line, '=');

	  if (ptr == NULL)
	    return 1;

	  index = ptr - line - 1;
	  printf ("This is the name length: %d\n", index);

	  strncpy (name[keys], line, index + 1);
	  name[keys][index + 1] = '\0';

	  index = strlen (line) - index - 3;
	  printf ("This is the val length: %d\n", index);
	  strncpy (value[keys], ptr + 1, index);
	  value[keys][index] = '\0';

	  printf ("Name[%d]:%s,value[%d]=%s\n", keys, name[keys], keys,
		  value[keys]);
	  keys++;
	}
      printf ("Config items length in read_config:%d\n", keys);
      fclose (file);
      return 0;
    }

  return 1;
}

char *
getVal (char *key)
{
  int i;
  printf ("Config items length in getVal:%d\n", keys);

  for (i = 0; i < keys; i++)
    {
      if (strcmp (name[i], key) == 0)
	return value[i];
    }
  return "-1";
}

void
setVal (char *key, char *val)
{
  int i;
  fprintf (fp, "Key=:%s\n", key);

  for (i = 0; i < keys; i++)
    {
      fprintf (fp, "name[%d]=%s\n", i, name[i]);

      if (strcmp (name[i], key) == 0)
	{
	  fprintf (fp, "name[%d]=%s,val=%s\n", i, name[i], val);
	  strcpy (value[i], val);
	  return;
	}
    }
}

int
write_config (char *configFile)
{
  FILE *file;
  int i = 0;
//      char cmd[100];

//      sprintf(cmd,"rm -rf %s",configFile);
//      system(cmd);

  file = fopen (configFile, "w+");

  printf ("Config items length in write_config:%d\n", keys);
  if (file != NULL)
    {
      for (i = 0; i < keys; i++)
	{
	  printf ("Writing:\n%s=%s\n", name[i], value[i]);
	  fprintf (file, "%s=%s\n", name[i], value[i]);
	}
    }
  fclose (file);
}

int
write_empty (char *configFile)
{
  FILE *file;
  int i = 0;

  file = fopen (configFile, "w+");

  printf ("Creating a new file with empty data\n");
  if (file != NULL)
    {
      fprintf (file, "show_splash=false\n");
      fprintf (file, "gprs_domain=0000\n");
      fprintf (file, "remote_url=http://192.168.1.25/AMS\n");
      fprintf (file, "admin_user=9999\n");
      fprintf (file, "admin_pwd=54321\n");
      fprintf (file, "server_user=3\n");
      fprintf (file, "server_pwd=padma\n");
      fprintf (file, "logFileSize=1048000\n");
      fprintf (file, "FilePath=/mnt/jffs2\n");
      fprintf (file, "uploadURL=/uploadfile.aspx\n");
      fprintf (file, "downloadURL=/Download.aspx\n");
      fprintf (file, "commMode=ETHERNET\n");	//ETHERNET/GPRS
      fprintf (file, "commType=DHCP\n");	//STATIC/DHCP
      fprintf (file, "ethType=ETH0\n");
      fprintf (file, "ipAddr=192.168.1.85\n");	//Device IP Address
      fprintf (file, "subnetMask=255.255.255.0\n");	//Subnet Mask
      fprintf (file, "defaultGw=192.168.1.8\n");	//Subnet Mask
      fprintf (file, "primaryDNS=192.168.1.8\n");	//Primary DNS
      fprintf (file, "secDNS=192.168.1.8\n");	//Secondary DNS
      fprintf (file, "sleepTime=60\n");
    }
  fclose (file);
}

int
write_val (char *configFile, char *key, char *value)
{
  FILE *file;
  int i = 0;

  file = fopen (configFile, "a+");

  printf ("Creating a new key value pair...%s=%s\n", key, value);
  if (file != NULL)
    {
      fprintf (file, "%s=%s\n", key, value);
    }
  fclose (file);
}
