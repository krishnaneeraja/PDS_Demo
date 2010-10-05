#include "header.h"

union {
	unsigned int temp;
	struct {
		unsigned char lsb; //datalen lower byte.
		unsigned char msb; //datalen higher byte.
	} uc;
} ui;

unsigned char READ[5]= { 0x00, 0xB0, 0x00, 0x00, 0xFF };
char response[2400];

int idx;

int res=0, count=0, brflag;

unsigned char Wrtbuf[100], temp_buf[100];

unsigned char SELECT_MF[] = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x3F, 0x00 };
unsigned char SELECT_DF[] = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x2F, 0x00 };
unsigned char SELECT_EF[7] = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x2F, 0x03 };

int Ifd_test() {

	unsigned char cmd[][15]= { { 0x00, 0x20, 0x00, 0x00, 0x05, 0x55, 0x55,
			0x55, 0x55, 0x55 },//verify
			{ 0x00, 0xDA, 0x00, 0x00 },//write
			{ 0x00, 0x21, 0x00, 0x00, 0x05, 0x55, 0x55, 0x55, 0x55, 0x55 },//change password
			{ 0x00, 0xDC, 0x00, 0x01, 0x05, 0x05, 0x04, 0x03, 0x02, 0x01 },//update/
			{ 0x00, 0xCA, 0x00, 0x04, 0xff },//read
			{ 0x00, 0x22, 0x00, 0x00, 0x02 },//read records
			{ 0x00, 0x9C, 0x00, 0x00, 0x00 }//format
	};

	MEMBER *memberCurr, *member;
	CARD *card;

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

	ret=SCR_Open();

	if (ret<0) {
		lk_dispclr();
		lk_disptext(2, 0, "IFD Open Failed ", 0);
		lk_disptext(3, 0, "Check the Connection", 0);
		lk_disptext(4, 0, "Reboot ", 0);
		fprintf(stderr, "SCR Open Error\n");
		lk_getkey();

	}

	start:

	Select_card(0x01);

	ret=SCR_PowerUP5();
	if (ret) {
		lk_dispclr();
		lk_disptext(2, 0, "IFD PowerUP Failed", 0);
		lk_disptext(3, 0, "Check the Connection", 0);
		lk_disptext(4, 0, "Reboot the System", 0);
		lk_getkey();
		fprintf(stderr, "SCR Open Error\n");
		return ERROR;

	}
	brflag=0;

	ret=SCR_Checkup_CardPresence();
	if (ret==12) {
		lk_dispclr();
		lk_disptext(2, 0, "CARD NOT PRESENT", 0);
		lk_disptext(3, 0, "  INSERT CARD  ", 0);
		brflag=1;
		sleep(2);
		return ERROR;
	}

	lk_dispclr();
	lk_disptext(1, 1, "Reading card...", 0);
	if (SendCommand_Smartcard(SELECT_MF, 7)==ERROR) {

		lk_dispclr();
		lk_disptext(2, 0, "1.Record Not Present", 1);
		sleep(2);

	}
	if (SendCommand_Smartcard(SELECT_DF, 7)==ERROR) {

		lk_dispclr();
		lk_disptext(2, 0, "2.Record Not Present", 1);
		sleep(2);

	}
	if (SendCommand_Smartcard(SELECT_EF, 7)==ERROR) {
		lk_dispclr();
		lk_disptext(2, 0, "2.Record Not Present", 1);
		sleep(2);
	}
	if (SendCommand_Smartcard(READ, 5)==ERROR) {

		lk_dispclr();
		lk_disptext(2, 0, "2.Record Not Present", 1);
		sleep(2);

	}

	//	fprintf(fp,"This is the response:%s\n",response);
	//	fflush(fp);

	return SUCCESS;
}//main


