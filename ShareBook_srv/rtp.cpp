#include "rtp.h"
#include <fstream>


RTP::RTP(char* videoPath, int &fd):
    m_video(videoPath),m_network(fd)
{
}

int RTP::sendFrames()
{
    //对视频进行一帧一帧的解析然后发送
    char* datas[MAX_FRAME_NUM]={};
    for(int i=0;i<MAX_FRAME_NUM;i++){
        datas[i]=(char*)malloc( sizeof(char)*MAX_FRAME_SIZE);
    }
    int *size=(int* )malloc(sizeof(int)*MAX_FRAME_NUM);

    int frameNum=m_video.getFrame(datas,size);
    if(frameNum<0){
        return -1;
    }
    printf("共有%d帧数据\n",frameNum);

    std::ofstream ofs;

       //3，打开文件流对象
  ofs.open("/root/test/test.txt", std::ios::out);

    for(int i=0;i<frameNum;i++){
        //4，写数据
        for(int j=0;j<size[i];j++){
            printf("%c",datas[i][j]);
            ofs<<datas[i][j];
        }
    }

    //5，关闭文件流对像
    ofs.close();
}
