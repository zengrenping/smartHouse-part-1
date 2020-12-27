#include "InputCommand.h"

int getSocketCMD(struct InputCommander *socketContrl)
{
    int c_fd;
    int rec_t;
    struct sockaddr_in c_addr;
    memset(&c_addr,0, sizeof(struct sockaddr_in));
    int clen = sizeof(struct sockaddr_in);

    c_fd = accept(socketContrl->s_fd, (struct sockaddr *)&c_addr, &clen);
    if(c_fd == -1){
        printf("网络连接失败。\n");
        perror("accept");
        exit(-1);
    }
    else
    {
        memset(&socketContrl->relCMD, '\0', sizeof(socketContrl->relCMD));//读指令前需要将上次的内容清除

        rec_t = recv(c_fd, socketContrl->relCMD, sizeof(socketContrl->relCMD), 0);
        if (rec_t == -1)
        {
            printf("内容接收失败。\n");
            return -1;
        }
        else if (rec_t == 0)
        {
            printf("客户端退出\n");
        }
        else
        {
            printf("内容接收成功。\n");
        }
        return rec_t;
        
    }
    
}


/********************初始化socket服务******************************/
int InitSocketCMD(struct InputCommander *socketContrl, char *ipAddr, char *port)
{
    int s_fd;
    struct sockaddr_in s_addr;
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd == -1)
    {
        printf("socket服务创建失败。\n");
        perror("socket_server");
        exit(-1);
    }
    else
    {
        printf("socket创建成功。\n");
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(socketContrl->port);
    inet_aton(socketContrl->ipAddress, &s_addr.sin_addr); //

    if (bind(s_fd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr_in)) == -1)
    {
        printf("绑定网络失败。\n");
        perror("bind");
        exit(-1);
    }

    if(listen(s_fd, 5) != -1){
        printf("程序正在监听。\n");
    }

    socketContrl->s_fd = s_fd;//将这个网络描述符直接传进街头体中，以便后面传结构体可直接调用

    return s_fd;
}



struct InputCommander socketContrl = {

    .commandName = "socketServer",
    .relCMD = {'\0'},
    .commandInit = InitSocketCMD,
    .getCommand = getSocketCMD,
    .log = {'\0'},
    .port = 8899,
    .ipAddress = "192.168.124.8",
    .next = NULL

};

struct InputCommander *addSocketContrlToInPutLink(struct InputCommander *phead)
{
    if (phead == NULL)
    {
        phead = &socketContrl;
        return phead;
    }
    else
    {
        socketContrl.next = phead;
        phead = &socketContrl;

        return phead;
    }
}
