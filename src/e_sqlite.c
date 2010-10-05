/***************************************************************************
 *            e_sqlite.c
 *
 *  Mon Jun 26 08:52:54 2006
 *  Copyright  2006  User
 *  Email: jwamicha@yahoo.co.uk
 *  
 *  Comments (no BLOB support):
 *  1. INSERT from sqlite database
 *  2. UPDATE sqlite database
 *  3. SELECT from sqlite database
 ****************************************************************************/
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <stdarg.h>
  
#define DEBUG 1
  
#define ELEMENT_NUM(_array) ( sizeof(_array)/sizeof(_array[0]) )
  typedef struct resultset_struct
{
  int rows;
   int cols;
   char ***recordset;
 } resultset;
int err_printf (const char *fmt, ...);
int open_sqlite (char *db_name);
int close_sqlite ();
int execute (const char *fmt, ...);
char *m_fgets (char *s, int n, FILE * f);
int execute_file (char *filename);
resultset get_result (const char *fmt, ...);
void free_result (resultset resultset_table);

//extern void err_printf(const char *fmt, ...);
  sqlite3 * db_conn;
int ret = 0;
int
open_sqlite (char *db_name) 
{
  if (sqlite3_open (db_name, &db_conn) == SQLITE_OK)
    
    {
      printf (" database opened \n");
      return 1;
    }
  
  else
    return 0;
}
int
close_sqlite () 
{
  if (sqlite3_close (db_conn) == SQLITE_OK)
    return 1;
  
  else
    return 0;
}


//only type s and d are allowed as arguments
  int
execute (const char *fmt, ...) 
{
  char *err_messg;
  int ret = 0, result = 0;
  char sql_string[3500];	//this honestly needs to be more elegant; will do for now
  va_list args;
  printf ("\n in execute fun");
  va_start (args, fmt);
  sql_string[0] = '\0';
  ret = vsprintf (sql_string, fmt, args);
  va_end (args);
  
    //err_printf(sql_string);
    printf ("\n sql_string  :  %s", sql_string);
  if (!ret)
    result = 0;
  
  else
    result = 1;
  if (result != -1)
    
    {
      if (sqlite3_exec (db_conn, sql_string, NULL, 0, &err_messg) ==
	   SQLITE_OK)
	
	{
	  printf ("\n  sqlite3_exec fun is successful");
	  result = 1;
	}
      
      else
	
	{
	  
	    //err_printf("SQL error: %s\n", err_messg);
	    printf ("\n SQL error in sqlite3_exec: %s\n", err_messg);
	  result = 0;
	}
    }
  return result;
}


//you must open_sqlite first before using execute_file
  int
execute_file (char *filename) 
{
  char *err_messg;
  FILE * read_fd = (FILE *) 0;
  char sql_string[1024];
  ret = 0;
  read_fd = fopen (filename, "r");	//open file for read    
  if (read_fd != NULL)
    
    {
      rewind (read_fd);
      while (!feof (read_fd))
	
	{
	  m_fgets (sql_string, 1024, read_fd);
	  
	    //ie if string is not empty, then execute - ha no more newline errors!
	    if (strcmp (sql_string, "") != 0)
	    
	    {
	      
		//err_printf("SQL_STRING: %s\n", sql_string);   
		if (sqlite3_exec (db_conn, sql_string, NULL, 0, &err_messg)
		     == SQLITE_OK)
		
		{
		  ret = 1;
		  continue;
		}
	      
	      else
		
		{
		  
		    //err_printf("SQL error: %s\n", err_messg);
		    printf ("\n SQL error: %s\n", err_messg);
		  ret = 0;
		  break;
		}
	    }
	}
    }
  fclose (read_fd);
  return ret;
}
char *
m_fgets (char *line, int n, FILE * fd) 
{
  int c = 0;
  char *cstring;
  cstring = line;
  while (--n > 0 && (c = getc (fd)) != EOF)
    
    {
      if (c == '\n')
	break;
      *cstring++ = c;
    }
  *cstring++ = '\0';
  if (c == EOF && cstring == line)	//ie nothing in file!
    line = NULL;
  if (c == EOF)
    line = NULL;
  return line;
}

resultset get_result (const char *fmt,...) 
{
  int success = 0;
  int nrow = 0, ncol = 0, i = 0, j = 0, count = 0;
  char *err_messg;
  char **result;
  char ***recordset;
  resultset resultset_table;
  char sql_string[1024];	//this honestly needs to be more elegant; will do for now
  va_list args;
  va_start (args, fmt);
  sql_string[0] = '\0';
  ret = vsprintf (sql_string, fmt, args);
  va_end (args);
  
    //err_printf("%s\n", sql_string);
printf("\n sql string :  %s\n", sql_string);
    
    //printf("\n in get_result function ");
    //initialize resultset_table;
resultset_table.rows = 0;
  resultset_table.cols = 0;
  resultset_table.recordset = NULL;
  ret =
    sqlite3_get_table (db_conn, sql_string, &result, &nrow, &ncol,
		       &err_messg );
  
    //err_printf("nrow=%d ncol=%d\n",nrow,ncol);
    //printf("nrow=%d ncol=%d\n",nrow,ncol);
    //printf("Inside e_sqlite.c before gechar function");
    //getchar();
    //printf("Inside e_sqlite.c");  
    recordset = (char ***) malloc (nrow * sizeof (char **));
  for (count = ncol; count < ((nrow + 1) * ncol); count++)
    
    {
      recordset[i] = (char **) malloc (ncol * sizeof (char *));
      for (j = 0; j < ncol; j++)
	
	{
	  
	    //err_printf("%s ",result[count]);
	    recordset[i][j] = (char *) malloc ((strlen (result[count]) + 1));
	  strcpy (recordset[i][j], result[count]);
	  if (j != (ncol - 1))
	    count++;
	}
      i++;
      
	//err_printf("\n");
    }
  sqlite3_free_table (result);
  if (ret != SQLITE_OK)
    
    {
      
	//err_printf("SQL error: %s\n", err_messg);
	printf ("\n SQL error in get_table: %s\n", err_messg);
      success = 0;
    }
  
  else
    
    {
      resultset_table.rows = nrow;
      resultset_table.cols = ncol;
      resultset_table.recordset = recordset;
      success = 1;
    }
  
    //printf("About to return");
    return resultset_table;
}


//will free all allocd memory ie only recordset memory (since only that allocd)
  void
free_result (resultset resultset_table) 
{
  if (resultset_table.recordset != NULL)
    free (resultset_table.recordset);
}


//if DEBUG is on then print message to stderr using fprintf function
  int
err_printf (const char *fmt, ...) 
{
  int i;
  if (DEBUG == 1)
    
    {
      va_list ap;
      va_start (ap, fmt);
      i = vfprintf (stderr, fmt, ap);
      va_end (ap);
    }
  return i;
}


/*
//returns the row index in the resultset that was selected
//column is the column of the resultset that you want to display
int menu_display(int column, char ***ptr, resultset *menu_info,int *len)
{
                                                                                                 
        char *menu_list[menu_info->rows + 1];
        int i;
                                                                                                 
        if(menu_info->rows > 0 && strcmp(menu_info->recordset[0][0], "") != 0)
        {
                //fill menu_list
                for(i=0; i<(menu_info->rows); i++)
                {
                        menu_list[i] = menu_info->recordset[i][column];
                }
        }
        menu_list[menu_info->rows] = "Back";//add a Back item
	
	scroll_menu(menu_list,i);
                                                                                                 
// *ptr = menu_list;
// *len = i;
return 1;
}
*/ 
