#include"header.h"
 char arr1[50] = "";
char arr2[50] = "";
char arr3[50] = "";
char arr4[50] = "";
char arr5[50] = "";
char arr6[50] = "";
char ethtype[30] = "eth0";
char IPADR[30] = "";
extern char *getVal (char *);
extern void setVal (char *, char *);

/*******************************************************************************************
                            Ethernet setup main menu
 *******************************************************************************************/ 
  int
ethernet_setup () 
{
  int value, ret;
  static int eth = 1;
  char str[50] = "", ipd[80] = "";
  while (1)
    
    {
      lk_dispclr ();
      lk_disptext (2, 3, "Ethernet Setup", 0);
      lk_disptext (3, 1, "0. STATIC ", 0);
      lk_disptext (4, 1, "1. DHCP", 0);
      value = lk_getkey ();
      switch (value)
	
	{
	case 0:
	  printf ("\nSTATIC IP SETTINGS\n");
	  ethernet_test ();
	  
//         sprintf(ipd,"ifconfig %s down",ethtype);             
//         system(ipd);
	    break;
	case 1:
	  lk_dispclr ();
	  printf ("\nDHCP .... \n");
	  lk_disptext (3, 3, "RUNNING DHCP ...", 0);
	  sprintf (str, "/home/dhcpcd -t 20 %s", ethtype);	//sprintf(str,"/home/dhcpcd -t 20 %s",ethtype);//commented on 070909 CNU
	  system (str);
	  ip_config ();	//displaying network information
//         sprintf(ipd, "ifconfig %s down",ethtype);
//         system(ipd);
	  break;
	case CANCEL:
	  break;
	default:
	  continue;
	}
      return SUCCESS;
    }
}


/***********************************************************************************
                     Ethernet Setting  Test
************************************************************************************/ 
  int
ethernet_test () 
{
  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;
  menu.start = 0;
  menu.maxEntries = 7;
  maxEntries = menu.maxEntries;
  strcpy (menu.title, "STATIC IP");
  strcpy (menu.menu[0], "Enter IP Address");
  strcpy (menu.menu[1], "Enter Subnet Mask");
  strcpy (menu.menu[2], "Enter Gateway");
  strcpy (menu.menu[3], "Enter Primary DNS");
  strcpy (menu.menu[4], "Enter Secondary DNS");
  strcpy (menu.menu[5], "IP Setting View");
  strcpy (menu.menu[6], "Test ");
  while (1)
    
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);
      switch (opt)
	
	{
	case CANCEL:
	  return -1;
	case ENTER:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	  switch (selItem + 1)
	    
	    {
	    case 1:
	      set_ip ();
	      break;
	    case 2:
	      set_subnet_mask ();
	      break;
	    case 3:
	      set_gateway ();
	      break;
	    case 4:
	      set_pri_dns ();
	      break;
	    case 5:
	      set_sec_dns ();
	      break;
	    case 6:
	      ip_config ();
	      break;
	    case 7:
	      png_test ();
	      break;
	    default:
	      return -1;
	    }
	}
    }
  return 0;
}


/****************************************************************************************
                             setting ip
*****************************************************************************************/ 
  int
set_ip (void) 
{
  char ip_addr[60] = "", dest[30] = "192.168.1.55";
  int retval = 0;
  for (;;)			// Validating given IP               
    {
      lk_dispclr ();
      lk_disptext (2, 2, "Enter IP Address", 0);
      if (lk_getalpha (4, 1, getVal ("ipAddr"), 15, IPADR, 4) < 0)
	return -1;
      if (!ip_chk ())
	break;
      
      else
	
	{
	  lk_dispclr ();
	  lk_disptext (3, 4, "TRY AGAIN", 1);
	  printf ("Enter Valid Address\n");
	  if (lk_getkey () == CANCEL)
	    return -1;
	}
    }
  setVal ("ipAddr", IPADR);
  write_config (CONFIG_FILE);
  sprintf (ip_addr, "ifconfig %s %s up", ethtype, IPADR);
  system (ip_addr);		// setting ip
  return 0;
}


/***********************************************************************************
                      setting Subnet Mask
****************************************************************************************/ 
  int
