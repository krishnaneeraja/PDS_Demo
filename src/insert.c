#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <stdio.h>
#include <time.h>
#include<sys/stat.h>
#include <config.h>
#include "header.h"

#define MAX_RECONNECTS 1
#define MAXLINE         1024	/* fgets buff */

int gprsConnect (char *username, char *password);

typedef enum
{
  IN = 1,
  OUT = 0
} IN_OUT;

int updateEmpDB (char **tokens);
int updateAttendance (char **tokens);

char tmpDir[50], curDate[9], cmd[100];
int Connect_GPRS ();
int Disconnect_GPRS ();
int download (char *url, char *tempFile, char *data);
int updateEmpDB (char **tokens);
int file_exists (char *fileName);
int genZip ();
int genFile (char *fileName);
int insertUpdate (char *username, IN_OUT * inOut);
int checkUserInDB (char *username);
int validateLogin (char *username);
int updateEnroll (char *username);
void getTime (char *format, char *li);
char *toUpper (char *strToConvert);
extern char *getVal (char *);
extern char *database;
extern int upload (char *uploadFileName, char *url, char *date,
		   char *responseFileName);
char *getName (char *username);
int deleteUser (char *username);
int processResponse (char *response);




int
genZip ()
{
  fprintf (fp, "Invoking genZip\n");

  sprintf (cmd, "rm -rf %s/%s.tar*", getVal ("FilePath"), curDate);
  system (cmd);

  sprintf (cmd, "tar -cvf %s/%s.tar %s/%s/*", getVal ("FilePath"), curDate,
	   getVal ("FilePath"), curDate);
  fprintf (fp, "Calling:\n%s\n", cmd);
  fflush (fp);

  system (cmd);

  sprintf (cmd, "gzip %s/%s.tar", getVal ("FilePath"), curDate);
  fprintf (fp, "Zipping:\n%s\n", cmd);
  fflush (fp);
  system (cmd);
}

int
Connect_GPRS ()
{
  int iRet, reconnect = 0;

  while (reconnect < MAX_RECONNECTS)
    {
      iRet = connect_To_GPRS ();
      if (iRet == 1)
	break;
      if (iRet != 1)
	reconnect++;
    }

  if (iRet != 1)
    {
      lk_dispclr ();
      lk_disptext (2, 1, "Net work error", 0);
      sleep (3);
      return 1;
    }

}

int
setupEthernet ()
{
  char str[100];
  sprintf (str, "/home/dhcpcd -t 20 %s", getVal ("ethtype"));

  fprintf (fp, "Starting DHCPCD:\n%s\n", str);
  fflush (fp);

  system (str);
}

