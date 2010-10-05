#include "header.h"

#define QUOTE	F1
#define BILL	F1
#define PAYMENT F2
#define CART	F4
#define VOID	F3

#define CASH	F1
#define DEBIT	F2
#define CREDIT	F3
#define CHEQUE	F4

#define NOT_ELIGIBLE -2

int key=0;

int retail_sale() {

	MEMBER *member;
	DISTRIBUTION *distribution;

	char thumbprint[MAX_THUMBPRINT_SIZE];
	
	fprintf(fp,"Entered retail_sale\n");
	fflush(fp);
	
	if ((order_header = (ORDER_HEADER *)malloc(sizeof(ORDER_HEADER)))== NULL)
		return ERROR;

	strcpy(order_header->office_id, OFFICE_ID);
	
	//Card can get instantiated in reprint
	if((card = (CARD *)malloc(sizeof(CARD))) == NULL)
		return ERROR;
  
    fprintf(fp,"Allocated memory for Card:%d\n",card);
    fflush(fp);
	
	if((member = (MEMBER *)malloc(sizeof(MEMBER))) == NULL)
		return ERROR;

	member->next = NULL;
	
	if((cart = (CART_ITEM *)malloc(sizeof(CART_ITEM))) == NULL)
		return ERROR;

    initCartItem(cart);
	fprintf(fp,"In retail, office_id:%s",OFFICE_ID);
	fflush(fp);

	fprintf(fp,"Before begin_order()\n");
	fflush(fp);
	
	strcpy(order_header->member,"");
	
	getTime ("%d-%m-%Y %H:%M:%S", order_header->order_date);
	
	if (authenticate(card,member,thumbprint)!=SUCCESS){
		prompt("Card does not exist or Card not recognised");
		
		return ERROR;
	}


	fprintf(fp,"Before item_menu(), card=%d, card_id:%s\n",card, card->card_id);
	fflush(fp);

	
	if (begin_order(card)==ERROR)
		return ERROR;
	
	fprintf(fp,"Before item_menu(), card=%d, card_id:%s\n",card, card->card_id);
	fflush(fp);

	if(item_menu(card,distribution) == NOT_ELIGIBLE){
		prompt("Card not eligible for any schemes.");
	}

	fprintf(fp,"Before show_cart():%s\n",order_header->order_id);
	fflush(fp);

	show_cart();

	fprintf(fp,"Before order_menu():%s\n",order_header->order_id);
	fflush(fp);

	if(order_menu() == ERROR)
		return ERROR;

	fprintf(fp,"Before clear_cart()\n");
	fflush(fp);

	if(strcmp(getVal("autoUpdate"),"true")==0)
		updatePending(order_header, cart);

//	clear_cart();
	
	freeCart(cart);
	
	fprintf(fp,"Back to Retail Menu()\n");
	fflush(fp);
	
	free(card);

	fprintf(fp,"After freeing card details()\n");
	fflush(fp);

	freeMember(member);

	fprintf(fp,"After freeing Member details()\n");
	fflush(fp);

	free(order_header);

	fprintf(fp,"After freeing Order Header details()\n");
	fflush(fp);

}

int freeMember(MEMBER *member){
	MEMBER *curr = member, *next;
	
	while(curr->next != NULL){
		next = curr->next;
		free(curr);
		curr = next;
	}
	
	free(curr);
}


int freeCart(CART_ITEM *cart){
	CART_ITEM *curr = cart, *next;
	
	while(curr->next != NULL){
		next = curr->next;
		free(curr);
		curr = next;
	}
	
	free(curr);
}


int authenticate(CARD *card, MEMBER *member, char *thumbprint)
{
	int ret;
	char path[150];
	char fingerType[5];
	
    fprintf(fp,"Authenticate:About to read Card data:%d\n",card);
    fflush(fp);

    ret = (*read_card)(card,member); 
	if(ret != SUCCESS)
		return ret;

	while(1)
	{
		if(display_member_menu(card, member) == ERROR){
			  prompt("Error displaying member menu");
			  return ERROR;
		}
	
		  if(strcmp(getVal("authBio"),"false")==0)
				return SUCCESS;
		
		  if(finger_new() == ERROR){
			  if(prompt("Finger print mismatch, try again?")==SUCCESS)
				  continue;
			  else
				  return ERROR;
		  }
		  else{
			  fprintf(fp,"Verification Successful");
			  fflush(fp);
			  
			  return SUCCESS;
		  }
	}
	
	return SUCCESS;
}

/*Card layout
1. HHCard Number            15 				0
2. DISTRICT                 2				15
3. MANDAL                   2				17
4. FPSHOPNO                 3				19
5. NO_OF_MEMBER        		2				22
6. HEADOFFAMILY       		60(30 +30)		24
7. CARDTYPE                 3				84
8. SURNAME                  60 ( 30+30)		87

1. SLNO                     2 				0
2. MEMBERNAME     			60(30+30)		2
3. AGE                      3				62
4. RELATION               	2				65
5. FP_LIF                   512				67
6. FP_RIF                   512				579
7. FP_LTF                 	1024			1091
*/



int check(char *fingerPrintMast,char *thumbprint)
{
	return SUCCESS;
}

int begin_order(CARD *card) {
	int rc;
	char sql[1000];

	fprintf(fp,"Before begin_order()\n");
	fflush(fp);
	
	set_order_id(); //order_header->order_id set here

	strcpy(order_header->card_id,card->card_id);
	strcpy(order_header->card_type,card->card_type);
	//order_header->order_date will be set at insert time
	strcpy(order_header->order_type,"0"); 
	order_header->amount = 0.0f;
	order_header->vat = 0.0f;
	order_header->discount = 0.0f;
	strcpy(order_header->payment_mode,"1");
	strcpy(order_header->status,"1");
	strcpy(order_header->remarks,"");
	strcpy(order_header->user_id,USER_NAME);
	strcpy(order_header->reconcile_status,"0");
	strcpy(order_header->sync_status,"0");
	
	//Block Item
	sprintf(sql,
			"UPDATE CONFIGURATION SET VALUE=VALUE+1 WHERE KEY='MAX_ORDER_ID'");
	rc = execute(sql);

	if (rc == 1) {
		fprintf(fp,
				"\nUpdated CONFIGURATION - MAX_ORDER_ID successfully.");
		return SUCCESS;
	} else {
		fprintf(fp, "\nUpdate CONFIGURATION - MAX_ORDER_ID failed.");
		return ERROR;
	}
	
	fprintf(fp,"Ended begin_order()\n");
	fflush(fp);
}

