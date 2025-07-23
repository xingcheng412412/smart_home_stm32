#ifndef _BMP_H
#define _BMP_H
extern int bmp_fd;
extern char *bmp_path;
int bmp_uninit(void);
void lcd_draw_bmp(char *bmp_path);
#endif