#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contrlDevices.h"

struct Devieces *findDeviecesByName(char *name, struct Devieces *head)
{
	struct Devieces *p = head;
	if(p == NULL){
		return NULL;
	}else{
	
		while(p != NULL){
			if(!strcmp(name, p->deviecesName)){
				return p;			
			}
			p = p->next;
		}
		
		return NULL;
	}

}


int main()
{
	char *name =(char *)malloc(128);
	int initWPSet;
	
	//1 指令工厂初始化
	struct Devieces *tmp = NULL;
	struct Devieces *pDeviecHead = NULL;
	/*========================加链表=====================================*/
	
	pDeviecHead = addBathroomLightToDeviecLink(pDeviecHead);//将指令加入指令列表中
	pDeviecHead = addUpstairLightToDeviecLink(pDeviecHead);
	pDeviecHead = addrestaurantLightToDeviecLink(pDeviecHead);
	pDeviecHead = addlivingRoomLightToDeviecLink(pDeviecHead);

	pDeviecHead = addfireSateToDeviecLink(pDeviecHead);//火灾传感器输入
	/*=============================================================*/

	initWPSet = wiringPiSetup();
	if(initWPSet == -1){
		printf("端口初始化失败。\n");
		exit(-1);
	}
	printf("请输入需要点亮灯的名字。\n");
	scanf("%s", name);
	
	tmp = findDeviecesByName(name, pDeviecHead);//通过链表查找指令
	if(tmp == NULL){
		printf("程序运行错误。\n");
		exit(-1);
	}else{
	
		tmp->devieceInit(tmp->pinNumb);
		tmp->open(tmp->pinNumb);

		memset(tmp,'\0', sizeof(struct Devieces));//清空结构体数据
	}
	

	//2 设备控制工厂初始化

	//3 线程池建立	
	//3.1 语音线程
	
	//3.2 socket线程

	//3.3 摄像头线程

	//3.4 火灾线程
	


	return 0;
}
