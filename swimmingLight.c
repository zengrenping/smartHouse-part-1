#include "contrlDevices.h"
#include <unistd.h>

int swimmingLightOpen(int serial_fd)
{
    printf("usart open\n");
    int nwrite = write(serial_fd, "a", sizeof(char));
    if(nwrite < 0 ){
        printf("串口数据写入失败。\n");
       return -1;
    }else{
        printf("写入成功。写入大小为%d\n",nwrite);
    }

}

int swimmingLightClose(int serial_fd)
{
    printf("usart close\n");
    int nwrite = write(serial_fd, "b", sizeof(char));
    if (nwrite < 0)
    {
        printf("串口数据写入失败。\n");
        return -1;
    }else{
        printf("写入成功。写入大小为%d\n",nwrite);
    }
}




struct Devieces swimmingLight = {
    .deviecesName = "swimmingLight",
    .open = swimmingLightOpen,
    .close = swimmingLightClose
    };

struct Devieces *addSwimmingLightToDeviecLink(struct Devieces *phead)
{
    if (phead == NULL)
    {
        phead = &swimmingLight;
        return phead;
    }
    else
    {
        swimmingLight.next = phead;
        phead = &swimmingLight;

        return phead;
    }
}
