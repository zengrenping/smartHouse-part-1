#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>



struct Devieces
{

	char deviecesName[128];//设备名称
	int status;//设备状态（开关）
	int pinNumb;

	int (*open)(int pinNumb);
	int (*close)(int pinNumb);
	int (*devieceInit)(int pinNumb);
	
	int (*readStarus)();
	int (*changeStarus)(int status);

	struct Devieces *next;
};


struct Devieces *addBathroomLightToDeviecLink(struct Devieces *phead);
struct Devieces *addUpstairLightToDeviecLink(struct Devieces *phead);
struct Devieces *addrestaurantLightToDeviecLink(struct Devieces *phead);
struct Devieces *addlivingRoomLightToDeviecLink(struct Devieces *phead);

struct Devieces *addfireSateToDeviecLink(struct Devieces *phead);//火灾传感器输入
struct Devieces *addAllLightToDeviecLink(struct Devieces *phead);

struct Devieces *addSwimmingLightToDeviecLink(struct Devieces *phead);	//Wemos控制的泳池灯