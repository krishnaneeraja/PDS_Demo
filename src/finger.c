#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <header.h>
#include "ptapi.h"

#define ENROLL	    1
#define VERIFY	    2
#define CAPTURE     3
#define VERIFYMATCH 4
#define ENROLLUSERS 5
#define LOAD_TEMPLATES 5
#define VERIFYALL   6
#define GRAB	    7
#define EXIT	    6 //5 //cnu 040310
#define DEBUG	    1
#define MAX_BIRS    6
#define PT_GRAB_TYPE_381_381_8 2
int e=0;
int v=0;
int c=0;
//*********default timeout is 5000 msec **************//

void Print_Save_BIR_Template(int TYPE);
void* PTMalloc(PT_DWORD size)
{
    return malloc(size);
}
void* context;
PT_WORD guiState;
PT_BYTE *resp;
PT_WORD guiMsg;
PT_BYTE progress;
void *sampler;
PT_DATA *data1;
int n=50;
PT_STATUS myCallbackFn (void *pGuiStateCallbackCtx,PT_DWORD dwGuiState,PT_BYTE *pbyResponse,PT_DWORD guiMsg,PT_BYTE byProgress,
void *pSampleBuffer,PT_DATA *pData)

//PT_STATUS  myCallbackFn (PT_DWORD guiMsg)

{
	
	printf("\nValue of gui is %d\n",guiMsg);
	if(n==guiMsg)
	return PT_STATUS_OK;
	else
	{
	n=(int)guiMsg;
	printf("\nValue of N is %d\n",n);
	if(e)
	{	
	switch(guiMsg) 
		{
		case 12:
			printf("\nPut finger for Enrollment 1:\n");
			lk_dispclr();
			lk_disptext(2,0,"   Enroll Process",0);
			lk_disptext(4,0,"   Swipe  Finger :1",0);
			//sleep(3);			
			break;
		case 13:
			printf("\nPut finger for Enrollment 2:\n");
			lk_dispclr();
			lk_disptext(2,0,"   Enroll Process",0);
			lk_disptext(4,0,"   Swipe Finger :2",0);
			//sleep(3);
			break;
		case 14:
			printf("\nPut finger for Enrollment 3\n");
			lk_dispclr();
			lk_disptext(2,0,"   Enroll Process",0);
			lk_disptext(4,0,"   Swipe Finger :3",0);
			//sleep(3);
			break;
		case 15:
			printf("\nRemove finger \n");
			lk_dispclr();
			lk_disptext(2,0,"   Enroll Process",0);
			lk_disptext(4,0,"    Remove Finger ",0);
			//sleep(2);
			break;
		case 0:
			printf("\n Enroll Over\n");
			lk_dispclr();
			lk_disptext(2,0,"   Enroll Over",0);
			//sleep(2);
			break;
		default:
			break;
		}

	}
	if(v)
	{
	switch(guiMsg) 
		{
		case 12:
			printf("\nPut finger for Verification\n");
			lk_dispclr();
			lk_disptext(2,0,"   Verify Process",0);
			lk_disptext(4,0,"    Swipe Finger",0);
			break;
		case 0:
			printf("\nRemove finger \n");
			lk_dispclr();
			lk_disptext(2,0,"   Verify Over",0);
			lk_disptext(4,0,"  Remove Finger ",0);
			//sleep(2);
			break;
		default:
			break;
		}
	}
	if(c)
	{
	switch(guiMsg) 
		{
		case 12:
			printf("\nPut finger for Capturing finger\n");
			lk_dispclr();
			lk_disptext(2,0,"   Capture Process",0);
			lk_disptext(4,0,"     Swipe Finger",0);
			break;
		case 0:
			printf("\nRemove finger \n");
			lk_dispclr();
			lk_disptext(2,0,"   Capture Over",0);
			lk_disptext(4,0,"  Remove  Finger ",0);
			//sleep(2);
			break;
		default:
			break;
		}
	}
	return PT_STATUS_OK;

	}
}



void PTFree(void *pMemblock)
{
    free(pMemblock);
}
int w=0;
PT_MEMORY_FUNCS PTMemoryFun = { PTMalloc, PTFree };
PT_INFO *ppInfo;
PT_CONNECTION handle;
PT_BIR *ppNewTemplate;
PT_LONG plSlotNr;
PT_INPUT_BIR pStoredTemplate,pCapturedTemplate;
PT_BOOL pboResult,boCapture;

