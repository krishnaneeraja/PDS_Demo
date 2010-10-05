#include "header.h"

void *syncThread (void *arg);
extern FILE *fp;
extern char *getVal (char *);
void synchronise();
void upDown(void *arg);

void synchronise()
{
}


void updatePendingBatch()
{
	int i,j;
	int rc;
	char sql[5000];
	char uCAN[10];
	char temp[100];
	resultset table;
	ORDER_HEADER *oh;

	if(strcmp(background,"true")!=0){
		lk_dispclr();
		sprintf(temp,"Connecting to server");
		lk_disptext(2,0,temp,0);
	}

	if(strcmp(toUpper(getVal("commMode")),"GPRS") == 0)
	{
		if(gprs_conn() == ERROR){
			fprintf(fp,"Error initialising GPRS\n");
			fflush(fp);
			return;
		}
	}

	fprintf(fp,"Reached here...1\n");
	fflush(fp);

	if ((oh = (ORDER_HEADER *)malloc(sizeof(ORDER_HEADER))) == NULL) {
			fprintf(fp,"ERROR ALLOCATING ORDER_HEADER\n");
			exit;
	}

	fprintf(fp,"Reached here...2:::%s\n",background);
	fflush(fp);

	if(strcmp(background,"true")!=0){
		lk_dispclr();

		sprintf(temp,"Synchronising Txn Data with Server...");
		lk_disptext(1,0,temp,0);
	}
	
	fprintf(fp,"Begun updatePending");
	fflush(fp);

	sprintf(sql,"SELECT OFFICE_ID,ID,CARD_ID,MEMBER, CARD_TYPE,ORDER_DATE,ORDER_TYPE,AMOUNT,VAT,ORDER_DISCOUNT,PAYMENT_MODE,\
				STATUS,REMARKS,USER_ID,RECONCILE_STATUS,SYNC_STATUS,MEMBER_SLNO FROM ORDER_HEADER WHERE SYNC_STATUS='0' LIMIT 50");

	table = get_result(sql);
	fprintf(fp,"After executing query:%s\n",sql);
	fprintf(fp,"No. of rows:%d\n",table.rows);

	fflush(fp);

	if(table.rows > 0 && strcmp(table.recordset[0][0], "") != 0)
	{
			fprintf(fp,"\n Data in users table...\n");
			fflush(fp);
			for(i=0;i<table.rows;i++)
			{
				CART_ITEM *next, *cartHead;
				CART_ITEM *curr;

				if ((cartHead = (CART_ITEM *)malloc(sizeof(CART_ITEM))) == NULL) {
						fprintf(fp,"ERROR ALLOCATING ORDER_TRAILER\n");
						exit;
				}
				
				curr = cartHead;

				if(strcmp(background,"true")!=0){
					lk_dispclr();
					sprintf(temp,"Processing Card: %s",table.recordset[i][2]);
					lk_disptext(2,0,temp,0);
				}
				
				if(populateOrder(oh, cartHead, i, table) == ERROR)
					continue;

				fprintf(fp,"After populating oh\n");
				fflush(fp);
				
//				printOrderHeader(oh);
				
				if(insert_remote_bill_details(oh, cartHead) == SUCCESS){
					j = update_order(oh,'1');
				}
				
				while(1){
					fprintf(fp,"%d~~",curr);
					fflush(fp);

					next = curr->next;
					free(curr);
					if(next == NULL)
						break;
					else
						curr = next;
				}

				if(j == 1)
					break;
			}
	}
	else
			fprintf(fp,"Data not Available\n");

	free(oh);
	

	fprintf(fp,"Freeing ~~~~~~~~~~~\n");
	fflush(fp);

	free_result(table);


	if(strcmp(toUpper(getVal("commMode")),"GPRS") == 0)
		Disconnect_GPRS();
	
	fprintf(fp,"End updatePendingBatch");
}


