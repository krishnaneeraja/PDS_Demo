#include"header.h"










extern void setVal (char *, char *);

/*******************************************************************************************
                            Ethernet setup main menu
 *******************************************************************************************/ 
  
ethernet_setup () 
{
  
  
  
  
    
    {
      
      
      
      
      
      
	
	{
	
	  
	  
	  
//         sprintf(ipd,"ifconfig %s down",ethtype);             
//         system(ipd);
	    break;
	
	  
	  
	  
	  
	  
	  
//         sprintf(ipd, "ifconfig %s down",ethtype);
//         system(ipd);
	  
	
	  
	
	  
	
      
    



/***********************************************************************************
                     Ethernet Setting  Test
************************************************************************************/ 
  int
ethernet_test () 
{
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
    
    {
      
      
	
	{
	
	  
	
	
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	  
	    
	    {
	    
	      set_ip ();
	      break;
	    
	      set_subnet_mask ();
	      break;
	    
	      set_gateway ();
	      break;
	    
	      set_pri_dns ();
	      break;
	    
	      set_sec_dns ();
	      break;
	    
	      ip_config ();
	      break;
	    
	      png_test ();
	      break;
	    
	      return -1;
	    
	
    
  



/****************************************************************************************
                             setting ip
*****************************************************************************************/ 
  int
set_ip (void) 
{
  
  
  
    {
      
      
      
	
      
	
      
      else
	
	{
	  
	  
	  
	  
	    
	
    
  
  write_config (configFile);
  
  
  return 0;



/***********************************************************************************
                      setting Subnet Mask
****************************************************************************************/ 
  
set_subnet_mask (void) 
{
  
  
  
  
    {
      
      
      
	
      
	
      
      else
	
	{
	  
	  
	  
	  
	    
	
    
  
  
  
  
  



/******************************************************************************************
                           Setting Default Gateway
******************************************************************************************/ 
  
set_gateway () 
{
  
  
  
    
    {
      
      
      
	
      
	
      
      else
	
	{
	  
	  
	  
	  
	    
	
    
  
  
  
  
  



/********************************************************************************************
                           Setting Primary DNS
 *******************************************************************************************/ 
  int
set_pri_dns (void) 
{
  
  
  
  
    
    {
      
      
      
	
      
	
      
      else
	
	{
	  
	  
	  
	  
	    
	
    
  setVal ("primaryDNS", IPADR);
  
  
  
  
  
  
  



/****************************************************************************************
                 Setting Secondary DNS
***************************************************************************************/ 
  int
set_sec_dns () 
{
  
  
  
    
    {
      
      
      
      
	
      
      else
	
	{
	  
	  
	  
	  
	    
	
    
  setVal ("secDNS", IPADR);
  
  
  
  
  
  
  



/*****************************************************************************************
                    Reading MAC Address
*****************************************************************************************/ 
  get_mac_id (void) 
{
  
  
  
  
  
  
  
  
  
	    (unsigned char) ifr.ifr_hwaddr.sa_data[0],
	    
	    
	    
	    
	    
	    
  
    
      
  
  
    //  printf("\nMAC : %s\n",macid);
}


/*****************************************************************************************                                                             IP Setting View
*******************************************************************************************/ 
  int
ip_config (void) 
{
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
    
    {
      
      
	
	{
	
	  
	
	
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	  
	    
	    {
	    
	      
		// get_ip();
		break;
	    
	      
		// get_mac_id();
		break;
	    
	      
		// get_subnet();
		break;
	    
	      
		// get_gateway();
		break;
	    
	      
		// get_pri_dns();
		break;
	    
	      
		// get_sec_dns();
		break;
	    
	
    



/*****************************************************************************************
                       Reading IP Addr
*****************************************************************************************/ 
  int
get_ip (void) 
{
  
  
  
  
  
  
  
  
  
	    (unsigned char) ifr.ifr_addr.sa_data[3], 
	    (unsigned char) ifr.ifr_addr.sa_data[4], 
	    (unsigned char) ifr.ifr_addr.sa_data[5], 
	    (unsigned char) ifr.ifr_addr.sa_data[6]);
  
  
  



/*********************************************************************************
                 Reading Default Gateway
*********************************************************************************/ 
  
{
  
  
  
  
  
  
    
  
  else
    
    {
      
	
	{
	  
	  
	    
	  
	  else if (!(strcmp (str, "default")))
	    
	    {
	      
	      
	    
	
      
      
    
  
  



/**************************************************************************************
                   Reading Subnet Mask
************************************************************************************/ 
  
{
  
  
  
  
  
  
  
  
  
	    (unsigned char) ifr.ifr_netmask.sa_data[3], 
	    (unsigned char) ifr.ifr_netmask.sa_data[4], 
	    (unsigned char) ifr.ifr_netmask.sa_data[5], 
	    (unsigned char) ifr.ifr_netmask.sa_data[6]);
  
    
  
  else
    
  



/************************************************************************************
                        Reading Primary DNS
*************************************************************************************/ 
  get_pri_dns () 
{
  
  
  
  
  
    
  
  else
    
      
      {
	
	
	  
	  {
	    
	    
	    
	  
      
  
  



/*************************************************************************************
                    Reading Secondary DNS
*************************************************************************************/ 
  get_sec_dns () 
{
  
  
  
  
  
    
  
  else
    
      
      {
	
	
	  
	  {
	    
	      
	      {
		
		
		
	      
	    
	    else
	      
	  
      
  
  


ip_chk () 
{
  
  
  
  
  
    
    {
      
      
      
	
	{
	  
	
      
      else
	
	{
	  
	    
	    {
	      
		
		{
		  
		  
		
	      
	      else if (str[i] == '.')
		
		{
		  
		  
		
	    
	
      
	
	{
	  
	  
	    
	    {
	      
	      
	      
		
		{
		  
		  
		
	      
		
		{
		
		  
		  
		    
		  
		
		  
		  
		    
		  
		  else
		    
		  
		
		  
		  
		    
		  
		  else
		    
		
	      
		
	    
	
      
	
      
      else
	
    


wlan_test (void) 
{
  
  
  
  
  
  
  
  
  
  
  
  
  
  
    
    {
      
      
	
	{
	
	  
	
	
	case 2:
	  
	    
	    {
	    
	      Radio_settings ();
	      break;
	    
	      ethernet_setup ();
	      break;
	    
	
	  break;
	
    
  


Radio_settings (void) 
{
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
    
    {
      
      
	
	{
	
	  
	
	
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	  
	    
	    {
	    
	      
	      break;
	    
	      
	      break;
	    
	      
	      break;
	    
	      
	      break;
	    
	      
	      break;
	    
	      
	      break;
	    
	      
	      break;
	    
	      
	      break;
	    
	
    
  


notimplemented (void) 
{
  
  
  
  


ip_setup ()
{
  char ip_addr[60] = "";
  char subnet_mask[60] = "";
  char gateway[60] = "";
  char pri_dns[60] = "";
  char sec_dns[60] = "";
  FILE *fp;
  char str[200];

  sprintf (ip_addr, "ifconfig %s %s up", ethtype, getVal ("ipAddr"));
  printf ("Setting up IP: %s\n", ip_addr);
  

  sprintf (subnet_mask, "ifconfig %s netmask %s up", ethtype,
	   getVal ("subnetMask"));
  
  

  sprintf (gateway, "route add default gw %s", getVal ("defaultGw"));
  
  

  sprintf (pri_dns, "nameserver %s\n", getVal ("primaryDNS"));
  
  
  
  
  
  printf ("Setting up gateway: %s\n", gateway);
  
  
  
  
}


{
  
  
  
  
  
    
    {
      
      
      
	
      
	
      
      else
	
	{
	  
	  
	  
	  
	    
	
    
  
  
  
  
  
  
    
    {
      
      
      
      
	
	{
	  
	  
	    
	    {
	      
	      
	    
	  
	  else
	    
	    {
	      
	      
	    
	  
	
    
  
  
  


