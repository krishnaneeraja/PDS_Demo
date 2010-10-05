LDIR=../lib/lib91sdepos.a
LDIR2=../lib/libusb.a
LDIR4=../lib/*.o /lib/libsqlite3.so /lib/libcurl.so.3
IDIR=../include
CC=arm-linux-gcc 

OBJS=main_menu.o scroll_menu.o retail.o e_sqlite.o  pppcom.o  reader.o parse.o ethernet.o gsm_gprs.o V91Modem.o utils.o synchronise.o returns.o price_list.o \
grn.o dn.o exch_rate.o download.o reports.o

test_apl:$(OBJS)
	$(CC) -o retail   $(OBJS) $(LDIR2) $(LDIR4) $(LDIR) -lpthread -lm -w -O2 
main_menu.o :main_menu.c 
	$(CC) -I $(IDIR) -c main_menu.c
scroll_menu.o:scroll_menu.c
	$(CC) -I $(IDIR) -c scroll_menu.c
retail.o:retail.c
	$(CC) -I $(IDIR) -c retail.c
e_sqlite.o:e_sqlite.c
	$(CC) -I $(IDIR) -c e_sqlite.c
pppcom.o:pppcom.c
	$(CC) -I $(IDIR) -c pppcom.c
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
gsm_gprs.o:gsm_gprs.c
	$(CC) -I $(IDIR) -c gsm_gprs.c
V91Modem.o:V91Modem.c
	$(CC) -I $(IDIR) -c V91Modem.c

clean:
	rm -f main_menu.o
	rm -f scroll_menu.o
	rm -f e_sqlite.o
	rm -f insert.o
	rm -f retail.o
	rm -f pppcom.o
	rm -f fileupload.o
	rm -f reader.o
	rm -f parse.o
	rm -f ethernet.o
	rm -f gsm_gprs.o
	rm -f V91Modem.o
	rm -f utils.o
	rm -f synchronise.o
	rm -f ams
	rm -f ams.IMG

image:
	arm-linux-strip  ams
	newmkapp ams ams.IMG