int reprint()
{
	ORDER_HEADER *oh;
	resultset table;
	CART_ITEM *next, *cartHead;
	CART_ITEM *curr;
	char card_id[20];
	char sql[1000], temp[100];
	int i,j;

	fprintf(fp,"Reached here...1\n");
	fflush(fp);

	if ((oh = (ORDER_HEADER *)malloc(sizeof(ORDER_HEADER))) == NULL) {
			fprintf(fp,"ERROR ALLOCATING ORDER_HEADER\n");
			exit;
	}

	setString("", "Card No:",card_id, 20, TRUE);
	
	sprintf(sql,"SELECT OFFICE_ID,ID,CARD_ID,MEMBER, CARD_TYPE,ORDER_DATE,ORDER_TYPE,AMOUNT,VAT,ORDER_DISCOUNT,PAYMENT_MODE,\
				STATUS,REMARKS,USER_ID,RECONCILE_STATUS,SYNC_STATUS FROM ORDER_HEADER  \
				WHERE ID=(select max(id+0) from order_header where card_id='%s')", card_id);

	table = get_result(sql);
	fprintf(fp,"After executing query:%s\n",sql);
	fprintf(fp,"No. of rows:%d\n",table.rows);

	fflush(fp);

	if(table.rows > 0 && strcmp(table.recordset[0][0], "") != 0)
	{
			fprintf(fp,"\n Data in users table...\n");
			fflush(fp);
			for(i=0;i<table.rows;i++)
			{
				CART_ITEM *next, *cartHead;
				CART_ITEM *curr;

				if ((cartHead = (CART_ITEM *)malloc(sizeof(CART_ITEM))) == NULL) {
						fprintf(fp,"ERROR ALLOCATING ORDER_TRAILER\n");
						exit;
				}
				
				curr = cartHead;

				if(strcmp(background,"true")!=0){
					lk_dispclr();
					sprintf(temp,"Processing Card: %s",table.recordset[i][2]);
					lk_disptext(2,0,temp,0);
				}
				
				if(populateOrder(oh, cartHead, i, table) == ERROR)
					break;

				fprintf(fp,"After populating oh\n");
				fflush(fp);
				
//				printOrderHeader(oh);
				order_header = oh;
				cart = cartHead;
				
				populateCard(oh);
				
				print_bill();
				
				while(1){
					fprintf(fp,"%d~~",curr);
					fflush(fp);

					next = curr->next;
					free(curr);
					if(next == NULL)
						break;
					else
						curr = next;
				}
				
				free(card);
			}
	}
	else
			fprintf(fp,"Data not Available\n");

	free(oh);
	

	fprintf(fp,"Freeing ~~~~~~~~~~~\n");
	fflush(fp);

	free_result(table);

	fprintf(fp,"End updatePending");	
	
}

int populateCard(ORDER_HEADER *oh)
{
	
	char sql[1000];
	resultset table;

	
	if((card = (CARD *)malloc(sizeof(CARD))) == NULL)
		return ERROR;
	
	fprintf(fp,"Populating card:%d\n",card);
	fflush(fp);
	
	//card is a global variable
	strcpy(card->family_head,oh->member);
	strcpy(card->card_id,oh->card_id);
	strcpy(card->card_type,oh->card_type);

	sprintf(sql,"SELECT NO_OF_MEMBERS FROM CARD_DETAILS WHERE CARD_ID='%s'",oh->card_id);

	table = get_result(sql);
	fprintf(fp,"After executing query:%s\n",sql);
	fprintf(fp,"No. of rows:%d\n",table.rows);

	fflush(fp);	
	
	if(table.rows > 0 && strcmp(table.recordset[0][0], "") != 0)
	{
			strcpy(card->no_of_members,table.recordset[0][0]); 
	}
	
	free_result(table);
}

void updatePending(ORDER_HEADER *oh, CART_ITEM *cart)
{
	int i,j;
	int rc;
	char sql[5000];
	char uCAN[10];
	char temp[100];
	resultset table;

	fprintf(fp,"Reached here...1\n");
	fflush(fp);

	if(strcmp(toUpper(getVal("commMode")),"GPRS") == 0)
	{
		if(gprs_conn() == ERROR){
			fprintf(fp,"Error initialising GPRS\n");
			fflush(fp);
			return;
		}
	}

	fprintf(fp,"Reached here...2:::%s\n",background);
	fflush(fp);

	if(strcmp(background,"true")!=0){
		lk_dispclr();

		sprintf(temp,"Synchronising Txn Data with Server...");
		lk_disptext(1,0,temp,0);
	}
	
	fprintf(fp,"Begun updatePending");
	fflush(fp);
				
	if(insert_remote_bill_details(oh, cart) == SUCCESS){
		j = update_order(oh,'1');
	}
	
	if(strcmp(toUpper(getVal("commMode")),"GPRS") == 0)
		Disconnect_GPRS();
	
	fprintf(fp,"End updatePending");

}


