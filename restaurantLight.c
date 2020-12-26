#include "contrlDevices.h"

int restaurantLightOpen(int pinNumb)
{
    digitalWrite(pinNumb, LOW); //引脚低电平
}

int restaurantLightClose(int pinNumb)
{
    digitalWrite(pinNumb, HIGH); //引脚高电平
}

int restaurantLightInit(int pinNumb)
{
    pinMode(pinNumb, OUTPUT);    //初始化
    digitalWrite(pinNumb, HIGH); //初始化高电平
}

int restaurantLightChangeST(int status)
{
}

struct Devieces restaurantLight = {
    .deviecesName = "restaurantLight",
    .open = restaurantLightOpen,
    .close = restaurantLightClose,
    .devieceInit = restaurantLightInit,
    .pinNumb = 24, //引脚号 (wiringPi定义)24
    .changeStarus = restaurantLightChangeST};

struct Devieces *addrestaurantLightToDeviecLink(struct Devieces *phead)
{
    if (phead == NULL)
    {
        phead = &restaurantLight;
        return phead;
    }
    else
    {
        restaurantLight.next = phead;
        phead = &restaurantLight;

        return phead;
    }
}
