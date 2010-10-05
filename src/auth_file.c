#include "header.h"

int read_card_file(CARD *card, MEMBER *member)
{
	  FILE *file = fopen ("/mnt/jffs2/card.txt", "r");
	  char line[LINESIZE];
	  int i=0;
	  MEMBER *memberCurr = member;
	  
	  if(file == NULL)
		  return ERROR; 
	  
	  while (fgets (line, sizeof line, file) != NULL){
		  fprintf(fp,"Processing line:%s with length:%d\n",line,strlen(line));
		  fflush(fp);
		  
		  if(line[0]=='#')
			  continue;
		  
		  if(strlen(line) == 148)
			  getCardDetails(card,line);
		  else if(strlen(line) > 1000){
			  getMemberDetails(card, memberCurr, i++, line);
			  
			  memberCurr = memberCurr->next;
		  }
	  }
	  fclose(file);
	  
	  return SUCCESS;
}

int getCardDetails_file(CARD *card, char *line){

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

int getMemberDetails_file(CARD *card, MEMBER *memberCurr, int i, char *line){
	
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
}

int readThumb_file(char *thumbprint){
	//For testing purposes only
	get_config("thumbprint",thumbprint,"0");
}

int auth_card_file(CARD *card, MEMBER *member, char *thumbprint)
{
	MEMBER *memberCurr = member;
	
	while(memberCurr != NULL){
		fprintf(fp, "Checking member:%d\n",memberCurr);
		fflush(fp);
		
		if(check(memberCurr->fp_lif,thumbprint)==SUCCESS || check(memberCurr->fp_rif,thumbprint)==SUCCESS || check(memberCurr->fp_ltf,thumbprint)==SUCCESS){
			strcpy(order_header->member,memberCurr->member);
			order_header->member_slno= atoi(memberCurr->slno);
			
			fprintf(fp,"Setting Order member to : %s\n",order_header->member);
			fflush(fp);
			
			return SUCCESS;
		}

		memberCurr = memberCurr->next;
	}
	
	return ERROR;
}