set_subnet_mask (void) 
{
  char subnet_mask[60] = "", dest[30] = "255.255.255.0";
  get_subnet (1);		// reading subnet mask
  strcpy (dest, arr3);
  while (1)			// checking given address 
    {
      lk_dispclr ();
      lk_disptext (2, 2, "Enter Subnet Mask", 0);
      if (lk_getalpha (4, 1, getVal ("subnetMask"), 15, IPADR, 4) < 0)
	return -1;
      if (!ip_chk ())
	break;
      
      else
	
	{
	  lk_dispclr ();
	  lk_disptext (3, 2, "TRY AGAIN", 1);
	  printf ("Enter Valid Address\n");
	  if (lk_getkey () == CANCEL)
	    return -1;
	}
    }
  setVal ("subnetMask", IPADR);
  write_config (CONFIG_FILE);
  sprintf (subnet_mask, "ifconfig %s netmask %s up", ethtype, IPADR);
  system (subnet_mask);
  return 0;
}


/******************************************************************************************
                           Setting Default Gateway
******************************************************************************************/ 
  int
set_gateway () 
{
  char gateway[60] = "", dest[30] = "", dest1[30] = "192.168.1.8";
  int result = 0;
  for (;;)
    
    {
      lk_dispclr ();
      lk_disptext (2, 0, "Enter Default Gateway", 0);
      if (lk_getalpha (4, 1, getVal ("defaultGw"), 15, IPADR, 4) < 0)
	return -1;
      if (!ip_chk ())
	break;
      
      else
	
	{
	  lk_dispclr ();
	  lk_disptext (3, 2, "TRY AGAIN", 1);
	  printf ("Enter Valid Address\n");
	  if (lk_getkey () < 0)
	    return -1;
	}
    }
  setVal ("defaultGw", IPADR);
  write_config (CONFIG_FILE);
  sprintf (gateway, "route add default gw %s", IPADR);
  system (gateway);
  return 0;
}


/********************************************************************************************
                           Setting Primary DNS
 *******************************************************************************************/ 
  int
set_pri_dns (void) 
{
  FILE * fp;
  char pri_dns[60] = "", dest[30] = "", dest1[30] = "192.168.1.8";
  char str[50] = "search localdomain\n";
  for (;;)
    
    {
      lk_dispclr ();
      lk_disptext (2, 2, "Enter Primary DNS", 0);
      if (lk_getalpha (4, 1, getVal ("primaryDNS"), 15, IPADR, 4) < 0)
	return -1;
      if (!ip_chk ())
	break;
      
      else
	
	{
	  lk_dispclr ();
	  lk_disptext (3, 2, "TRY AGAIN", 1);
	  printf ("Enter Valid Address\n");
	  if (lk_getkey () < 0)
	    return -1;
	}
    }
  setVal ("primaryDNS", IPADR);
  sprintf (pri_dns, "nameserver %s\n", IPADR);
  fprintf (stderr, "\n%s\n", pri_dns);
  fp = fopen ("/etc/resolv.conf", "w+");
  fputs (str, fp);
  fputs (pri_dns, fp);
  fclose (fp);
  return 0;
}


/****************************************************************************************
                 Setting Secondary DNS
***************************************************************************************/ 
  int
set_sec_dns () 
{
  FILE * fp;
  char sec_dns[60] = "", dest[30] = "", dest1[30] = "192.168.1.3";
  while (1)
    
    {
      lk_dispclr ();
      lk_disptext (2, 1, "Enter Secondary DNS", 0);
      lk_getalpha (4, 1, getVal ("secDNS"), 15, IPADR, 4);
      if (!ip_chk ())
	break;
      
      else
	
	{
	  lk_dispclr ();
	  lk_disptext (3, 2, "TRY AGAIN", 1);
	  printf ("Enter Valid Address\n");
	  if (lk_getkey () < 0)
	    return -1;
	}
    }
  setVal ("secDNS", IPADR);
  write_config (CONFIG_FILE);
  printf ("\n%s\n", sec_dns);
  sprintf (sec_dns, "nameserver %s\n", IPADR);
  fp = fopen ("/etc/resolv.conf", "a");
  fputs (sec_dns, fp);
  fclose (fp);
  return 0;
}


/*****************************************************************************************
                    Reading MAC Address
*****************************************************************************************/ 
  get_mac_id (void) 
{
  int fd, value, i;
  char macid[30];
  struct ifreq ifr;
  fd = socket (AF_INET, SOCK_DGRAM, 0);
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy (ifr.ifr_name, ethtype, IFNAMSIZ - 1);
  ioctl (fd, SIOCGIFHWADDR, &ifr);
  close (fd);
  sprintf (macid, "%2x:%2x:%2x:%2x:%2x:%2x",
	    (unsigned char) ifr.ifr_hwaddr.sa_data[0],
	    (unsigned char) ifr.ifr_hwaddr.sa_data[1],
	    (unsigned char) ifr.ifr_hwaddr.sa_data[2],
	    (unsigned char) ifr.ifr_hwaddr.sa_data[3],
	    (unsigned char) ifr.ifr_hwaddr.sa_data[4],
	    (unsigned char) ifr.ifr_hwaddr.sa_data[5],
	    (unsigned char) ifr.ifr_hwaddr.sa_data[6]);
  for (i = 0; i < (sizeof (macid)); i++)
    if (macid[i] == ' ')
      macid[i] = '0';
  sprintf (arr2, "MAC:%s", macid);
  
    //  printf("\nMAC : %s\n",macid);
}


