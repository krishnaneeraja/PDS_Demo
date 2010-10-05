#ifndef MAGSWIPE_H
#define  MAGSWIPE_H

#define MAG_SW_ERR -1
#define  MSCR_OFF     0
#define  MSCR_ON      1
#define  MSCR_BUSY    2
#define  MSCR_FAIL    0
#define  MSCR_SUCCESS 1

//This functon opens the device 
//It returns SUCCESS if the device opened successfully, otherwise reurn MAG_SW_ERR
int mscr_open();

//This function reads the data of the both tracks from the card reader 
//Pointers to the buffers where tracks data to be copied are to be passed as arguments, track1 and track2
//are charecter pointer data types and the memory allocated to them should be length of the expected data+1 
//(suggested 256 bytes each)
// returns SUCCESS if the data is valid and copied into the buffers, returns MAG_SW_ERR if there is no valid data or device not opened. 
int mscr_read(char *track_1_data, char *track_2_data); 

//This function clears the card reader device buffer returns SUCCESS on success, returns MAG_SW_ERR if the device not opened.
int mscr_flush(); 

//This fucntion is used to check wether the device is having valid data ( both track1 and track2 data).
// returns SUCCESS if the valid data present, return MAG_SW_ERR if there is no valid data.
int mscr_getstate(); 

//This function close the device
// returns SUCCESS if the device closed successfully and returns MAG_SW_ERR if the device is not opened.
int mscr_close();

#endif 
