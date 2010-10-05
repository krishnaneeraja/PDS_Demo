#ifndef __RFID_H__
#define __RFID_H__

/*------------------------------------------------------------------------------
Function: tama_open
Desc. :   Used to open the tama device
Inputs:   --None--
Output:   Returns opened file descriptor on success and < 1 on failure
------------------------------------------------------------------------------*/
int tama_open(void);

/*------------------------------------------------------------------------------
Function: tama_close
Desc. :   Used to close the tama device
Inputs:   --None--
Output:   --None--
-----------------------------------------------------------------------------*/
void tama_close(void);

/*------------------------------------------------------------------------------
Function: tama_detectTarget
Desc. :   Used to detect the targets of RFID Card reader
Inputs:   Number of targets to detect(Max. 2), Address of memory to store number of
          Tags detected & Address of memory to store Tag ID
Output:   Number of Tags read/initialized is stored in the address passed to Tg_Detected.
          The read Tag is stored in location pointed by Tag ID pointer and returns 0
          on success. Returns -1 on failure
------------------------------------------------------------------------------*/
int tama_detectTarget(unsigned char Tg_Nos, unsigned char *Tg_Detected, unsigned char *Tg_ID);

/*------------------------------------------------------------------------------
Function: tama_authenticate_mifare
Desc. :   Authenticates a Tag's Block for writing/reading with the said key
Inputs:   Tag Number to authenticate(0 - Card 1, 1 - Card 2), Block address, key index , and
          Key type (Key A - 0x0A /Key B - 0x0B)
Output:   Returns 0 on success and < 1 on failure
------------------------------------------------------------------------------*/
int tama_authenticate_mifare(unsigned char Tg_No, unsigned char Blk_Addr, unsigned char index, unsigned char Key_Type);

/*-----------------------------------------------------------------------------------
Function: tama_authenticate_mifare
Desc. :   Authenticates a Tag's Block for writing/reading with the said key
Inputs:   Tag Number to authenticate(1 - Card 1, 2 - Card 2), Block address, Key, and
          Key type (Key A - 0x0A /Key B - 0x0B)
Output:   Returns 0 on success and < 1 on failure
-----------------------------------------------------------------------------------*/
int tama_authenticate_mifare_key (unsigned char Tg_No, unsigned char Blk_Addr, unsigned char *Key, unsigned char Key_Type);

/*------------------------------------------------------------------------------
Function: tama_halt_mifare
Desc. :   Halts (Deselects) a tag for access
Inputs:   Tag's logical ID
Output:   Returns 0 on success and < 1 on failure
------------------------------------------------------------------------------*/
int tama_halt_mifare(unsigned char Tg_LID);

/*------------------------------------------------------------------------------
Function: tama_read_target
Desc. :   Reads the block data and stores it into passed address
Inputs:   Block address to read, pointer to the data in buffer
Output:   Stores the read data from the card into the address pointed to by "Data_In"
          Returns the number of bytes read from the card.
------------------------------------------------------------------------------*/
int tama_read_target(unsigned char Blk_Addr, unsigned char *Data_In);

/*------------------------------------------------------------------------------
Function: tama_write_target
Desc. :   Writes the data sent to the said block
Inputs:   Block address to write, pointer to the data, length of data to write
Output:   Returns 0 on success and < 1 on failure
------------------------------------------------------------------------------*/
int tama_write_target(unsigned char Blk_Addr, unsigned char *Data_Out, int Dlen);

/*------------------------------------------------------------------------------
Function: tama_prepare_valueBlock
Desc. :   Used to prepare a Value Block and write that at the specified block
Inputs:   Block's address tobe prepared, Value (Shld be <= 9999), Address to build a 
          backup mechanism for the block
Output:   Returns 0 On success and < 0 on failure
------------------------------------------------------------------------------*/
int tama_prepare_valueBlock(unsigned char Blk_Addr, unsigned int Value, unsigned char Addr);

/*------------------------------------------------------------------------------
Function: tama_decrement_block
Desc. :   Used to decrement a block with the said value
Inputs:   Block number tobe decremented, Value
Output:   Returns 0 on success, < 0 on failure
------------------------------------------------------------------------------*/
int tama_decrement_block(unsigned char Blk_Addr, unsigned int Value);

/*------------------------------------------------------------------------------
Function: tama_increment_block
Desc. :   Used to increment a block with the said value
Inputs:   Block number tobe incremented, Value
Output:   Returns 0 on success, < 0 on failure
------------------------------------------------------------------------------*/
int tama_increment_block(unsigned char Blk_Addr, unsigned int Value);

/*-----------------------------------------------------------------------------------
Function: tama_transfer_block
Desc. :   Used to transfer data from temporary register to a value block
Inputs:   Block Address to transfer
Output:   Returns 0 on success, and < 0 on failure
-----------------------------------------------------------------------------------*/
int tama_transfer_block(unsigned char Blk_Addr);

/*-----------------------------------------------------------------------------------
Function: tama_read_valueBlock
Desc. :   Used to read a value from a value block
Inputs:   Block to read, Address of location to store the read value
Output:   Returns 0 on success and < 0 on failure
-----------------------------------------------------------------------------------*/
int tama_read_valueBlock(unsigned char Blk_Addr,unsigned int *Value);

#endif
