#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

extern void *malloc(size_t);
float _round(float,int);

typedef struct resultset_struct {
int rows;
                                        int cols;
                                        char ***recordset;
                                } resultset;

int initialize_db();
extern int open_sqlite(char *);
extern int close_sqlite();
extern resultset get_result(const char* fmt, ...);
extern int execute(const char *fmt, ...);
extern void free_result(resultset resultset_table);
void parseFiles();
void select1(char *sql);
void execFile(char *sql);

int main()
{
parseFiles();
}

void select1(char *sql)
{
int rc, i;
resultset table;

table = get_result(sql);
printf("After executing query:%s\n",sql);
printf("No. of rows:%d\n",table.rows);


if(table.rows > 0 && strcmp(table.recordset[0][0], "") != 0)
{
printf("\n Data in users table...\n");
for(i=0;i<table.rows;i++)
{
printf(table.recordset[i][0]);
}
}
else
printf("Data not Available\n");

free_result(table);
}

void parseFiles()
{
FILE *fileS = fopen ( "s.sql", "r" );
FILE *fileE = fopen ( "e.sql", "r" );
char line[5000];
int rc;

initialize_db();

        if(rc == 1)
                printf("\n Database initialised");
        else
                printf("\n Init failed");

open_sqlite("/mnt/jffs2/epos.db");


if ( fileS != NULL )
{
int i=0;
while ( fgets ( line, sizeof line, fileS ) != NULL )
{
select1(line);
}
fclose ( fileS );
}

if ( fileE != NULL )
{
int i=0;
while ( fgets ( line, sizeof line, fileE ) != NULL )
{
execFile(line);
printf("Size of line:%d\n",strlen(line));
}
fclose ( fileE );
}

}

void execFile(char *sql)
{
int rc;

rc = execute(sql);

if(rc == 1)
printf("\n Updated successfully.");
else
printf("\n Unable to update.");
}

int initialize_db()
{
        char *database = "/mnt/jffs2/epos.db";
        int rc = 1;
        struct stat buf;
        int i = 0;
FILE *fp;

fp=stdout;

fprintf(fp,"Begun initialize_db\n");

                                                                                                 
        fprintf(fp,"1...Initializing database from path...%s\n",database);
        i = stat ( database, &buf );
        fprintf(fp,"2...Initializing database...:%d\n",i);

        if( (i<0)||( buf.st_size == 0 ))
        {
                fprintf(fp,"Initializing database...\n");
                // lk_disptext(1,0,"initialising database",0);
                fprintf(fp,"Initializing database...\n");
                // lk_disptext(1,0,"initialising database",0);
                rc = open_sqlite(database);
                if(rc == 0)
                {
                        fprintf(fp,"Create database failure!\n");
//lk_disptext(2,0,"create database ail",0);
                }
                close_sqlite();
        }

fprintf(fp,"Ended initialize_db\n");

return rc;
}