PT_INPUT_BIR pInputBirList[8]; //6 //cnu 040310



PT_GLOBAL_INFO *ppt_global_info; 
typedef struct Item {
    int index;
    PT_BIR *pBir;
}BIR_LIST;

BIR_LIST birlist[MAX_BIRS];
int finger_new()
{
	int opt=0,ret;
        MENU_T menu;
        char event;
        int selItem  = 0;
        int acceptKbdEvents=TRUE;
        int maxEntries = 0;
        short scroll = 0;
        char index[10]="";
	int ch=0;
	FILE *fp;
	int n = 0;
	unsigned char *Buff;
	int ID,i;
	char key;
	unsigned char *pInputBuffer = NULL;
    	unsigned char *pOutputBuffer = NULL;
	char file[25]="/mnt/jffs2/test.bir"; 
    char file1[20]="CAPTURE.bir";
	char ID0[20]="";
	int status;
        char ID1[20]; //="100_1.bir";//cnu 040310
        //char ID2[20]="100_2.bir";//cnu 040310
        //char ID3[20]="100_3.bir";//cnu 040310

 	//PT_INPUT_BIR *pInputBirList[3]; //cnu 040310

    	PT_DWORD imageWidth, imageHeight, alignedWidth;
	PT_DATA *pImage = NULL;
	PT_SHORT *pBitmap = NULL;
	PT_BIR *pNewBir = NULL;
	PT_BOOL ptfalse,pttrue; 
	PT_STATUS Status=-1;
	//PT_CHAR dsn[]="usb,vid=0x0483,pid=0x2016";// for blue reader(old)
	PT_CHAR dsn[]="usb,vid=0x147e,pid=0x2016";//for new reader
	PT_DWORD pdwScanQuality;          
	PT_LONG plMaxFRRRequested = 2;
	PT_LONG maxFarRequested = 1;
	PT_SHORT matchResult = -1;
	PT_DWORD ReadBytes;  
	menu.start=0;
        menu.maxEntries=6; //5;//CNU  040310
        maxEntries=menu.maxEntries;
  	printf("\n MAX is %d",menu.maxEntries);
	ret=lk_open();
	printf("\n Value of ret is %d",ret);
	ret=lk_dispinit();
	printf("\n Value of ret for init is %d",ret);
	printf("Initializing PTAPI...\n");
	Status = PTInitialize(NULL);
	if(Status)
	{
		fprintf(stdout,"PTInitialize Status= %#x\n",Status);
//		exit(EXIT_FAILURE);
	}
	fprintf(stdout,"PTlibrary Successfully initialized\n");
	Status = PTOpen(dsn,&handle);
	if(Status)
	{
		fprintf(stdout,"PTOpen Status= %d\n",Status);
	}
	if(PT_STATUS_OK == PTGlobalInfo(&ppt_global_info))
	{	
		fprintf(stdout,"\n\n\nGlobal Info\n");
		fprintf(stdout,"ApiVersion:%d\n",ppt_global_info->ApiVersion);
		fprintf(stdout,"Functionality:%d\n",ppt_global_info->Functionality);
		fprintf(stdout,"Flags:%d\n\n\n\n",ppt_global_info->Flags);
	}
	PTFree(ppt_global_info);
	
	if(PT_STATUS_OK == PTInfo(handle,&ppInfo))
	{
		
		fprintf(stdout,"\n\n\nPTInfo\n");
		fprintf(stdout,"FwVersion %d\n",ppInfo->FwVersion);
		fprintf(stdout,"FwMinNextVersion%d\n",ppInfo-> FwMinNextVersion);
		fprintf(stdout,"FwVariant %d\n",ppInfo->FwVariant);
		fprintf(stdout,"FwFunctionality %d\n",ppInfo->FwFunctionality);
		fprintf(stdout,"FwConfig %d\n",ppInfo->FwConfig);
		fprintf(stdout,"Id %d\n",ppInfo->Id);
		fprintf(stdout,"AuthentifyId %d\n",ppInfo->AuthentifyId);
		fprintf(stdout,"Usage %d\n",ppInfo->Usage);
		fprintf(stdout,"SensorType %d\n",ppInfo->SensorType);
		fprintf(stdout,"ImageWidth %d\n",ppInfo->ImageWidth);
		fprintf(stdout,"ImageHeight %d\n",ppInfo->ImageHeight);
		fprintf(stdout,"MaxGrabWindow %d\n",ppInfo->MaxGrabWindow);
		fprintf(stdout,"CompanionVendorCode %d\n",ppInfo->CompanionVendorCode);
	}
	PTFree(ppInfo);
	Status = PTSetGUICallbacks (handle, NULL, NULL,myCallbackFn, NULL);
	if(Status==0)
	printf("\nCallBack function has been set\n");
	

				pStoredTemplate.byForm = PT_SLOT_INPUT;
				pStoredTemplate.InputBIR.lSlotNr = plSlotNr;
				lk_dispclr();
				lk_disptext(2,0,"    Verify Starts",1);
				//sleep(2);				
				v=1;
				printf("Value of v is %d",v);					
				w=LoadDataFromFile( file, &pInputBuffer, &ReadBytes ); //Master thumbprint file
				if(w<0)
				{
					printf("\n Enrolled file(test.bir) not present\n");
					lk_dispclr();
					lk_disptext(2,0," Verify file not present",1);
					//sleep(2);
					v=0;					
					printf("Value of v is %d",v);
					return ERROR;
				}				
				pStoredTemplate.byForm = PT_FULLBIR_INPUT;
				
				pStoredTemplate.InputBIR.pBIR = (PT_BIR *)pInputBuffer;
			
			
				boCapture = PT_TRUE ;
			        if (PT_STATUS_OK == PTVerify(handle,NULL,NULL,NULL,&pStoredTemplate,NULL,&pboResult,NULL,NULL,NULL,60000,boCapture,NULL,NULL,NULL))
				{
				fprintf(stdout,"Verify success\n");
				lk_dispclr();
				lk_disptext(2,0,"    Verify Over",1);				
				//sleep(2);
				}
				if(pboResult)
					{
						fprintf(stdout,"FingerPrint Matched\n");
						lk_dispclr();
						lk_disptext(2,0,"    Finger Matched",1);
						return SUCCESS;
						//sleep(2);
					}
				else
					{					
					fprintf(stdout,"FingerPrint NOT Matched\n");
					lk_dispclr();
					lk_disptext(2,0,"    Not Matched",1);
					return ERROR;
					//sleep(2);					
					}
				PTClose(handle);
				PTTerminate();
				return SUCCESS;
			
}

