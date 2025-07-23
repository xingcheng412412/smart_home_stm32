#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <linux/input.h>
#include "bmp.h"
#include "dev.h"
int endx = -1;
int endy = -1;      // 保存读取出来的终点坐标值
int flag = -1;      // 保存触摸模式：0=>点击  1=>滑动
int exti_flag = 0;  // 保存退出状态：0 => 不退出   1 => 退出
int move_flag = -1; // 保存滑动状态：1 => Up  2 => Dowm   3 => Left   4 =>Right
int begin = 0;
int last_i; // 记录上一次的logo值
int smarthome_order = 0;
int logo = 5;
int music_logo = 0;
int music = 0;
int environment_flag = 0;
int first_music = 0;
int led = 1;
int photo_flag = 0;
int photo_mode = 0;
int piano_logo = 0;
void *get_touch(void *arg)
{
    // 1. open
    int ts_fd = open("/dev/input/event0", O_RDONLY);
    if (-1 == ts_fd)
    {
        printf("open ts failed!\n");
    }

    // 2.read
    struct input_event ev; // 定义一个结构体变量，保存从文件中读取的数据
    int x0 = -1;
    int y0 = -1; // 保存读取出来的起点坐标值

    while (1)
    {
        int res = read(ts_fd, &ev, sizeof(ev));
        if (res != sizeof(ev))
        {
            continue;
        }

        // 3. 解析
        if (ev.type == EV_ABS && ev.code == ABS_X) // 触摸屏 X轴事件
        {
            if (x0 == -1)
            {
                x0 = ev.value * 1.0 * 800 / 1024;
            }

            endx = ev.value * 1.0 * 800 / 1024;
        }

        if (ev.type == EV_ABS && ev.code == ABS_Y) // 触摸屏 Y轴事件
        {
            if (y0 == -1)
            {
                y0 = ev.value * 1.0 * 480 / 600;
            }

            endy = ev.value * 1.0 * 480 / 600;
        }

        if (ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0 || ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value == 0)
        {
            // 手指离开屏幕
            if (x0 == -1 || y0 == -1 || endx == -1 || endy == -1)
            {
                continue;
            }

            int d_x = endx - x0;
            int d_y = endy - y0;

            if (abs(d_x) < 50 && abs(d_y) < 50)
            {
                // 触摸屏点击
                printf("(%d, %d)\n", endx, endy);
                printf("begin=%d\n", begin);
                flag = 0;
                if (endx > 700 && endx < 800 && endy > 0 && endy < 60)
                {
                    exti_flag = 1;
                    begin = 0;
                    if (smarthome_order == 1 || smarthome_order == 5)
                    {
                        char cmd[32];
                        sprintf(cmd, "killall -9 madplay");
                        system(cmd);
                        first_music = 0;
                        music = 0;
                    }
                    smarthome_order = 0;
                    environment_flag = 0;
                    photo_flag = 0;
                    photo_mode = 0;
                    piano_logo = 0;
                }
                if (endx > 700 && endx < 800 && endy > 60 && endy < 120)
                {
                    exti_flag = 1;
                    music = 0;
                    environment_flag = 0;
                    led = 0;
                    photo_flag = 0;
                    photo_mode = 0;
                }
                if (flag == 0 && endx > 165 && endx < 379 && endy > 354 && endy < 408)
                {
                    if (environment_flag == 1)
                    {
                        temp += 10;
                    }
                    else if (environment_flag == 2)
                    {
                        water += 10;
                    }
                    else if (environment_flag == 3)
                    {
                        smoke += 10;
                    }
                }
                if (flag == 0 && endx > 418 && endx < 632 && endy > 354 && endy < 408)
                {
                    if (environment_flag == 1)
                    {
                        temp -= 10;
                    }
                    else if (environment_flag == 2)
                    {
                        water -= 10;
                    }
                    else if (environment_flag == 3)
                    {
                        smoke -= 10;
                    }
                }
            }
            else
            {
                // 触摸屏滑动
                flag = 1;
                if (abs(d_x) < abs(d_y)) // 垂直方向滑动
                {
                    if (d_y < 0)
                    {
                        // Up
                        printf("Up\n");
                        move_flag = 1;
                        if (smarthome_order == 1)
                        {
                            char cmd1[32];
                            sprintf(cmd1, "killall -9 madplay");
                            system(cmd1);
                            printf("%s\n", cmd1);
                            char cmd[32];
                            sprintf(cmd, "madplay -a +10 %d.mp3 &", music_logo + 1);
                            printf("%s\n", cmd);
                            system(cmd);
                        }
                    }

                    if (d_y > 0)
                    {
                        // Down
                        printf("Down\n");
                        move_flag = 2;
                        if (smarthome_order == 2)
                        {
                            char cmd[32];
                            char cmd1[32];
                            sprintf(cmd1, "killall -9 madplay");
                            system(cmd1);
                            printf("%s\n", cmd1);
                            sprintf(cmd, "madplay -a -10 %d.mp3 &", music_logo + 1);
                            printf("%s\n", cmd);
                            system(cmd);
                        }
                    }
                }

                if (abs(d_x) > abs(d_y)) // 水平方向滑动
                {
                    if (d_x < 0)
                    {
                        // Left
                        printf("Left\n");
                        move_flag = 3;
                    }

                    if (d_x > 0)
                    {
                        // Right
                        printf("Right\n");
                        move_flag = 4;
                    }
                }
            }

            x0 = -1;
            y0 = -1;
            endx = -1;
            endy = -1; // 恢复起点、终点坐标的默认值，保证下次触摸或者滑动操作正常运行
            // break;
        }
    }
    // 4. close
    close(ts_fd);
}