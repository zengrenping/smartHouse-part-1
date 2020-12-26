#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>


struct InputCommander
{
	char commandName[128];//指令名称
	char deviceName[128];//设备名称
	char relCMD[32];//真实指令
	int (*commandInit)(char *name, char *ipAddr, char *port);//初始化socket连接可以用
	int (*getCommand)(char *cmd);
	char log[1024];
	
	struct InputCommander *next;

};