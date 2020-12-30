#include "InputCommand.h"


int InitVoicCMD(struct InputCommander *voiceContrl, char *ipAddr, char *port)
{
    int fd;
    if ((fd = serialOpen(voiceContrl->deviceName, 9600)) == -1)
    {
        return -1;
    } //初始化串口
    else{
        printf("串口初始化成功。\n");
    
        voiceContrl->fd = fd;
        return 0;
    }
    return -1;
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