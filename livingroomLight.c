#include "contrlDevices.h"

int livingRoomLightOpen(int pinNumb)
{
    digitalWrite(pinNumb, LOW); //引脚低电平
}

int livingRoomLightClose(int pinNumb)
{
    digitalWrite(pinNumb, HIGH); //引脚高电平
}

int livingRoomLightInit(int pinNumb)
{
    pinMode(pinNumb, OUTPUT);    //初始化
    digitalWrite(pinNumb, HIGH); //初始化高电平
}

int livingRoomLightChangeST(int status)
{
}

struct Devieces livingRoomLight = {
    .deviecesName = "livingRoomLight",
    .open = livingRoomLightOpen,
    .close = livingRoomLightClose,
    .devieceInit = livingRoomLightInit,
    .pinNumb = 23, //引脚号 (wiringPi定义)23
    .changeStarus = livingRoomLightChangeST};

struct Devieces *addlivingRoomLightToDeviecLink(struct Devieces *phead)
{
    if (phead == NULL)
    {
        phead = &livingRoomLight;
        return phead;
    }
    else
    {
        livingRoomLight.next = phead;
        phead = &livingRoomLight;

        return phead;
    }
}
