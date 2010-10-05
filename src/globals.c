#include "header.h"

void init_globals()
{

	  fprintf(fp,"Begun init_globals()");
	  fflush(fp);
	  
	strcpy(LOG_FILE, get_config("LOG_FILE",tmpbuf,"0"));	
	strcpy(CURRENCY, get_config("CURRENCY",tmpbuf,"0")); //Get from CONFIG table	
	strcpy(TILL,getVal("till_no"));
	
	strcpy(OFF_ADDRESS.company_name,get_prop("COMPANY_NAME",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.address_line_1,get_prop("ADDRESS_LINE_1",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.address_line_2,get_prop("ADDRESS_LINE_2",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.address_line_3,get_prop("ADDRESS_LINE_3",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.city,get_prop("CITY",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.state,get_prop("STATE",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.country,get_prop("COUNTRY",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.postcode,get_prop("POSTCODE",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.telephone,get_prop("TELEPHONE",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.mobile,get_prop("MOBILE",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.fax,get_prop("FAX",tmpbuf,OFFICE_ID));
	strcpy(OFF_ADDRESS.e_mail,get_prop("E_MAIL",tmpbuf,OFFICE_ID));
	

	  fprintf(fp,"Ended init_globals()");
	  fflush(fp);
	  
}