int item_menu(CARD *card, DISTRIBUTION *distribution)
{
	char sql[5000];
	resultset ptable;
	BOOL ret;
	DISTRIBUTION *distCurr = distribution, *distPrev= distribution;
	int i;

	fprintf(fp, "Begin of item_menu(), card_id:%s",card->card_id);
	fflush(fp);

	sprintf(sql,"select d.product_id,(select name from product_details where id=d.product_id) name,c.card_type,allotment_type,quantity,price,uom, \
			(select case when product_id=6 and lpgtype=4 then 4 else d.max_quantity end from card_details where card_id='%s') max_quantity, \
			scheme_id, \
					ifnull((SELECT SUM(OT.QUANTITY) QTY FROM ORDER_HEADER OH, ORDER_TRAILER OT \
						WHERE OH.ID=OT.ORDER_ID AND OH.CARD_ID='%s' \
						AND substr(OH.ORDER_DATE,4,2) = substr(date('now'),6,2) \
						and d.product_id=ot.product_id ),0)  QTY \
				 from shop_details s, distribution d, card_details c \
				where (s.area_type = d.area_type  or d.area_type=0) and d.card_type=c.card_type \
				and c.card_id='%s'",card->card_id,card->card_id, card->card_id);
					 
	fprintf(fp,"This is the sql:%s",sql);
	fflush(fp);
	
	ptable = get_result(sql);
	
	if (ptable.rows > 0 && strcmp(ptable.recordset[0][0], "0") != 0) {
		
		for(i=0;i<ptable.rows;i++)
		{
			if((distCurr = (DISTRIBUTION *)malloc(sizeof(DISTRIBUTION))) == NULL)
				return ERROR;
			
			fprintf(fp,"This is the current dist:%d, head:%d\n",distCurr, distribution);
			fflush(fp);
			
			if(i>0)
				distPrev->next = distCurr;
			else
				distribution = distCurr;
			
			strcpy(distCurr->product_id, ptable.recordset[i][0]);
			strcpy(distCurr->name, ptable.recordset[i][1]);
			strcpy(distCurr->card_type, ptable.recordset[i][2]);
			strcpy(distCurr->allotment_type, ptable.recordset[i][3]);
			strcpy(distCurr->quantity, ptable.recordset[i][4]);
			strcpy(distCurr->price, ptable.recordset[i][5]);
			strcpy(distCurr->uom, ptable.recordset[i][6]);
			strcpy(distCurr->max_quantity, ptable.recordset[i][7]);
			strcpy(distCurr->scheme_id, ptable.recordset[i][8]);
			strcpy(distCurr->availed, ptable.recordset[i][9]);
			
			distCurr->next = NULL;
			
			fprintf(fp,"Distribution[%d]:product_id=%s,name=%s,card_type=%s,allotment_type=%s,quantity=%s,price=%s,uom=%s,max_qty=%s,scheme=%s,availed=%s\n",
					i, distCurr->product_id,distCurr->name,distCurr->card_type,distCurr->allotment_type,distCurr->quantity,distCurr->price,distCurr->uom,distCurr->max_quantity,distCurr->scheme_id,distCurr->availed);
			
			distPrev = distCurr;
		}
		

		fprintf(fp,"1.Distribution:%d\n",distribution);
		fflush(fp);
		
		display_menu(card, ptable, distribution);

		fprintf(fp,"2.Distribution:%d\n",distribution);
		fflush(fp);

		ret = SUCCESS;
	} else
		ret = NOT_ELIGIBLE;

	
	fprintf(fp,"Order id:%s\n",order_header->order_id);
	fflush(fp);
	
	fprintf(fp, "End of item_menu:%s", (ret==SUCCESS ? "SUCCESS" : "ERROR"));
	fflush(fp);

	free_result(ptable);
	free(distCurr);

	return ret;	
}

int display_menu(CARD *card, resultset ptable, DISTRIBUTION *distribution)
{
        int c, i;
        int breakFlag=1;
    	CART_ITEM *cart_item=cart, *prvcart;

        MENU_T *menu, mainmenu;
        int opt=0;
        int selItem  = 0;
        int acceptKbdEvents=0;
        
        fprintf(fp,"Begun display_menu\n");
        fflush(fp);

        menu = &mainmenu;
        
	    menu->start          =       0;
	    menu->maxEntries     =       ptable.rows;

        while(breakFlag)
        {
	            fprintf(fp,"About to show menu\n");
	            fflush(fp);
	
	        	lk_dispclr();

                strcpy(menu->title,"Commodity List");

                for(i=0;i<ptable.rows;i++)
                        strcpy(menu->menu[i],ptable.recordset[i][1]);

                opt = scroll_menu(menu,&selItem,acceptKbdEvents);

                switch(opt)
                {
                   case CANCEL:
                               return ERROR;

                   case ENTER:
                	   
                   default:
                            fprintf(fp,"Adding: Selected item[%d]=%s, Product id:%s",selItem,ptable.recordset[selItem][1],ptable.recordset[selItem][0]);
                            fflush(fp);
                            
                            if(isItemNew(ptable.recordset[selItem][0])){
                                strcpy(cart_item->product_id,ptable.recordset[selItem][0]);
                                
                        		fprintf(fp,"3.Distribution:%d\n",distribution);
                        		fflush(fp);

                            	if(add_item(card, cart_item, distribution) == ERROR)
                            	{
                            		initCartItem(cart_item);
                            		continue;
                            	}
                            	
                        		fprintf(fp,"4.Distribution:%d\n",distribution);
                        		fflush(fp);                                
                            }
                            else
                            	edit_item(card, ptable.recordset[selItem][0], distribution);
                            

                        	prvcart = cart_item;
                            if((cart_item=(CART_ITEM *)malloc(sizeof(CART_ITEM)))==NULL){
                            	return ERROR;
                            }
                            else{                                    
                                initCartItem(cart_item);
                            	prvcart->next=cart_item;
                            	cart_item->next = NULL;
                            }
                            
                            printCart(cart);
                            
                }
        }
        
        return SUCCESS;

        fprintf(fp,"End display_menu\n");
        fflush(fp);
}

