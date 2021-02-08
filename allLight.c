#include "contrlDevices.h"

int allLightOpen(int pinNumb)
{
    digitalWrite(pinNumb, LOW); //引脚低电平
}

int allLightClose(int pinNumb)
{
    digitalWrite(pinNumb, HIGH); //引脚高电平
}

int allLightInit(int pinNumb)
{
    pinMode(pinNumb, OUTPUT);    //初始化
    digitalWrite(pinNumb, HIGH); //初始化高电平
}

int allLightChangeST(int status)
{

    
}

struct Devieces allLight = {
    .deviecesName = "allLight",
    .open = allLightOpen,
    .close = allLightClose,
    .devieceInit = allLightInit,
    .pinNumb = 23, //引脚号 (wiringPi定义)23
    .changeStarus = allLightChangeST
    };

struct Devieces *addAllLightToDeviecLink(struct Devieces *phead)
{
    if (phead == NULL)
    {
        phead = &allLight;
        return phead;
    }
    else
    {
        allLight.next = phead;
        phead = &allLight;

        return phead;
    }
}
