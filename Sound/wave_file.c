
  #include<stdio.h>
  #include<stdlib.h>
  #include "soundapp.h"

  int LittleEndianIntRead(int num_bytes,FILE *fp);

  int get_wave(char path[], struct wave *wave0)
  {

	int i;
	FILE *fp;

//	signed short int temp[1];
//	signed short int *ptemp;

        char WavePath[40]="/mnt/jffs2/";
                                                                                   
// 	ptemp=&temp[0];
        strcat(WavePath,path);

	                                                                          
//	wave0->channel1=NULL;
//	wave0->channel2=NULL;

	fp=fopen(WavePath,"rb");
//	fp=fopen("/mnt/jffs2/gotmail.wav","rb");
	
	if(fp==NULL){
		printf("Could not open file : %s\n",WavePath);		
		return -1;
	}

	rewind(fp); // points to starting position of file

	fseek (fp , 4 , SEEK_CUR );
	
	wave0->total_chunk_size=LittleEndianIntRead(4,fp);

	fseek (fp , 4 , SEEK_CUR );
	fseek (fp , 4 , SEEK_CUR );
	fseek (fp , 4 , SEEK_CUR );
	fseek (fp , 2 , SEEK_CUR );

	wave0->number_of_channel=LittleEndianIntRead(2,fp);
	
	wave0->samples_per_second=LittleEndianIntRead(4,fp);
	
	wave0->bytes_per_second=LittleEndianIntRead(4,fp);
	
	wave0->block_align=LittleEndianIntRead(2,fp);
	
	wave0->bits_per_sample=LittleEndianIntRead(2,fp);
	
	fseek (fp , 4 , SEEK_CUR );
	wave0->second_chunk_size=LittleEndianIntRead(4,fp);

	wave0->num_blocks=(wave0->second_chunk_size)/(wave0->block_align); //number of blocks;
	
	wave0->time_length=(float)wave0->num_blocks;
	wave0->time_length/=(float)wave0->samples_per_second;


	if(wave0->number_of_channel==2){
                wave0->channel1=malloc(wave0->num_blocks*sizeof(float));
                wave0->channel2=malloc(wave0->num_blocks*sizeof(float));
        }
        else{
                printf("This program only supports 2 channels waves:\n");
                fclose(fp);
                return 0;
        }
                                                                                                 
/*        for(i=0;i<wave0->num_blocks;i++){
                        fread (ptemp,1,(wave0->bits_per_sample)/8,fp);
                        wave0->channel1[i]=(float)temp[0]/32768.0f;
                                                                                                 
                        fread (ptemp,1,(wave0->bits_per_sample)/8,fp);
                        wave0->channel2[i]=(float)temp[0]/32768.0f;
        }
*/

	fclose(fp);
	return 0;

}

int LittleEndianIntRead(int num_bytes,FILE *fp){
	int i,s=0;
	for(i=0;i<num_bytes;i++){
		s+=(1<<i*8)*getc(fp);
	}
	return(s);
}