int initCartItem(CART_ITEM *cart_item)
{
	fprintf(fp,"Begin initCartItem:%d\n",cart_item);
	fflush(fp);
	
     strcpy(cart_item->product_id,"");
     strcpy(cart_item->unique_code,"");
     strcpy(cart_item->product_name,"");
     strcpy(cart_item->allotment_type,"");
     cart_item->price_upper_limit=0.0f;
     cart_item->price_lower_limit=0.0f;
     cart_item->quantity=0.0f;
     cart_item->actual_price=0.0f;
     cart_item->vat=0.0f;
     cart_item->uom=0.0f;
     cart_item->max_quantity=0.0f;
     cart_item->availed=0.0f;
     cart_item->scheme_id=0.0f;
     strcpy(cart_item->warranty_start_date,"");
     strcpy(cart_item->warranty_end_date,"");
     strcpy(cart_item->prod_code ,"");
     cart_item->next = NULL;
     
 	fprintf(fp,"End initCartItem:%d\n",cart_item);
 	fflush(fp);

}

BOOL isItemNew(char *product_id)
{
	CART_ITEM *cart_item = cart;
	
	while(cart_item!=NULL)
	{
		if(strcmp(cart_item->product_id,product_id)==0)
			return FALSE;
		
		fprintf(fp, "Comparing Cart Item product id: %s, product_id: %s, cart_item=%d\n", cart_item->product_id, product_id, cart_item);
		fflush(fp);
		
		cart_item = cart_item->next;
	}
	return TRUE;
}

int edit_item(CARD *card, char *product_id, DISTRIBUTION *distribution) //Called from item_menu
{
	//Remove item and then call add item
	CART_ITEM *curr=cart,*prev = NULL, *next=NULL, *cart_item;
	
	fprintf(fp,"\nIn Edit item\n");
	fflush(fp);
	
	while(curr!=NULL)
	{
		fprintf(fp,"Curr node: %d\n",curr);
		fflush(fp);

		fprintf(fp,"In Edit item - Comparing: Curr::%s::, Cart::%s::\n",curr->product_id,product_id);
		fflush(fp);
		
		if(strcmp(curr->product_id,product_id)==0) //If a match is found remove that node
		{

			if(prev!=NULL && curr->next!=NULL) //Item to be removed is not head node
			{
				prev->next = curr->next;
				order_header->amount -= curr->actual_price * curr->quantity; 

				fprintf(fp,"Not Head Node::order_header->amount=%.2f::\n",order_header->amount);
				fflush(fp);

				free(curr);
				
				break;
			}
			else if (prev == NULL)
			{
				if(curr->next != NULL) // Scenario will not occur
					cart = curr->next; //Reset head node
				else
					cart = NULL;
				
				order_header->amount -= curr->actual_price * curr->quantity; 

				fprintf(fp,"Head Node::order_header->amount=%.2f::\n",order_header->amount);
				fflush(fp);

				free(curr);
				
				break;
			}
			else if (curr->next == NULL) //Scenario will not occur as last node is always new node
			{
				fprintf(fp,"Tail Node::Freeing curr\n");
				fflush(fp);

				order_header->amount -= curr->actual_price * curr->quantity; 

				fprintf(fp,"Head Node::order_header->amount=%.2f::\n",order_header->amount);
				fflush(fp);
				
				free(curr);

				break;
			}				
		}
		prev = curr;
		curr = curr->next;
	}
	fprintf(fp,"Order header amount:%.2f\n",order_header->amount);
	fflush(fp);
	
	curr=cart;
	while(curr!=NULL){
		prev = curr;
		curr = curr->next;
	}

	//Add product to last empty node
	fprintf(fp, "Last node:%d\n", prev);
	fflush(fp);
	
    strcpy(prev->product_id,product_id);
    
	if(add_item(card, prev, distribution) == ERROR){
		return ERROR;
	}
	
	return SUCCESS;
}

int add_item(CARD *card, CART_ITEM *cart_item, DISTRIBUTION *distribution) //Called from item_menu
{
	fprintf(fp,"Begin add_item()\n");
	fflush(fp);
			
	if(populate_cart(card, cart_item, distribution) == ERROR)
		return ERROR;
	
	if(show_item(cart_item) == ERROR)
		return ERROR;
	
	fprintf(fp,"This is the item added: %s\n",cart_item->product_id);
	fflush(fp);
}

int populate_cart(CARD *card, CART_ITEM *cart_item, DISTRIBUTION *distribution) {
	char sql[500];
	resultset ptable;
	BOOL ret;
	char strQty[10], tmp[100];
	float quantity, max_quantity;
	DISTRIBUTION *distCurr = distribution;
	int i=0;
	
	fprintf(fp, "Begin of populate_cart()");
	fflush(fp);
	
	while(distCurr != NULL)
	{

		fprintf(fp,"Distribution[%d]:product_id=%s,name=%s,card_type=%s,allotment_type=%s,quantity=%s,price=%s,uom=%s,max_qty=%s,scheme=%s,availed=%s\n",
				i, distCurr->product_id,distCurr->name,distCurr->card_type,distCurr->allotment_type,distCurr->quantity,distCurr->price,distCurr->uom,distCurr->max_quantity,distCurr->scheme_id,distCurr->availed);
		fflush(fp);
		
		if(strcmp(distCurr->product_id,cart_item->product_id)==0)
			break;
		
		distCurr = distCurr->next;
		i++;
	}
	
	fprintf(fp,"Current distribution:%d\n",distCurr);
	fflush(fp);
	
	//Need to check whether Card level or member level
	if(strcmp(distCurr->allotment_type,"2")==0) //Member Level
	{
		max_quantity = atoi(card->no_of_members)*atof(distCurr->quantity);
		if(max_quantity > atof(distCurr->max_quantity))
			max_quantity = atof(distCurr->max_quantity);
	}
	else
		max_quantity = atof(distCurr->max_quantity);

	fprintf(fp,"Max Quantity:%.2f, No. of members:%d, Max quantity per member:%s\n",max_quantity, atoi(card->no_of_members),distCurr->max_quantity);
	fflush(fp);
	

	lk_dispclr();

	sprintf(tmp,"%s Rs%.2f",distCurr->name, atof(distCurr->price));
	lk_disptext(3,0,tmp,0);
	
	sprintf(tmp,"Max:%.2f",max_quantity);
	lk_disptext(4,0,tmp,0);

	sprintf(tmp,"Availed:%.2f", atof(distCurr->availed));
	lk_disptext(5,0,tmp,0);

	setString("", "Quantity:",strQty, 20, FALSE);
	quantity = atof(strQty);


	if(atof(distCurr->availed) + quantity > max_quantity){
		prompt("Quantity exceeds eligible amount");
		
		quantity = 0.0;
		
		return ERROR;
	}
	
	fprintf(fp,"Quantity:%.2f, distribution:%d\n",quantity,distribution);
	fflush(fp);
	
	strcpy(cart_item->product_id,distCurr->product_id); 
	strcpy(cart_item->unique_code,distCurr->product_id); 
	strcpy(cart_item->product_name,distCurr->name);
	strcpy(cart_item->allotment_type,distCurr->allotment_type);
	cart_item->price_upper_limit = 0.0f;
	cart_item->price_lower_limit = 0.0f;

	fprintf(fp,"1. Max Quantity:%.2f, No. of members:%d, Max quantity per member:%s\n",max_quantity, atoi(card->no_of_members),distCurr->max_quantity);
	fflush(fp);

	cart_item->quantity = quantity;
	cart_item->actual_price = atof(distCurr->price);
	cart_item->vat = 0.0f;
	cart_item->uom=atoi(distCurr->uom);
	cart_item->max_quantity = atof(distCurr->max_quantity);
	cart_item->availed = atof(distCurr->availed);
	cart_item->scheme_id = atoi (distCurr->scheme_id);
	strcpy(cart_item->warranty_start_date,"01-01-1900");
	strcpy(cart_item->warranty_end_date,"01-01-1900");
	strcpy(cart_item->prod_code,distCurr->product_id);

	printCartItem(cart_item);
	
	fprintf(fp,"2. Max Quantity:%.2f, No. of members:%d, Max quantity per member:%s\n",max_quantity, atoi(card->no_of_members),distCurr->max_quantity);
	fflush(fp);

	order_header->amount += quantity * cart_item->actual_price;
			
	fprintf(fp, "End of populate_cart()");
	fflush(fp);

	return SUCCESS;
}

