#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"soundapp.h"

#define OSS_GETVERSION          0x80044d76
#define SNDCTL_DSP_SYNC         0x5001
#define SNDCTL_DSP_GETCAPS      0x8004500f
#define SNDCTL_DSP_RESET        0x5000
#define SNDCTL_DSP_SPEED        0xc0045002
#define SNDCTL_DSP_STEREO       0xc0045003


int fd;
int sound_open()
{

        fd = open ("/dev/dsp",O_WRONLY);
        if (fd < 0){
                printf ("Unable to open DSP device\n") ;
                return -1 ;
        }
	return 0;
}

//int sound_write(char *path1 ,int Frequ[],int mono)
int sound_write(char *path1 ,int Freq,int mono)
{      

	int ret=0;
	int raw_fd;
	int len,flen ;
	int speed;
	unsigned int rbuf[4096+1];
	char path[]="/mnt/jffs2/";


	strcat(path,path1);

//	if(Freq>=10000 && Freq<20000) 
	if(Freq>=11000 && Freq<11999) 		// exact frq: 11025 hz
		speed=0;  
	
//	else if(Freq>=20000 && Freq<35000) 
	else if(Freq>=22000 && Freq<22999)  	//exact freq: 22050 hz
		speed=1;  
	
	else if(Freq>=30000 && Freq<50000)
//	else if(Freq>=44000 && Freq<44999) 	//exact freq: 44100 hz
		speed=2;
	
//	else if(Freq>=5000 && Freq<10000) 
	else if(Freq>=8000 && Freq<8999)  	// exact frq: 8000 hz
		speed=3;

	else{
		printf("\nInvalid Frequency:\n");
		return -1;
	}

	ret = ioctl(fd,SNDCTL_DSP_RESET) ; 	// DSP Reset
        if (ret < 0){
                printf ("Unable to Reset device\n");
                return -1 ;
        }
                                                                                                
        ret = ioctl(fd,SNDCTL_DSP_SPEED,&speed); // DSP SPEED
        if (ret < 0){
                printf ("Unable to Set Speed\n") ;
                return -1 ;
        }
        printf ("speed-%x\n",speed) ;

                                                                                                
        ret = ioctl(fd,SNDCTL_DSP_STEREO,&mono); // Set Mono/Stereo
        if (ret < 0){
                printf ("Unable to Set Stereo/Mono\n") ;
                return -1 ;
 	}
        printf ("Mono-stereo-%x\n",mono) ;


	raw_fd = open (path,O_RDONLY);

//	raw_fd = open ("/mnt/jffs2/gotmail.wav",O_RDONLY);

        while(1){
                flen = read (raw_fd,rbuf,1024) ;
                len = write (fd,rbuf,flen) ;
                if(flen <1024)
                        break ;
		if(lk_getkey_wait()==23)
			break;
        }
        printf ("LEN = %d\n",len) ;
	
	close(raw_fd);	
	return len;

}
void sound_close()
{
	close(fd);
//	return 0;
}