int insert_remote_bill_details(ORDER_HEADER *oh, CART_ITEM *cart)
{
	int fd,pos,iRet,len = 0,first_number=1,reconnect=0,category=0;
	FILE *file;

	int rc, i, j;

	char buff512[512];
	
	char *ptr;
	
	TransactionRequest tr;
	LoginRequest lrq;
	LoginResponse lrr;
	
	if(printFlag)
		return ERROR;
	
	ptr = strstr(SESSION_ID,"E00"); //Check for Error code
 	
	if(ptr == NULL) 
	{
		svrLogin(&lrq);
		fprintf(fp,"Logged in with SESSION_ID:%s\n",SESSION_ID);
		fflush(fp);
		
		if(strcmp(SESSION_ID,"") != 0)
		{
			fprintf(fp,"Logged in with SESSION_ID:%s\n",SESSION_ID);
			fflush(fp);
			
			sleep(2);

			ptr = strstr(SESSION_ID,"E00");
			
			if(ptr == NULL && postTxn(oh, cart) == SUCCESS)
				return SUCCESS;
		}
	}
	return ERROR;
}

int printOrderHeader(ORDER_HEADER *oh)
{
	fprintf(fp,"Begun printOrderHeader\n");
	fflush(fp);
	
	fprintf(fp,"oh->office_id=%s,oh->order_id=%s,oh->card_id=%s,oh->member=%s,oh->card_type=%s,oh->order_date=%s,oh->order_type=%s,oh->amount=%.2f,oh->vat=%.2f,oh->discount=%.2f,oh->payment_mode=%s,oh->status=%s,oh->remarks=%s,oh->user_id=%s,oh->reconcile_status=%s,oh->sync_status=%s\n",
				oh->office_id,oh->order_id,oh->card_id,oh->member,oh->card_type,oh->order_date,oh->order_type,oh->amount,oh->vat,oh->discount,oh->payment_mode,oh->status,oh->remarks,oh->user_id,oh->reconcile_status,oh->sync_status);
	
	fflush(fp);
	
	fprintf(fp,"Ended printOrderHeader\n");
	fflush(fp);

}

int printCart(CART_ITEM *cart)
{
	CART_ITEM *cart_item = cart;
	
	fprintf(fp,"*************** BEGIN: DUMP CART ******************\n");
	fflush(fp);
	
	while(cart_item != NULL && strcmp(cart_item->product_id,"")!=0)
	{
		printCartItem(cart_item);
		
		cart_item = cart_item->next;
	}

	fprintf(fp,"*************** END: DUMP CART ******************\n");
	fflush(fp);
	
}

int printCartItem(CART_ITEM *cart_item)
{
	fprintf(fp,"Begun dump of printCartItem\n");
	fflush(fp);
	
	fprintf(fp,"cart_item=%d, cart_item->product_id=%s,cart_item->unique_code=%s,cart_item->product_name=%s,cart_item->allotment_type=%s,cart_item->price_upper_limit=%.2f,cart_item->price_lower_limit=%.2f,cart_item->quantity=%.2f,cart_item->actual_price=%.2f,cart_item->vat=%.2f,cart_item->uom=%d,cart_item->max_quantity=%.2f,cart_item->availed=%.2f,cart_item->scheme_id=%d,cart_item->warranty_start_date=%s,cart_item->warranty_end_date=%s,cart_item->prod_code=%s\n",
			cart_item, cart_item->product_id,cart_item->unique_code,cart_item->product_name,cart_item->allotment_type,cart_item->price_upper_limit,cart_item->price_lower_limit,cart_item->quantity,cart_item->actual_price,cart_item->vat,cart_item->uom,cart_item->max_quantity,cart_item->availed,cart_item->scheme_id,cart_item->warranty_start_date,cart_item->warranty_end_date,cart_item->prod_code);
	fflush(fp);

	fprintf(fp,"End of printCartItem\n");
	fflush(fp);
	
}
int printTxn(TransactionRequest *tr)
{
	fprintf(fp,"Begun dump of txnRequest\n");
	fflush(fp);
	
//	fprintf(fp,"tr->MsgNo=%d,tr->TransactionNo=%d,tr->CardNo=%s,tr->MemberId=%s,tr->commodity=%d,tr->TotalPrice=%.2f,tr->SchemeCode=%d,tr->SessionId=%s,tr->DateTime=%s\n",
//			tr->MsgNo,tr->TransactionNo,tr->CardNo,tr->MemberId,tr->commodity,tr->TotalPrice,tr->SchemeCode,tr->SessionId,tr->DateTime);
		fprintf(fp,"tr->MsgNo=%d,tr->TransactionNo=%d,tr->CardNo=%s,tr->MemberId=%s,tr->TotalPrice=%.2f,tr->SchemeCode=%d,tr->SessionId=%s,tr->DateTime=%s\n",
				tr->MsgNo,tr->TransactionNo,tr->CardNo,tr->MemberId,tr->TotalPrice,tr->SchemeCode,tr->SessionId,tr->DateTime);
	fflush(fp);

	fprintf(fp,"End of txnRequest\n");
	fflush(fp);

}

