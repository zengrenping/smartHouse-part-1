#include "contrlDevices.h"

int upstairLightOpen(int pinNumb)
{
	digitalWrite (pinNumb, LOW);//引脚低电平

}

int upstairLightClose(int pinNumb)
{
	digitalWrite (pinNumb, HIGH);//引脚高电平
}

int upstairLightInit(int pinNumb)
{
	pinMode(pinNumb, OUTPUT);//初始化
	digitalWrite (pinNumb, HIGH);//初始化高电平
}

int upstairLightChangeST(int status)
{


}

struct Devieces upstairLight = {
 	.deviecesName  = "upstairLight",
 	.open  = upstairLightOpen,
 	.close = upstairLightClose,
 	.devieceInit  = upstairLightInit,
 	.pinNumb = 25,//引脚号 (wiringPi定义)25
 	.changeStarus = upstairLightChangeST
};


struct Devieces *addUpstairLightToDeviecLink(struct Devieces *phead)
{
	if(phead == NULL){
		phead = &upstairLight;
		return phead;
	
	}else{
		upstairLight.next = phead;
		phead = &upstairLight;

		return phead;
	}


}
