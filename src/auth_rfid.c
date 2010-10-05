#include "header.h"
#define KEYA 0x0a
#define KEYB 0x0b

int rfid_setup(CARD *card)
{

        int i;


        unsigned char Auth_block_addr =3;

        unsigned char Read_block_addr =1;

		unsigned char Write_block_addr=1;

        unsigned char key [] = {0xff,0xff,0xff,0xff,0xff,0xff} ;   // transport key
 
        unsigned char write_data[32],read_data[32],tg_det=0,tag_id[10];

//      for(i=0; i<16; i++)   write_data[i] = i;   write_data[i]='\0';
        
        //strcpy(write_data,"WAP052002200073");

        // this buffer is using for writing block  or reading block not for trailer 

        system("rmmod gsm_pstn >/dev/null 2>&1") ;

		lk_dispclr();
	
		fprintf(stdout,"Show customer tag\n");
        lk_disptext(1,0,"Show customer tag",0);

        if(tama_open() < 0)  { fprintf(stderr ,"Device Open Failed\n" );  return -1; }       
 
        if(tama_detectTarget( 0x01,&tg_det,tag_id ) < 0)
        {
		fprintf(stderr,"No Target In the RFID Field\n");
                lk_disptext(2,0,"Target not Detected",1);
                goto out;
        }

        fprintf (stdout,"Target Detected\n");
        lk_dispclr(); 
		lk_disptext(1,0,"Target Detected",0);
	
		if(tama_authenticate_mifare_key (1,Auth_block_addr,key,KEYA) !=0) // Transport key Configuration
	          {
			fprintf(stderr,"Authentication Failed\n");
			lk_disptext(2,0,"Authentication...",0);
			lk_disptext(3,0,"Failed",1);
			goto out;
		  }

		fprintf(stdout,"Authentication Success\n");
		lk_disptext(2,0,"Authenticated",0);

/***		
        if(tama_write_target(Write_block_addr,write_data,16) < 0)
          {
		fprintf(stderr,"Write Failed\n");
         	lk_disptext(3,0,"Write Failed",0);
                goto out;
           }

		fprintf (stdout,"Write Success\n");
		lk_disptext(3,0,"Write Success",0);
***/
	memset (read_data, 0x00,32);

	if(tama_read_target(Read_block_addr,read_data) < 0)
         {
		fprintf(stderr,"Read Failed\n");
		lk_disptext(4,0,"Read Failed",0);
		goto out;
	 }
	
	fprintf (stdout,"Read Success and Data is\n");
	lk_disptext(4,0,"Read Success",0);
	
	for (i=0;i<16;i++)
	fprintf(stderr," 0x%2x  ",read_data[i]);

	strcpy(card->card_id,read_data);
	
    fprintf (stdout,"\n") ;
        
out:
        tama_close();
        lk_disptext(5,0,"Press Any Key To Exit",0);
        lk_getkey();

        system("insmod /home/gsm_pstn.o >/dev/null 2>&1") ;
        
        return 0;
}


int read_card_rfid(CARD *card, MEMBER *member)
{
	  int i=0;
	  MEMBER *memberCurr = member;

	  strcpy(card->card_id,"");
	  
	  rfid_setup(card);

//	  strcpy(card->card_id,"WAP052002100054");
	  
      fprintf(fp,"Read Card RFID.About to read Card data:%d\n",card);
      fflush(fp);

	  if(strcmp(card->card_id,"")==0)
		  return ERROR;
	  
	  if(getCardDetails_rfid(card,"") == ERROR){
		  prompt("Error retrieving card data");
		  return ERROR;
	  }
	  if(getMemberDetails_rfid(card, member,0,"") == ERROR){
		  prompt("Error retrieving member data");
		  return ERROR;
	  }
	  
	  return SUCCESS;
}