int populateRequest(TransactionRequest *tr, ORDER_HEADER *oh, CART_ITEM *cartHead)	
{
	Commodity *commodity, *prevCommodity, _commodity;
	CART_ITEM *cart_item = cartHead;
	
	int i;
	
	commodity = (Commodity *)malloc(sizeof(Commodity));
	prevCommodity = commodity;
		
//	printOrderHeader(oh);

	fprintf(fp, "0. Reached here");
	fflush(fp);

	tr->MsgNo = 1L;
	tr->TransactionNo = atoi(oh->order_id);
	strcpy(tr->CardNo, oh->card_id);
	strcpy(tr->MemberId, oh->member);	

	fprintf(fp, "0.1. Reached here");
	fflush(fp);

	tr->TotalPrice = oh->amount;

	fprintf(fp, "0.2. Reached here");
	fflush(fp);

	printCart(cartHead);
//	tr->SchemeCode = (int)cartHead->scheme_id;  //Populate first item's scheme code

	tr->SchemeCode =1;
	
	fprintf(fp,"*****SESSION_ID FOR POSTING TXN=%s\n",SESSION_ID);
	fflush(fp);
	
	strcpy(tr->SessionId, SESSION_ID);
	
	getTime("%d-%m-%Y %H:%M:%S",tr->DateTime);
	
	printTxn(tr);
}

int update_order(ORDER_HEADER *oh,char status)
{
	 char sql[5000];
	 int rc = 0;
	 
	 fprintf(fp,"Begun update_order\n");

	 sprintf(sql,
	 "UPDATE ORDER_HEADER SET SYNC_STATUS='%c' WHERE ID='%s' AND ORDER_DATE='%s'",
	 	status, oh->order_id, oh->order_date);

	 fprintf(fp,"This is the sql:\n%s",sql);
	 rc = execute(sql);

	 fprintf(fp,"This is the result from update:%d\n",rc);
	 if(rc == 1){
		fprintf(fp,"Update Order successful: Order Id:%s\n",oh->order_id);
		return 0;
	 }
	 else{
		fprintf(fp,"Update Order failed:SCNO:%s\n",oh->order_id);
		return 1;
	 }	
}