/*****************************************************************************************                                                             IP Setting View
*******************************************************************************************/ 
  int
ip_config (void) 
{
  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;
  menu.start = 0;
  menu.maxEntries = 6;
  maxEntries = menu.maxEntries;
  lk_dispclr ();
  lk_disptext (2, 3, "Getting Network", 0);
  lk_disptext (3, 5, "Information", 0);
  strcpy (menu.title, "IP-CONFIG");
  get_ip ();
  get_mac_id ();
  get_subnet ();
  get_gateway ();
  get_pri_dns ();
  get_sec_dns ();
  strcpy (menu.menu[0], arr2);
  strcpy (menu.menu[1], arr1);
  strcpy (menu.menu[2], arr3);
  strcpy (menu.menu[3], arr4);
  strcpy (menu.menu[4], arr5);
  strcpy (menu.menu[5], arr6);
  while (1)
    
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);
      switch (opt)
	
	{
	case CANCEL:
	  return;
	case ENTER:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	  switch (selItem + 1)
	    
	    {
	    case 1:
	      
		// get_ip();
		break;
	    case 2:
	      
		// get_mac_id();
		break;
	    case 3:
	      
		// get_subnet();
		break;
	    case 4:
	      
		// get_gateway();
		break;
	    case 5:
	      
		// get_pri_dns();
		break;
	    case 6:
	      
		// get_sec_dns();
		break;
	    }
	}
    }
}


/*****************************************************************************************
                       Reading IP Addr
*****************************************************************************************/ 
  int
get_ip (void) 
{
  int fd, value;
  char str[40] = "";
  struct ifreq ifr;
  fd = socket (AF_INET, SOCK_DGRAM, 0);
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy (ifr.ifr_name, ethtype, IFNAMSIZ - 1);
  ioctl (fd, SIOCGIFADDR, &ifr);
  close (fd);
  sprintf (str, "%d.%d.%d.%d", (unsigned char) ifr.ifr_addr.sa_data[2], 
	    (unsigned char) ifr.ifr_addr.sa_data[3], 
	    (unsigned char) ifr.ifr_addr.sa_data[4], 
	    (unsigned char) ifr.ifr_addr.sa_data[5], 
	    (unsigned char) ifr.ifr_addr.sa_data[6]);
  sprintf (arr1, "IP  :%s", str);
  printf ("\nIP Address : %s\n", str);
  return 0;
}


/*********************************************************************************
                 Reading Default Gateway
*********************************************************************************/ 
  get_gateway () 
{
  FILE * fp;
  int value, i;
  char str[50], str1[50] = "Not found";
  system ("route | grep default > dfgw");
  fp = fopen ("dfgw", "r");
  if (fp == NULL)
    fclose (fp);
  
  else
    
    {
      for (i = 0; i < 10; i++)
	
	{
	  fscanf (fp, "%s", str);
	  if (fp == NULL)
	    break;
	  
	  else if (!(strcmp (str, "default")))
	    
	    {
	      fscanf (fp, "%s", str1);
	      break;
	    }
	}
      fclose (fp);
      remove ("dfgw");
    }
  sprintf (arr4, "GW  :%s", str1);
  printf ("\nGATEWAY : %s\n", str1);
}


/**************************************************************************************
                   Reading Subnet Mask
************************************************************************************/ 
  get_subnet (int var) 
{
  char str[40] = "";
  int value, fd;
  struct ifreq ifr;
  fd = socket (AF_INET, SOCK_DGRAM, 0);
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy (ifr.ifr_name, ethtype, IFNAMSIZ - 1);
  ioctl (fd, SIOCGIFNETMASK, &ifr);
  close (fd);
  sprintf (str, "%d.%d.%d.%d", (unsigned char) ifr.ifr_netmask.sa_data[2], 
	    (unsigned char) ifr.ifr_netmask.sa_data[3], 
	    (unsigned char) ifr.ifr_netmask.sa_data[4], 
	    (unsigned char) ifr.ifr_netmask.sa_data[5], 
	    (unsigned char) ifr.ifr_netmask.sa_data[6]);
  if (var == 1)
    strcpy (arr3, str);
  
  else
    sprintf (arr3, "NET :%s", str);
  printf ("\nSUBNET MASK : %s\n", str);
}


