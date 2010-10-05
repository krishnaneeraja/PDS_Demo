#include "header.h"

int ping()
{
        FILE *file;
        char line[100];

        system("rm -rf /mnt/jffs2/.ping_test");
        system("ping -c 1 209.85.231.104|grep icmp_seq|grep time|wc -l > /mnt/jffs2/.ping_test");

        if((file = fopen ( "/mnt/jffs2/.ping_test", "r" )) == NULL){
                printf("Unable to open file");
                return ERROR;
        }

        if ( file != NULL )
        {
                if(fgets ( line, sizeof line, file ) != NULL)
                {
                        printf("This is the line:::%s::::\n",line);

						trim(line);

                        if(line[0]=='1')
                                return SUCCESS; //Ping successful
                        else
                                return ERROR; //Ping failed
                }
        }
}