int show_cart() {
	char line[100];
	char total[10];

	fprintf(fp,"Begin show_cart()\n");
	fflush(fp);
	
	lk_dispclr();
	
	sprintf(line, "%s(%s)     %.2f", 
			get_prop("TOTAL", tmpbuf, OFFICE_ID),"Rs.",order_header->amount);
	lk_disptext(1,1,line,0);
	
	sprintf(line,"F2-%s","PAY AND PRINT");
	lk_disptext(3,1,line,0);
	sprintf(line,"F3-%s","CANCEL");
	lk_disptext(4,1,line,0);
	
	fprintf(fp,"Ended show_cart()\n");
	fflush(fp);
}

int set_order_id() {
	char sql[500];
	resultset ptable;
	BOOL ret;

	fprintf(fp, "Begin of set_order_id()");
	fflush(fp);

	sprintf(
			sql,
			"SELECT VALUE FROM CONFIGURATION WHERE KEY='MAX_ORDER_ID' AND OFFICE_ID='%s'",
			OFFICE_ID);

	ptable = get_result(sql);
	
	if (ptable.rows > 0 && strcmp(ptable.recordset[0][0], "0") != 0) {
		strcpy(order_header->order_id, ptable.recordset[0][0]);
		ret = SUCCESS;
	} else
		ret = ERROR;

	fprintf(fp,"Order id:%s\n",order_header->order_id);
	fflush(fp);

	free_result(ptable);

	return ret;
}

int updateOrderId()
{

	char sql[1000];
	
	int rc;
	
	//Update Order Id
	sprintf(sql,
			"UPDATE CONFIGURATION SET VALUE=VALUE+1 WHERE KEY='MAX_ORDER_ID'");
	rc = execute(sql);

	if (rc == 1) {
		fprintf(fp,
				"\nUpdated CONFIGURATION WITH ORDERS_IN_PROCESS successfully.");
		return SUCCESS;
	} else {
		fprintf(fp, "\nUpdate CONFIGURATION WITH ORDERS_IN_PROCESS failed.");
		return ERROR;
	}
	
}

int order_menu() {
	int key;
	int ret = ERROR;

	fprintf(fp,"Begin order_menu()\n");
	fflush(fp);
	
	while(TRUE)
	{
		key = lk_getkey();
		switch (key) {
				case PAYMENT:
//					payment();
					printFlag = TRUE;
					insert_order();
					print_bill();
					return SUCCESS;
					break;
				case VOID:
					return ERROR;
					break;
				default:
					return ERROR;
					
			}
	}
	
	fprintf(fp,"Ended order_menu()\n");
	fflush(fp);
}

int bill_menu() {
	
	fprintf(fp,"Begin bill_menu()\n");
	fflush(fp);	

	while (1) {
		fprintf(fp,"Begin bill_menu(): Inside while\n");
		fflush(fp);	

		switch (key) {
		case CART:
			edit_cart();
			return 0;
		case BILL:
			print_bill();
			break;
		case PAYMENT:
			payment();
		case VOID:
			clear_cart();
			break;
		}
	}
}

int edit_cart() {
	fprintf(fp,"Begin edit_cart()\n");
	fflush(fp);	

	fprintf(fp,"Ended edit_cart()\n");
	fflush(fp);	
}

int print_quote() {
	fprintf(fp,"Begin print_quote()\n");
	fflush(fp);	

	print_address("quote");
	print_order("quote");
	print_footer("quote");
	
	sprintf(tmpbuf,"%s","--------------------------------");

	fprintf(fp,"Ended print_quote()\n");
	fflush(fp);	
}