int read_card_smart(CARD *card, MEMBER *member) {
	int i=0;
	MEMBER *memberCurr = member;
	int ret=SUCCESS;

	strcpy(card->card_id, "");
	strcpy(response, "");
	idx = 0;

	READ[2]=0x00;
	READ[3]=0x00;
	READ[4]=0xFF;

	fprintf(fp, "1. Read Card Smart.About to read Card data:%s\n", card->card_id);
	fflush(fp);

	Ifd_test();

	fprintf(fp, "2. Read Card Smart.About to read Card data:%s\n", card->card_id);
	fflush(fp);

	if(getCardDetails_smart(card, response) == ERROR)
		ret = ERROR; //Get Card details - 0x03

//    strcpy(card->card_id,"WAP052000900023");
    
	fprintf(fp, "3. Read Card Smart.About to read Card data:%s\n", card->card_id);
	fflush(fp);
	sleep(2);

	if (strcmp(card->card_id, "")==0){
		ret=ERROR;
		goto end;
	}

	if (strcmp(getVal("authBio"), "true")==0) {
		if(readMembersFromCard(card, member) == ERROR){
			ret = ERROR;
			goto end;
		}
	} else {
		if (getMemberDetails_rfid(card, member, 0, "") == ERROR) {
			prompt("Error retrieving member data");
			ret=ERROR;
			goto end;
		}
	}

end:	

	SELECT_EF[6]=0x03;
	SCR_Powerdown();

	SCR_Close();
	
	fprintf(fp, "Closing SCR\n");
	fflush(fp);

	return ret;
}

int readMembersFromCard(CARD *card, MEMBER *member) {

	MEMBER *memberCurr = member, *memberPrev;
	int k;

	SELECT_EF[6]+=0x01;

	fprintf(fp, "This is the current EF:%x, %d\n", SELECT_EF[6], SELECT_EF[6]);
	fflush(fp);

	if (SendCommand_Smartcard(SELECT_EF, 7)==ERROR) {
		lk_dispclr();
		lk_disptext(2, 0, "2.Record Not Present", 1);
		sleep(2);
	}

	fprintf(fp, "This is the number of members:%d\n", atoi(card->no_of_members));
	fflush(fp);

	for (k=0; k<atoi(card->no_of_members); k++) {
		if (k!=0) {
			if ((memberCurr = (MEMBER *)malloc(sizeof(MEMBER))) == NULL)
				return ERROR;

			memberPrev->next = memberCurr;
			memberCurr->next = NULL;
		}
		if (readMember(k*2200, 2200) == ERROR)
			return ERROR;

		getMemberDetails_smart(card, memberCurr, k, response);

		memberPrev = memberCurr;
	}
}

int readMember(int pos, int recordSize) {
	int remLen = recordSize, fetchSize;

	int curPos = pos;

	strcpy(response, "");
	idx = 0;

	while (remLen>0) {
		if (remLen > 255)
			fetchSize = 255;
		else
			fetchSize = remLen;

		READ[2]=(char)(curPos >> 8);
		READ[3]=(char)(curPos & 0xff);
		READ[4]=(char)(fetchSize);

		//		fprintf(fp,"\nThis is the read:[2]=%x, [3]=%x, [4]=%x\n", READ[2], READ[3], READ[4]);
		//		fflush(fp);

		idx = curPos-pos;

		//		fprintf(fp,"Original Pos:%d, Reading from:%d to %d, idx:%d\n", pos, curPos, curPos + fetchSize, idx);
		//		fflush(fp);

		if (SendCommand_Smartcard(READ, 5)==ERROR) {
			lk_dispclr();
			lk_disptext(2, 0, "2.Record Not Present", 1);
			sleep(2);
		}

		remLen -= 255;
		curPos += 255;
	}

	return SUCCESS;
}

