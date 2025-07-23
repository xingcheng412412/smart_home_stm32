#include <stdio.h>
#include "lcd.h"
#include "touch.h"
#include "bmp.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    lcd_init();
    char *bmp_path[] = {"1.bmp", "2.bmp", "3.bmp"};
    pthread_t tid;
    int res = pthread_create(&tid, NULL, get_touch, NULL);
    // char cmd[5][32];
    // for (int i = 0; i < 5; i++)
    // {
    //     sprintf(cmd[i], "madplay %d.mp3 &", i + 1);
    // }
    char sunxu[3][32];
    for (int i = 0; i < 3; i++)
    {
        sprintf(sunxu[i], "madplay %d.mp3 &", i + 1);
    }
    int logo = 0;
    int school_ID = 4352212;
    while (1)
    {
        if (begin == 0)
        {
            // 封面
            lcd_draw_bmp("begin.bmp");
            lcd_draw_num(500, 50, school_ID);
            if (flag == 0 && endx > 26 && endx < 149 && endy > 383 && endy < 447)
            {
                // 手动模式
                flag = -1; // 恢复触摸模式的默认值
                move_flag = 0;
                while (exti_flag != 1)
                {
                    if (move_flag == 0)
                    {
                        lcd_draw_bmp(bmp_path[0]); // 进入手动模式，先显示第一张图片
                        lcd_draw_clear(650, 0, 150, 100, 0xFF0000);
                        lcd_draw_clear(650, 100, 150, 100, 0x00FF00);
                        lcd_draw_clear(650, 200, 150, 100, 0x0000FF);
                        if (last_i == 0 && music == 1)
                        {
                            char cmd[32];
                            sprintf(cmd, "killall -9 madplay");
                            system(cmd);
                            system(sunxu[move_flag]);
                        }
                    }
                    if (move_flag == 1)
                    {
                        lcd_draw_bmp(bmp_path[1]); // 进入手动模式，先显示第一张图片
                        lcd_draw_clear(650, 0, 150, 100, 0xFF0000);
                        lcd_draw_clear(650, 100, 150, 100, 0x00FF00);
                        lcd_draw_clear(650, 200, 150, 100, 0x0000FF);
                        if (last_i == 0 && music == 1)
                        {
                            char cmd[32];
                            sprintf(cmd, "killall -9 madplay");
                            system(cmd);
                            system(sunxu[move_flag]);
                        }
                    }

                    if (move_flag == 2)
                    {
                        lcd_draw_bmp(bmp_path[2]); // 进入手动模式，先显示第一张图片
                        lcd_draw_clear(650, 0, 150, 100, 0xFF0000);
                        lcd_draw_clear(650, 100, 150, 100, 0x00FF00);
                        lcd_draw_clear(650, 200, 150, 100, 0x0000FF);
                        if (last_i == 0 && music == 1)
                        {
                            char cmd[32];
                            sprintf(cmd, "killall -9 madplay");
                            system(cmd);
                            system(sunxu[move_flag]);
                        }
                    }
                    last_i++;
                }
                exti_flag = 0; // 恢复退出标志位的默认值
            }

            if (flag == 0 && endx > 653 && endx < 773 && endy > 381 && endy < 443)
            {
                // 自动模式
                flag = -1; // 恢复触摸模式的默认值
                int i = 0;
                while (exti_flag != 1)
                {
                    lcd_draw_bmp(bmp_path[i]);
                    lcd_draw_clear(650, 0, 150, 100, 0xFF0000);
                    lcd_draw_clear(650, 100, 150, 100, 0x00FF00);
                    lcd_draw_clear(650, 200, 150, 100, 0x0000FF);
                    if (music == 1)
                    {
                        system(sunxu[i]);
                    }
                    sleep(3);
                    if (music == 1)
                    {
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        system(cmd);
                    }

                    i++;
                    if (i == 3)
                    {
                        i = 0;
                    }
                }
                exti_flag = 0; // 恢复退出标志位的默认值
            }
            if (flag == 0 && endx > 25 && endx < 148 && endy > 31 && endy < 163)
            {
                // 音乐选择模式
                flag = -1; // 恢复触摸模式的默认值
                while (exti_flag != 1)
                {

                    // lcd_draw_exit_with_frame(650, 0, 800, 100, 0xFF0000, 0xFF0000, 0x0000FF);
                    char cmd[32];
                    // sprintf(cmd, "killall -STOP madplay &");
                    // system(cmd);
                    begin = 1;
                    logo = 0;
                    printf("logo =%d,bengin =%d\n", logo, begin);
                    break;
                }
                exti_flag = 0; // 恢复退出标志位的默认值
            }
        }
        if (begin == 1)
        {
            lcd_draw_bmp("second.bmp");
            if (flag == 0 && endx > 340 && endx < 461 && endy > 55 && endy < 93)
            {
                // 音乐选择模式
                flag = -1; // 恢复触摸模式的默认值
                while (exti_flag != 1)
                {
                    switch (logo)
                    {
                    case 0:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 1);
                        break;
                    case 1:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 1);
                        break;
                    case 2:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 1);
                        break;
                    }
                    printf("logo = %d\n", logo);
                    printf("sunxu = %s\n", sunxu[logo]);
                    logo++;
                    if (logo == 3)
                    {
                        begin = 0;
                    }
                    break;
                }
                exti_flag = 0; // 恢复退出标志位的默认值
            }
            if (flag == 0 && endx > 340 && endx < 457 && endy > 119 && endy < 153)
            {
                // 音乐选择模式
                flag = -1; // 恢复触摸模式的默认值
                while (exti_flag != 1)
                {

                    switch (logo)
                    {
                    case 0:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 2);
                        break;
                    case 1:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 2);
                        break;
                    case 2:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 2);
                        break;
                    }
                    printf("logo = %d\n", logo);
                    printf("sunxu = %s\n", sunxu[logo]);
                    logo++;
                    if (logo == 3)
                    {
                        begin = 0;
                    }
                    break;
                }
                exti_flag = 0; // 恢复退出标志位的默认值
            }
            if (flag == 0 && endx > 339 && endx < 458 && endy > 184 && endy < 220)
            {
                // 音乐选择模式
                flag = -1; // 恢复触摸模式的默认值
                while (exti_flag != 1)
                {
                    switch (logo)
                    {
                    case 0:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 3);
                        break;
                    case 1:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 3);
                        break;
                    case 2:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 3);
                        break;
                    }
                    printf("logo = %d\n", logo);
                    printf("sunxu = %s\n", sunxu[logo]);
                    logo++;
                    if (logo == 3)
                    {
                        begin = 0;
                    }
                    break;
                }
                exti_flag = 0; // 恢复退出标志位的默认值
            }
            if (flag == 0 && endx > 341 && endx < 458 && endy > 250 && endy < 286)
            {
                // 音乐选择模式
                flag = -1; // 恢复触摸模式的默认值
                while (exti_flag != 1)
                {
                    switch (logo)
                    {
                    case 0:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 4);
                        break;
                    case 1:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 4);
                        break;
                    case 2:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 4);
                        break;
                    }
                    printf("logo = %d\n", logo);
                    printf("sunxu = %s\n", sunxu[logo]);
                    logo++;
                    if (logo == 3)
                    {
                        begin = 0;
                    }
                    break;
                }
                exti_flag = 0; // 恢复退出标志位的默认值
            }
            if (flag == 0 && endx > 339 && endx < 460 && endy > 311 && endy < 343)
            {
                // 音乐选择模式
                flag = -1; // 恢复触摸模式的默认值
                while (exti_flag != 1)
                {
                    switch (logo)
                    {
                    case 0:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 5);
                        break;
                    case 1:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 5);
                        break;
                    case 2:
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 5);
                        break;
                    }
                    printf("logo = %d\n", logo);
                    printf("sunxu = %s\n", sunxu[logo]);
                    logo++;
                    if (logo == 3)
                    {
                        begin = 0;
                    }
                    break;
                }
                exti_flag = 0; // 恢复退出标志位的默认值
            }
        }
    }

    lcd_uninit();
}