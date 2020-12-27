#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "contrlDevices.h"
#include "InputCommand.h"

struct Devieces *pDeviecHead = NULL;	  //设备链表头
struct InputCommander *pInPutHead = NULL; //输入设备链表头
struct InputCommander *socketHead = NULL; //socket服务
int c_fd;

/**********socket/语音查找工具***********/
struct InputCommander *findInPutByName(char *name, struct InputCommander *head)
{
	struct InputCommander *p = head;
	if (p == NULL)
	{
		return NULL;
	}
	else
	{
		while (p != NULL)
		{
			if (!strcmp(name, p->commandName))
			{ //对比输入指令和每个结构体中的指令是否一致
				return p;
			}
			p = p->next;
		}

		return NULL;
	}
}

/*************设备查找工具**************/
struct Devieces *findDeviecesByName(char *name, struct Devieces *head)
{
	struct Devieces *p = head;
	if (p == NULL)
	{
		return NULL;
	}
	else
	{

		while (p != NULL)
		{
			if (!strcmp(name, p->deviecesName))
			{ //对比输入指令和每个结构体中的指令是否一致
				return p;
			}
			p = p->next;
		}

		return NULL;
	}
}


void *vociec_doMain()
{
	int nread;
	struct InputCommander *vociecHead = NULL;
	vociecHead = findInPutByName("voiceContrl", pInPutHead);
	if(vociecHead == NULL)
	{
		printf("没有找到voice指令。\n");
		pthread_exit(NULL);

	}
	else
	{
		if(vociecHead->commandInit(vociecHead, NULL, NULL) ==-1 )
		{
			printf("vociec串口初始化失败。\n");
			pthread_exit(NULL);
		}
		else
		{
			while(1){
				nread = vociecHead->getCommand(vociecHead); 
				if (nread == 0)
				{
					printf("获取串口“语音模块”指令超时/没有收到指令。\n");
					continue;
				}
				else
				{
					printf("语音模块指令为：%s\n", vociecHead->relCMD);
				}
			}
		}
	}
}

void *readMain()
{
	int rec_t;

	rec_t = recv(c_fd, socketHead->relCMD, sizeof(socketHead->relCMD), 0);
	if (rec_t == -1)
	{
		printf("内容接收失败。\n");
		
	}
	else if (rec_t == 0)
	{
		printf("客户端退出\n");
	}
	else
	{
		printf("内容接收成功。\n");
		printf("接收到的数据大小：%d\n",rec_t);
		printf("接收到的指令内容：%s\n",socketHead->relCMD);
	}
}

void *socket_doMain()
{
	struct sockaddr_in c_addr;
	pthread_t read_thr;

	memset(&c_addr, 0, sizeof(struct sockaddr_in));
	int clen = sizeof(struct sockaddr_in);

	socketHead = findInPutByName("socketServer", pInPutHead);
	if (socketHead == NULL)
	{
		printf("没有找到socket指令。\n");
		pthread_exit(NULL);
	}
	else
	{
		socketHead->commandInit(socketHead,NULL, NULL);
		
		while(1){
			c_fd = accept(socketHead->s_fd, (struct sockaddr *)&c_addr, &clen);
			if (c_fd == -1)
			{
				printf("网络连接失败。\n");
				perror("accept");
				exit(-1);
			}
			else
			{
				pthread_create(&read_thr, NULL, readMain, NULL);
			}
		}
	}
}




int main()
{
	char *name =(char *)malloc(128);
	int initWPSet;

	pthread_t vociec_pthread;
	pthread_t socket_pthread;
	
	
	struct Devieces *tmp = NULL;//实际执行的指令的设备链表

	/*========================加IO设备链表=====================================*/
	//1 设备控制工厂初始化
	pDeviecHead = addBathroomLightToDeviecLink(pDeviecHead);//将指令加入指令列表中
	pDeviecHead = addUpstairLightToDeviecLink(pDeviecHead);
	pDeviecHead = addrestaurantLightToDeviecLink(pDeviecHead);
	pDeviecHead = addlivingRoomLightToDeviecLink(pDeviecHead);

	pDeviecHead = addfireSateToDeviecLink(pDeviecHead);//火灾传感器输入

/*==========================加语音/Socket链表===================================*/
	//2 指令工厂初始化
	pInPutHead = addVoiceContrlToInPutLink(pInPutHead);
	pInPutHead = addSocketContrlToInPutLink(pInPutHead);
	/*=============================================================*/


	//3 线程池建立 线程如何建立/如何使用 
	//原型：int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *),  void *arg);
	//3.1 语音线程
	pthread_create(&vociec_pthread, NULL, vociec_doMain, NULL);
 
	//3.2 socket线程
	pthread_create(&socket_pthread, NULL, socket_doMain, NULL);

	//3.3 摄像头线程

	//3.4 火灾线程

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
	

	

	


	return 0;
}
