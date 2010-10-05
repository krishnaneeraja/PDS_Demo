#include "header.h"

union
{
 unsigned int temp;
 struct
   {
     unsigned char lsb;  //datalen lower byte.
     unsigned char msb;  //datalen higher byte.
   } uc;
} ui;

unsigned char READ[5]={0x00,0xB0,0x00,0x00,0xFF};
char response[2400];

int getCardDetails_smart1(CARD *card, char *line);
int getMemberDetails_Smart1(CARD *card, MEMBER *memberCurr, int i, char *response);//Get Card details - 0x04
int j;

int Ifd_test() 
{
int ret=0,res=0,count=0,brflag;

unsigned char Wrtbuf[100],temp_buf[100];

unsigned char cmd[][15]={
{0x00,0x20,0x00,0x00,0x05,0x55,0x55,0x55,0x55,0x55},//verify
{0x00,0xDA,0x00,0x00},//write
{0x00,0x21,0x00,0x00,0x05,0x55,0x55,0x55,0x55,0x55},//change password
{0x00,0xDC,0x00,0x01,0x05,0x05,0x04,0x03,0x02,0x01},//update/
{0x00,0xCA,0x00,0x04,0xff},//read
{0x00,0x22,0x00,0x00,0x02},//read records
{0x00,0x9C,0x00,0x00,0x00}//format
	};

MEMBER *memberCurr, *member;
CARD *card; 

unsigned char SELECT_MF[]  = {0x00,0xA4,0x00,0x00,0x02,0x3F,0x00};
unsigned char SELECT_DF[]  = {0x00,0xA4,0x00,0x00,0x02,0x2F,0x00};
unsigned char SELECT_EF[7]  = {0x00,0xA4,0x00,0x00,0x02,0x2F,0x03};


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
 

 cmdBuff[0] =  0x00;
 cmdBuff[1] =  0xB0;
 cmdBuff[2] =  (BYTE) offset >>8;  // Higher byte (MSB) of the offset to read
 cmdBuff[3] =  (BYTE ) offset;  // LSB of offset to read from ( it start fom 0 to.. 
 cmdBuff[4] =  (BYTE) len;  // Number of byte to read from the file , lenght is <=255 bytes
*/



//
  unsigned char temp[5]; 
  int i;
  
  if((member = (MEMBER *)malloc(sizeof(MEMBER)))==NULL)
	  return ERROR;
  
  if((card = (CARD *)malloc(sizeof(CARD))) == NULL)
	  return ERROR;
  
  member->next = NULL;
  
  ret=SCR_Open();
                if(ret<0)
                {
                        lk_dispclr();
                        lk_disptext(2,0,"IFD Open Failed ",0);
                        lk_disptext(3,0,"Check the Connection",0);
			lk_disptext(4,0,"Reboot ",0);
                        fprintf(stderr,"SCR Open Error\n");
                        lk_getkey();
 
                        
                }
	
                start:	

		lk_dispclr();
		lk_disptext(1,0,"IFD Menu",0);
		lk_disptext(2,2,"1.External IFD CARD",0);
		lk_disptext(3,2,"2.Internal SAM CARD",0);

		lk_disptext(5,0,"Enter Your Option",0);		

		while (1)
		{
		res = lk_getkey();	

		if(res==1) { Select_card(0x01); break; }

		else if(res==2) { Select_card(0x02); break;}

		else if(res==CANCEL)  return ERROR;
		}
		
                ret=SCR_PowerUP5();
                if(ret)
                {
                        lk_dispclr();
                        lk_disptext(2,0,"IFD PowerUP Failed",0);
                        lk_disptext(3,0,"Check the Connection",0);
			lk_disptext(4,0,"Reboot the System",0);
			lk_getkey();
                        fprintf(stderr,"SCR Open Error\n");
                        return ERROR;

                } 
		brflag=0;
		while(1)
		{

			ret=SCR_Checkup_CardPresence();
	                if(ret==12)
        	        {
                	        lk_dispclr();
                        	lk_disptext(2,0,"CARD NOT PRESENT",0);
	                        lk_disptext(3,0,"  INSERT CARD  ",0);
				brflag=1;
        	                sleep(2);
                	        break;
	                }
	
			
			lk_dispclr();
			lk_disptext(1,2,"1.Write Record",0);
			lk_disptext(2,2,"2.Read Record",0);	
			lk_disptext(3,2,"3.No.of Records",0);
			lk_disptext(4,2,"4.Format Card",0);
			lk_disptext(5,0,"Enter Your Option",0);

			while (1)
			{

				res=lk_getkey();	

				if(res==CANCEL) {  brflag=1; goto start; }

				 else if ( res > 0  &&  res < 5 )
					break;
			}


			switch(res)
        	        {

				case 0x01:
					       lk_dispclr();
					       memset(temp_buf,0,100);
					       memset(Wrtbuf,0,100);
					       count=lk_mgetalpha(2,0,"Enter The Data","","",50,temp_buf);
					       if(count<0)
					       {
								lk_dispclr();
								lk_disptext(2,0,"Operation Cancel",0);
								sleep(2);
								break;	
					        }
					        if(count>0)
					        {
				                temp_buf[count]='\0';
								memcpy(Wrtbuf,&cmd[1][0],4);
								Wrtbuf[4]=count;
				                memcpy(&Wrtbuf[5],temp_buf,count);
				                Wrtbuf[count+5]='\0';
        					}

							lk_dispclr();
							SendCommand_Smartcard(cmd[0],10);
							SendCommand_Smartcard(Wrtbuf,(count+5));
                                	break;
	            case 0x02:

					lk_dispclr();
//			                memset(temp,0,5);
//			                res=lk_mgetnumeric(2,0,"Enter Record No.","","",3,temp);
//			                if(res<=0)
//                        			break;
					lk_disptext(1,1,"Reading file..",0);
//					lk_getkey();
					
//					temp[res]='\0';
//					ui.temp=atoi(temp);
//					cmd[4][2]=ui.uc.msb;
//					cmd[4][3]=ui.uc.lsb;
//					if(SendCommand_Smartcard(cmd[4],5)==ERROR)
					if(SendCommand_Smartcard(SELECT_MF,7)==ERROR)
					{

                                              lk_dispclr();
                                              lk_disptext(2,0,"1.Record Not Present",1);
                                              sleep(2);

					}
					if(SendCommand_Smartcard(SELECT_DF,7)==ERROR)
					{

                                              lk_dispclr();
                                              lk_disptext(2,0,"2.Record Not Present",1);
                                              sleep(2);

					}		
					if(SendCommand_Smartcard(SELECT_EF,7)==ERROR)
					{
                                              lk_dispclr();
                                              lk_disptext(2,0,"2.Record Not Present",1);
                                              sleep(2);
					}		
					if(SendCommand_Smartcard(READ,5)==ERROR)
					{

                                              lk_dispclr();
                                              lk_disptext(2,0,"2.Record Not Present",1);
                                              sleep(2);

					}
					
					fprintf(fp,"This is the response:%s\n",response);
					fflush(fp);
					
					getCardDetails_smart1(card,response); //Get Card details - 0x03

					SELECT_EF[6]=0x04;
						
					fprintf(fp,"This is the current EF:%x, %d, i=%d\n",SELECT_EF[6], SELECT_EF[6],i);
					fflush(fp);
					
					if(SendCommand_Smartcard(SELECT_EF,7)==ERROR)
					{
                          lk_dispclr();
                          lk_disptext(2,0,"2.Record Not Present",1);
                          sleep(2);
					}		

					for(j=0;j<9;j++)
					{
						fprintf(fp, "This is the current offset:%d\n",READ[2]);
						fflush(fp);
						if(SendCommand_Smartcard(READ,5)==ERROR)
						{
                          lk_dispclr();
                          lk_disptext(2,0,"2.Record Not Present",1);
                          sleep(2);
						}

						READ[2]++;
					}
					READ[2]=0x00;

					getMemberDetails_smart1(card, memberCurr, i, response);//Get Card details - 0x04

					break;

				case 0x03:
					SendCommand_Smartcard(cmd[5],5);
					break;
				case 0x04:
					lk_dispclr();
					lk_disptext(2,3,"Formatting .. ",1);
					SendCommand_Smartcard(cmd[6],5);
					break;
				default:
					break;
			}//switch

		}//while
	if(brflag==1)goto start;
	SCR_Powerdown();

	SCR_Close();
    fprintf(fp,"Closing SCR\n");
    fflush(fp);

	return SUCCESS;
}//main

