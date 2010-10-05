#ifndef __LK__KERMIT__
#define __LK__KERMIT__

int send_text_file(const char *filename);

int send_image_file(const char *filename);

int rcv_text_file();

int rcv_image_file(const char *path);

#endif



