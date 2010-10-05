#include "header.h"

int svrLogin(LoginRequest *lr)
{
	char requestBuf[5000];
	char responseBuf[5000];
	char tmpbuf[5000];	
	
	strcpy(requestBuf,"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	strcat(requestBuf,"<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">");
	strcat(requestBuf,"  <soap:Body>");
	strcat(requestBuf,"    <login xmlns=\"http://microsoft.com/webservices/\">");
	strcat(requestBuf,"      <objdetails>");
	sprintf(tmpbuf,"%s        <LoginID>%s</LoginID>",requestBuf,getVal("server_user"));
	strcpy(requestBuf,tmpbuf);
	sprintf(tmpbuf,"%s       <Password>%s</Password>",requestBuf,getVal("server_pwd"));
	strcpy(requestBuf,tmpbuf);
	sprintf(tmpbuf,"%s        <DeviceID>%s</DeviceID>",requestBuf,getVal("terminal"));
	strcpy(requestBuf,tmpbuf);
	strcat(requestBuf,"      </objdetails>");
	strcat(requestBuf,"    </login>");
	strcat(requestBuf,"  </soap:Body>");
	strcat(requestBuf,"</soap:Envelope>");

	fprintf(fp,"Soap Login Request:%s\n",requestBuf);
	fflush(fp);
	
	return invokeSoap(requestBuf,getVal("remote_url"),"login",responseBuf);	
}

int postTxn(ORDER_HEADER *oh, CART_ITEM *cart)
{
	char requestBuf[5000]="";
	char responseBuf[500]="";
	char tmpbuf[5000];
	
	CART_ITEM *cart_item = cart;
	
	fprintf(fp, "Dumping txn in soap.c\n");
	fflush(fp);
	//printTxn(tr);


	strcpy(requestBuf,"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	strcat(requestBuf,"<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">");
	strcat(requestBuf,"  <soap:Body>");
	strcat(requestBuf,"    <postTransaction xmlns=\"http://microsoft.com/webservices/\">");
	strcat(requestBuf,"      <objTransactions>");
	sprintf(tmpbuf,"%s			<DeviceTxnNo>%d</DeviceTxnNo>",requestBuf,atoi(oh->order_id));
	strcpy(requestBuf,tmpbuf);
	sprintf(tmpbuf,"%s        	<CardNo>%s</CardNo>",requestBuf,oh->card_id);
	strcpy(requestBuf,tmpbuf);
	sprintf(tmpbuf,"%s        	<MemberID>%d</MemberID>",requestBuf,oh->member_slno);
	strcpy(requestBuf,tmpbuf);
	strcat(requestBuf,"        <ArrayOfCommodity>");

	while(cart_item!=NULL  && strcmp(cart_item->product_id,"")!=0)
	{

		fprintf(fp, "********Cart_item=%d, %s, %.2f, %.2f\n",cart_item,cart_item->product_id,cart_item->quantity,cart_item->actual_price);
		fflush(fp);
		
		strcat(requestBuf,"          <Commodity>");
		sprintf(tmpbuf,"%s            <CommodityCode>%s</CommodityCode>",requestBuf,cart_item->product_id);
		strcpy(requestBuf,tmpbuf);
		sprintf(tmpbuf,"%s            <Quantity>%.2f</Quantity>",requestBuf,cart_item->quantity);
		strcpy(requestBuf,tmpbuf);
		sprintf(tmpbuf,"%s            <Price>%.2f</Price>",requestBuf,(cart_item->actual_price*cart_item->quantity));
		strcpy(requestBuf,tmpbuf);
		sprintf(tmpbuf,"%s        <SchemeCode>%d</SchemeCode>",requestBuf,cart_item->scheme_id);
		strcpy(requestBuf,tmpbuf);
		strcat(requestBuf,"          </Commodity>");
			
		cart_item = cart_item->next;
	}
	
	strcat(requestBuf,"	        </ArrayOfCommodity>");
	sprintf(tmpbuf,"%s        <TotalPrice>%.2f</TotalPrice>",requestBuf,oh->amount);
	strcpy(requestBuf,tmpbuf);
	sprintf(tmpbuf,"%s        <SessionID>%s</SessionID>",requestBuf,SESSION_ID);
	strcpy(requestBuf,tmpbuf);
	strcat(requestBuf,"      </objTransactions>");
	strcat(requestBuf,"    </postTransaction>");
	strcat(requestBuf,"</soap:Body>");
	strcat(requestBuf,"</soap:Envelope>");	

	fprintf(fp, "%s",requestBuf);
	fflush(fp);

	return invokeSoap(requestBuf,getVal("remote_url"),"postTransaction",responseBuf);
	
}


