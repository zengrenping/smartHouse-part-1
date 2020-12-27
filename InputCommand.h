#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringSerial.h>
#include <unistd.h>

struct InputCommander
{
	char commandName[128];//指令名称
	char deviceName[128];//设备名称
	char relCMD[32];//真实指令
	int (*commandInit)(struct InputCommander *initInput, char *ipAddr, char *port); //初始化socket连接可以用
	int (*getCommand)(struct InputCommander *initInput);
	char log[1024];
	int fd;//句柄

	int port;//端口号
	char ipAddress[32];//IP地址
	int s_fd;
	
	struct InputCommander *next;

};


struct InputCommander *addVoiceContrlToInPutLink(struct InputCommander *phead);
struct InputCommander *addSocketContrlToInPutLink(struct InputCommander *phead);