int readMembersFromCardOld(CARD *card, MEMBER *member) {
	int k, j;

	MEMBER *memberCurr = member, *memberPrev;

	SELECT_EF[6]+=0x01;

	fprintf(fp, "This is the current EF:%x, %d\n", SELECT_EF[6], SELECT_EF[6]);
	fflush(fp);

	if (SendCommand_Smartcard(SELECT_EF, 7)==ERROR) {
		lk_dispclr();
		lk_disptext(2, 0, "2.Record Not Present", 1);
		sleep(2);
	}

	fprintf(fp, "This is the number of members:%d\n", atoi(card->no_of_members));
	fflush(fp);

	for (k=0; k<atoi(card->no_of_members); k++) {
		if (k!=0) {
			if ((memberCurr = (MEMBER *)malloc(sizeof(MEMBER))) == NULL)
				return ERROR;

			memberPrev->next = memberCurr;
			memberCurr->next = NULL;
		}
		strcpy(response, "");

		for (j=0; j<8; j++) {
			fprintf(fp, "This is the current offset:%d\n", READ[2]);
			fflush(fp);
			if (SendCommand_Smartcard(READ, 5)==ERROR) {
				lk_dispclr();
				lk_disptext(2, 0, "2.Record Not Present", 1);
				sleep(2);
			}

			READ[3] = 0xFF;
			READ[4] = 0x01;
			if (SendCommand_Smartcard(READ, 5)==ERROR) {
				lk_dispclr();
				lk_disptext(2, 0, "2.Record Not Present", 1);
				sleep(2);
			}

			READ[2]++;
			READ[3]=0x00;
			READ[4]=0xFF;
		}
		READ[4]=0xA0;
		if (SendCommand_Smartcard(READ, 5)==ERROR) {
			lk_dispclr();
			lk_disptext(2, 0, "2.Record Not Present", 1);
			sleep(2);
		}

		getMemberDetails_smart(card, memberCurr, k, response);
		//		fprintf(fp,"This is the dump of member[%d]:%s\n",k, response);
		//		fflush(fp);

		memberPrev = memberCurr;

		READ[2]=0x00;
		READ[4]=0xFF;
	}
}

int SendCommand_Smartcard(unsigned char *commd, int clen) {
	int rlen=0, rec, i;
	unsigned char res[500], temp[500];

	memset(res, 0, 100);
	SCR_SendCommand(commd, clen, res, &rlen);

	if (rlen>0&&res[rlen-2]==0x90&&res[rlen-1]==0x00) {
		if (commd[1]==0x22) {
			ui.uc.msb=res[0];
			ui.uc.lsb=res[1];
			rec=ui.temp;
		}

		if (commd[1]==0xB0) {
			//				   fprintf(fp, "Size:%d, rlen=%d\n",idx, rlen);
			//				   fflush(fp);

			for (i=0; i<rlen, res[i]!=0xff; i++) {
//				fprintf(fp,"%d=%x~",idx,res[i]);
//				fflush(fp);

				response[idx++]=res[i];
			}
		}
		return SUCCESS;
	}
}

int getCardDetails_smart(CARD *card, char *line) {

	fprintf(fp, "Entered getCardDetails");
	fflush(fp);

	strncpy(card->card_id, line+0, 15);
	card->card_id[15]='\0';
	strncpy(card->district, line+15, 2);
	card->district[2]='\0';
	strncpy(card->mandal, line+17, 2);
	card->mandal[2]='\0';
	strncpy(card->fpshop, line+19, 3);
	card->fpshop[3]='\0';
	strncpy(card->no_of_members, line+22, 2);
	card->no_of_members[2]='\0';
	strncpy(card->family_head, line+24, 60);
	card->family_head[60]='\0';
	strncpy(card->card_type, line+84, 3);
	card->card_type[3]='\0';
	strncpy(card->surname, line+87, 60);
	card->surname[60]='\0';

	fprintf(
			fp,
			"Completed getCardDetails:card_id=%s,district=%s,mandal=%s,fpshop=%s,no_of_members=%s,\
			familyhead=%s,cardtype=%s,surname=%s\n",
			card->card_id, card->district, card->mandal, card->fpshop,
			card->no_of_members, card->family_head, card->card_type,
			card->surname);
	fflush(fp);

}

