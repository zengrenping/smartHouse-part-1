#include "InputCommand.h"

static char *cmdChaiFen(char *cmd)
{
    char *p;

    p = strtok(cmd, " ");
    p = strtok(NULL, " ");

    return p;
}

static char *devNamePanduan(char *name)
{
  
    if (!strcmp(name, "CSD\r\n"))   return "bathroomLight";
    if (!strcmp(name, "KTD\r\n"))   return "livingRoomLight";
    if (!strcmp(name, "CTD\r\n"))   return "restaurantLight";
    if (!strcmp(name, "ELD\r\n"))   return "upstairLight";
    if (!strcmp(name, "SYD\r\n"))   return "allLight";
    
    else{
        printf("以上都找不到。\n");
        return "none";
    }
}

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
    char getCmd[256] = {'\0'};
    char *newDev = NULL;//驱动的缩写
    
    nread = read(voiceContrl->fd, getCmd, sizeof(getCmd));
    if(nread == 0){
        return nread;

    }else{
        
        newDev = cmdChaiFen(&getCmd[0]);
        if(newDev != NULL){
            memset(voiceContrl->relCMD, '\0', sizeof(voiceContrl->relCMD));
            memset(voiceContrl->wiPiCmd, '\0', sizeof(voiceContrl->wiPiCmd));

            strcpy(voiceContrl->relCMD, devNamePanduan(newDev));
            strcpy(voiceContrl->wiPiCmd, getCmd);

            return nread;

        }else{          //这里返回0是因为 语音模块在唤醒时，会发送check指令，不希望程序对这个指令做操作

            return 0;
        }

    }
   
}


struct InputCommander voiceContrl = {

    .commandName = "voiceContrl",
    .deviceName = "/dev/ttyAMA0",
    .relCMD = {'\0'},
    .wiPiCmd = {'\0'},
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