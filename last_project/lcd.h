#ifndef _LCD_H
#define _LCD_H

extern int lcdfd;
extern int *plcd;
int lcd_init(void);
int lcd_uninit(void);
int lcd_draw_point(int x, int y, int color);
void lcd_draw_fourleafclover(void);
void lcd_draw_circle(int x0, int y0, int radius, int color);
void lcd_draw_clear(int x0, int y0, int w, int h, int color);
void lcd_draw_word(int x0, int y0, int w, int h, char *dat);
void lcd_draw_num(int x0, int y0, int num);
void lcd_draw_clear1(int x0, int y0, int w, int h, int color);
#endif