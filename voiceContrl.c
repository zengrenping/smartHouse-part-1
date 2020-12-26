#include "InputCommand.h"


/*
struct InputCommander
{
    char commandName[128];                                    //指令名称
    char relCMD[32];                                          //真实指令
    int (*commandInit)(char *name, char *ipAddr, char *port); //初始化socket连接可以用
    int (*getCommand)(char *cmd);
    char log[1024];

    struct InputCommander *next;
};
*/

int InitCMD(char *name, char *ipAddr, char *port)
{



}

int getCMD(char *cmd)
{


}


struct InputCommander voiceContrl = {

    .commandName = "voiceContrl",
    .relCMD = {'\0'},
    .commandInit = InitCMD,
    .getCommand  = getCMD,
    .log = {'\0'},

    .next = NULL; 

};