int populateOrder(ORDER_HEADER *oh, CART_ITEM *cartHead, int i, resultset table)
{
	char sql[1000];
	resultset carttable;
	
	CART_ITEM *cart_item=cartHead, *cart_prev=NULL;
	
	strcpy(oh->office_id,table.recordset[i][0]); 
	strcpy(oh->order_id,table.recordset[i][1]);
	strcpy(oh->card_id,table.recordset[i][2]);
	strcpy(oh->member,trim(table.recordset[i][3]));	
	strcpy(oh->card_type,table.recordset[i][4]);
	strcpy(oh->order_date,table.recordset[i][5]);
	strcpy(oh->order_type,table.recordset[i][6]);
	oh->amount	=	atof(table.recordset[i][7]);
	oh->vat		=	atof(table.recordset[i][8]);
	oh->discount=	atof(table.recordset[i][9]);
	strcpy(oh->payment_mode,table.recordset[i][10]);
	strcpy(oh->status,table.recordset[i][11]);
	strcpy(oh->remarks,table.recordset[i][12]);
	strcpy(oh->user_id,table.recordset[i][13]);
	strcpy(oh->reconcile_status,table.recordset[i][14]);
	strcpy(oh->sync_status,table.recordset[i][15]);
	oh->member_slno = atoi(table.recordset[i][16]);
	
	fprintf(fp,"Inside populateOrder\n");
	fflush(fp);
	
//	printOrderHeader(oh);

	fprintf(fp,"Populating cart items:\n");
	fflush(fp);

	sprintf(sql,"SELECT PRODUCT_ID,UNIQUE_CODE,PRODUCT_NAME,ALLOTMENT_TYPE,PRICE_UPPER_LIMIT,LOWER_LIMIT,QUANTITY, \
			ACTUAL_PRICE,VAT,UOM,MAX_QUANTITY,AVAILED,SCHEME_ID,WARRANTY_START_DATE,WARRANTY_END_DATE,PROD_CODE,\
			ORDER_ID FROM ORDER_TRAILER WHERE ORDER_ID=%s",oh->order_id);

	carttable = get_result(sql);
	fprintf(fp,"After executing query:%s\n",sql);
	fprintf(fp,"No. of rows:%d\n",carttable.rows);

	fflush(fp);	
	
	if(carttable.rows > 0 && strcmp(carttable.recordset[0][0], "") != 0)
	{
		for(i=0;i<carttable.rows;i++)
		{
			if(i!=0)
			{
				if ((cart_item = (CART_ITEM *)malloc(sizeof(CART_ITEM))) == NULL) {
						fprintf(fp,"ERROR ALLOCATING ORDER_TRAILER\n");
						return ERROR;
				}
				cart_prev->next = cart_item;
			}

			strcpy(cart_item->product_id,carttable.recordset[i][0]); 
			strcpy(cart_item->unique_code,carttable.recordset[i][1]); 
			strcpy(cart_item->product_name,carttable.recordset[i][2]);
			strcpy(cart_item->allotment_type,carttable.recordset[i][3]);
			cart_item->price_upper_limit = atof(carttable.recordset[i][4]);
			cart_item->price_lower_limit = atof(carttable.recordset[i][5]);
			cart_item->quantity = atof(carttable.recordset[i][6]);
			cart_item->actual_price = atof(carttable.recordset[i][7]);
			cart_item->vat = atof(carttable.recordset[i][8]);
			cart_item->uom=	atoi(carttable.recordset[i][9]);
			cart_item->max_quantity = atof(carttable.recordset[i][10]);
			cart_item->availed = atof(carttable.recordset[i][11]);
			cart_item->scheme_id = atol(carttable.recordset[i][12]);
			strcpy(cart_item->warranty_start_date,carttable.recordset[i][13]);
			strcpy(cart_item->warranty_end_date,carttable.recordset[i][14]);
			strcpy(cart_item->prod_code,carttable.recordset[i][15]);
			
			fprintf(fp, "5.############# Ended population of cart item:%d:\n",cart_item);
			fflush(fp);
			
//			printCartItem(cart_item);
			
			cart_prev = cart_item;
			cart_item->next = NULL;
		}
		ret = SUCCESS;
	}
	else
	{
		//Error in order
		update_order(oh,'2');
		ret = ERROR;
	}
	
	free_result(carttable);
	
	return ret;
}


void *
syncThread (void *arg)
{
  int sleepTime = 30, temp = 0;

  if(strcmp(toUpper(getVal("commMode")),"ETHERNET") != 0)
	  if(gprs_conn()==ERROR){
		fprintf(fp,"Unable to initialise GPRS\n");
		fflush(fp);
	  }

  while (1)
    {
      fprintf (fp, "Sleeping now for Sleeptime:%d\n", sleepTime);
      fflush (fp);
	  
	  sleep (sleepTime);

      fprintf (fp, "About to synchronise\n");
      fflush (fp);

		if(strcmp(getVal("background"),"false")==0){
			continue;
		}

	  fprintf(fp, "Print flag: %s\n", (printFlag==TRUE?"TRUE":"FALSE"));
	  fflush(fp);

//	  if(!printFlag)
//		updatePending();

      temp = (atoi) (getVal ("sleepTime"));

      if (temp > 30)
		sleepTime = temp;

    }
}