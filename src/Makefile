LDIR=../lib/lib91sdepos.a
LDIR2=/mnt/jffs2/libtfm.so 
LDIR3=/lib/libsqlite3.so /lib/libcurl.so.3
LDIR4=../lib/libusb.a 
#LDIR5=../lib/libSC2_ANSI378.a
LDIR6=../lib/libbclib.a

IDIR=../include
CC=arm-linux-gcc 

OBJS= main_menu.o scroll_menu.o retail.o e_sqlite.o  ppp_api.o  reader.o parse.o ethernet.o gprs.o V91Modem.o utils.o synchronise.o returns.o price_list.o \
grn.o dn.o exch_rate.o download.o reports.o login.o globals.o db_config.o setup_menu.o ping.o fileupload.o soap.o rfid.o firmware.o auth_file.o auth_smart.o auth_contact.o auth_rfid.o finger.o usb.o convert-template4.o 

#test_apl:removeAll $(OBJS)
test_apl:$(OBJS)
	$(CC) -o retail   $(OBJS) $(LDIR) $(LDIR2) $(LDIR3) $(LDIR4) $(LDIR5) $(LDIR6) -lpthread -lm -w -O2 
	 
login.o :login.c 
	$(CC) -I $(IDIR) -c login.c
globals.o :globals.c 
	$(CC) -I $(IDIR) -c globals.c
db_config.o :db_config.c 
	$(CC) -I $(IDIR) -c db_config.c
main_menu.o :main_menu.c 
	$(CC) -I $(IDIR) -c main_menu.c
scroll_menu.o:scroll_menu.c
	$(CC) -I $(IDIR) -c scroll_menu.c
retail.o:retail.c
	$(CC) -I $(IDIR) -c retail.c
e_sqlite.o:e_sqlite.c
	$(CC) -I $(IDIR) -c e_sqlite.c
ppp_api.o:ppp_api.c
	$(CC) -I $(IDIR) -c ppp_api.c
reader.o:reader.c
	$(CC) -I $(IDIR) -c reader.c
parse.o:parse.c
	$(CC) -I $(IDIR) -c parse.c
utils.o:utils.c
	$(CC) -I $(IDIR) -c utils.c
grn.o:grn.c
	$(CC) -I $(IDIR) -c grn.c
dn.o:dn.c
	$(CC) -I $(IDIR) -c dn.c
reports.o:reports.c
	$(CC) -I $(IDIR) -c reports.c
download.o:download.c
	$(CC) -I $(IDIR) -c download.c
exch_rate.o:exch_rate.c
	$(CC) -I $(IDIR) -c exch_rate.c
price_list.o:price_list.c
	$(CC) -I $(IDIR) -c price_list.c
returns.o:returns.c
	$(CC) -I $(IDIR) -c returns.c
synchronise.o:synchronise.c
	$(CC) -I $(IDIR) -c synchronise.c
ethernet.o:ethernet.c
	$(CC) -I $(IDIR) -c ethernet.c
gprs.o:gprs.c
	$(CC) -I $(IDIR) -c gprs.c
V91Modem.o:V91Modem.c
	$(CC) -I $(IDIR) -c V91Modem.c
setup_menu.o:setup_menu.c
	$(CC) -I $(IDIR) -c setup_menu.c
ping.o:ping.c
	$(CC) -I $(IDIR) -c ping.c
fileupload.o:fileupload.c
	$(CC) -I $(IDIR) -c fileupload.c
soap.o:soap.c
	$(CC) -I $(IDIR) -c soap.c
rfid.o:rfid.c
	$(CC) -I $(IDIR) -c rfid.c
firmware.o:firmware.c
	$(CC) -I $(IDIR) -c firmware.c
auth_file.o:auth_file.c
	$(CC) -I $(IDIR) -c auth_file.c
auth_rfid.o:auth_rfid.c
	$(CC) -I $(IDIR) -c auth_rfid.c
auth_smart.o:auth_smart.c
	$(CC) -I $(IDIR) -c auth_smart.c
auth_contact.o:auth_contact.c
	$(CC) -I $(IDIR) -c auth_contact.c
finger.o:finger.c
	$(CC) -I $(IDIR) -c finger.c
usb.o:usb.c
	$(CC) -I $(IDIR) -c usb.c
convert-template4.o:convert-template4.c
	$(CC) -I $(IDIR) -c convert-template4.c

removeAll:
	rm -rf *.o
clean:
	rm -f main_menu.o
	rm -f scroll_menu.o
	rm -f e_sqlite.o
	rm -f retail.o
	rm -f ppp_api.o
	rm -f fileupload.o
	rm -f reader.o
	rm -f parse.o
	rm -f ethernet.o
	rm -f gsm_gprs.o
	rm -f V91Modem.o
	rm -f utils.o
	rm -f synchronise.o
	rm -f upek_menu.o
	rm -f setup_menu.o
	rm -f auth_rfid.o
	rm -f auth_smart.o
	rm -f insert.o
	rm -f ping.o
	rm -f fileupload.o
	rm -f ifd.o
	rm -f rfid.o
	rm -f firmware.o
	rm -f finger.o
	rm -f ams
	rm -f ams.IMG

image:
	arm-linux-strip  ams
	newmkapp ams ams.IMG
