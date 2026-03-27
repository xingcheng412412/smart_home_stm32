#include <pthread.h>
#include <stdio.h>
#include "lcd.h"
#include "bmp.h"
#include "touch.h"
#include <unistd.h>

struct Param
{
    int a;
    int b;
};

void *my_thread(void *arg)
{
    struct Param x = *(struct Param *)arg;
    int i;
    for (i = x.a; i <= x.b; i++)
    {
        printf("%d\n", i);
    }
}

int main()
{
    struct Param x;
    printf("input a & b:");
    scanf("%d %d", &x.a, &x.b);
    pthread_t id = 0;
    // 创建线程
    int res = pthread_create(&id, NULL, my_thread, &x);
    if (res == -1)
    {
        printf("thread fail");
    }
    while (1);
    return 0;
    
}