#ifndef __DEV_H__
#define __DEV_H__

#define LED_D7	"/sys/kernel/gec_ctrl/led_d7"
#define LED_D8  "/sys/kernel/gec_ctrl/led_d8"
#define LED_D9  "/sys/kernel/gec_ctrl/led_d9"
#define LED_D10 "/sys/kernel/gec_ctrl/led_d10"
#define BEEP    "/sys/kernel/gec_ctrl/beep"

extern int temp;
extern int water;
extern int smoke;
extern char word_code[4][288];
enum DEV_ID
{
    D7 = 7,
    D8,
    D9,
    D10,
    FM
};

enum DEV_STATE
{
    OFF = 0,
    ON
};

void Led_Beep_Ctrl(int dev, int status);
void get_dht11();
void get_mq2();

#endif