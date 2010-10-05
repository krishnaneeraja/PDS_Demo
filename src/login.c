#include "header.h"

BOOL login()
{
	fprintf(fp,"Begin of Login()");
	fflush(fp);
	
	getUserName();
	getPasswd();
	
/**	userVerifStatus = FALSE;
	
	upek_menu();
	
	if(userVerifStatus)
		return TRUE;
	else
		return FALSE;
**/	
	fprintf(fp,"End of validate()");
	fflush(fp);
	
	return validate();
	
}

int getUserName()
{
	setString("", "Username:",userName, 20, TRUE);
	return SUCCESS;
}

int getPasswd()
{
	setString("", "Password:",password, 20, TRUE);
	return SUCCESS;
}

int validate()
{
	//Validate userName and password
    char sql[100];
    resultset ptable;
    int ret;

    fprintf(fp,"Begin of validate()");
    fflush(fp);

    sprintf(sql,"SELECT role_id FROM USER_DETAILS WHERE USER_NAME='%s' AND PASSWORD='%s'",userName,password);
    
    ptable = get_result(sql);
                                              
    if(ptable.rows > 0 && strcmp(ptable.recordset[0][0], "0") == 0)
    {
    		strcpy(USER_NAME, userName);
            ret = ADMIN;
    }
    else if(ptable.rows > 0 && strcmp(ptable.recordset[0][0], "1") == 0)
    {
    		strcpy(USER_NAME, userName);
            ret = USER;
    }
    else
            ret = USER_DOES_NOT_EXIST;

    fprintf(fp,"End of validate:%s",(ret==ADMIN?"ADMIN":(ret==USER?"USER":"USER_DOES_NOT_EXIST")));
    fflush(fp);

    free_result(ptable);

    return ret;
}