/************************************************************************************
                        Reading Primary DNS
*************************************************************************************/ 
  get_pri_dns () 
{
  FILE * fp;
  int value, i, j;
  char str[80] = "", str1[50] = "Not Found";
  fp = fopen ("/etc/resolv.conf", "r");
  if (fp == NULL)
    fclose (fp);
  
  else
    for (j = 0; j < 20; j++)
      
      {
	fscanf (fp, "%s", str);
	if (!strcmp (str, "nameserver"))
	  
	  {
	    fscanf (fp, "%s", str1);
	    fclose (fp);
	    break;
	  }
      }
  sprintf (arr5, "DNS1:%s", str1);
  printf ("\nPRIMARY DNS : %s\n", str1);
}


/*************************************************************************************
                    Reading Secondary DNS
*************************************************************************************/ 
  get_sec_dns () 
{
  FILE * fp;
  char str[80] = "", str1[50] = "Not Found";
  int i, value, flag = 0, j;
  fp = fopen ("/etc/resolv.conf", "r");
  if (fp == NULL)
    fclose (fp);
  
  else
    for (j = 0; j < 50; j++)
      
      {
	fscanf (fp, "%s", str);
	if (!(strcmp (str, "nameserver")))
	  
	  {
	    if (flag == 1)
	      
	      {
		fscanf (fp, "%s", str1);
		fclose (fp);
		break;
	      }
	    
	    else
	      flag = 1;
	  }
      }
  sprintf (arr6, "DNS2:%s", str1);
  printf ("\nSECONDARY DNS : %s\n", str1);
}
int
ip_chk () 
{
  char i, j, k = 0, flag = 0, len = 0, cnt = 0;
  char str[30] = "";
  int chk[10], val = 0, val1 = 0, l = 0;
  chk[0] = -1;
  while (1)
    
    {
      strcpy (str, IPADR);
      len = strlen (str);
      if ((len < 7) || (len > 15) || (str[0] == '.'))
	
	{
	  flag = 1;
	}
      
      else
	
	{
	  for (i = 0, j = 0, flag = 0; i < len; i++)
	    
	    {
	      if (((str[i] < '0') || (str[i] > '9')) && (str[i] != '.'))
		
		{
		  flag = 1;
		  break;
		}
	      
	      else if (str[i] == '.')
		
		{
		  j++;
		  chk[j] = i;
		}
	    }
	}
      if (flag == 0 && j == 3)
	
	{
	  chk[4] = len;
	  for (i = 0, val = 0, l = 0; i < 4; i++)
	    
	    {
	      cnt = chk[i + 1] - chk[i];
	      k = chk[i + 1];
	      if (cnt < 2 || cnt > 4)
		
		{
		  flag = 1;
		  break;
		}
	      switch (cnt)
		
		{
		case 2:
		  val = (str[k - 1] - 48);
		  if (i == 0 && val == 0)
		    flag = 1;
		  break;
		case 3:
		  l = 10 * (str[k - 2] - 48);
		  if (l == 0)
		    flag = 1;
		  
		  else
		    val = l + (str[k - 1] - 48);
		  break;
		case 4:
		  l = 100 * (str[k - 3] - 48);
		  if (l == 0)
		    flag = 1;
		  
		  else
		    val = l + 10 * (str[k - 2] - 48) + (str[k - 1] - 48);
		}
	      if (val < 0 || val > 255)
		flag = 1;
	    }
	}
      if (flag == 0 && j == 3)
	return 0;
      
      else
	return -1;
    }
}
int
wlan_test (void) 
{
  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 0;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;
  menu.start = 0;
  menu.maxEntries = 2;
  maxEntries = menu.maxEntries;
  strcpy (menu.title, "WLAN SETTINGS");
  strcpy (menu.menu[0], "1. Radio Settings");
  strcpy (menu.menu[1], "2. Ethernet Setup");
  while (1)
    
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);
      switch (opt)
	
	{
	case CANCEL:
	  return;
	case ENTER:
	case 1:
	case 2:
	  switch (selItem + 1)
	    
	    {
	    case 1:
	      Radio_settings ();
	      break;
	    case 2:
	      ethernet_setup ();
	      break;
	    }
	default:
	  break;
	}
    }
  return 0;
}
int
Radio_settings (void) 
{
  int opt = 0;
  MENU_T menu;
  char event;
  int selItem = 00;
  int acceptKbdEvents = TRUE;
  int maxEntries = 0;
  short scroll = 0;
  menu.start = 0;
  menu.maxEntries = 8;
  maxEntries = menu.maxEntries;
  strcpy (menu.title, "Radio Settings");
  strcpy (menu.menu[0], "SSID");
  strcpy (menu.menu[1], "Security");
  strcpy (menu.menu[2], "Format");
  strcpy (menu.menu[3], "Key ID");
  strcpy (menu.menu[4], "Mode");
  strcpy (menu.menu[5], "Authentication");
  strcpy (menu.menu[6], "Power save Mode");
  strcpy (menu.menu[7], "Channel");
  for (;;)
    
    {
      opt = scroll_menu (&menu, &selItem, acceptKbdEvents);
      switch (opt)
	
	{
	case CANCEL:
	  return;
	case ENTER:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	  switch (selItem + 1)
	    
	    {
	    case 1:
	      notimplemented ();	//enter_ssid
	      break;
	    case 2:
	      notimplemented ();	//enter_security
	      break;
	    case 3:
	      notimplemented ();	//enter_format
	      break;
	    case 4:
	      notimplemented ();	//enter_keyid();
	      break;
	    case 5:
	      notimplemented ();	//enter_mode
	      break;
	    case 6:
	      notimplemented ();	//enter_authentication
	      break;
	    case 7:
	      notimplemented ();	//power_save_mode
	      break;
	    case 8:
	      notimplemented ();	//enter_channel();
	      break;
	    }
	}
    }
  return 0;
}
int
notimplemented (void) 
{
  lk_dispclr ();
  lk_disptext (3, 2, "Not Implemented", 1);
  lk_getkey ();
  return 0;
}
int
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
  system (ip_addr);		// setting ip

  sprintf (subnet_mask, "ifconfig %s netmask %s up", ethtype,
	   getVal ("subnetMask"));
  printf ("Setting up subnet mask: %s\n", subnet_mask);
  system (subnet_mask);

  sprintf (gateway, "route add default gw %s", getVal ("defaultGw"));
  printf ("Setting up gateway: %s\n", gateway);
  system (gateway);

  sprintf (pri_dns, "nameserver %s\n", getVal ("primaryDNS"));
  fprintf (stderr, "\n%s\n", pri_dns);
  fp = fopen ("/etc/resolv.conf", "w+");
  fputs (str, fp);
  fputs (pri_dns, fp);
  fclose (fp);
  printf ("Setting up gateway: %s\n", gateway);
  sprintf (sec_dns, "nameserver %s\n", getVal ("secDNS"));
  fp = fopen ("/etc/resolv.conf", "a");
  fputs (sec_dns, fp);
  fclose (fp);
}

