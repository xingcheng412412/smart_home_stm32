#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "dev.h"
#include <sys/ioctl.h>
#include <termios.h>
#include "lcd.h"
#include <string.h>
#include "touch.h"
#include "bmp.h"
#define GEC6818_GET_DHTDATA _IOR('K', 0, unsigned int) // 注册宏
/*
    Led_Beep_Ctrl:控制Led和Beep
        @dev		指定你要控制的文件路径
        @status		指定状态
*/
char word_code[4][288] = {
    // 退
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0xE0, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x70, 0x0E, 0x00, 0x1E, 0x00, 0x00, 0x38,
     0x07, 0x0F, 0xFF, 0x00, 0x00, 0x3C, 0x07, 0xFF, 0xFE, 0x00, 0x00, 0x1E, 0x07, 0xFF, 0x9C, 0x00,
     0x00, 0x1F, 0x07, 0x00, 0x1C, 0x00, 0x00, 0x0F, 0x07, 0x00, 0x1C, 0x00, 0x00, 0x0F, 0x07, 0x00,
     0x1C, 0x00, 0x00, 0x06, 0x07, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x07, 0x7F, 0xFC, 0x00, 0x00, 0x00,
     0x07, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x38, 0x00, 0x00, 0x03, 0x07, 0x00, 0x38, 0x00,
     0x00, 0x03, 0xC7, 0x00, 0x38, 0x00, 0x00, 0x3F, 0xC7, 0x00, 0x38, 0x00, 0x0F, 0xFF, 0x87, 0x03,
     0xF8, 0x00, 0x07, 0xFF, 0x87, 0xFF, 0xFC, 0x00, 0x03, 0x07, 0x07, 0xFF, 0xF8, 0x00, 0x00, 0x0E,
     0x07, 0x00, 0x0E, 0x00, 0x00, 0x0E, 0x07, 0x00, 0x0E, 0x00, 0x00, 0x1C, 0x07, 0x70, 0x1F, 0x00,
     0x00, 0x1C, 0x07, 0x3C, 0x1E, 0x00, 0x00, 0x1C, 0x07, 0x1E, 0x38, 0x00, 0x00, 0x1E, 0x07, 0x07,
     0xF0, 0x00, 0x00, 0x0E, 0x07, 0x03, 0xE0, 0x00, 0x00, 0x0F, 0x07, 0x01, 0xF0, 0x00, 0x00, 0x0F,
     0x07, 0x03, 0xF8, 0x00, 0x00, 0x0F, 0x07, 0x0F, 0x3E, 0x00, 0x00, 0x0F, 0x07, 0x3E, 0x1F, 0x80,
     0x00, 0x0E, 0x07, 0xF8, 0x0F, 0xC0, 0x00, 0x0E, 0x07, 0xE0, 0x07, 0xC0, 0x00, 0x1C, 0x0F, 0xC0,
     0x01, 0xC0, 0x07, 0xFE, 0x0F, 0x80, 0x00, 0xC0, 0x0F, 0xFF, 0xF6, 0x00, 0x00, 0x00, 0x07, 0x01,
     0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x7F, 0x80, 0x38, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x01,
     0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // 出
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00,
     0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00,
     0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x0E, 0x00, 0x00, 0x70, 0x01, 0xC0,
     0x0F, 0x00, 0x00, 0x38, 0x01, 0xC0, 0x0F, 0x00, 0x00, 0x3C, 0x01, 0xC0, 0x0F, 0x00, 0x00, 0x1C,
     0x01, 0xC0, 0x0F, 0x00, 0x00, 0x1C, 0x01, 0xC0, 0x0E, 0x00, 0x00, 0x1C, 0x01, 0xC0, 0x0E, 0x00,
     0x00, 0x1C, 0x01, 0xC0, 0x0E, 0x00, 0x00, 0x1C, 0x01, 0xC0, 0x0E, 0x00, 0x00, 0x1C, 0x01, 0xC0,
     0x0E, 0x00, 0x00, 0x1C, 0x01, 0xC0, 0x1E, 0x00, 0x00, 0x1C, 0x01, 0xC0, 0x1C, 0x00, 0x00, 0x1C,
     0x01, 0xC1, 0xFC, 0x00, 0x00, 0x1C, 0x0F, 0xFF, 0xFC, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFC, 0x00,
     0x00, 0x3F, 0xFF, 0xC0, 0x1C, 0x00, 0x00, 0x1E, 0x01, 0xC0, 0x18, 0x00, 0x00, 0x08, 0x01, 0xC0,
     0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x06, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x07, 0x80, 0x01, 0xC0,
     0x01, 0xC0, 0x03, 0xC0, 0x00, 0xF0, 0x01, 0xC0, 0x03, 0x80, 0x00, 0xF0, 0x01, 0xC0, 0x07, 0x80,
     0x00, 0x70, 0x01, 0xC0, 0x07, 0x80, 0x00, 0x70, 0x01, 0xC0, 0x07, 0x80, 0x00, 0x70, 0x01, 0xC0,
     0x07, 0x00, 0x00, 0x70, 0x01, 0xC0, 0x07, 0x00, 0x00, 0x70, 0x01, 0xC0, 0x07, 0x00, 0x00, 0x70,
     0x01, 0xC0, 0x07, 0x00, 0x00, 0x70, 0x01, 0xC0, 0x0F, 0x00, 0x00, 0x70, 0x01, 0xC0, 0x7F, 0x00,
     0x00, 0x70, 0x1F, 0xFF, 0xFE, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0xFF, 0xFF, 0xC0,
     0x0E, 0x00, 0x00, 0x70, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x60, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // 返
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0xE0, 0x0C, 0x00, 0xFE, 0x00, 0x00, 0x70,
     0x0C, 0x07, 0xFC, 0x00, 0x00, 0x78, 0x0E, 0x3F, 0xC0, 0x00, 0x00, 0x3C, 0x0E, 0xFE, 0x00, 0x00,
     0x00, 0x1E, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0E, 0x00,
     0x00, 0x00, 0x00, 0x0E, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0E, 0x00, 0x38, 0x00, 0x00, 0x00,
     0x0E, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x0E, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF8, 0x00,
     0x00, 0x03, 0x0F, 0xFC, 0x78, 0x00, 0x00, 0x07, 0x8E, 0x00, 0x78, 0x00, 0x00, 0x1F, 0xCE, 0x00,
     0x70, 0x00, 0x01, 0xFF, 0x8E, 0x00, 0xF0, 0x00, 0x1F, 0xFF, 0x0E, 0x60, 0xE0, 0x00, 0x0F, 0x8E,
     0x0E, 0x71, 0xE0, 0x00, 0x00, 0x0E, 0x0E, 0x3D, 0xC0, 0x00, 0x00, 0x1C, 0x1E, 0x1F, 0xC0, 0x00,
     0x00, 0x1C, 0x1C, 0x0F, 0x80, 0x00, 0x00, 0x1C, 0x1C, 0x07, 0x80, 0x00, 0x00, 0x38, 0x1C, 0x0F,
     0xC0, 0x00, 0x00, 0x1C, 0x38, 0x1F, 0xE0, 0x00, 0x00, 0x1E, 0x38, 0x1C, 0xF8, 0x00, 0x00, 0x0E,
     0x38, 0x38, 0x7C, 0x00, 0x00, 0x0E, 0x70, 0x70, 0x3E, 0x00, 0x00, 0x0E, 0x60, 0xE0, 0x1E, 0x00,
     0x00, 0x0E, 0xE1, 0xC0, 0x0E, 0x00, 0x00, 0x0F, 0xC3, 0x80, 0x0E, 0x00, 0x00, 0x1D, 0x8F, 0x00,
     0x06, 0x00, 0x07, 0xFC, 0x1E, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xE8, 0x00, 0x00, 0x00, 0x0F, 0xDF,
     0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x00,
     0x00, 0x00, 0x00, 0xFF, 0xC0, 0x3C, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x01,
     0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // 回
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
     0x00, 0x00, 0x03, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x03, 0x80, 0x00, 0x70, 0x00, 0x07, 0xFF, 0xC0,
     0x00, 0x70, 0x7F, 0xFF, 0xFF, 0x80, 0x00, 0x7F, 0xFF, 0xFF, 0xF7, 0x80, 0x00, 0x7F, 0xFF, 0x80,
     0x07, 0x80, 0x00, 0x78, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78,
     0x00, 0x00, 0x07, 0x00, 0x00, 0x78, 0x00, 0x00, 0x07, 0x00, 0x00, 0x78, 0x60, 0x07, 0x07, 0x00,
     0x00, 0x78, 0x70, 0x0F, 0x87, 0x00, 0x00, 0x78, 0x7F, 0xFF, 0x87, 0x00, 0x00, 0x78, 0x7F, 0xFF,
     0x07, 0x00, 0x00, 0x78, 0x7F, 0x8F, 0x07, 0x00, 0x00, 0x38, 0x78, 0x0F, 0x07, 0x00, 0x00, 0x38,
     0x78, 0x0F, 0x07, 0x00, 0x00, 0x38, 0x38, 0x0E, 0x07, 0x00, 0x00, 0x38, 0x38, 0x0E, 0x07, 0x00,
     0x00, 0x38, 0x38, 0x0E, 0x07, 0x00, 0x00, 0x38, 0x38, 0x0E, 0x0F, 0x00, 0x00, 0x38, 0x3F, 0xFF,
     0x0F, 0x00, 0x00, 0x38, 0x3F, 0xFF, 0x8F, 0x00, 0x00, 0x38, 0x7F, 0xF0, 0x0F, 0x00, 0x00, 0x38,
     0x78, 0x00, 0x0F, 0x00, 0x00, 0x38, 0x38, 0x00, 0x0F, 0x00, 0x00, 0x38, 0x18, 0x00, 0x0E, 0x00,
     0x00, 0x38, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x38, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x38, 0x00, 0x00,
     0x0E, 0x00, 0x00, 0x38, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x38, 0x00, 0xFF, 0xEE, 0x00, 0x00, 0x3F,
     0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x1E, 0x00,
     0x00, 0x78, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x38, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x38, 0x00, 0x00,
     0x04, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
char word_cord[10][288] =
    {
        // 温
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x1C, 0x00, 0x00, 0xE0, 0x0E, 0x00, 0x3E, 0x00, 0x00, 0x78,
         0x07, 0xFF, 0xFE, 0x00, 0x00, 0x3C, 0x07, 0xFF, 0xFC, 0x00, 0x00, 0x1E, 0x07, 0xFC, 0x38, 0x00,
         0x00, 0x0F, 0x07, 0x00, 0x38, 0x00, 0x00, 0x0E, 0x07, 0x00, 0x38, 0x00, 0x00, 0x06, 0x07, 0x00,
         0x38, 0x00, 0x00, 0x00, 0x07, 0x3F, 0xB8, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xF8, 0x00, 0x00, 0x00,
         0x07, 0xFF, 0xB8, 0x00, 0x00, 0x00, 0x07, 0x00, 0x78, 0x00, 0x00, 0x01, 0x87, 0x00, 0x78, 0x00,
         0x08, 0x03, 0x87, 0x00, 0x78, 0x00, 0x0E, 0x03, 0x87, 0x00, 0x70, 0x00, 0x07, 0x83, 0x07, 0xFF,
         0xF8, 0x00, 0x03, 0xC7, 0x0F, 0xFF, 0xF8, 0x00, 0x01, 0xE7, 0x0F, 0xFF, 0x80, 0x00, 0x00, 0xE6,
         0x07, 0x00, 0x00, 0x00, 0x00, 0x66, 0x03, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x0E, 0x60, 0x00, 0x07, 0x00, 0x00, 0x0C, 0x60, 0x00, 0x3F, 0x80, 0x00, 0x1C, 0x73, 0xFF,
         0xFF, 0x80, 0x00, 0x1C, 0x7F, 0xFF, 0xFF, 0x00, 0x00, 0x1C, 0x7F, 0xFF, 0x07, 0x00, 0x00, 0x38,
         0x71, 0xC7, 0x87, 0x00, 0x00, 0x38, 0x71, 0xE7, 0x8F, 0x00, 0x00, 0x38, 0x71, 0xC7, 0x0F, 0x00,
         0x00, 0x78, 0x71, 0xC7, 0x0F, 0x00, 0x00, 0x70, 0x71, 0xC7, 0x0F, 0x00, 0x00, 0x70, 0x71, 0xC7,
         0x0E, 0x00, 0x00, 0xF0, 0x71, 0xC7, 0x0E, 0x00, 0x1C, 0xE0, 0x71, 0xC7, 0x0E, 0x00, 0x0F, 0xE0,
         0x71, 0xC7, 0x0E, 0x00, 0x07, 0xE0, 0x71, 0xC7, 0x0E, 0x00, 0x03, 0xE0, 0x71, 0xC7, 0x0E, 0x30,
         0x03, 0xE0, 0x71, 0xFF, 0xFF, 0xF8, 0x01, 0xDF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0xDF, 0xFF, 0xFF,
         0xF8, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        // 度
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
         0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00,
         0x00, 0xF0, 0x00, 0x00, 0x00, 0x18, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x70, 0x03, 0x00,
         0x00, 0x1C, 0x00, 0x03, 0xFF, 0x80, 0x00, 0x1C, 0x0F, 0xFF, 0xFF, 0x80, 0x00, 0x1F, 0xFF, 0xFF,
         0xE0, 0x00, 0x00, 0x1F, 0xFF, 0x87, 0x00, 0x00, 0x00, 0x1C, 0x1E, 0x07, 0x80, 0x00, 0x00, 0x1C,
         0x0E, 0x07, 0x80, 0x00, 0x00, 0x1C, 0x0E, 0x07, 0x80, 0x00, 0x00, 0x1C, 0x0E, 0x07, 0x8F, 0x00,
         0x00, 0x1C, 0x0E, 0x1F, 0xFF, 0x80, 0x00, 0x1C, 0x1F, 0xFF, 0xFF, 0x80, 0x00, 0x1F, 0xFF, 0xFF,
         0x80, 0x00, 0x00, 0x1F, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x1F, 0x0E, 0x0F, 0x00, 0x00, 0x00, 0x1C,
         0x0E, 0x0F, 0x00, 0x00, 0x00, 0x1C, 0x0E, 0x3F, 0x00, 0x00, 0x00, 0x1C, 0x0F, 0xFF, 0x00, 0x00,
         0x00, 0x3C, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x3C, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x38, 0x0E, 0x00,
         0x00, 0x00, 0x00, 0x38, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x38, 0x03, 0xFF, 0xF0, 0x00, 0x00, 0x38,
         0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x70, 0x7F, 0xE3, 0xC0, 0x00, 0x00, 0x70, 0x0C, 0x07, 0x80, 0x00,
         0x00, 0x70, 0x3E, 0x0F, 0x00, 0x00, 0x00, 0xE0, 0x1F, 0x1E, 0x00, 0x00, 0x00, 0xE0, 0x07, 0xBC,
         0x00, 0x00, 0x00, 0xE0, 0x03, 0xFC, 0x00, 0x00, 0x01, 0xC0, 0x01, 0xF8, 0x00, 0x00, 0x01, 0xC0,
         0x01, 0xF8, 0x00, 0x00, 0x03, 0x80, 0x03, 0xFC, 0x00, 0x00, 0x03, 0x80, 0x0F, 0x9E, 0x00, 0x00,
         0x07, 0x00, 0x3F, 0x0F, 0x80, 0x00, 0x0E, 0x00, 0xFC, 0x03, 0xC0, 0x00, 0x0E, 0x03, 0xF0, 0x01,
         0xF0, 0x00, 0x1C, 0x0F, 0xC0, 0x00, 0xFF, 0xF0, 0x18, 0x7E, 0x00, 0x00, 0x7F, 0xF0, 0x00, 0x70,
         0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        // 湿
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x80, 0x38, 0x00, 0x03, 0x00, 0x01, 0xE0, 0x18, 0x00, 0x07, 0x80, 0x00, 0xF0,
         0x1C, 0x3F, 0xFF, 0x80, 0x00, 0x7C, 0x1F, 0xFF, 0xFF, 0x00, 0x00, 0x3E, 0x1F, 0xFF, 0x87, 0x00,
         0x00, 0x1F, 0x1C, 0x00, 0x07, 0x00, 0x00, 0x0F, 0x1C, 0x00, 0x07, 0x00, 0x00, 0x07, 0x1C, 0x00,
         0x07, 0x00, 0x00, 0x02, 0x1C, 0x03, 0xEF, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0x00, 0x00, 0x00,
         0x1F, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0xDF, 0xE0, 0x0E, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x0E, 0x00,
         0x1C, 0x01, 0xDE, 0x00, 0x0E, 0x00, 0x0F, 0x01, 0xCE, 0x00, 0x0E, 0x00, 0x07, 0x81, 0x8E, 0x01,
         0xFE, 0x00, 0x03, 0xE3, 0x9F, 0xFF, 0xFF, 0x00, 0x01, 0xF3, 0x9F, 0xFF, 0xFE, 0x00, 0x00, 0xE3,
         0x8F, 0xE3, 0x00, 0x00, 0x00, 0xE3, 0x06, 0x03, 0x80, 0x00, 0x00, 0x67, 0x00, 0xC3, 0xC0, 0x00,
         0x00, 0x07, 0x00, 0xE3, 0xC1, 0x80, 0x00, 0x07, 0x00, 0xF1, 0xC1, 0xC0, 0x00, 0x0E, 0x00, 0xF1,
         0xC1, 0xC0, 0x00, 0x0E, 0xE0, 0xF1, 0xC3, 0xE0, 0x00, 0x0E, 0x70, 0xF1, 0xC3, 0xC0, 0x00, 0x1C,
         0x70, 0xF1, 0xC7, 0x80, 0x00, 0x1C, 0x38, 0xF1, 0xCF, 0x00, 0x00, 0x1C, 0x1C, 0xF1, 0xCE, 0x00,
         0x00, 0x3C, 0x1E, 0xF1, 0xDC, 0x00, 0x00, 0x38, 0x0F, 0xE3, 0xF8, 0x00, 0x00, 0x38, 0x0F, 0xE3,
         0xF8, 0x00, 0x18, 0x78, 0x07, 0xE3, 0xF0, 0x00, 0x1E, 0x70, 0x06, 0xE3, 0xE0, 0x00, 0x0F, 0x70,
         0x00, 0xE3, 0xC0, 0x00, 0x07, 0xF0, 0x00, 0xE3, 0xC0, 0x00, 0x03, 0xF0, 0x00, 0xE3, 0xC0, 0x00,
         0x01, 0xE0, 0x00, 0xE3, 0xFF, 0xF0, 0x00, 0xE3, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0xE3, 0xFF, 0xFF,
         0xFE, 0x00, 0x00, 0x01, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        // 烟
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E,
         0x08, 0x00, 0x00, 0x40, 0x00, 0x1E, 0x1C, 0x00, 0x00, 0xF0, 0x00, 0x1C, 0x0C, 0x00, 0x7F, 0xF0,
         0x00, 0x1C, 0x0E, 0xFF, 0xFF, 0xE0, 0x00, 0x1C, 0x0F, 0xFF, 0xF8, 0xE0, 0x00, 0x1E, 0x0F, 0xF0,
         0x00, 0xE0, 0x00, 0x1E, 0x0E, 0x06, 0x00, 0xE0, 0x00, 0x1E, 0x6E, 0x07, 0x00, 0xE0, 0x00, 0x1E,
         0x7E, 0x07, 0x81, 0xE0, 0x0C, 0x1E, 0x7E, 0x07, 0x81, 0xE0, 0x0E, 0x1E, 0xFE, 0x07, 0x81, 0xE0,
         0x0E, 0x1F, 0xEE, 0x07, 0x81, 0xE0, 0x07, 0x1D, 0xCE, 0x07, 0x81, 0xE0, 0x07, 0x1F, 0x8E, 0x07,
         0xBD, 0xE0, 0x03, 0x9F, 0x0E, 0x07, 0xFF, 0xC0, 0x03, 0xDF, 0x0E, 0xFF, 0xFF, 0xC0, 0x03, 0xDE,
         0x0E, 0xFF, 0x81, 0xC0, 0x01, 0xFC, 0x0E, 0x77, 0x01, 0xC0, 0x01, 0xDC, 0x0E, 0x07, 0x01, 0xC0,
         0x01, 0xDC, 0x0E, 0x07, 0x01, 0xC0, 0x00, 0x9C, 0x0E, 0x0F, 0x81, 0xC0, 0x00, 0x1C, 0x0E, 0x0F,
         0xC1, 0xC0, 0x00, 0x3C, 0x0E, 0x0E, 0xF1, 0xC0, 0x00, 0x3C, 0x0E, 0x1E, 0x79, 0xC0, 0x00, 0x3F,
         0x0E, 0x1C, 0x3D, 0xC0, 0x00, 0x3F, 0x8E, 0x3C, 0x3F, 0xC0, 0x00, 0x3B, 0xCE, 0x38, 0x1F, 0xC0,
         0x00, 0x79, 0xEE, 0x70, 0x0D, 0xC0, 0x00, 0x70, 0xFE, 0xE0, 0x01, 0xC0, 0x00, 0x70, 0xFE, 0xE0,
         0x01, 0xC0, 0x00, 0xE0, 0x6E, 0xC0, 0x01, 0xC0, 0x00, 0xE0, 0x0E, 0x00, 0x01, 0xC0, 0x01, 0xC0,
         0x0E, 0x00, 0x01, 0xC0, 0x01, 0xC0, 0x0E, 0x00, 0x3F, 0xC0, 0x03, 0x80, 0x0E, 0xFF, 0xFF, 0xC0,
         0x03, 0x80, 0x0F, 0xFF, 0xFF, 0xC0, 0x07, 0x00, 0x0F, 0xFC, 0x01, 0xC0, 0x0E, 0x00, 0x0E, 0x00,
         0x01, 0xC0, 0x0C, 0x00, 0x06, 0x00, 0x01, 0xC0, 0x1C, 0x00, 0x06, 0x00, 0x00, 0x00, 0x38, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        // 雾
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
         0xC0, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xE0, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x1F,
         0xFF, 0xC0, 0x06, 0x00, 0x00, 0x0F, 0x83, 0x80, 0x3F, 0x00, 0x00, 0xC0, 0x03, 0xBF, 0xFF, 0x80,
         0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFE, 0x0E, 0x00, 0x01, 0xFF, 0xFF, 0x80,
         0x1C, 0x00, 0x01, 0xF0, 0x03, 0x87, 0xFC, 0x00, 0x01, 0xC0, 0x23, 0xFF, 0xF0, 0x00, 0x03, 0xDF,
         0xFB, 0xBF, 0x00, 0x00, 0x07, 0xBF, 0xFB, 0x80, 0x30, 0x00, 0x03, 0x9C, 0x07, 0x9F, 0xF8, 0x00,
         0x03, 0x83, 0xF7, 0xBF, 0xF8, 0x00, 0x00, 0x3F, 0xFF, 0x9C, 0x00, 0x00, 0x00, 0x3F, 0xFB, 0x80,
         0x00, 0x00, 0x00, 0x00, 0x79, 0x81, 0xC0, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xE0, 0x00, 0x00, 0x01,
         0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x03, 0xFF, 0xEF, 0x80, 0x00, 0x00, 0x07, 0xB0, 0x3E, 0x00, 0x00,
         0x00, 0x0F, 0xFC, 0x7C, 0x00, 0x00, 0x00, 0x3C, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x78, 0x0F, 0xE0,
         0x00, 0x00, 0x01, 0xE0, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0xC0, 0x7E, 0x7F, 0x00, 0x00, 0x00, 0x03,
         0xFE, 0x0F, 0xE0, 0x00, 0x00, 0x0F, 0xCE, 0x03, 0xFF, 0xF8, 0x00, 0x7E, 0x1E, 0x03, 0xFF, 0xF8,
         0x03, 0xF0, 0x1E, 0x7F, 0xDF, 0xC0, 0x1F, 0x80, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x1F, 0xFF, 0xF7,
         0x00, 0x00, 0x00, 0x0F, 0xF8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0F, 0x00, 0x00, 0x00, 0x00,
         0x70, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xE3, 0x0E, 0x00, 0x00, 0x00, 0x01, 0xE3, 0x8E, 0x00, 0x00,
         0x00, 0x03, 0xC1, 0xFE, 0x00, 0x00, 0x00, 0x07, 0x80, 0xFC, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x7C,
         0x00, 0x00, 0x00, 0x7C, 0x00, 0x38, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x38, 0x00, 0x00, 0x01, 0xC0,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        // 浓
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0xE0,
         0x00, 0x0E, 0x00, 0x00, 0x00, 0x70, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x1E, 0x00, 0x00,
         0x00, 0x3E, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1E, 0x18, 0x1C, 0x03, 0x00, 0x00, 0x0E, 0x18, 0x1C,
         0x07, 0x80, 0x00, 0x0E, 0x38, 0x3F, 0xFF, 0xC0, 0x00, 0x06, 0x3F, 0xFF, 0xFF, 0x80, 0x00, 0x00,
         0x3F, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x7F, 0x38, 0x1E, 0x00, 0x00, 0x00, 0x70, 0x78, 0x1C, 0x00,
         0x00, 0x00, 0xF0, 0x78, 0x38, 0x00, 0x0E, 0x01, 0xF0, 0x70, 0x30, 0x00, 0x0F, 0x00, 0xE0, 0xF0,
         0x00, 0x00, 0x07, 0xC0, 0x60, 0xF8, 0x03, 0x00, 0x03, 0xE3, 0x81, 0xFC, 0x07, 0x00, 0x01, 0xF3,
         0x01, 0xFC, 0x07, 0x00, 0x00, 0xE3, 0x03, 0xCE, 0x0F, 0x80, 0x00, 0x67, 0x03, 0x8E, 0x1F, 0x00,
         0x00, 0x07, 0x07, 0x87, 0x3C, 0x00, 0x00, 0x0E, 0x07, 0x87, 0x78, 0x00, 0x00, 0x0E, 0x0F, 0x83,
         0xE0, 0x00, 0x00, 0x0E, 0x1F, 0x83, 0xC0, 0x00, 0x00, 0x1C, 0x3F, 0x81, 0xC0, 0x00, 0x00, 0x1C,
         0x3B, 0x81, 0xC0, 0x00, 0x00, 0x1C, 0x73, 0x80, 0xE0, 0x00, 0x00, 0x3C, 0xE3, 0x80, 0xE0, 0x00,
         0x00, 0x39, 0xC3, 0x80, 0x70, 0x00, 0x00, 0x3B, 0x83, 0x80, 0x78, 0x00, 0x00, 0x7F, 0x03, 0x80,
         0x38, 0x00, 0x00, 0x7E, 0x03, 0x80, 0x3C, 0x00, 0x00, 0x7C, 0x03, 0x83, 0x1C, 0x00, 0x1C, 0xF0,
         0x07, 0x8E, 0x1E, 0x00, 0x0F, 0xF0, 0x07, 0x9C, 0x0F, 0x00, 0x07, 0xE0, 0x07, 0xF8, 0x0F, 0xF0,
         0x03, 0xE0, 0x07, 0xF0, 0x0F, 0xFC, 0x01, 0xE0, 0x07, 0xE0, 0x0F, 0xF0, 0x01, 0xC0, 0x07, 0xC0,
         0x00, 0x00, 0x00, 0xC0, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        // 阈
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x03, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x07, 0x80, 0x00, 0x1E,
         0x00, 0x0F, 0xFF, 0xC0, 0x00, 0x0F, 0x3F, 0xFF, 0xFF, 0x80, 0x00, 0x07, 0x1F, 0xFF, 0xE7, 0x80,
         0x00, 0x06, 0x04, 0x00, 0x07, 0x80, 0x01, 0x80, 0x00, 0x00, 0x07, 0x80, 0x01, 0xC0, 0x03, 0xC0,
         0x07, 0x80, 0x01, 0xE0, 0x03, 0xC3, 0x07, 0x80, 0x01, 0xE0, 0x01, 0xC3, 0xC7, 0x80, 0x01, 0xE0,
         0x01, 0xC1, 0xE7, 0x80, 0x01, 0xE0, 0x01, 0xC2, 0xF7, 0x80, 0x01, 0xE0, 0x03, 0xFF, 0xF7, 0x80,
         0x01, 0xE3, 0xFF, 0xFF, 0xA7, 0x80, 0x01, 0xEF, 0xFF, 0xE0, 0x07, 0x80, 0x01, 0xE7, 0xC0, 0xE0,
         0x07, 0x80, 0x01, 0xE0, 0x00, 0xE3, 0x07, 0x80, 0x01, 0xE3, 0x0E, 0xE3, 0x87, 0x80, 0x01, 0xE3,
         0xFF, 0xE3, 0xC7, 0x80, 0x01, 0xE3, 0xFF, 0xF7, 0x87, 0x80, 0x01, 0xE3, 0xFE, 0x77, 0x07, 0x80,
         0x01, 0xE3, 0x8E, 0x77, 0x07, 0x80, 0x01, 0xE3, 0x8E, 0x7E, 0x07, 0x80, 0x01, 0xE3, 0xFE, 0x3E,
         0x07, 0x80, 0x01, 0xE3, 0xFF, 0x3C, 0x07, 0x80, 0x01, 0xE3, 0xF8, 0x3C, 0x37, 0x80, 0x01, 0xE3,
         0x80, 0x3C, 0x37, 0x80, 0x01, 0xE1, 0x87, 0xFE, 0x77, 0x80, 0x01, 0xE0, 0x7E, 0xEF, 0x77, 0x80,
         0x01, 0xE7, 0xF9, 0xE7, 0xF7, 0x80, 0x01, 0xFF, 0xC3, 0xC3, 0xF7, 0x80, 0x01, 0xE7, 0x07, 0x81,
         0xF7, 0x00, 0x01, 0xE0, 0x0E, 0x00, 0xF7, 0x00, 0x01, 0xE0, 0x0C, 0x00, 0x77, 0x00, 0x01, 0xE0,
         0x00, 0x00, 0x37, 0x00, 0x01, 0xE0, 0x00, 0x01, 0x87, 0x00, 0x01, 0xE0, 0x00, 0x01, 0xE7, 0x00,
         0x01, 0xE0, 0x00, 0x00, 0xF7, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x7F, 0x00, 0x01, 0xE0, 0x00, 0x00,
         0x3F, 0x00, 0x01, 0xE0, 0x00, 0x00, 0x1F, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x0F, 0x00, 0x00, 0xC0,
         0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        // 值
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,
         0x00, 0x00, 0x00, 0x01, 0xC0, 0x1C, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x1E, 0x00, 0x00, 0x00, 0x01,
         0xE0, 0x1C, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x1C, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x1C, 0x00, 0x00,
         0x00, 0x07, 0x80, 0x1C, 0x07, 0x80, 0x00, 0x07, 0x00, 0x1C, 0xFF, 0xC0, 0x00, 0x0F, 0x00, 0xFF,
         0xFF, 0xC0, 0x00, 0x0E, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x1C, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x3C,
         0x7C, 0x1C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x1C, 0x10, 0x00, 0x00, 0x7C, 0x0C, 0x1C, 0x38, 0x00,
         0x00, 0xFC, 0x0C, 0x3C, 0xFC, 0x00, 0x00, 0xFC, 0x0E, 0x7F, 0xFC, 0x00, 0x01, 0xDC, 0x0F, 0xFF,
         0xF8, 0x00, 0x03, 0x9C, 0x0F, 0xF8, 0x38, 0x00, 0x07, 0x1C, 0x0F, 0x00, 0x38, 0x00, 0x0E, 0x1C,
         0x0F, 0x01, 0x38, 0x00, 0x1C, 0x1C, 0x0F, 0x3F, 0xB8, 0x00, 0x38, 0x1C, 0x0F, 0xFF, 0xF8, 0x00,
         0x00, 0x1C, 0x07, 0xFC, 0x38, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x38, 0x00, 0x00, 0x1C, 0x07, 0x00,
         0x38, 0x00, 0x00, 0x1C, 0x07, 0x03, 0xB8, 0x00, 0x00, 0x1C, 0x07, 0xFF, 0xF8, 0x00, 0x00, 0x1C,
         0x07, 0xFF, 0xF8, 0x00, 0x00, 0x1C, 0x07, 0xC0, 0x38, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x38, 0x00,
         0x00, 0x1C, 0x07, 0x00, 0x38, 0x00, 0x00, 0x1C, 0x07, 0x7F, 0xB8, 0x00, 0x00, 0x1C, 0x07, 0xFF,
         0xF8, 0x00, 0x00, 0x1C, 0x07, 0xF8, 0x38, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x38, 0x00, 0x00, 0x1C,
         0x07, 0x00, 0x38, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x38, 0x20, 0x00, 0x1C, 0x07, 0x00, 0xFF, 0xF0,
         0x00, 0x3C, 0x3F, 0xFF, 0xFF, 0xF8, 0x00, 0x3F, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x1D, 0xFF, 0x80,
         0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        // 预
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x70, 0x00, 0x1F, 0xF0, 0x00, 0x3F,
         0xF8, 0x1F, 0xFF, 0xF8, 0x07, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x03, 0xFC, 0xF3, 0xFF, 0x80, 0x00,
         0x00, 0x01, 0xE0, 0x03, 0xC0, 0x00, 0x00, 0x01, 0xC0, 0x03, 0xC0, 0x00, 0x00, 0x03, 0x80, 0x03,
         0x80, 0x00, 0x00, 0xE7, 0x00, 0x03, 0x80, 0x00, 0x00, 0x7E, 0x01, 0x83, 0x83, 0x80, 0x00, 0x3E,
         0x01, 0xC3, 0x0F, 0xC0, 0x00, 0x0F, 0x81, 0xCF, 0xFF, 0xC0, 0x00, 0x07, 0x98, 0xFF, 0xFF, 0x80,
         0x00, 0x07, 0x3C, 0xFF, 0x03, 0x80, 0x00, 0x3F, 0xFE, 0xE0, 0x03, 0x80, 0x1F, 0xFF, 0xFC, 0xE0,
         0x03, 0x80, 0x0F, 0xFE, 0x78, 0xE0, 0x03, 0x80, 0x06, 0x0E, 0x70, 0xE3, 0x83, 0x80, 0x00, 0x0E,
         0x60, 0xE3, 0xC3, 0x80, 0x00, 0x0E, 0xE0, 0xE3, 0xC3, 0x80, 0x00, 0x0E, 0xC0, 0xE3, 0xC3, 0x80,
         0x00, 0x0E, 0x00, 0xE3, 0xC3, 0x80, 0x00, 0x0E, 0x00, 0xE3, 0xC3, 0x80, 0x00, 0x0E, 0x00, 0xE3,
         0xC3, 0x80, 0x00, 0x0E, 0x00, 0xE3, 0x83, 0x80, 0x00, 0x0E, 0x00, 0xE3, 0x87, 0x80, 0x00, 0x0E,
         0x00, 0xE3, 0x87, 0x80, 0x00, 0x0E, 0x00, 0xE3, 0x87, 0x80, 0x00, 0x1E, 0x01, 0xE7, 0x87, 0x80,
         0x00, 0x1E, 0x01, 0xE7, 0x07, 0x00, 0x00, 0x1E, 0x00, 0xE7, 0x63, 0x00, 0x00, 0x1E, 0x00, 0x6F,
         0x73, 0x00, 0x00, 0x1E, 0x00, 0x0E, 0x38, 0x00, 0x00, 0x1E, 0x00, 0x1E, 0x1C, 0x00, 0x07, 0x1E,
         0x00, 0x3C, 0x0E, 0x00, 0x03, 0x9E, 0x00, 0x38, 0x07, 0x80, 0x01, 0xFE, 0x00, 0xF0, 0x03, 0xC0,
         0x00, 0xFC, 0x01, 0xE0, 0x03, 0xE0, 0x00, 0x7C, 0x03, 0xC0, 0x01, 0xF0, 0x00, 0x3C, 0x0F, 0x80,
         0x00, 0xF0, 0x00, 0x1C, 0x1E, 0x00, 0x00, 0x70, 0x00, 0x18, 0x78, 0x00, 0x00, 0x60, 0x00, 0x00,
         0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        // 警
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x07,
         0x00, 0x00, 0x00, 0x0E, 0x0E, 0x07, 0x00, 0x00, 0x00, 0x0F, 0x0E, 0x07, 0x80, 0x00, 0x00, 0x07,
         0x0E, 0x07, 0x80, 0x00, 0x00, 0x07, 0x1E, 0xE7, 0x00, 0x00, 0x00, 0x07, 0x1F, 0xF7, 0x00, 0x00,
         0x00, 0x1F, 0xFF, 0xEF, 0x03, 0x80, 0x0F, 0xFF, 0xFC, 0x0E, 0x0F, 0xC0, 0x07, 0xFF, 0x1C, 0x0E,
         0xFF, 0xC0, 0x00, 0x3F, 0x3B, 0x9F, 0xF8, 0x00, 0x00, 0x3B, 0x3F, 0xDF, 0xBC, 0x00, 0x00, 0x3C,
         0x7F, 0xDC, 0x3C, 0x00, 0x00, 0x7F, 0xFF, 0xBE, 0x38, 0x00, 0x00, 0x7F, 0xC3, 0xBF, 0x70, 0x00,
         0x00, 0xF0, 0x73, 0xF3, 0xF0, 0x00, 0x01, 0xF3, 0xFF, 0x73, 0xE0, 0x00, 0x01, 0xFF, 0xF7, 0xE1,
         0xE0, 0x00, 0x03, 0xBE, 0x77, 0x43, 0xF8, 0x00, 0x07, 0x38, 0xE7, 0x07, 0xBC, 0x00, 0x0E, 0x3F,
         0xF7, 0x0F, 0x1F, 0x00, 0x1C, 0x3F, 0xEF, 0x1C, 0x0F, 0xF8, 0x38, 0x38, 0xFF, 0xB8, 0x07, 0xF8,
         0x00, 0x08, 0x7F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xC0, 0x7F, 0x00, 0x00, 0x00, 0x0C, 0xFF,
         0xFF, 0x80, 0x00, 0x00, 0xFF, 0xFF, 0xF8, 0x00, 0x07, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x03, 0xFF,
         0x80, 0x0F, 0x80, 0x00, 0x01, 0x80, 0x07, 0xFF, 0xC0, 0x00, 0x00, 0x03, 0xFF, 0xF8, 0x00, 0x00,
         0x00, 0x01, 0xF8, 0x07, 0x80, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xC0, 0x00, 0x00, 0x01, 0xFF, 0xFC,
         0x00, 0x00, 0x00, 0x01, 0xFE, 0x01, 0x80, 0x00, 0x00, 0x03, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x03,
         0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x03, 0xFF, 0xFB, 0xC0, 0x00, 0x00, 0x03, 0xF0, 0x03, 0x80, 0x00,
         0x00, 0x03, 0x80, 0x03, 0x80, 0x00, 0x00, 0x03, 0xC0, 0x07, 0x80, 0x00, 0x00, 0x03, 0xFF, 0xFF,
         0xC0, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x01,
         0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

int temp = 38;
int water = 50;
int smoke = 150;
void Led_Beep_Ctrl(int dev, int status)
{
    int dev_fd = -1;
    if (status == ON)
    {
        // 要打开某个设备
        switch (dev)
        {
        case D7:
            dev_fd = open(LED_D7, O_RDWR);
            break;

        case D8:
            dev_fd = open(LED_D8, O_RDWR);
            break;

        case D9:
            dev_fd = open(LED_D9, O_RDWR);
            break;

        case D10:
            dev_fd = open(LED_D10, O_RDWR);
            break;

        case FM:
            dev_fd = open(BEEP, O_RDWR);
            break;
        }

        if (-1 == dev_fd)
        {
            printf("open dev filed failed!\n");
            return;
        }
        int s = 1;
        write(dev_fd, &s, sizeof(s));
    }

    else
    {
        // 要关闭某个设备
        switch (dev)
        {
        case D7:
            dev_fd = open(LED_D7, O_RDWR);
            break;

        case D8:
            dev_fd = open(LED_D8, O_RDWR);
            break;

        case D9:
            dev_fd = open(LED_D9, O_RDWR);
            break;

        case D10:
            dev_fd = open(LED_D10, O_RDWR);
            break;

        case FM:
            dev_fd = open(BEEP, O_RDWR);
            break;
        }

        if (-1 == dev_fd)
        {
            printf("open dev filed failed!\n");
            return;
        }
        int s = 0;
        write(dev_fd, &s, sizeof(s));
    }
}

void get_dht11()
{
    int dht_flag = 0;
    // open
    int dev_fd = open("/dev/dht11_dev", O_RDWR);
    if (-1 == dev_fd)
    {
        printf("open dht11 failed\n");
        return;
    }

    // 不断的读取温度湿度信息
    unsigned char data[2];
    int res = ioctl(dev_fd, GEC6818_GET_DHTDATA, &data[0]); // 读取数据
                                                            // 使用该函数，需要包含头文件：<sys/ioctl.h>

    if (res != 0)
    {
        printf("get temp humi data failed\n");
        printf("%s\n", data);
    }
    else
    {
        printf("温度%d\n", data[0]);
        printf("湿度%d\n", data[1]);
        if (environment_flag == 1)
        {
            lcd_draw_word(350, 100, 48, 48, word_cord[0]);
            lcd_draw_word(400, 100, 48, 48, word_cord[1]);
            lcd_draw_num(376, 160, data[0]);
            if (data[0] > temp)
            {
                dht_flag = 1;
            }
        }
        if (environment_flag == 2)
        {
            lcd_draw_word(350, 100, 48, 48, word_cord[2]);
            lcd_draw_word(400, 100, 48, 48, word_cord[1]);
            lcd_draw_num(376, 160, data[1]);
            if (data[1] > water)
            {
                dht_flag = 1;
            }
        }
    }
    lcd_draw_word(300, 350, 48, 48, word_cord[6]);
    lcd_draw_word(350, 350, 48, 48, word_cord[7]);
    lcd_draw_word(400, 350, 48, 48, word_cord[8]);
    lcd_draw_word(450, 350, 48, 48, word_cord[9]);
    if (environment_flag == 1)
    {
        lcd_draw_num(376, 400, temp);
    }
    else if (environment_flag == 2)
    {
        lcd_draw_num(376, 400, water);
    }
    if (dht_flag)
    {
        Led_Beep_Ctrl(FM, ON);
        sleep(2);
        Led_Beep_Ctrl(FM, OFF);
    }
    sleep(2);
    // close
    close(dev_fd);
}

void get_mq2()
{
    int serial_fd = open("/dev/ttySAC2", O_RDWR);
    if (-1 == serial_fd)
    {
        printf("open serial failed!\n");
        return;
    }
    // printf("error\n");
    struct termios myserial; // 使用这个结构体，需要包含头文件：<termios.h>

    memset(&myserial, 0, sizeof(myserial)); // 清空结构体

    myserial.c_cflag |= (CLOCAL | CREAD); // 设置为可读可写
    myserial.c_cflag &= ~CSIZE;           // 清空数据为
    myserial.c_cflag &= ~CRTSCTS;         // 无硬件流控
    myserial.c_cflag |= CS8;              // 设置数据位为8位
    myserial.c_cflag &= ~CSTOPB;          // 1位停止位
    myserial.c_cflag &= ~PARENB;          // 无校验位

    cfsetospeed(&myserial, B9600);
    cfsetispeed(&myserial, B9600);

    // 刷新输出队列，清除正在接收的数据
    tcflush(serial_fd, TCIFLUSH);

    // 改变配置
    tcsetattr(serial_fd, TCSANOW, &myserial);

    while (environment_flag == 3)
    {
        int mq_flag = 0;
        char w_buf[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
        write(serial_fd, w_buf, 9);
        usleep(1000);
        char r_buf[9];
        int r = read(serial_fd, r_buf, 9);
        printf("r=%d\n", r);
        lcd_draw_bmp("device.bmp");
        lcd_draw_clear1(700, 0, 100, 60, 0xFF0000);
        lcd_draw_word(700, 10, 48, 48, word_code[0]);
        lcd_draw_word(750, 10, 48, 48, word_code[1]);
        lcd_draw_clear1(700, 60, 100, 60, 0x00FF00);
        lcd_draw_word(700, 70, 48, 48, word_code[2]);
        lcd_draw_word(750, 70, 48, 48, word_code[3]);
        lcd_draw_word(300, 100, 48, 48, word_cord[3]);
        lcd_draw_word(350, 100, 48, 48, word_cord[4]);
        lcd_draw_word(400, 100, 48, 48, word_cord[5]);
        lcd_draw_word(450, 100, 48, 48, word_cord[1]);
        lcd_draw_num(354, 160, 115);
        if (r == 9 && r_buf[0] == 0xff && r_buf[1] == 0x86)
        {
            int n = r_buf[2] << 8 | r_buf[3];
            printf("n = %d\n", n);
            lcd_draw_num(354, 160, n);
            printf("n=%d", n);
            if (n > smoke)
            {
                mq_flag = 1;
            }
        }
        lcd_draw_word(300, 350, 48, 48, word_cord[6]);
        lcd_draw_word(350, 350, 48, 48, word_cord[7]);
        lcd_draw_word(400, 350, 48, 48, word_cord[8]);
        lcd_draw_word(450, 350, 48, 48, word_cord[9]);
        lcd_draw_num(354, 400, smoke);
        if (mq_flag)
        {
            Led_Beep_Ctrl(FM, ON);
            sleep(2);
            Led_Beep_Ctrl(FM, OFF);
        }
        sleep(1);
    }
    close(serial_fd);
}