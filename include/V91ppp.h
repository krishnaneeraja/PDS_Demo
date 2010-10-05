#ifndef __V91__PPP__H__
#define __V91__PPP__H__
#define USE_PAP_AUTH 0
#define USE_CHAP_AUTH 1
                                                                                                                             
typedef struct {
 char dialnum[64];
 char account[32];
 char password[32];
 char auth;
 char localip[16];
 char remoteip[16];
 char netmask[16];
 }PPP_PSTN_SETTINGS;

int ppp_pstn_update_settings (PPP_PSTN_SETTINGS *settings);
void ppp_close_pstn();
int  ppp_open_pstn();

#endif