int svrLogout(LogoutRequest *lr)
{
	char requestBuf[5000];
	char responseBuf[5000];
	char dateTime[20];

	getTime("%d-%m-%Y %H:%M:%S",dateTime);
	
	strcpy(requestBuf,"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	strcat(requestBuf,"<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">");
	strcat(requestBuf,"  <soap:Body>");
	strcat(requestBuf,"    <logout xmlns=\"http://microsoft.com/webservices/\">");
	strcat(requestBuf,"      <objLogoutRequest>");

	sprintf(tmpbuf,"%s<SessionID>%s</SessionID>",requestBuf,SESSION_ID);
	strcpy(requestBuf,tmpbuf);
	
	strcat(requestBuf,"      </objLogoutRequest>");
	strcat(requestBuf,"    </logout>");
	strcat(requestBuf,"  </soap:Body>");
	strcat(requestBuf,"</soap:Envelope>");
	
	fprintf(fp,"Soap Logout Request:%s\n",requestBuf);
	fflush(fp);
	
	return invokeSoap(requestBuf,getVal("remote_url"),"logout",responseBuf);	
}

int invokeSoap(char *requestBuf, char *method, char *action, char *response)
{
	char *uploadFile="upload.xml";
	char *responseFile="response.xml";
	char invokeUrl[500];
	char cmd[500];
	char *ptr, *ptr1;
	int len;
	int ret;

	
	char requestPath[MAX_FILE_PATH];
	char responsePath[MAX_FILE_PATH];
	char line[512];
	
	FILE *u, *r;
	
	sprintf(requestPath,"%s/%s",getVal("FilePath"),uploadFile);
	sprintf(responsePath,"%s/%s",getVal("FilePath"),responseFile);
	
	if((u = fopen(requestPath,"w"))== NULL)
		return ERROR;
	
	fprintf(u,requestBuf);
	fclose(u);

	fprintf(fp,"This is the XML:%s\n",requestBuf);
	fflush(fp);
	
	sprintf(invokeUrl,"%s?op=%s",getVal("remote_url"),action);
	
//	upload("upload.txt", invokeUrl, action, responsePath);

	sprintf(cmd,"curl -H \"Content-Type: text/xml; charset=utf-8\" -H \"SOAPAction:\" -d @/mnt/jffs2/%s -X POST %s?op=%s -o /mnt/jffs2/%s",uploadFile,getVal("remote_url"),action, responseFile);

	fprintf(fp,"Calling %s\n",cmd);
	system(cmd);
	
	sleep(1);
	
	fprintf(fp,"Invoking soap with: requestPath=%s, url=%s, action=%s, responsePath=%s\n",
			requestPath, invokeUrl, action, responsePath);
	fflush(fp);
	
	if((r = fopen(responsePath,"r"))==NULL)
		return ERROR;
	
	while ( fgets ( line, sizeof line, r ) != NULL )
	{
		strcat(response,line);
	}
	
	if(strcmp(action,"postTransaction")==0)
	{
		ptr=strstr(response,"<postTransactionResult>Saved Successfully</postTransactionResult>");
		if(ptr == NULL){
			strcpy(SESSION_ID,"");
			ret = ERROR;
		}
		else
			ret = SUCCESS;
		
		fprintf(fp,"Result after postTransaction:%s, ptr=%d\n",ret == SUCCESS?"SUCCESS":"FAILURE",ptr);
		fflush(fp);
	}
	else if(strcmp(action,"login")==0)
	{	
		ptr=strstr(response, "<loginResult>");
		
		if(ptr != NULL)
			ptr1 = strstr(ptr,"</loginResult>");
		else
			strcpy(SESSION_ID,"");
	
		if(ptr1 != NULL){
			len = ptr1-(ptr+13);
			strncpy(SESSION_ID,ptr+13,len);
			SESSION_ID[len]='\0';
		}

		fprintf(fp,"*****SESSION_ID AFTER LOGIN=%s\n",SESSION_ID);
		fflush(fp);
	}
	
	
	fprintf(fp,"This is the response:%s\n",response);
	fflush(fp);
	
	fclose(r);	
	
	return ret;
}