int getMemberDetails_smart(CARD *card, MEMBER *memberCurr, int i, char *line) {

	int sz;
	fprintf(fp, "Processing line ::::%s\n with memberCurr:%d\n", line,
			memberCurr);
	fflush(fp);
	/*
	 1. SLNO                     2 				0
	 2. MEMBERNAME     			60(30+30)		2
	 3. AGE                      3				62
	 4. RELATION               	2				65
	 5. FP_LIF                   512				67
	 6. FP_RIF                   512				579
	 7. FP_LTF                 	1024			1091
	 */

	strncpy(memberCurr->slno, line+0, 2);
	memberCurr->slno[2]='\0';
	strncpy(memberCurr->member, line+2, 60);
	memberCurr->member[60]='\0';
	strncpy(memberCurr->age, line+62, 3);
	memberCurr->age[3]='\0';
	strncpy(memberCurr->relation, line+65, 2);
	memberCurr->relation[2]='\0';

	memberCurr->sz_lif = (((int)(line[77]) << 8) | (int)(line[78]));
	fprintf(fp,"This is the LIF file size:%d\n",memberCurr->sz_lif);
	fflush(fp);
		
	bulkcopy(memberCurr->fp_lif, line+67, memberCurr->sz_lif);
	memberCurr->fp_lif[memberCurr->sz_lif]='\0';

	memberCurr->sz_rif = (((int)(line[589]) << 8) | (int)(line[590]));
	fprintf(fp,"This is the RIF file size:%d\n",memberCurr->sz_rif);
	fflush(fp);

	bulkcopy(memberCurr->fp_rif, line+579, memberCurr->sz_rif);
	memberCurr->fp_rif[memberCurr->sz_rif]='\0';

	memberCurr->sz_ltf = (((int)(line[1101]) << 8) | (int)(line[1102]));
	fprintf(fp,"This is the LTF file size:%d\n",memberCurr->sz_ltf);
	fflush(fp);
	
	bulkcopy(memberCurr->fp_ltf, line+1091, memberCurr->sz_ltf);
	memberCurr->fp_ltf[memberCurr->sz_ltf]='\0';

	fprintf(
			fp,
			"Processing Member S.No.:%s, Member:%s, Age:%s, relation:%s, LIF:%s, RIF:%s, LTF:%s\n",
			memberCurr->slno, memberCurr->member, memberCurr->age,
			memberCurr->relation, memberCurr->fp_lif, memberCurr->fp_rif,
			memberCurr->fp_ltf);
	fflush(fp);

}


int bulkcopy(char *dest, char *src, int sz)
{
	int i;
	for(i=0;i<sz;i++)
	{
		dest[i]=src[i];
	}
	return SUCCESS;
}

int readThumb_smart(char *thumbprint) {
	//For testing purposes only
	//get_config("thumbprint",thumbprint,"0");

}

int auth_card_smart(CARD *card, MEMBER *member, char *thumbprint) {
	MEMBER *memberCurr = member;

	//	readThumb_rfid(thumbprint);

	while (memberCurr != NULL) {
		fprintf(fp, "Checking member:%d\n", memberCurr);
		fflush(fp);

		if (check(memberCurr->fp_lif, thumbprint)==SUCCESS || check(
				memberCurr->fp_rif, thumbprint)==SUCCESS || check(
				memberCurr->fp_ltf, thumbprint)==SUCCESS) {
			strcpy(order_header->member, memberCurr->member);
			order_header->member_slno= atoi(memberCurr->slno);

			fprintf(fp, "Setting Order member to : %s, Sl no: %d\n",
					order_header->member, order_header->member_slno);
			fflush(fp);

			return SUCCESS;
		}

		memberCurr = memberCurr->next;
	}

	return ERROR;
}