int
updateEmpDB (char **tokens)	//Copy thumbprints into tmpDir for every un-registered emp, while parsing downloaded file
{
  char format[10];
  long cur_id;
  char sql[1000];
  int rc;
  char filePath[200];
  char url[100];

  fprintf (fp, "Begun insert\n");

  rc = checkUserInDB (tokens[0]);
  sprintf (filePath, "%s/%s%s", PAM_BIRDIR, tokens[0], BIR_EXTENSION);

  if (rc == 0)			//Employee does not exist in local DB
    {
      fprintf (fp, "Employee does not exist in local DB\n");
      fflush (fp);

      sprintf (sql,
	       "insert into employee(emp_id,emp_name,dob,doj,registered) values ('%s','%s','%s','%s','%s');",
	       tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
      rc = execute (sql);

      if (rc == 1)
	fprintf (fp, "\n Inserted into Employee successfully.");
      else
	fprintf (fp, "\n Insert into Employee failed.");

      if (strcmp (tokens[4], "1") == 0)
	{			//If remote shows registered, download BIR file.
	  downloadBIR (tokens[0]);
	}
    }
  else if (rc == 1)		//Employee exists, but is not registered.
    {
      fprintf (fp, "Employee exists in local DB, but not registered.\n");
      fflush (fp);

      if (strcmp (tokens[4], "1") == 0)
	{			//If remote shows registered, update registered in local as well.
	  if (file_exists (filePath))
	    {
	      sprintf (sql,
		       "update employee set registered=1 where emp_id='%s';",
		       tokens[0]);
	      rc = execute (sql);

	      if (rc == 1)
		fprintf (fp, "\nUpdated Employee successfully.");
	      else
		fprintf (fp, "\nUpdate Employee failed.");
	    }
	  else
	    {
	      //Download BIR from server.
	      downloadBIR (tokens[0]);
	    }
	}
    }
  else if (rc == 2)		//Employee already registered in local DB.
    {
      //Check status in remote.
      if (strcmp (tokens[4], "0") == 0)
	{			//New registrant. To be updated in remote. Copy BIR file into tmpDir
	  fprintf (fp, "New registrant. To be updated in remote.\n");

	  sprintf (cmd, "cp %s/%s.bir %s", getVal ("FilePath"), tokens[0],
		   tmpDir);
	  fprintf (fp, "Invoking command:%s.\n", cmd);
	  fflush (fp);

	  system (cmd);
	}

      rc = 0;
    }

  fprintf (fp, "Ended updateEmpDB\n");

  return rc;
}

int
downloadBIR (char *userId)
{
  char tempFile[100];
  char data[100];
  char url[100];

  sprintf (url, "%s/%s?empbio=%s", getVal ("remote_url"),
	   getVal ("downloadURL"), userId);
  sprintf (tempFile, "%s/%s.bir", getVal ("FilePath"), userId);

  sprintf (data, "userid=%s&password=%s&date=", getVal ("server_user"),
	   getVal ("server_pwd"));

  fprintf(fp,"Downloading BIR file:%s\n",url);

  if (!GET_file_clear (url, tempFile, data))
    {
      return 1;
    }
  return 0;
}

int
file_exists (char *fileName)
{
  struct stat buf;
  int i = stat (fileName, &buf);
  /* File found */
  if (i == 0)
    {
      return 1;
    }
  return 0;

}


int
genFile (char *fileName)
{
  char sql[1000];
  int rc = 0;
  resultset ptable;
  int i;
  FILE *file;

  fprintf (fp, "Begun genFile\n");

  sprintf (sql,
	   "select emp_id,intime,ifnull(outtime,0) from attendance_info where (svr_flag=0 or (svr_flag=1 and outtime is not null))");
  fprintf (fp, "Gen File SQL:%s\n", sql);
  fflush (fp);

  fprintf (fp, "Debug:1");
  fflush (fp);

  ptable = get_result (sql);
  fprintf (fp, "Debug:2");
  fflush (fp);

  file = fopen (fileName, "w+");

  fprintf (fp, "Debug:3");
  fflush (fp);

  fprintf (fp, "ptable.rows:%d\n", ptable.rows);
  fflush (fp);

  if (file != NULL)
    {
      fprintf (fp, "Debug:4");
      fflush (fp);
      if (ptable.rows > 0 && strcmp (ptable.recordset[0][0], "") != 0)
	
	{
	  for (i = 0; i < ptable.rows; i++)
	    fprintf (file, "%s,%s,%s\n", ptable.recordset[i][0],
		     ptable.recordset[i][1], ptable.recordset[i][2]);
	}
    }

  fprintf (fp, "Debug:5");
  fflush (fp);

  fclose (file);
  free_result (ptable);

  fprintf (fp, "Debug:6");
  fflush (fp);

  fprintf (fp, "Ended genFile\n");
  fflush (fp);

  return rc;
}

int
insertUpdate (char *username, IN_OUT * inOut)
{
  char format[10];
  long cur_id;
  char sql[1000];
  int rc;

  fprintf (fp, "Begun insert\n");

  if (validateLogin (username) == 0)
    {
      sprintf (sql,
	       "insert into attendance_info(emp_id,intime,outtime,svr_flag) values ('%s',datetime('now'),null,0);",
	       username);
      *inOut = IN;
    }
  else
    {
      sprintf (sql,
	       "update attendance_info set outtime=datetime('now') where emp_id='%s' and outtime is null;",
	       username);
      *inOut = OUT;
    }

  fprintf (fp, "\nThis is the sql: %s\n", sql);

  rc = execute (sql);

  if (rc == 1)
    fprintf (fp, "\n Inserted into ATTENDANCE_INFO successfully.");
  else
    fprintf (fp, "\n Insert into ATTENDANCE_INFO failed.");

  fprintf (fp, "Ended insert_login\n");

  return rc;
}

int
checkUserInDB (char *username)
{
  char sql[1000];
  int rc;
  resultset ptable;

  fprintf (fp, "Begun checkUserInDB\n");

  sprintf (sql, "select registered from employee where emp_id='%s';",
	   username);
  fprintf (fp, "checkUserInDB sql:%s\n", sql);
  fflush (fp);

  ptable = get_result (sql);
  fprintf (fp, "ptable.rows:%d\n", ptable.rows);

  if (ptable.rows == 0)
    {
      fprintf (fp, "Employee ID not found");
      rc = 0;
    }
  if (ptable.rows > 0 && strcmp (ptable.recordset[0][0], "") != 0)
    
    {
      if (strcmp (ptable.recordset[0][0], "0") == 0)
	{
	  fprintf (fp, "Employee not registered");
	  rc = 1;
	}
      
      else if (strcmp (ptable.recordset[0][0], "1") == 0)
	{
	  fprintf (fp, "Employee registered already");
	  rc = 2;
	}
    }
  free_result (ptable);
  fprintf (fp, "Ended checkUserInDB:Returning::%d::\n", rc);
  return rc;
}



int
validateLogin (char *username)
{
  char format[10];
  long cur_id;
  char sql[1000];
  int rc;
  resultset ptable;

  fprintf (fp, "Begun validateLogin\n");

  sprintf (sql,
	   "select count(*) from attendance_info where emp_id='%s' and outtime is null;",
	   username);
  fprintf (fp, "validateLogin sql:%s\n", sql);
  fflush (fp);

  ptable = get_result (sql);
  fprintf (fp, "ptable.rows:%d\n", ptable.rows);
  if (ptable.rows > 0 && strcmp (ptable.recordset[0][0], "") != 0)
    
    {
      if (strcmp (ptable.recordset[0][0], "0") == 0)
	{
	  fprintf (fp, "No IN entry found");
	  rc = 0;		//In
	}
      else
	{
	  rc = 1;		//Out
	}
    }
  free_result (ptable);
  fprintf (fp, "Ended validateLogin\n");
  return rc;
}

int
updateEnroll (char *username)
{
  char format[10];
  long cur_id;
  char sql[1000];
  int rc;

  fprintf (fp, "Begun updateEnroll\n");

  sprintf (sql, "update employee set registered=1 where emp_id='%s';",
	   username);

  fprintf (fp, "\nThis is the sql: %s\n", sql);

  rc = execute (sql);

  if (rc == 1)
    fprintf (fp, "\nUpdate Employee successfully.");
  else
    fprintf (fp, "\n Update Employee failed.");

  fprintf (fp, "Ended updateEnroll\n");

  return rc;
}

char *
getName (char *username)
{
  static char name[100];
  char sql[1000];
  int rc;
  resultset ptable;

  fprintf (fp, "Begun getName\n");

  sprintf (sql, "select emp_name from employee where emp_id='%s'", username);
  fprintf (fp, "Employee sql:%s\n", sql);
  fflush (fp);

  ptable = get_result (sql);
  fprintf (fp, "ptable.rows:%d\n", ptable.rows);
  if (ptable.rows > 0 && strcmp (ptable.recordset[0][0], "") != 0)
    
    {
      strcpy (name, ptable.recordset[0][0]);
    }
  free_result (ptable);
  fprintf (fp, "Ended getName\n");
  return name;
}

int
deleteUser (char *username)
{
  char format[10];
  long cur_id;
  char sql[1000];
  int rc;
  char cmd[100];

  fprintf (fp, "Begun deleteUser\n");

  sprintf (sql, "update employee set registered=0 where emp_id='%s';",
	   username);

  fprintf (fp, "\nThis is the sql: %s\n", sql);

  rc = execute (sql);

  if (rc == 1)
    {
      sprintf (cmd, "rm -f %s/%s.bir", getVal ("FilePath"), username);
      system (cmd);
      fprintf (fp, "%s\n", cmd);
      fprintf (fp, "Deleted thumbrint info for user\n");
      fprintf (fp, "\nUnregistered Employee successfully\n.");
      fflush (fp);
    }
  else
    fprintf (fp, "\n Unregister Employee failed.");

  fprintf (fp, "Ended deleteUser\n");

  return rc;
}

int
updateAttendance (char **tokens)
{
  char format[10];
  long cur_id;
  char sql[1000];
  int rc;

  fprintf (fp, "Begun updateAttendance\n");

  sprintf (sql,
	   "update attendance_info set svr_flag='%s' where emp_id='%s' and intime='%s';",
	   tokens[2], tokens[0], tokens[1]);

  fprintf (fp, "\nThis is the sql: %s\n", sql);

  rc = execute (sql);

  if (rc == 1)
    {
      fprintf (fp, "\nUpdated attendance successfully\n.");
      fflush (fp);
    }
  else
    fprintf (fp, "\nUpdate attendance failed.");

  fprintf (fp, "Ended updateAttendance\n");

  return rc;
}