void Print_Save_BIR_Template(int TYPE)
{
	FILE *fp;
	int i=0,len=0;
	unsigned char *buff;
	int type;

	type = TYPE;

	buff = (unsigned char *) malloc(2048);
	if(type==ENROLL)
	{

		fp = fopen("test.bir","w");
		if(fp==NULL){
			fprintf(stderr,"Unbable to open file \n");
			exit(EXIT_FAILURE);
		}
	}
	if(type==CAPTURE)
	{

		fp = fopen("CAPTURE.bir","w");
		if(fp==NULL){
			fprintf(stderr,"Unbable to open file \n");
			exit(EXIT_FAILURE);
		}

	}

	memcpy(buff,(unsigned char *)ppNewTemplate,ppNewTemplate->Header.Length+2);	
	len = fwrite(buff,1,ppNewTemplate->Header.Length,fp);
	fclose(fp);
	/*while (i< len )
		fprintf(stdout,"%#x ",buff[i++]);
	puts("");
*/
	
	free(buff);
}


PT_LONG LoadDataFromFile( const char *pFileName, unsigned char **ppBufer, PT_LONG *pLenData )
{
    PT_LONG retVal = 0;
    PT_DWORD fileLength = 0,len,i;
    unsigned char *buff;
    unsigned char buff1[500];
    FILE *fil = NULL;

    fil = fopen( pFileName, "rb" );
    if ( fil != NULL )
    {
       
        fseek( fil, 0, SEEK_END );
        fileLength = ftell( fil );

        if ( fileLength > 0 )
        {
            *ppBufer = (unsigned char *) malloc( fileLength );
            *pLenData = (PT_DWORD) fileLength;

	    printf("\n\n test file length:%d\n\n",fileLength);

            fseek( fil, 0, SEEK_SET );
      
            fread( *ppBufer, 1, fileLength, fil );
            			           
        }
        else
        {
            
            retVal = -2;
        }

        fclose( fil );
    }
    else
    {
        
        retVal = -1;
    }

    return retVal;
}
