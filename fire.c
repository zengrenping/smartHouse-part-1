#include "contrlDevices.h"

int fireStateInit(int pinNumb)
{
    pinMode(pinNumb, INPUT);    //初始化 输入
    digitalWrite(pinNumb, HIGH); //初始化高电平
}

int fireStateReadStarus(int pinNumb)
{
    return digitalRead(pinNumb);//读取pinNumb引脚上的电位，并返回
}

struct Devieces fireState = {
    .deviecesName = "fireState",
    .pinNumb = 21, //wiringPi库定义的引脚号
    .devieceInit = fireStateInit,
    .readStarus = fireStateReadStarus
};

struct Devieces *addfireSateToDeviecLink(struct Devieces *phead)
{
    if (phead == NULL){
        phead = &fireState;
        return phead;

    }else{
        fireState.next = phead;
        phead = &fireState;

        return phead;
    }
}
