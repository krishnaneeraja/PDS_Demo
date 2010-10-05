
#include<header.h>

int
sound_test ()
{

  int ret = 0, key;
  int type;			// 0 for Mono & 1 for stero
  char path[25];		// file name

  system ("insmod /home/audio_enable.o >/dev/null 2>&1");	// enabling sound

  /*
     while(1)
     {
     lk_dispclr();

     lk_disptext (1,0,"select wave file :",1 );
     lk_disptext (3,0,"1.gotmail.wav",0 );
     lk_disptext (4,0,"2.horse.wav",0 );
     lk_disptext (5,0,"3.jetplanef16.wav",0 );


     key = lk_getkey ( );
     strcpy(path," ");

     if ( key == 1 )      
     //           lk_getalpha ( 3,0,"gotmail.wav",20,path,3 );    
     strcpy(path,"gotmail.wav");
     else if ( key == 2 )
     //           lk_getalpha ( 3,0,"horse.wav",20,path,3 );      
     strcpy(path,"horse.wav");
     else if ( key == 3 )
     //           lk_getalpha ( 3,0,"jetplanef16.wav",20,path,3 );        
     strcpy(path,"jetplanef16.wav");
     else if ( key == 23 )
     break;
     else{
     lk_dispclr();
     lk_disptext(2,0,"Invalid Option...",0);
     lk_disptext(3,0,"Try Again...!",0);
     sleep(2);
     break;
     }

     //           sleep (1);
     lk_dispclr ( );
     lk_disptext ( 2,2,"visiontek",1 );
     lk_disptext ( 4,1,"Testing......",0 );

     ret=get_wave ( path ,&data ); //Wave_file data

     if ( ret < 0 ){
     printf( "unable to get wave_data\n" );       
     return -1;
     }


     //           system("insmod /mnt/jffs2/sounddriver.o"); //temparary testing
     ret = sound_open ( );

     if ( ret < 0 ) {
     printf( "unable to open DSP device:\n" );
     return -1 ;
     }


     if( data.number_of_channel == 1 )
     type=0;   

     else if ( data.number_of_channel == 2 )
     type=1;   

     ret = sound_write ( path, data.samples_per_second , type );

     if ( ret < 0 ) {
     printf("unable to write Voice:\n");
     return -1;
     }

     sound_close ( );
     //           system("rmmod /mnt/jffs2/sounddriver.o"); 
     } //while    
   */


  while (1)
    {
      lk_dispclr ();

      lk_disptext (1, 0, "Enter Numeric Key :", 1);
      lk_disptext (3, 0, "0-9 To Play", 0);
      lk_disptext (5, 0, "Numeric Audio Files", 0);


      key = lk_getkey ();
      strcpy (path, " ");

      if (key == 0)
	strcpy (path, "0r.wav");
      else if (key == 1)
	strcpy (path, "1r.wav");
      else if (key == 2)
	strcpy (path, "2r.wav");
      else if (key == 3)
	strcpy (path, "3r.wav");
      else if (key == 4)
	strcpy (path, "4r.wav");
      else if (key == 5)
	strcpy (path, "5r.wav");
      else if (key == 6)
	strcpy (path, "6r.wav");
      else if (key == 7)
	strcpy (path, "7r.wav");
      else if (key == 8)
	strcpy (path, "8r.wav");
      else if (key == 9)
	strcpy (path, "9r.wav");


      else if (key == 23)
	break;
      else
	{
	  lk_dispclr ();
	  lk_disptext (2, 0, "Invalid Option...", 0);
	  lk_disptext (3, 0, "Try Again...!", 0);
	  sleep (2);
	  break;
	}

      //sleep (1);
      lk_dispclr ();
      lk_disptext (2, 2, "visiontek", 1);
      lk_disptext (4, 1, "Testing......", 0);

      ret = get_wave (path, &data);	//Wave_file data

      if (ret < 0)
	{
	  printf ("unable to get wave_data\n");
	  return -1;
	}


      //system("insmod /mnt/jffs2/sounddriver.o"); //temparary testing
      ret = sound_open ();

      if (ret < 0)
	{
	  printf ("unable to open DSP device:\n");
	  return -1;
	}


      if (data.number_of_channel == 1)
	type = 0;

      else if (data.number_of_channel == 2)
	type = 1;

      ret = sound_write (path, data.samples_per_second, type);

      if (ret < 0)
	{
	  printf ("unable to write Voice:\n");
	  return -1;
	}

      sound_close ();
      //system("rmmod /mnt/jffs2/sounddriver.o");
    }				//while

  return SUCCESS;

}