int print_address(char *type)
{
	fprintf(fp,"Begin print_address():%s\n",OFF_ADDRESS.company_name);
	fflush(fp);	

    sprintf(tmpbuf,"                BILL");
    ret=prn(tmpbuf,2);    

    sprintf(tmpbuf,"%s", "       CIVIL SUPPLIES DEPARTMENT");
    ret=prn(tmpbuf,2);
    sprintf(tmpbuf,"%s", "             GOVT OF AP");
    ret=prn(tmpbuf,2);
	
    sprintf(tmpbuf,"%s","-----------------------------------");
    ret=prn(tmpbuf,2);    
	
	sprintf(tmpbuf,"%s", OFF_ADDRESS.company_name);
    ret=prn(tmpbuf,2);

    sprintf(tmpbuf,"%s, %s",OFF_ADDRESS.address_line_1,OFF_ADDRESS.address_line_2);
    ret=prn(tmpbuf,1);
	
    sprintf(tmpbuf,"%s, %s",OFF_ADDRESS.address_line_3,OFF_ADDRESS.city );
    ret=prn(tmpbuf,1);

    if(strcmp(OFF_ADDRESS.state,"")!=0)
    {
	    sprintf(tmpbuf,"%s",OFF_ADDRESS.state);
	    ret=prn(tmpbuf,1);
    }

    if(strcmp(OFF_ADDRESS.postcode,"")!=0)
     {  
	    sprintf(tmpbuf,"%s",OFF_ADDRESS.postcode);
	    ret=prn(tmpbuf,1);
     }

    if(strcmp(OFF_ADDRESS.telephone,"")!=0 || strcmp(OFF_ADDRESS.mobile,""))
     {
	    sprintf(tmpbuf,"%s  %s",OFF_ADDRESS.telephone, OFF_ADDRESS.mobile);
	    ret=prn(tmpbuf,1);
     }

    if(strcmp(OFF_ADDRESS.fax,"")!=0)
    {
	    sprintf(tmpbuf,"%s",OFF_ADDRESS.fax);
	    ret=prn(tmpbuf,1);
    }
    
    if(strcmp(OFF_ADDRESS.e_mail,"")!=0)
    {
	    sprintf(tmpbuf,"%s",OFF_ADDRESS.e_mail);
	    ret=prn(tmpbuf,1);    
    }   

	fprintf(fp,"Ended print_address()\n");
	fflush(fp);	
}

int print_header(char *type)
{
	fprintf(fp,"Begin print_header()\n");
	fflush(fp);	

	sprintf(tmpbuf,"Order Ref: %s", order_header->order_id);
    ret=prn(tmpbuf,2);

	getTime ("%d/%m/%Y %H:%M:%S", tmpbuf);
    ret=prn(tmpbuf,2);
	
  
	fprintf(fp,"Ended print_header()\n");
	fflush(fp);	
}

int print_order(char *type)
{
	fprintf(fp,"Begin print_order()\n");
	fflush(fp);	

	print_item_header();
	print_items();

	fprintf(fp,"Ended print_order()\n");
	fflush(fp);	
}

int print_item_header()
{
	fprintf(fp,"Begin print_item_header()\n");
	fflush(fp);	

	sprintf(tmpbuf,"Item            Price  Qty   Tot");
	ret=prn(tmpbuf,2);
	
	fprintf(fp,"Ended print_item_header()\n");
	fflush(fp);	
}

int print_items()
{	
	char sql[500];
	BOOL ret;
	int i;
	CART_ITEM *cart_item = cart;
	char prev_cart_prod_id[100] = "";

	fprintf(fp,"Begin print_items()\n");
	fflush(fp);	

	for(i=0;cart_item!=NULL && strcmp(cart_item->product_id,"")!=0;i++)
	{
		fprintf(fp,"This is the product_id:%s\n",cart_item->product_id);
		fflush(fp);
		
//		print_prod_name(cart_item->product_name);
		print_product(cart_item);
		
		fprintf(fp,"This is the cart_item:%d\n",cart_item);
		fflush(fp);
		
		cart_item = cart_item->next;
	}
	ret = SUCCESS;
	
	fprintf(fp,"Ended print_items()\n");
	fflush(fp);	

	return ret;
}

int print_total()
{
	sprintf(tmpbuf,"Total Amount:             %7.2f", order_header->amount);
    ret=prn(tmpbuf,2);
}

int print_prod_name(char *name)
{
	int ret;

	fprintf(fp,"Begin print_prod_name()\n");
	fflush(fp);	

	ret = prn(name,0);
	
	fprintf(fp,"Ended print_prod_name()\n");
	fflush(fp);	
}

int print_product(CART_ITEM *cart_item)
{
	int ret;

	fprintf(fp,"Begin print_product()\n");
	fflush(fp);	

	sprintf(tmpbuf,"%-15s %5.2f %4.1f %6.2f",cart_item->product_name,cart_item->actual_price, cart_item->quantity, cart_item->quantity*cart_item->actual_price);
	ret = prn(tmpbuf,0);

	fprintf(fp,"Ended print_product()\n");
	fflush(fp);	
}

int print_footer(char *type)
{
	fprintf(fp,"Begin print_footer()\n");
	fflush(fp);	

    sprintf(tmpbuf,"%s",get_config("QUOTE_FOOTER_MSG",tmpbuf, OFFICE_ID));
    ret=prn(tmpbuf,0);
    
    if (ret != ERROR )
            prn_paper_feed(5);
    
	fprintf(fp,"Begin print_footer()\n");
	fflush(fp);	
}

int payment() {	
	fprintf(fp,"Begin payment()\n");
	fflush(fp);	

	show_payment_methods();
	payment_menu();
	
	fprintf(fp,"Ended payment()\n");
	fflush(fp);	
}

int payment_menu() {
	int key;
	int ret = ERROR;

	fprintf(fp,"Begin order_menu()\n");
	fflush(fp);
	
	while(TRUE)
	{
		key = lk_getkey();
		switch (key) {
				case CASH:
					print_quote();
					updateOrderId();
					ret = SUCCESS;
					break;
				case CREDIT:
					payment();
					bill_menu();
					ret = SUCCESS;
					break;
				case DEBIT:
					edit_cart();
					ret = SUCCESS;					
					break;
				case CHEQUE:
					clear_cart();
					ret = SUCCESS;
					break;
			}
		if(ret == SUCCESS)
			return SUCCESS;
	}
	
	fprintf(fp,"Ended order_menu()\n");
	fflush(fp);
}

int show_payment_methods() {
	char line[100];
	char total[10];

	fprintf(fp,"Begin show_payment_methods()\n");
	fflush(fp);	

	sprintf(line, "%s(%s)     %.2f", 
			get_prop("TOTAL", tmpbuf, OFFICE_ID),CURRENCY,order_header->amount);
	lk_disptext(1,1,line,0);
	sprintf(line,"F1-%s  F2-%s\n", get_prop("CASH", tmpbuf, OFFICE_ID), get_prop("CREDIT", tmpbuf, OFFICE_ID));
	lk_disptext(2,1,line,0);
	sprintf(line,"F3-%s  F4-%s\n",get_prop("DEBIT", tmpbuf, OFFICE_ID), get_prop("CHEQUE", tmpbuf, OFFICE_ID));
	lk_disptext(3,1,line,0);

	fprintf(fp,"Ended show_payment_methods()\n");
	fflush(fp);	

}

