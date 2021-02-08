#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "contrlDevices.h"
#include "InputCommand.h"

struct Devieces *pDeviecHead = NULL;	  //设备链表头
struct InputCommander *pInPutHead = NULL; //输入设备链表头
struct InputCommander *socketHead = NULL; //socket服务

pthread_mutex_t mutex; //互斥锁
pthread_cond_t cond;   //线程的同步 条件

int c_fd;
int serial_fd; //打开串口后的文件描述符号

char wiPiCmd[32] = {'\0'};
char devName[128] = {'\0'};

/* 各个链表、wringPi库初始化	*/
void init_DevAndInPutLink()
{
	int initWPSet;
	/*========================加IO设备链表=====================================*/
	//1 设备控制工厂初始化
	pDeviecHead = addBathroomLightToDeviecLink(pDeviecHead); //将指令加入指令列表中
	pDeviecHead = addUpstairLightToDeviecLink(pDeviecHead);
	pDeviecHead = addrestaurantLightToDeviecLink(pDeviecHead);
	pDeviecHead = addlivingRoomLightToDeviecLink(pDeviecHead);
	pDeviecHead = addAllLightToDeviecLink(pDeviecHead);
	pDeviecHead = addfireSateToDeviecLink(pDeviecHead); //火灾传感器输入
	pDeviecHead = addSwimmingLightToDeviecLink(pDeviecHead);

	/*==========================加语音/Socket链表===================================*/
	//2 指令工厂初始化
	pInPutHead = addVoiceContrlToInPutLink(pInPutHead);
	pInPutHead = addSocketContrlToInPutLink(pInPutHead);
	/*=============================================================*/

	initWPSet = wiringPiSetup();
	if (initWPSet == -1)
	{
		printf("端口初始化失败。\n");
		exit(-1);
	}
}

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

/* 指令拆分函数 */
char *cmdChaiFen(char *cmd)
{
	char *p;

	p = strtok(cmd, " ");
	p = strtok(NULL, " ");

	return p;
}

/* 语音线程 主函数 */
void *vociec_doMain()
{
	int nread;
	int voice;

	struct InputCommander *vociecHead = NULL;
	vociecHead = findInPutByName("voiceContrl", pInPutHead);
	if (vociecHead == NULL)
	{
		printf("没有找到voice设备模块。\n");
		pthread_exit(NULL);
	}
	else
	{
		/* 语音初始化 */
		voice = vociecHead->commandInit(vociecHead, NULL, NULL);
		if (voice == -1)
		{
			printf("vociec串口初始化失败。\n");
			pthread_exit(NULL);
		}
		else
		{
			/* 将串口的文件描述符赋值给全局变量，以便后面进行控制时用 */
			serial_fd = vociecHead->fd;

			/* 获取语音模块的指令 */
			while (1)
			{
				nread = vociecHead->getCommand(vociecHead);
				if (nread == 0)
				{
					printf("获取串口“语音模块”指令超时/没有收到指令。\n");
					continue;
				}
				else
				{
					memset(&devName[0], '\0', sizeof(devName));
					memset(&wiPiCmd[0], '\0', sizeof(wiPiCmd));

					strcpy(devName, &vociecHead->relCMD[0]);
					strcpy(wiPiCmd, &vociecHead->wiPiCmd[0]);

					printf("wiPiCmd指令内容：%s\n", wiPiCmd);
					printf("devName-p指令内容：%s\n", devName);

					pthread_cond_signal(&cond);
					pthread_mutex_unlock(&mutex);
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
		pthread_mutex_lock(&mutex);
		
		char *p = NULL;
		memset(&devName[0], '\0', sizeof(devName));
		memset(&wiPiCmd[0], '\0', sizeof(wiPiCmd));

		printf("内容接收成功。\n");
		p = cmdChaiFen(&socketHead->relCMD[0]);
		if (p != NULL)
		{
			strcpy(&devName[0], p);
			strcpy(&wiPiCmd[0], &socketHead->relCMD[0]);

			printf("接收到的指令内容：%s\n", wiPiCmd);
			printf("接收到的指令内容：%s\n", devName);

			//1.拆分指令
			//2.将指令分配给不同的变量

			memset(socketHead->relCMD, '\0', sizeof(socketHead->relCMD));
			//int pthread_cond_signal(pthread_cond_t cond);
			pthread_cond_signal(&cond);
		}
		else
		{
			printf("指令输入有误。\n");
		}
		pthread_mutex_unlock(&mutex);
	}
}

/* socket 线程主函数 */
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
		socketHead->commandInit(socketHead, NULL, NULL);

		while (1)
		{
			c_fd = accept(socketHead->s_fd, (struct sockaddr *)&c_addr, &clen);
			if (c_fd == -1)
			{
				printf("网络连接失败。\n");
				perror("accept");
				exit(-1);
			}
			else
			{
				pthread_create(&read_thr, NULL, readMain, NULL); //创建线程进行消息的接收
			}
		}
	}
}

/* 舵机控制线程主函数 */
void *wiPi_doMain()
{
	struct Devieces *tmp = NULL; //实际执行的指令的设备链表

	while (1)
	{
		pthread_cond_wait(&cond, &mutex);

		tmp = findDeviecesByName(devName, pDeviecHead); //将拆分后的指令传入链表进行寻找
		if (tmp == NULL)
		{
			printf("没有这个指令。\n");
			memset(devName, '\0', 128);
		}
		else
		{
			/* 对指令进行判断，若这个指令为Wemos控制，那执行串口发送指令*/
			if(!strcmp(devName,"swimmingLight") || !strcmp(devName,"TVcontrl")){
				
				printf("Wemos指令开始执行。\n");

				if (!strcmp(wiPiCmd, "open"))
				{
					printf("open\n");
				/* 将串口的文件描述符传给子函数执行	*/
					tmp->open(serial_fd);
				}
				else if (!strcmp(wiPiCmd, "close"))
				{
					printf("close\n");
					tmp->close(serial_fd);
				}
				else
				{
					printf("Wemos指令有误，请重新输入。\n");
				}

			}else{
				printf("指令开始执行。\n");

				tmp->devieceInit(tmp->pinNumb);

				if (!strcmp(wiPiCmd, "open"))
				{
					printf("open\n");
					tmp->open(tmp->pinNumb);
				}
				else if (!strcmp(wiPiCmd, "close"))
				{
					printf("close\n");
					tmp->close(tmp->pinNumb);
				}
				else
				{
					printf("指令有误，请重新输入。\n");
				}
			}
			memset(devName, '\0', sizeof(devName));
			memset(wiPiCmd, '\0', sizeof(wiPiCmd));
		}
		
	}
}

int main()
{
	pthread_t vociec_pthread;
	pthread_t socket_pthread;
	pthread_t wiPi_pthread;

	pthread_cond_init(&cond, NULL); //条件初始化
	pthread_mutex_init(&mutex, NULL);

	init_DevAndInPutLink();

	//3 线程池建立 线程如何建立/如何使用
	//原型：int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *),  void *arg);

	//3.1 语音线程
	pthread_create(&vociec_pthread, NULL, vociec_doMain, NULL);

	//3.2 socket线程
	pthread_create(&socket_pthread, NULL, socket_doMain, NULL);

	//3.5 wiPi执行线程
	pthread_create(&wiPi_pthread, NULL, wiPi_doMain, NULL);

	pthread_join(vociec_pthread, NULL); //线程等待
	pthread_join(socket_pthread, NULL);
	pthread_join(wiPi_pthread, NULL);

	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);

	//3.3 摄像头线程

	//3.4 火灾线程
	

	return 0;
}
