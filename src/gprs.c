#include "header.h"

int gprs_conn()
{

        int reconnect=0,iRet=0;

        while(reconnect<3)
        {
			if(printFlag == TRUE)
				return ERROR;

                iRet = connect_To_GPRS();
                if (iRet == 1)break;
                if (iRet != 1)reconnect++;
        }

        if (iRet != 1)
        {
		    if(strcmp(background,"true")!=0){
		          lk_dispclr();
		          lk_disptext(2,1,"Network error",0);
			}
                sleep(3);
                return ERROR;
        }
		return SUCCESS;
}

int
connect_To_GPRS ()
{
  int fd, pos;
  int conres;

  printf ("Begun connect_To_GPRS");

  ppp_options("1234","1234"); // username and passworrd
  conres = pppdial_gprs ("*99***1#");
  
  if(conres<0) return -1;
  ppp_dial_update ("*99***1#", getVal ("gprs_domain"));

  printf ("End of connect_To_GPRS");

  if (!ppp_open ())
    return 1;
  else
    return 0;

}

