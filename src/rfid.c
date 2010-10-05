#include<header.h>
#define KEYA 0x0a
#define KEYB 0x0b

int rfid_test ( void )
{

        int i;


        unsigned char Auth_block_addr =3;

        unsigned char Read_block_addr =1;

		unsigned char Write_block_addr=1;

        unsigned char key [] = {0xff,0xff,0xff,0xff,0xff,0xff} ;   // transport key
 
        unsigned char write_data[32],read_data[32],tg_det=0,tag_id[10];

//        for(i=0; i<16; i++)   write_data[i] = i;   write_data[i]='\0';
        
        strcpy(write_data,"WAP052002200073");

        // this buffer is using for writing block  or reading block not for trailer 


        system("rmmod gsm_pstn >/dev/null 2>&1") ;


		lk_dispclr();
	
		fprintf(stdout,"Waiting For Target\n");
        lk_disptext(1,0,"Waiting for Target",0);


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

        if(tama_write_target(Write_block_addr,write_data,16) < 0)
          {
		fprintf(stderr,"Write Failed\n");
         	lk_disptext(3,0,"Write Failed",0);
                goto out;
           }

		fprintf (stdout,"Write Success\n");
		lk_disptext(3,0,"Write Success",0);

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

        fprintf (stdout,"\n") ;

        
out:
        tama_close();
	lk_disptext(5,0,"Press Any Key To Exit",0);
        lk_getkey();

        system("insmod /home/gsm_pstn.o >/dev/null 2>&1") ;
        
        return 0;
}