png_test () 
{
  FILE * fp;
  char a, b, c;
  int value, i;
  char ipaddr[80] = "", dest[30] = "", dest1[30] = "192.168.1.1";
  while (1)
    
    {
      lk_dispclr ();
      lk_disptext (2, 2, "Enter IP to Png", 0);
      if (lk_getalpha (4, 1, dest1, 15, IPADR, 4) < 0)
	return -1;
      if (!ip_chk ())
	break;
      
      else
	
	{
	  lk_dispclr ();
	  lk_disptext (3, 2, "TRY AGAIN", 1);
	  printf ("Enter Valid Address\n");
	  if (lk_getkey () == CANCEL)
	    return -1;
	}
    }
  lk_dispclr ();
  lk_disptext (2, 4, "Pnging Status", 0);
  sprintf (ipaddr, "ping -c 2 %s | grep loss > png_file", IPADR);
  system (ipaddr);
  fp = fopen ("png_file", "r");
  for (i = 0; i < 60; i++)
    
    {
      b = a;
      a = c;
      c = fgetc (fp);
      if (c == '%')
	
	{
	  lk_dispclr ();
	  if ((a == '0') && (b == ' '))
	    
	    {
	      lk_disptext (3, 3, "System is Alive", 0);
	      printf ("System is Alive\n");
	    }
	  
	  else
	    
	    {
	      lk_disptext (3, 3, "System is Dead", 0);
	      printf ("\nSystem is Dead\n");
	    }
	  break;
	}
    }
  fclose (fp);
  remove ("png_file");
  lk_getkey ();
}


