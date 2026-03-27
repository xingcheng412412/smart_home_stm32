#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lcd.h"

char *bmp_path;
int bmp_fd;

// 输出bmp图片
void lcd_draw_bmp(char *bmp_path)
{
    bmp_fd = open(bmp_path, O_RDONLY);
    if (-1 == bmp_fd)
    {
        printf("bmp_path=%s\n",bmp_path);
        printf("open bmp failed!\n");
    }
    // 解析宽度
    int width = 0;
    lseek(bmp_fd, 0x12, SEEK_SET);
    read(bmp_fd, &width, 4);
    // printf("bmp width = %d\n", width);

    // 解析高度
    int height = 0;
    lseek(bmp_fd, 0x16, SEEK_SET);
    read(bmp_fd, &height, 4);
    // printf("bmp height = %d\n", height);

    // 解析色深
    unsigned short depth = 0;
    lseek(bmp_fd, 0x1c, SEEK_SET);
    read(bmp_fd, &depth, 2);
    // printf("bmp depth = %d\n", depth);

    // 解析数组大小
    int line_effective_bytes = 0; // 一行的有效字节数
    int fill_bytes = 0;           // 填充字节数
    int line_total_bytes = 0;     // 一行的总字节数
    int total_bytes = 0;          // 整个像素数组的字节数

    line_effective_bytes = abs(width) * (depth / 8);
    if (line_effective_bytes % 4)
    {
        fill_bytes = 4 - (line_effective_bytes % 4);
    }
    line_total_bytes = line_effective_bytes + fill_bytes;
    total_bytes = line_total_bytes * abs(height);

    // 数组赋值
    char pixel[total_bytes];
    lseek(bmp_fd, 0x36, SEEK_SET);
    read(bmp_fd, pixel, total_bytes);

    // 解析得到的每一个像素点的颜色值
    char a, r, g, b; // 保存每一个颜色分量
    int color;       // 保存解析出来的颜色值
    int i = 0;       // 遍历pixel数组下标

    for (int y = 0; y < abs(height); y++)
    {
        for (int x = 0; x < abs(width); x++)
        {
            b = pixel[i++];
            g = pixel[i++];
            r = pixel[i++];
            if (depth == 24)
            {
                a = 0;
            }
            if (depth == 32)
            {
                a = pixel[i++];
            }

            color = (a << 24) | (r << 16) | (g << 8) | b;

            // 把解析得到的颜色值显示到相应的LCD屏幕的像素点上
            // 假设我要求从LCD的（x0，y0）位置开始显示这张图片
            int end_x = (width > 0) ? x : abs(width) - x - 1;
            int end_y = (height < 0) ? y : abs(height) - y - 1;

            lcd_draw_point(end_x, end_y, color);
        }

        i += fill_bytes; // 跳过每行后面填充的字节数
    }
}

// 释放bmp资源
int bmp_uninit(void)
{
    close(bmp_fd);
    return 0;
}