#ifndef TOUCH_H
#define TOUCH_H
extern int endx;
extern int endy; // 保存读取出来的终点坐标值
extern int flag;
extern int exti_flag;
extern int move_flag;
extern int begin;
extern int last_i;
extern int smarthome_order;
extern int logo;
extern int music_logo;
extern int music;
extern int environment_flag;
extern int first_music;
extern int led;
extern int photo_flag;
extern int photo_mode;
extern int piano_logo;
void *get_touch(void *arg);

#endif