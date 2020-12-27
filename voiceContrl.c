#include "InputCommand.h"


int InitVoicCMD(struct InputCommander *voiceContrl, char *ipAddr, char *port)
{
    int fd;
    if((fd = serialOpen(voiceContrl->commandName, 9600)) == -1 ){
        return -1;
    }//初始化串口

    voiceContrl->fd = fd;
}

int getVoicCMD(struct InputCommander *voiceContrl)
{
    int nread = 0;
    nread = read(voiceContrl->fd, voiceContrl->relCMD, sizeof(voiceContrl->relCMD));
    
    return nread;

}


struct InputCommander voiceContrl = {

    .commandName = "voiceContrl",
    .deviceName = "/dev/ttyAMA0",
    .relCMD = {'\0'},
    .commandInit = InitVoicCMD,
    .getCommand  = getVoicCMD,
    .log = {'\0'},

    .next = NULL

};

struct InputCommander *addVoiceContrlToInPutLink(struct InputCommander *phead)
{
    if (phead == NULL)
    {
        phead = &voiceContrl;
        return phead;
    }
    else
    {
        voiceContrl.next = phead;
        phead = &voiceContrl;

        return phead;
    }
}

//测试文件