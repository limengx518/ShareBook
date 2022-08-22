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
    RTP rtp("/root/test/test.h264",fd);
    rtp.sendFrames();

//    //以只读的方式打开文件
//    FILE* p1=fopen("/root/test/1.h264","r");
//    assert(p1!=NULL);
//    FILE* p2=fopen("/root/test/test.h264","r");
//    assert(p2!=NULL);

//    while(!feof (p1)&&!feof(p2)){
//        char c1=fgetc(p1);
//        char c2=fgetc(p2);
//        if(c1==c2){
////            printf("%c,%c,不相同\n",c1,c2);
////            break;
//        }else{
//            break;
//        }
//    }

//    uint8_t* datas[1000]={};

//    char *buf=(char* )malloc(sizeof(char)*10);
//    memset(buf,0,10);
//    for(int i=0;i<10;i++){
//        printf("%s\n",buf[i]);
//    }
//    printf("%s",buf);



}