int populate_product(CART_ITEM *cart_item, resultset ptable,int i)
{	
	fprintf(fp,"Begin populate_product()\n");
	fflush(fp);	
	
	strcpy(cart_item->unique_code, ptable.recordset[i][0]);
	strcpy(cart_item->product_id, ptable.recordset[i][1]);
	strcpy(cart_item->product_name, ptable.recordset[i][2]);
	cart_item->actual_price = atof(ptable.recordset[i][3]);	
	cart_item->vat =  atof(ptable.recordset[i][4]);

	fprintf(fp,"product_id:%s",cart_item->product_id);
	fflush(fp);	
	fprintf(fp,"unique_code:%s",cart_item->unique_code);
	fflush(fp);	
	fprintf(fp,"product_name:%s",cart_item->product_name);
	fflush(fp);	
	fprintf(fp,"actual_price:%f",cart_item->actual_price);
	fflush(fp);	
		
	fprintf(fp,"Ended populate_product()\n");
	fflush(fp);	
}

int print_bill() {

	fprintf(fp,"Begin print_bill()\n");
	fflush(fp);	
	
	print_address("bill");

	sprintf(tmpbuf,"%s","-----------------------------------");
    ret=prn(tmpbuf,2);   

	print_card("bill");
	
	sprintf(tmpbuf,"%s","-----------------------------------");
    ret=prn(tmpbuf,2);   

	print_header("bill");

	sprintf(tmpbuf,"%s","-----------------------------------");
    ret=prn(tmpbuf,2);   

	print_order("bill");
	
    sprintf(tmpbuf,"%s","-----------------------------------");
    ret=prn(tmpbuf,2);   

	print_total();

	sprintf(tmpbuf,"%s","-----------------------------------");
    ret=prn(tmpbuf,2);   
	
	print_footer("bill");

	sprintf(tmpbuf,"%s","-----------------------------------");
    ret=prn(tmpbuf,2);   

	printFlag = FALSE;

	sprintf(tmpbuf,"%s","-----------------------------------");
	
	fprintf(fp,"Ended print_bill()\n");
	fflush(fp);	
}

int print_card()
{
	char tmp[100];
	
	sprintf(tmpbuf,"Card Holder: %s",trim(card->family_head));
	ret=prn(tmpbuf,2);

	if(ret!=ERROR)
	{
		sprintf(tmpbuf,"Id: %s",card->card_id);
		ret=prn(tmpbuf,2);
	}
	
	if(ret!=ERROR)
	{
		sprintf(tmpbuf,"Type: %s",getCardTypeName(card->card_type,tmp));
		ret=prn(tmpbuf,2);
	}

	if(ret!=ERROR)
	{
		sprintf(tmpbuf,"Members: %d",atoi(card->no_of_members));
		ret=prn(tmpbuf,2);
	}
}

char *getCardTypeName(char *card_type, char *card_type_name){
	char sql[1000];
	resultset ptable;
	int i;

	fprintf(fp, "Begin of getCardTypeName(), card_type:%s\n",card->card_type);
	fflush(fp);

	sprintf(sql,"select card_name from card_types where id='%d'",atoi(card->card_type));
					 
	fprintf(fp,"This is the sql:%s",sql);
	fflush(fp);
	
	ptable = get_result(sql);

	strcpy(card_type_name,"");
	
	if (ptable.rows > 0 && strcmp(ptable.recordset[0][0], "0") != 0) {
		strcpy(card_type_name,ptable.recordset[0][0]);
	}
	
	free_result(ptable);
	
	return card_type_name;
}

int bill() {
	fprintf(fp,"Begin bill()\n");
	fflush(fp);	
	

	fprintf(fp,"Ended bill()\n");
	fflush(fp);		
}

int clear_cart() {
	char *unique_code; //unique_code
	int i;
	int cnt;
	//SELECT ALL ITEMS IN ORDERS_IN_PROCESS

	fprintf(fp,"Begin remove_all()\n");
	fflush(fp);	
	
	for (i=0; i<cnt; i++) {
		//DELETE ITEM FROM ORDERS_IN_PROCESS
		remove_item(unique_code);
	}
	
	fprintf(fp,"Ended remove_all()\n");
	fflush(fp);	
}

int remove_item(char *unique_code) {
	fprintf(fp,"Begin remove_item()\n");
	fflush(fp);	
	
	fprintf(fp,"Ended remove_item()\n");
	fflush(fp);	
}


int show_item(CART_ITEM *cart_item) {
	char temp[50];
	
	fprintf(fp,"Begin show_item()\n");
	fflush(fp);	
	
	lk_dispclr();
	sprintf(temp, "Confirm Item");
	lk_disptext(1, 4, temp, 0);

	sprintf(temp, "%s", cart_item->product_name);
	lk_disptext(2,0, temp, 0);

	sprintf(temp, "Qty:%.2f  Price:%.2f", cart_item->quantity,cart_item->actual_price);
	lk_disptext(3,0, temp, 0);

	if(dispCE() == ERROR)
		return ERROR;
	
	fprintf(fp,"Ended show_item()\n");
	fflush(fp);	
	
	return SUCCESS;
}

int getItemPrice(CART_ITEM *cart_item)
{
	char temp[20];
	
	while(1)
	{
		setString("", "Item Price:", temp, 20, TRUE);
		
		if(strlen(temp) == 0)
			continue;
	
		if(cart_item->price_lower_limit > atof(temp) || cart_item->price_upper_limit < atof(temp))
		{
			if(prompt("Invalid price") == ERROR)
				continue;
			else
				break;
		}
	}

	cart_item->actual_price = atof(temp);

	return SUCCESS;
}