int SendCommand_Smartcard(unsigned char *commd, int clen )
{
        int  rlen=0,rec,i;
        unsigned char res[500],temp[500];
                                                                                                                             
        memset(res,0,100);
        SCR_SendCommand(commd,clen,res,&rlen);
                                                                                                                             
        if(rlen>0&&res[rlen-2]==0x90&&res[rlen-1]==0x00)
        {
			   if(commd[1]==0x22)
			    {
			            ui.uc.msb=res[0];
			            ui.uc.lsb=res[1];
			            rec=ui.temp;
			    }
			
			   for(i=0;i<5;i++){
				   fprintf(fp,"%02x ",commd[i]);
				   fflush(fp);
			   }
			   
			   if(commd[1]==0xB0)
			   {
					fprintf(fp,"This is the result::%s::\n",res);
					fflush(fp);
										
					for(i=0;i<rlen,res[i]!=0xff;i++)
					{
				       	response[j*255+i]=res[i];
					}
			   }
            return SUCCESS;
        }
        else
        {
                return ERROR;
        }
}

int getCardDetails_smart1(CARD *card, char *line){

	fprintf(fp,"Entered getCardDetails");
	fflush(fp);
	
	strncpy(card->card_id,line+0,15);
	card->card_id[15]='\0';
	strncpy(card->district,line+15,2);
	card->district[2]='\0';
	strncpy(card->mandal,line+17,2);
	card->mandal[2]='\0';
	strncpy(card->fpshop,line+19,3);
	card->fpshop[3]='\0';
	strncpy(card->no_of_members,line+22,2);
	card->no_of_members[2]='\0';
	strncpy(card->family_head,line+24,60);
	card->family_head[60]='\0';
	strncpy(card->card_type,line+84,3);
	card->card_type[3]='\0';
	strncpy(card->surname,line+87,60);
	card->surname[60]='\0';

	fprintf(fp,"Completed getCardDetails:card_id=%s,district=%s,mandal=%s,fpshop=%s,no_of_members=%s,\
			familyhead=%s,cardtype=%s,surname=%s\n",card->card_id,card->district,card->mandal,
			card->fpshop,card->no_of_members,card->family_head,card->card_type,card->surname);
	fflush(fp);
	
}

