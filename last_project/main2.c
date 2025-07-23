#include <stdio.h>
#include "lcd.h"
#include "touch.h"
#include "bmp.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "dev.h"

int main()
{
    lcd_init();
    char *bmp_path[] = {"1.bmp", "2.bmp", "3.bmp"};
    pthread_t tid;
    int res = pthread_create(&tid, NULL, get_touch, NULL);
    char sunxu[20][32];
    char piano_mp3[7][32];
    for (int i = 0; i < 5; i++)
    {
        sprintf(sunxu[i], "madplay %d.mp3 &", i + 1);
    }
    for (int i = 0; i < 7; i++)
    {
        sprintf(piano_mp3[i], "madplay piano_%d.mp3 &", i + 1);
    }
    int player_flag = 0;
    int light[3] = {0, 0, 0};
    int music_flag = 0;

    while (1)
    {
        if (begin == 0)
        {
            // 封面
            lcd_draw_bmp("smarthome.bmp");
            if (flag == 0 && endx > 126 && endx < 291 && endy > 100 && endy < 217)
            {
                // 智能家居
                flag = -1; // 恢复触摸模式的默认值
                smarthome_order = 1;
                begin = 1;
                printf("welcome dvd player\n");
            }
            if (flag == 0 && endx > 522 && endx < 682 && endy > 103 && endy < 220)
            {
                // 智能家居
                flag = -1; // 恢复触摸模式的默认值
                smarthome_order = 2;
                begin = 1;
                printf("welcome environment detection\n");
            }
            if (flag == 0 && endx > 518 && endx < 675 && endy > 288 && endy < 405)
            {
                // 智能家居
                flag = -1; // 恢复触摸模式的默认值
                smarthome_order = 3;
                begin = 1;
                printf("welcome running light\n");
            }
            if (flag == 0 && endx > 125 && endx < 286 && endy > 297 && endy < 409)
            {
                // 智能家居
                flag = -1; // 恢复触摸模式的默认值
                smarthome_order = 4;
                begin = 1;
                printf("welcome photo\n");
            }
            if (flag == 0 && endx > 313 && endx < 486 && endy > 214 && endy < 327)
            {
                // 智能家居
                flag = -1; // 恢复触摸模式的默认值
                smarthome_order = 5;
                begin = 1;
                printf("welcome photo\n");
            }
        }
        if (begin == 1)
        {
            if (smarthome_order == 1)
            {
                if (music == 0)
                {
                    lcd_draw_bmp("dvd_player.bmp");
                    lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                    lcd_draw_word(700, 10, 48, 48, word_code[0]);
                    lcd_draw_word(750, 10, 48, 48, word_code[1]);
                    if (flag == 0 && endx > 15 && endx < 153 && endy > 58 && endy < 452)
                    {
                        music = 1;
                        if (music_flag == 0)
                        {
                            char cmd[32];
                            sprintf(cmd, "killall -9 madplay");
                            system(cmd);
                            music_flag = 1;
                        }
                        logo = music_logo + 1;
                        printf("logo = %d\n", logo);
                    }
                    if (flag == 0 && endx > 220 && endx < 364 && endy > 58 && endy < 452)
                    {
                        // 上一首
                        flag = -1; // 恢复触摸模式的默认值
                        music_logo = music_logo - 1;
                        if (music_logo == -1)
                        {
                            music_logo = logo;
                        }
                        // logo = music_logo + 1;
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        if (first_music == 0 || music_flag == 1)
                        {
                            first_music = 1;
                            music_flag = 0;
                        }
                        else
                        {
                            system(cmd);
                        }
                        player_flag = 0;
                        system(sunxu[music_logo]);
                        printf("music_logo = %d\n", music_logo);
                        printf("sunxu = %s\n", sunxu[music_logo]);
                    }
                    if (flag == 0 && endx > 432 && endx < 571 && endy > 58 && endy < 452)
                    {
                        // 暂停与继续
                        flag = -1; // 恢复触摸模式的默认值
                        if ((music == 0 && first_music == 0) || music_flag == 1)
                        {
                            system(sunxu[music_logo]);
                            first_music = 1;
                            music_flag = 0;
                            // logo = music_logo + 1;
                        }
                        else
                        {
                            if (player_flag == 0)
                            {
                                char cmd[32];
                                sprintf(cmd, "killall -STOP madplay &");
                                system(cmd);
                                printf("%s\n", cmd);
                                player_flag = 1;
                            }
                            else
                            {
                                char cmd[32];
                                sprintf(cmd, "killall -CONT madplay &");
                                system(cmd);
                                printf("%s\n", cmd);
                                player_flag = 0;
                            }
                        }
                    }
                    if (flag == 0 && endx > 644 && endx < 785 && endy > 58 && endy < 452)
                    {
                        // 下一首
                        flag = -1; // 恢复触摸模式的默认值
                        music_logo = music_logo + 1;
                        printf("logo = %d\n", logo);
                        if (music_logo == logo)
                        {
                            music_logo = 0;
                        }
                        // logo = music_logo + 1;
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        if (first_music == 0 || music_flag == 1)
                        {
                            first_music = 1;
                            music_flag = 0;
                        }
                        else
                        {
                            system(cmd);
                        }
                        player_flag = 0;
                        system(sunxu[music_logo]);
                        printf("music_logo = %d\n", music_logo);
                        printf("sunxu = %s\n", sunxu[music_logo]);
                    }
                }
                if (music == 1)
                {
                    lcd_draw_bmp("second.bmp");
                    lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                    lcd_draw_word(700, 10, 48, 48, word_code[0]);
                    lcd_draw_word(750, 10, 48, 48, word_code[1]);
                    lcd_draw_clear1(700, 60, 100, 60, 0x00FF00);
                    lcd_draw_word(700, 70, 48, 48, word_code[2]);
                    lcd_draw_word(750, 70, 48, 48, word_code[3]);
                    if (flag == 0 && endx > 340 && endx < 461 && endy > 55 && endy < 93)
                    {
                        // 音乐选择模式
                        flag = -1; // 恢复触摸模式的默认值
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 1);
                        printf("%s\n", sunxu[logo]);
                        logo++;
                    }
                    if (flag == 0 && endx > 340 && endx < 457 && endy > 119 && endy < 153)
                    {
                        // 音乐选择模式
                        flag = -1; // 恢复触摸模式的默认值
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 2);
                        printf("%s\n", sunxu[logo]);
                        logo++;
                    }
                    if (flag == 0 && endx > 339 && endx < 458 && endy > 184 && endy < 220)
                    {
                        // 音乐选择模式
                        flag = -1; // 恢复触摸模式的默认值
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 3);
                        printf("%s\n", sunxu[logo]);
                        logo++;
                    }
                    if (flag == 0 && endx > 341 && endx < 458 && endy > 250 && endy < 286)
                    {
                        // 音乐选择模式
                        flag = -1; // 恢复触摸模式的默认值
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 4);
                        printf("%s\n", sunxu[logo]);
                        logo++;
                    }
                    if (flag == 0 && endx > 339 && endx < 460 && endy > 311 && endy < 343)
                    {
                        // 音乐选择模式
                        flag = -1; // 恢复触摸模式的默认值
                        sprintf(sunxu[logo], "madplay %d.mp3 &", 5);
                        printf("%s\n", sunxu[logo]);
                        logo++;
                    }
                }
            }
            if (smarthome_order == 2)
            {
                if (environment_flag == 0)
                {
                    lcd_draw_bmp("environment.bmp");
                    lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                    lcd_draw_word(700, 10, 48, 48, word_code[0]);
                    lcd_draw_word(750, 10, 48, 48, word_code[1]);
                    if (flag == 0 && endx > 140 && endx < 660 && endy > 71 && endy < 154)
                    {
                        environment_flag = 1;
                    }
                    if (flag == 0 && endx > 140 && endx < 660 && endy > 193 && endy < 277)
                    {
                        environment_flag = 2;
                    }
                    if (flag == 0 && endx > 140 && endx < 660 && endy > 311 && endy < 391)
                    {
                        environment_flag = 3;
                    }
                }
                if (environment_flag == 1)
                {
                    lcd_draw_bmp("device.bmp");
                    lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                    lcd_draw_word(700, 10, 48, 48, word_code[0]);
                    lcd_draw_word(750, 10, 48, 48, word_code[1]);
                    lcd_draw_clear1(700, 60, 100, 60, 0x00FF00);
                    lcd_draw_word(700, 70, 48, 48, word_code[2]);
                    lcd_draw_word(750, 70, 48, 48, word_code[3]);
                    get_dht11();
                }
                if (environment_flag == 2)
                {
                    lcd_draw_bmp("device.bmp");
                    lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                    lcd_draw_word(700, 10, 48, 48, word_code[0]);
                    lcd_draw_word(750, 10, 48, 48, word_code[1]);
                    lcd_draw_clear1(700, 60, 100, 60, 0x00FF00);
                    lcd_draw_word(700, 70, 48, 48, word_code[2]);
                    lcd_draw_word(750, 70, 48, 48, word_code[3]);
                    get_dht11();
                }
                if (environment_flag == 3)
                {
                    lcd_draw_bmp("device.bmp");
                    lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                    lcd_draw_word(700, 10, 48, 48, word_code[0]);
                    lcd_draw_word(750, 10, 48, 48, word_code[1]);
                    lcd_draw_clear1(700, 60, 100, 60, 0x00FF00);
                    lcd_draw_word(700, 70, 48, 48, word_code[2]);
                    lcd_draw_word(750, 70, 48, 48, word_code[3]);
                    get_mq2();
                }
            }
            if (smarthome_order == 3)
            {
                lcd_draw_bmp("button.bmp");
                lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                lcd_draw_word(700, 10, 48, 48, word_code[0]);
                lcd_draw_word(750, 10, 48, 48, word_code[1]);
                lcd_draw_clear1(700, 60, 100, 60, 0x00FF00);
                lcd_draw_word(700, 70, 48, 48, word_code[2]);
                lcd_draw_word(750, 70, 48, 48, word_code[3]);
                if (flag == 0 && endx > 77 && endx < 247 && endy > 42 && endy < 112)
                {
                    while (led)
                    {
                        for (int i = 7; i < 10; i++)
                        {
                            Led_Beep_Ctrl(i, 1);
                            sleep(1);
                        }
                        for (int i = 7; i < 10; i++)
                        {
                            Led_Beep_Ctrl(i, 0);
                            sleep(1);
                        }
                    }
                    led = 1;
                }
                if (flag == 0 && endx > 540 && endx < 714 && endy > 47 && endy < 118)
                {
                    Led_Beep_Ctrl(D7, 1);
                    Led_Beep_Ctrl(D8, 1);
                    Led_Beep_Ctrl(D9, 1);
                }
                if (flag == 0 && endx > 72 && endx < 247 && endy > 178 && endy < 242)
                {
                    Led_Beep_Ctrl(D7, 0);
                    Led_Beep_Ctrl(D8, 0);
                    Led_Beep_Ctrl(D9, 0);
                }
                if (flag == 0 && endx > 567 && endx < 692 && endy > 173 && endy < 244)
                {
                    if (light[0] == 0)
                    {
                        Led_Beep_Ctrl(D7, 1);
                        light[0] = 1;
                    }
                    else
                    {
                        Led_Beep_Ctrl(D7, 0);
                        light[0] = 0;
                    }
                }
                if (flag == 0 && endx > 93 && endx < 216 && endy > 370 && endy < 436)
                {
                    if (light[1] == 0)
                    {
                        Led_Beep_Ctrl(D8, 1);
                        light[1] = 1;
                    }
                    else
                    {
                        Led_Beep_Ctrl(D8, 0);
                        light[1] = 0;
                    }
                }
                if (flag == 0 && endx > 564 && endx < 688 && endy > 370 && endy < 440)
                {
                    if (light[2] == 0)
                    {
                        Led_Beep_Ctrl(D9, 1);
                        light[2] = 1;
                    }
                    else
                    {
                        Led_Beep_Ctrl(D9, 0);
                        light[2] = 0;
                    }
                }
            }
            if (smarthome_order == 4)
            {
                if (photo_flag == 0)
                {
                    // 封面
                    lcd_draw_bmp("photomode.bmp");
                    lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                    lcd_draw_word(700, 10, 48, 48, word_code[0]);
                    lcd_draw_word(750, 10, 48, 48, word_code[1]);
                    if (flag == 0 && endx > 112 && endx < 369 && endy > 198 && endy < 269)
                    {
                        photo_flag = 1;
                    }
                    if (flag == 0 && endx > 440 && endx < 701 && endy > 196 && endy < 268)
                    {
                        photo_flag = 2;
                    }
                }
                if (photo_flag == 1)
                {
                    // 自动模式
                    flag = -1; // 恢复触摸模式的默认值
                    int i = 0;
                    photo_mode = 1;
                    while (photo_mode == 1)
                    {
                        lcd_draw_bmp(bmp_path[i]);
                        lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                        lcd_draw_word(700, 10, 48, 48, word_code[0]);
                        lcd_draw_word(750, 10, 48, 48, word_code[1]);
                        lcd_draw_clear1(700, 60, 100, 60, 0x00FF00);
                        lcd_draw_word(700, 70, 48, 48, word_code[2]);
                        lcd_draw_word(750, 70, 48, 48, word_code[3]);
                        i++;
                        if (i == 3)
                        {
                            i = 0;
                        }
                        sleep(2);
                    }
                }
                if (photo_flag == 2)
                {
                    // 手动模式
                    flag = -1; // 恢复触摸模式的默认值
                    move_flag = 0;
                    photo_mode = 1;
                    while (photo_mode == 1)
                    {
                        if (move_flag == 0)
                        {
                            lcd_draw_bmp(bmp_path[0]); // 进入手动模式，先显示第一张图片
                        }
                        if (move_flag == 1)
                        {
                            lcd_draw_bmp(bmp_path[1]);
                        }
                        if (move_flag == 2)
                        {
                            lcd_draw_bmp(bmp_path[2]);
                        }
                        lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                        lcd_draw_word(700, 10, 48, 48, word_code[0]);
                        lcd_draw_word(750, 10, 48, 48, word_code[1]);
                        lcd_draw_clear1(700, 60, 100, 60, 0x00FF00);
                        lcd_draw_word(700, 70, 48, 48, word_code[2]);
                        lcd_draw_word(750, 70, 48, 48, word_code[3]);
                    }
                }
            }
            if (smarthome_order == 5)
            {
                // 封面
                lcd_draw_bmp("piano.bmp");
                lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
                lcd_draw_word(700, 10, 48, 48, word_code[0]);
                lcd_draw_word(750, 10, 48, 48, word_code[1]);
                if (flag == 0 && endx > 2 && endx < 107 && endy > 280 && endy < 440)
                {
                    if (piano_logo != 0)
                    {
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        system(cmd);
                    }
                    else
                    {
                        piano_logo++;
                    }
                    system(piano_mp3[0]);
                }
                if (flag == 0 && endx > 117 && endx < 221 && endy > 280 && endy < 440)
                {
                    if (piano_logo != 0)
                    {
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        system(cmd);
                    }
                    else
                    {
                        piano_logo++;
                    }
                    system(piano_mp3[1]);
                }
                if (flag == 0 && endx > 230 && endx < 339 && endy > 280 && endy < 440)
                {
                    if (piano_logo != 0)
                    {
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        system(cmd);
                    }
                    else
                    {
                        piano_logo++;
                    }
                    system(piano_mp3[2]);
                }
                if (flag == 0 && endx > 347 && endx < 455 && endy > 280 && endy < 440)
                {
                    if (piano_logo != 0)
                    {
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        system(cmd);
                    }
                    else
                    {
                        piano_logo++;
                    }
                    system(piano_mp3[3]);
                }
                if (flag == 0 && endx > 461 && endx < 571 && endy > 280 && endy < 440)
                {
                    if (piano_logo != 0)
                    {
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        system(cmd);
                    }
                    else
                    {
                        piano_logo++;
                    }
                    system(piano_mp3[4]);
                }
                if (flag == 0 && endx > 580 && endx < 686 && endy > 280 && endy < 440)
                {
                    if (piano_logo != 0)
                    {
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        system(cmd);
                    }
                    else
                    {
                        piano_logo++;
                    }
                    system(piano_mp3[5]);
                }
                if (flag == 0 && endx > 692 && endx < 800 && endy > 280 && endy < 440)
                {
                    if (piano_logo != 0)
                    {
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        system(cmd);
                    }
                    else
                    {
                        piano_logo++;
                    }
                    system(piano_mp3[6]);
                }
            }
        }
        bmp_uninit();
    }

    lcd_uninit();
}