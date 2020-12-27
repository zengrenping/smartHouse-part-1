#include "contrlDevices.h"

int bathroomLightOpen(int pinNumb)
{
	digitalWrite (pinNumb, LOW);//引脚低电平

}

int bathroomLightClose(int pinNumb)
{
	digitalWrite (pinNumb, HIGH);//引脚高电平
}

int bathroomLightInit(int pinNumb)
{
	pinMode(pinNumb, OUTPUT);//初始化
	digitalWrite (pinNumb, HIGH);//初始化高电平
}

int bathroomLightChangeST(int status)
{


}
/*每个设备都使用struct Devieces 结构体 进行内容的配置*/

struct Devieces bathroomLight = {  
 	.deviecesName  = "bathroomLight",
 	.open  = bathroomLightOpen,
 	.close = bathroomLightClose,
 	.devieceInit  = bathroomLightInit,
 	.pinNumb = 22,
 	.changeStarus = bathroomLightChangeST
};


struct Devieces *addBathroomLightToDeviecLink(struct Devieces *phead)
{
	if(phead == NULL){
		phead = &bathroomLight;
		return phead;
	
	}else{
		bathroomLight.next = phead;
		phead = &bathroomLight;

		return phead;
	}


}

