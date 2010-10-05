
// This function is to set the SSID of the wireless LAN device 
// on success returns 0 and returns -1 on failure
// ssid charecter array is to be passed and the size of the array must be (32+1)
int Set_SSID(char *ssidname);


// This function is to get the channel number used by the wireless LAN module 
// on success returns channel number, on failure returns -1
int Get_Channel();

// This function is to get the SSID of the wireless LAN device 
// on success returns 0 and returns -1 on failure
// charecter array is to be passed and the size of the array must be (32+1)
int Get_SSID(char *str);

// This function is to get the Signal strength of the wireless LAN module 
// on success returns signal strength, on failure returns -1
int Get_SignalStrength();

// This function is to get the Link quality of the wireless LAN module 
// on success returns Link quality, on failure returns -1
int Get_LinkQuality();

// This function is to get the MAC address of the wireless LAN module 
// on success returns 0, on failure returns -1
// unsigned charecter array is to be passed and the size of the array must be 6
int Get_MacAddress(unsigned char *str);

// This function is to get the Access point MAC address  
// on success returns 0, on failure returns -1
// unsigned charecter array is to be passed and the size of the array must be 6
int Get_AP_MacAddress(unsigned char *str);