int getCardDetails_rfid(CARD *card, char *line){

	//Validate userName and password
    char sql[500];
    resultset table2;
    int ret;

	fprintf(fp,"Entered getCardDetails");
	fflush(fp);

    sprintf(sql,"SELECT CARD_ID,DISTRICT, MANDAL,NO_OF_MEMBERS,FAMILY_HEAD,CARD_TYPE,SURNAME \
    		FROM CARD_DETAILS A WHERE CARD_ID='%s'",card->card_id);
	
    table2 = get_result(sql);
                                              
    if(table2.rows > 0 && strcmp(table2.recordset[0][0], "0") != 0)
    {
    		strcpy(card->district,table2.recordset[0][1]);
    		strcpy(card->mandal,table2.recordset[0][2]);
    		strcpy(card->fpshop,getVal("shop_id"));
    		strcpy(card->no_of_members,table2.recordset[0][3]);
    		strcpy(card->family_head,table2.recordset[0][4]);
    		strcpy(card->card_type,table2.recordset[0][5]);
    		strcpy(card->surname,table2.recordset[0][6]);
    		ret = SUCCESS;
    }
    else
            ret = CARD_DOES_NOT_EXIST;

    fprintf(fp,"End of getCardDetails\n");
    fflush(fp);

	fprintf(fp,"Completed getCardDetails:card_id=%s,district=%s,mandal=%s,fpshop=%s,no_of_members=%s,\
			familyhead=%s,cardtype=%s,surname=%s\n",card->card_id,card->district,card->mandal,
			card->fpshop,card->no_of_members,card->family_head,card->card_type,card->surname);
	fflush(fp);
    
    free_result(table2);

    return ret;
	
}

int getMemberDetails_rfid(CARD *card, MEMBER *member, int j, char *line){

	//Validate userName and password
    char sql[500];
    resultset table;
    int ret;
    MEMBER *memberCurr=member, *memberPrev;
    int i=0;
    
	fprintf(fp,"0.RFID:Entered getMemberDetails:Card:%d, Member:%d, Card id:%s\n",card,member, card->card_id);
	fflush(fp);

    sprintf(sql,"SELECT SLNO,MEMBERNAME,AGE,RELATION,ifnull(FP_LIF,'') FP_LIF,ifnull(FP_RIF,'') FP_RIF,ifnull(FP_LTF,'') FP_LTF \
    		FROM MEMBERS WHERE CARD_ID='%s' order by slno",card->card_id);
	
    table = get_result(sql);
                                          
    fprintf(fp,"RFID:No of rows:%d\n",table.rows);
    fflush(fp);
    
    if(table.rows > 0 && strcmp(table.recordset[0][0], "0") != 0)
    {
    	for(i=0; i<table.rows;i++)
    	{

    		if(i != 0){
				if ((memberCurr = (MEMBER *)malloc(sizeof(MEMBER)))==NULL)
				{
					return ERROR;
				}
    			memberPrev->next = memberCurr;

				fprintf(fp,"Member Curr=%d, Next=%d\n",memberCurr, memberCurr->next );
				fflush(fp);
    		}
			memberCurr->next = NULL;

    		fprintf(fp,"Table details:%s, %s, %s\n",table.recordset[i][0],table.recordset[i][1], table.recordset[i][2]);
    		fflush(fp);
    		
			strcpy(memberCurr->slno,table.recordset[i][0]);
			if(memberCurr->slno!=1)
				strcpy(memberCurr->member,table.recordset[i][1]);
			else
				strcpy(memberCurr->member,card->family_head);
/**
			strcpy(memberCurr->age,table.recordset[i][2]);
			strcpy(memberCurr->relation,table.recordset[i][3]);
			strcpy(memberCurr->fp_lif,table.recordset[i][4]);
			strcpy(memberCurr->fp_rif,table.recordset[i][5]);
			strcpy(memberCurr->fp_ltf,table.recordset[i][6]);
**/			
			memberPrev = memberCurr;
    	}


    	sprintf(card->no_of_members,"%d",table.rows);

    	return SUCCESS;
	}
    else
    	return ERROR;

}

int readThumb_rfid(char *thumbprint){
	//For testing purposes only
	//get_config("thumbprint",thumbprint,"0");
	
	finger_new();
}

int auth_card_rfid(CARD *card, MEMBER *member, char *thumbprint)
{
	MEMBER *memberCurr = member;
	
//	readThumb_rfid(thumbprint);
	
	while(memberCurr != NULL){
		fprintf(fp, "Checking member:%d\n",memberCurr);
		fflush(fp);
				
		if(check(memberCurr->fp_lif,thumbprint)==SUCCESS || check(memberCurr->fp_rif,thumbprint)==SUCCESS || check(memberCurr->fp_ltf,thumbprint)==SUCCESS){
			strcpy(order_header->member,memberCurr->member);
			order_header->member_slno= atoi(memberCurr->slno);
			
			fprintf(fp,"Setting Order member to : %s, Sl no: %d\n",order_header->member, order_header->member_slno);
			fflush(fp);
			
			return SUCCESS;
		}

		memberCurr = memberCurr->next;
	}
	
	return ERROR;
}