int insert_order()
{
	int rc;
	char sql[1000];
	CART_ITEM *cart_item = cart;

	fprintf(fp,"Begin insert_order():%s\n",order_header->order_id);
	fflush(fp);	
	
	printOrderHeader(order_header);
					
	sprintf (sql,
			"INSERT INTO ORDER_HEADER(OFFICE_ID,ID,CARD_ID,MEMBER, MEMBER_SLNO, CARD_TYPE,ORDER_DATE,ORDER_TYPE,AMOUNT,VAT, ORDER_DISCOUNT,PAYMENT_MODE,STATUS,REMARKS,USER_ID,RECONCILE_STATUS,SYNC_STATUS) \
				VALUES ('%s','%s','%s','%s','%d','%s',strftime('%s','now','localtime'),'%s','%.2f','%.2f','%.2f','%s','%s','%s','%s','%c','%c');",
				order_header->office_id,order_header->order_id,order_header->card_id,order_header->member,order_header->member_slno,order_header->card_type,"%%d-%%m-%%Y %%H:%%M:%%S",order_header->order_type, order_header->amount, 
				order_header->vat,order_header->discount,order_header->payment_mode,order_header->status, order_header->remarks,userName,'0','0');
	
	fprintf(fp,"Insert query:%s\n",sql);
	fflush(fp);
	
	rc = execute(sql);
	
	if (rc == 1) {
		fprintf(fp, "\nInsert into order_header successful.");
	} else {
		fprintf(fp, "\nInsert into order_header failed.");
		return ERROR;
	}

	fprintf(fp,"This is the cart head:%d\n",cart);
	fflush(fp);
	
	while(cart_item != NULL && strcmp(cart_item->product_id,"")!=0){
		
		printCartItem(cart_item);
		sprintf(sql,"INSERT INTO ORDER_TRAILER(PRODUCT_ID,UNIQUE_CODE,PRODUCT_NAME,ALLOTMENT_TYPE,PRICE_UPPER_LIMIT,LOWER_LIMIT,QUANTITY,ACTUAL_PRICE,VAT,UOM,MAX_QUANTITY,AVAILED,SCHEME_ID,WARRANTY_START_DATE,WARRANTY_END_DATE,PROD_CODE,ORDER_ID) VALUES( \
					'%s','%s','%s','%s','%.2f','%.2f','%.2f','%.2f','%.2f','%d','%.2f','%.2f','%d','%s','%s','%s','%s')",
			cart_item->product_id,cart_item->unique_code,cart_item->product_name,cart_item->allotment_type,
			cart_item->price_upper_limit,cart_item->price_lower_limit,cart_item->quantity,cart_item->actual_price,
			cart_item->vat,cart_item->uom,cart_item->max_quantity,cart_item->availed,cart_item->scheme_id,
			cart_item->warranty_start_date,cart_item->warranty_end_date,cart_item->prod_code,order_header->order_id);
		
		fprintf(fp,"Order_trailer:%s\n",sql);
		fflush(fp);
		
		rc = execute(sql);
		
		if (rc == 1) {
			fprintf(fp, "\nInsert into order_trailer successful.");
		} else {
			fprintf(fp, "\nInsert into order_trailer failed.");
			return ERROR;
		}
		
		cart_item=cart_item->next;
		fprintf(fp,"This is the cart's next item:%d\n",cart_item);
		fflush(fp);

	}
	fprintf(fp,"Ended insert_order()\n");
	fflush(fp);	
	
	return SUCCESS;
	
}

int insert_orders_in_process(CART_ITEM *cart_item) {
	int rc;
	char sql[1000];

	fprintf(fp,"Begin insert_orders_in_process():%s\n",order_header->order_id);
	fflush(fp);	
	fprintf(fp,"unique_code:%s\n",cart_item->unique_code);
	fflush(fp);	
	fprintf(fp,"actual_price:%f\n",cart_item->actual_price);
	fflush(fp);	
	fprintf(fp,"vat:%f\n",cart_item->vat);
	
	
	
	fflush(fp);	
			
	//Block Item
	sprintf (sql,
			"INSERT INTO ORDERS_IN_PROCESS(ID,UNIQUE_CODE,AMOUNT,VAT) VALUES ('%s','%s','%f','%f');",
					order_header->order_id,cart_item->unique_code, cart_item->actual_price,cart_item->vat);
	rc = execute(sql);
	
	if (rc == 1) {
		fprintf(fp, "\nInserted into orders_in_process successfully.");
		return SUCCESS;
	} else {
		fprintf(fp, "\nInserted into orders_in_process failed.");
		return ERROR;
	}

	fprintf(fp,"Ended insert_orders_in_process()\n");
	fflush(fp);	
}

int mark_item_blocked(char *unique_code) {
	int rc;
	char sql[1000];

	fprintf(fp,"Begin mark_item_blocked()\n");
	fflush(fp);	
	
	//Block Item
	sprintf(
			sql,
			"UPDATE PRODUCT_INSTANCE SET STATUS='5' WHERE UNIQUE_CODE='%s'",
			unique_code);
	rc = execute(sql);

	if (rc == 1) {
		fprintf(fp, "\nUpdated Product_instance successfully.");
		return SUCCESS;
	} else {
		fprintf(fp, "\nUpdate Product_instance failed.");
		return ERROR;
	}
	
	fprintf(fp,"Ended mark_item_blocked()\n");
	fflush(fp);	
}

BOOL is_item_available(char *unique_code) {
	//Validate userName and password
	char sql[500];
	resultset ptable;
	BOOL ret;

	fprintf(fp, "Begin of is_item_available()");
	fflush(fp);

	//PRODUCT_INSTANCE.STATUS = 0-ACCEPTED, 1-SUPPLY REJECT, 2-SHOP REJECT, 3-CUST REJECT, 4-WARRANTY EXPIRED,
	//							5-BLOCKED, 6-SOLD

	/****************************************************
	//Bypass Item availability check ********************
	****************************************************/
	return TRUE;
	
	sprintf(
			sql,
			"SELECT count(*) FROM PRODUCT_INSTANCE WHERE UNIQUE_CODE='%s' AND STATUS='0'",
			unique_code);
	
	ptable = get_result(sql);	
	
	if (ptable.rows > 0 && strcmp(ptable.recordset[0][0], "0") != 0) {
		ret = TRUE;
	} else
		ret = FALSE;

	fprintf(fp, "End of is_item_available:%s",
			(ret==TRUE ? "TRUE" : "FALSE"));
	fflush(fp);

	free_result(ptable);

	return ret;
}

void get_item_details() {

	fprintf(fp,"Begin get_item_details()\n");
	fflush(fp);	
	

	fprintf(fp,"Ended get_item_details()\n");
	fflush(fp);		
}

