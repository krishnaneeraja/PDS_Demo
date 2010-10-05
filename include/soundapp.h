
/*Wave file structure  */
 
struct wave{
                                                                                                 
  int total_chunk_size;   	//total size
  int number_of_channel;  	// mono or stereo
  int samples_per_second; 	//frequency
  int bytes_per_second;  	//samples per second * block align
  int block_align;       	//num_channels*(bits per sample /8 = bytes per sample)
  int bits_per_sample;   	//bits 
  int num_blocks;
                                                                                                 
  int second_chunk_size;
  float time_length;
                                                                                                 
  float *channel1;
  float *channel2;
 };
                                                                                                 
  struct wave data;


// returns 0 if the device is opened successfully, returns -1 the device is not opened.
int sound_open();

// returns 0 if the device is closed successfully, returns -1 device is not closed.
//int sound_close();
void  sound_close();

// returns 0 if the device is Successful.Here there are two types Supported i.e 0 and 1             and speed 0 for song , 1 for voice.	
//int sound_write(char *File_Name,int *Frequ ,int mono );
int sound_write(char *File_Name,int speed ,int mono );

// Get data from wave_file. return 0 for success.
int get_wave(char *, struct wave *wave0 );

