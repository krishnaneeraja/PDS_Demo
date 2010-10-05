#include<header.h>

int pinpad_programming ( void )
  {
    struct termios my_termios1,my_termios3;
    struct termios get_termios;
    register int fd1,fd3,w,r,result;

    static int sam_fd;
    unsigned char buffer1[1024];
    unsigned char buffer[1024];
                                                                                                                                                          	
    lk_dispclr();
    lk_disptext(2,6,"FIRMWARE",1);
    lk_disptext(4,7,"UPGRADE",1);
                                                                                                                                                          
    lk_close();   // closing pinpad
    prn_close();  // closing printer
    mscr_close(); // closing magnetic swipe
                                                                                                                                                          
                                                                                                                                                          
    fprintf(stdout,"pinpad in programming mode\n");
                                                                                                                                                          
    w = system("rmmod ppserial");
    
    if(w == 256)
    {
      w = system ("rmmod ppserialwlan");
      if(w == 256 )
      {
         fprintf(stderr,"unable to remove PINPAD driver\n");
         return ERROR;
      }
    }
                                                                                                                                                          
    sleep(2);

    sam_fd = open ("/dev/sam",O_RDWR) ;
                                                                                                                                                          
    if (sam_fd<0)
    {
      fprintf(stderr,"unable to open sam (portc.o)\n");
      return ERROR;
    }
                                                                                                                                                          
                                                                                                                                                          
    else
    {
      ioctl (sam_fd,0x06,0);  // changing  console control to pinpad conftrol
      ioctl (sam_fd,0x04,0) ; /* PRT C4 (nRESET1) pin low ie D6 pin*/
     
      ioctl (sam_fd,0x00,0) ; /* PRT C2 (PINPAD_RST) pin low */
      sleep(1);
      ioctl (sam_fd,0x01,0) ; /* PRT C2 (PINPAD_RST) pin High */
    }
                                                                                                                                                          
                                                                                                                                                          
    fd1=open("/dev/ttyS1",O_RDWR|O_NOCTTY|O_NDELAY );
    memset(buffer, 0, sizeof(buffer));
                                                                                                                                                          
    memset(&my_termios1, 0, sizeof(struct termios));
    my_termios1.c_cflag=B19200|CS8|CREAD|CLOCAL;
    tcsetattr(fd1,TCSANOW,&my_termios1);
                                                                                                                                                          
                                                                                                                                                          
    fd3=open("/dev/ttyS3",O_RDWR|O_NOCTTY|O_NDELAY );
    memset(buffer1,0, sizeof(buffer1));
    memset(&my_termios3, 0, sizeof(struct termios));
    my_termios3.c_cflag=B19200|CS8|CREAD|CLOCAL;
    tcsetattr(fd3,TCSANOW,&my_termios3);

    while(1)
    {
       r=read(fd1,buffer,sizeof(buffer));
       if(r>0)
          w=write(fd3,buffer,r);
                                                                                                                                                          
        r=read(fd3,buffer1,sizeof(buffer1));
         if(r>0)
         w=write(fd1,buffer1,r);
     }

    close(fd1);
    close(fd3);
    close(sam_fd);
    return SUCCESS;
                                                                                                                                                          
 }