int display_member_menu(CARD *card, MEMBER *member)
{
        int c, i;
        int breakFlag=1;
    	MEMBER *memberCurr = member;

        MENU_T *menu, mainmenu;
        int opt=0;
        int selItem  = 0;
        int acceptKbdEvents=0;
        
        fprintf(fp,"Begun display_menu\n");
        fflush(fp);

        menu = &mainmenu;
        
	    menu->start          =       0;
	    menu->maxEntries     =       atoi(card->no_of_members);

        fprintf(fp,"About to show member menu, No. of members:%d\n",atoi(card->no_of_members));
        fflush(fp);

    	lk_dispclr();

        strcpy(menu->title,"Member List");

        for(i=0;i<atoi(card->no_of_members) && memberCurr != NULL;i++){
        		fprintf(fp,"Setting members:%d->%s\n",i,memberCurr->member);
        		fflush(fp);
        		if(strcmp(memberCurr->member,"")==0)
        		{
        			sprintf(menu->menu[i],"Member: %d", i);
        		}
        		else
        		{
        			strcpy(menu->menu[i],memberCurr->member);
        		}
                memberCurr = memberCurr->next;
        }
        opt = scroll_menu(menu,&selItem,acceptKbdEvents);

        switch(opt)
        {
           case CANCEL:
                       return ERROR;

           case ENTER:
        	   
           default: 
	   			strcpy(order_header->member,menu->menu[selItem]);
	   			order_header->member_slno=selItem+1;
	   			
	   			fprintf(fp,"This is the member:%s, sl.no.=%d\n",order_header->member,order_header->member_slno);
	   			fflush(fp);
	   			
        	   if(strcmp(getVal("authBio"),"true") == 0){
        		    show_thumb_menu(member);
        	   }
        	   else
        		  return SUCCESS;         		  
        }

        return SUCCESS;

        fprintf(fp,"End display_menu\n");
        fflush(fp);
}

int show_thumb_menu(MEMBER *member)
{
    int c, i;
    int breakFlag=1;
	MEMBER *memberCurr = member;
	
	char path[200];
	char fingerType[4];
    MENU_T *menu, mainmenu;
    int opt=0;
    int selItem  = 0;
    int acceptKbdEvents=0;
    
    fprintf(fp,"Begun Thumb menu\n");
    fflush(fp);

    menu = &mainmenu;
    
    menu->start          =       0;
    menu->maxEntries     =       3;

    fprintf(fp,"About to show thumb menu\n");
    fflush(fp);

	lk_dispclr();

    strcpy(menu->title,"Thumb List");

    strcpy(menu->menu[0],"Left Thumb finger");
    strcpy(menu->menu[1],"Left Index finger");
    strcpy(menu->menu[2],"Right Index finger");
    opt = scroll_menu(menu,&selItem,acceptKbdEvents);

   fprintf(fp, "Writing ltf to file:%d\n",memberCurr);
   fflush(fp);

   for(i=1;i<order_header->member_slno;i++){
	   memberCurr = memberCurr->next;
   }
   
    switch(opt)
    {
       case CANCEL:
                   return ERROR;

       case ENTER:
    	   
       default: 
    	   switch(selItem+1)
    	   {
    	   case 1:
    		   if(strcmp(getVal("authMode"),"rfid")==0)
    		   {
    				  sprintf(path,"/mnt/jffs2/LTF/%s_%d_LTF",card->card_id,order_header->member_slno);
    				  
    				  fprintf(fp, "Reading from path:%s\n",toLower(path));
    				  fflush(fp);
    				  
    				  convertTOUPEK(toLower(path),"/mnt/jffs2/test.bir");
    				  
    		   }
    		   else if(strcmp(getVal("authMode"),"smart")==0)
    		   {
// 				  	sprintf(path,"/mnt/jffs2/LTF/%s_%d_LTF",card->card_id,order_header->member_slno);
//    			   	StoreDataToFile( toLower(path), memberCurr->fp_ltf, memberCurr->sz_ltf);

    			   	StoreDataToFile( "/mnt/jffs2/test_iso.bir", memberCurr->fp_ltf, memberCurr->sz_ltf);
					fprintf(fp, "Writing ltf to file:%s::, bytes::%d\n",memberCurr->fp_ltf, memberCurr->sz_ltf);
					fflush(fp);
					
					convertTOUPEK("/mnt/jffs2/test_iso.bir","/mnt/jffs2/test.bir");
    		   }
    		   break;
    	   case 2:
    		   if(strcmp(getVal("authMode"),"rfid")==0)
    		   {
    				  sprintf(path,"/mnt/jffs2/LIF/%s_%d_LIF",card->card_id,order_header->member_slno);

    				  fprintf(fp, "Reading from path:%s\n",toLower(path));
    				  fflush(fp);
    				  
    				  convertTOUPEK(toLower(path),"/mnt/jffs2/test.bir");
    				  
    		   }
    		   else if(strcmp(getVal("authMode"),"smart")==0)
    		   {
//				  	sprintf(path,"/mnt/jffs2/LIF/%s_%d_LIF",card->card_id,order_header->member_slno);
// 			   		StoreDataToFile( toLower(path), memberCurr->fp_lif, memberCurr->sz_lif);

					StoreDataToFile( "/mnt/jffs2/test_iso.bir", memberCurr->fp_lif, memberCurr->sz_lif);
					fprintf(fp, "Writing lif to file:%s::, bytes::%d\n",memberCurr->fp_lif, memberCurr->sz_lif);
					fflush(fp);
					
					convertTOUPEK("/mnt/jffs2/test_iso.bir","/mnt/jffs2/test.bir");
    		   }
    		   break;

    	   case 3:
    		   if(strcmp(getVal("authMode"),"rfid")==0)
    		   {
    				  sprintf(path,"/mnt/jffs2/RIF/%s_%d_RIF",card->card_id,order_header->member_slno);

    				  fprintf(fp, "Reading from path:%s\n",toLower(path));
    				  fflush(fp);
    				  
    				  convertTOUPEK(toLower(path),"/mnt/jffs2/test.bir");    				  
    		   }
    		   else if(strcmp(getVal("authMode"),"smart")==0)
    		   {
//				  	sprintf(path,"/mnt/jffs2/RIF/%s_%d_RIF",card->card_id,order_header->member_slno);
// 			   		StoreDataToFile( toLower(path), memberCurr->fp_rif, memberCurr->sz_rif);

					StoreDataToFile( "/mnt/jffs2/test_iso.bir", memberCurr->fp_rif,memberCurr->sz_rif);
					fprintf(fp, "Writing rif to file:%s::, bytes::%d\n",memberCurr->fp_rif, memberCurr->sz_rif);
					fflush(fp);
            	   
					convertTOUPEK("/mnt/jffs2/test_iso.bir","/mnt/jffs2/test.bir");
    		   }
    		   break;
    		   

    	   }
    }
    

    
    return SUCCESS;

    fprintf(fp,"End display_menu\n");
    fflush(fp);	
}

