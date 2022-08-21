/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       main.cpp
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/13
 * @history
 *****************************************************************************/

#include <iostream>

#include "boostserver.h"
#include "server.h"
#include "rtp.h"


//void boostServer(){
//    BoostServer boostServer;
//    boostServer.start();
//}
//void cServer(){
//    Server server;
//    server.start();
//}
//#define MAX_FRAME_NUM 1080000

int main()
{
//    std::thread threada(boostServer);
//    std::thread threadb(cServer);
//    threada.join();
//    threadb.join();
//    cServer();
//    boostServer();

    int fd=0;
    RTP rtp("/run/media/root/study/想书/New/ShareBook/ShareBook_srv/test.h264",fd);
    rtp.sendFrames();
    uint8_t* datas[1000]={};

//    char *buf=(char* )malloc(sizeof(char)*10);
//    memset(buf,0,10);
//    for(int i=0;i<10;i++){
//        printf("%c\n",buf[i]);
//    }



}