int getMemberDetails_smart1(CARD *card, MEMBER *memberCurr, int i, char *line){
	
	if(i > 0){
		if ((memberCurr = (MEMBER *)malloc(sizeof(MEMBER)))==NULL)
		{
			return ERROR;
		}
		fprintf(fp,"Member Curr=%d\n",memberCurr);
		fflush(fp);
	}
	memberCurr->next = NULL;
	
	fprintf(fp, "Processing line ::::%s\n with memberCurr:%d\n",line, memberCurr);
	fflush(fp);
	
	strncpy(memberCurr->slno,line+0,2);
	memberCurr->slno[2]='\0';
	strncpy(memberCurr->member,line+2,60);
	memberCurr->member[60]='\0';
	strncpy(memberCurr->age,line+62,3);
	memberCurr->age[3]='\0';
	strncpy(memberCurr->relation,line+65,2);
	memberCurr->relation[2]='\0';
	strncpy(memberCurr->fp_lif,line+67,512);
	memberCurr->fp_lif[512]='\0';
	strncpy(memberCurr->fp_rif,line+579,512);
	memberCurr->fp_rif[512]='\0';
	strncpy(memberCurr->fp_ltf,line+1603,1024);
	memberCurr->fp_ltf[1024]='\0';

	fprintf(fp, "Processing Member S.No.:%s, Member:%s, Age:%s, relation:%s, LIF:%s, RIF:%s, LTF:%s\n",
			memberCurr->slno, memberCurr->member, memberCurr->age, memberCurr->relation, memberCurr->fp_lif, memberCurr->fp_rif, memberCurr->fp_ltf);
	fflush(fp);

}

