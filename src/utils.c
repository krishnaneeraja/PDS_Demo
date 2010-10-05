#include "header.h"

extern FILE *fp;

char *
toUpper (char *strToConvert)
{				//change each element of the string to upper case

  unsigned int i = 0;
  fprintf (fp, "Inside toUpper: String to Convert: %s\n", strToConvert);
  fflush(fp);
  
  for (i = 0; i < strlen (strToConvert); i++)
    {
      strToConvert[i] = toupper (strToConvert[i]);
    }
  fprintf (fp, "Inside toUpper: Converted String: %s\n", strToConvert);
  fflush(fp);
  
  return strToConvert;		//return the converted string
}

char *
toLower (char *strToConvert)
{				//change each element of the string to upper case

  unsigned int i = 0;
  fprintf (fp, "Inside toUpper: String to Convert: %s\n", strToConvert);
  fflush(fp);
  
  for (i = 0; i < strlen (strToConvert); i++)
    {
      strToConvert[i] = tolower (strToConvert[i]);
    }
  fprintf (fp, "Inside toLower: Converted String: %s\n", strToConvert);
  fflush(fp);
  
  return strToConvert;		//return the converted string
}


int setString(char *defaultVal, char *display, char *var, int length, BOOL clrFlag)
{
	int i, ikey;
	char temp[255];

	fprintf (fp, "Begun setString\n");
	fflush(fp);
	
	if(clrFlag)
		lk_dispclr();
	
	lk_disptext(1,0,display,0);
	
	for(i=0;i<length;i++)
	        temp[i]='\0';
	
	ikey = lk_getalpha(2,0,defaultVal,length,temp,3);

	fprintf(fp,"This is the return value:%d",ikey);
	fflush(fp);

	strcpy(var,temp);

	fprintf(fp,"Ended setString:This is the var:%s",temp);
	fflush(fp);

	if(ikey > 0)
		return SUCCESS;
	else
		return ERROR;
}


int setNumeric(char *defaultVal, char *display, char *var, int length)
{
	int i, ikey;
	char temp[100];

	fprintf (fp, "Begun setString\n");
	fflush(fp);
	
	lk_dispclr();
	lk_disptext(1,0,display,0);
	
	for(i=0;i<length;i++)
	        temp[i]='\0';
	
	ikey = lk_getnumeric(2,0,defaultVal,length,temp,3);

	fprintf(fp,"This is the return value:%d",ikey);
	fflush(fp);

	strcpy(var,temp);

	fprintf(fp,"Ended setString:This is the var:%s",temp);
	fflush(fp);

	if(ikey > 0)
		return SUCCESS;
	else
		return ERROR;
}

void
getTime (char *format, char *li)
{

  time_t now;
  struct tm *d, d1;

  fprintf (fp, "Begun getTime\n");
  time (&now);
  d=&d1;
  d = localtime (&now);

  strftime (li, 25, format, d);

  fprintf (fp, "%s\n", li);
  fprintf (fp, "Ended getTime\n");
  fflush(fp);
}


int prompt(char *msg)
{
	char sprompt[2];
	char prompt;
	char temp[4][100];
	int ikey;
	int pMetReading;
	
	lk_dispclr();
	
	lk_disptext(1,0,msg,0);
	
	sprintf(temp[3],"X-CANCEL    ENTER-OK");
	lk_disptext(4,0,temp[3],1);
	
	ikey = lk_getkey();
	
	if(ikey == ENTER){
	return SUCCESS;
	}
	else if(ikey == CANCEL){
	return ERROR;
	}
	
	return SUCCESS;
}


int dispCE() {
	int i;
	
	fprintf(fp,"Begin dispCE()\n");
	fflush(fp);	
	
	lk_disptext(5, 0, "X-Cancel  Enter-OK", 0);
	
	i = lk_getkey();
	
	if(i == CANCEL)
		return ERROR;
	else
		return SUCCESS;
	
	fprintf(fp,"Ended dispCE()\n");
	fflush(fp);	
}

char *trim(char *src)
{
	int srcIndex=0;
	int dstIndex=0;

	while (src[srcIndex] != 0) {
		if (src[srcIndex] != ' ') {
		src[dstIndex++] = src[srcIndex]; // copy
		}
		srcIndex++;
	}
	src[dstIndex] = 0;

	return src;
}


int prn(char *str, int fontSize)
{
	char temp[100];
	int ret = SUCCESS, i;
	for(i=0;i<3;i++)
	{
		sprintf(temp,"      %s",str);
		ret = prn_write_text(temp, strlen(temp),0);
		if(ret == SUCCESS)
			break;
	}
	return SUCCESS;
}
