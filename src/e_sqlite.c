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
  
{
  int rows;
   
   
 









//extern void err_printf(const char *fmt, ...);
  


open_sqlite (char *db_name) 
{
  
    
    {
      
      
    
  
  else
    


close_sqlite () 
{
  
    
  
  else
    



//only type s and d are allowed as arguments
  int
execute (const char *fmt, ...) 
{
  
  
  
  va_list args;
  
  
  
  
  
  
    //err_printf(sql_string);
    printf ("\n sql_string  :  %s", sql_string);
  
    
  
  else
    
  
    
    {
      
	   SQLITE_OK)
	
	{
	  
	  
	
      
      else
	
	{
	  
	    //err_printf("SQL error: %s\n", err_messg);
	    printf ("\n SQL error in sqlite3_exec: %s\n", err_messg);
	  
	
    
  



//you must open_sqlite first before using execute_file
  int
execute_file (char *filename) 
{
  
  
  
  
  
  if (read_fd != NULL)
    
    {
      
      
	
	{
	  
	  
	    //ie if string is not empty, then execute - ha no more newline errors!
	    if (strcmp (sql_string, "") != 0)
	    
	    {
	      
		//err_printf("SQL_STRING: %s\n", sql_string);   
		
		     == SQLITE_OK)
		
		{
		  
		  
		
	      
	      else
		
		{
		  
		    //err_printf("SQL error: %s\n", err_messg);
		    printf ("\n SQL error: %s\n", err_messg);
		  
		  
		
	    
	
    
  
  


m_fgets (char *line, int n, FILE * fd) 
{
  
  
  
  
    
    {
      
	
      
    
  
  
    line = NULL;
  
    
  



{
  
  
  
  
  
  
  
  va_list args;
  
  
  
  
  
    //err_printf("%s\n", sql_string);
printf("\n sql string :  %s\n", sql_string);
    
    //printf("\n in get_result function ");
    //initialize resultset_table;
resultset_table.rows = 0;
  
  
  
    sqlite3_get_table (
		       
  
    //err_printf("nrow=%d ncol=%d\n",nrow,ncol);
    //printf("nrow=%d ncol=%d\n",nrow,ncol);
    //printf("Inside e_sqlite.c before gechar function");
    //getchar();
    //printf("Inside e_sqlite.c");  
    recordset = (char ***) malloc (nrow * sizeof (char **));
  
    
    {
      
      
	
	{
	  
	    //err_printf("%s ",result[count]);
	    recordset[i][j] = (char *) malloc ((strlen (result[count]) + 1));
	  
	  
	    
	
      
      
	//err_printf("\n");
    }
  
  
    
    {
      
	//err_printf("SQL error: %s\n", err_messg);
	printf ("\n SQL error in get_table: %s\n", err_messg);
      
    
  
  else
    
    {
      
      
      
      
    
  
    //printf("About to return");
    return resultset_table;



//will free all allocd memory ie only recordset memory (since only that allocd)
  void
free_result (resultset resultset_table) 
{
  
    



//if DEBUG is on then print message to stderr using fprintf function
  int
err_printf (const char *fmt, ...) 
{
  
  
    
    {
      
      
      
      
    
  



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