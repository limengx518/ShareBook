#include "rtp.h"
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

RTP::RTP(char* videoPath, int &fd):
    m_video(videoPath),m_network(fd)
{

}

int RTP::sendFrames()
{
    //对视频进行一帧一帧的解析然后发送
    char* datas[MAX_FRAME_NUM]={};
    for(int i=0;i<MAX_FRAME_NUM;i++){
        datas[i]=(char*)malloc(sizeof(char)*MAX_FRAME_SIZE);
        memset(datas[i],0,MAX_FRAME_SIZE);
    }
    int *size=(int* )malloc(sizeof(int)*MAX_FRAME_NUM);
    memset(size,0,MAX_FRAME_NUM);

    int frameNum=m_video.getFrame(datas,size);
    if(frameNum<0){
        return -1;
    }
    printf("共有%d帧数据\n",frameNum);
    int seq=0;
    //将数据进行循环发送
    for(int i=0;i<frameNum;i++){
        if(size[i]<=MAX_RTPPACKET_SIZE-MAX_RTPHEADER_SIZE){
            sendFrameMin(datas[i],++seq,size[i]+MAX_RTPHEADER_SIZE);
        }else{
            seq=sendFrameMax(datas[i],size[i],++seq);
        }
    }

    for(int i=0;i<MAX_FRAME_NUM;i++){
        free(datas[i]);
    }
    free(size);

    return 1;
//    std::ofstream ofs;

       //3，打开文件流对象
//  ofs.open("/root/test/test.txt", std::ios::out);

//    for(int i=0;i<frameNum;i++){
//        //4，写数据
//        for(int j=0;j<size[i];j++){
//            printf("%c",datas[i][j]);
//            ofs<<datas[i][j];
//        }
//    }

//    //5，关闭文件流对象
    //    ofs.close();
}

void RTP::initRtpHeader(RtpHeader& rtpHeader, uint8_t csrLen, uint8_t extension, uint8_t padding, uint8_t version, uint8_t payloadType, uint8_t marker, uint16_t seq, uint32_t timestamp, uint32_t ssrc)
{
    rtpHeader.csrcLen=csrLen;
    rtpHeader.extension=extension;
    rtpHeader.padding=padding;
    rtpHeader.version=version;
    rtpHeader.payloadType=payloadType;
    rtpHeader.marker=marker;
    rtpHeader.seq=seq;
    rtpHeader.timestamp=timestamp;
    rtpHeader.ssrc=ssrc;
}

int RTP::sendFrameMax(char* data, int size, int seq)
{
    /*
     * nalu长度大于最大包长：分片模式，一个NALU分成几个RTP包（FUs模式）。
     * 要在RTP头12自己后添加FU Indicator 和  FU Header  再添加NALU的数据：
     * RTP header (12bytes)+ FU Indicator (1byte)  +  FU header(1 byte) + NALU payload
     *
     *  0                   1                   2
     *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * | FU Indicator  |   FU Header   |   FU payload   ...  |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     */

    /*
     *     FU Indicator
     *    0 1 2 3 4 5 6 7
     *   +-+-+-+-+-+-+-+-+
     *   |F|NRI|  Type   |
     *   +---------------+
     *
     *   F和NRI字段为NAL头部中对应字段的值，TYPE用来表示FU-A或者FU-B之一
     *
     */

    /*
     *      FU Header
     *    0 1 2 3 4 5 6 7
     *   +-+-+-+-+-+-+-+-+
     *   |S|E|R|  Type   |
     *   +---------------+
     *
     *   S位表示该分片是首个分片，E未表示该分片是末个分片，R为保留位,Type字段则为NAL单元的类型
     *   从上面的结构可以看出，同一个NAL单元分片后的FU INdicator所有字段以及FU Header中的Type字段是相同的
     *   在对NAL单元进行分片时，应该将原始NAL单元中的头部去除，因为每一个FU-A分片已经包含了原始NAL单元头部的信息了
     */

    //完整的RTP包的个数
    int n=size/MAX_RTPPACKET_SIZE;
    //剩余不完整包的大小
    int remain=size%MAX_FRAME_SIZE;
    //NALU头
    uint8_t naluHeader=data[0];

    //创建rtp包
   struct RtpPacket* rtpPacket = (struct RtpPacket*)malloc(MAX_RTPPACKET_SIZE);
   memset(rtpPacket,0,MAX_RTPPACKET_SIZE);

    //对rtp包的头进行初始化
   initRtpHeader(rtpPacket->header,
                  0,0,0,RTP_VER,
                  RTP_PAYLOAD_TYPE_H264,0,
                  seq,
                  0,
                  0);
   int pos=1;

    //发送完整的数据
    for(int i=0;i<n;i++){
       //前面的文章说过NAL头TYPE值28是FU-A分片,把原先nal头的 |F|NRI|赋值到FU Indicator的 |F|NRI|
       rtpPacket->data[0] = (naluHeader & NALU_NRI) | 28;
       //把原先nal头的type的5bits赋值到 FU Header的type的5bits
       rtpPacket->data[1] = naluHeader & NALU_TYPE;
       //第一包数据
       if(i==0){
           rtpPacket->data[1] |= 0x80;//S位是1  1000 0000
       }else if(i==n-1&& remain==0){//最后一包数据
           rtpPacket->data[1] |= 0x40; //E位是1 0100 0000
       }

       //把NALU数据填充到RTP包数据段
       memcpy(rtpPacket->data+2,data+pos,MAX_RTPPACKET_SIZE);

       //发送RTP包
       int ret= sendPacket(rtpPacket,MAX_RTPPACKET_SIZE);
       if(ret<0){
           return -1;
       }
       seq++;
       rtpPacket->header.seq=seq;
       pos+=MAX_RTPPACKET_SIZE;
    }


    //发送剩余的数据
    if(remain>0){
        rtpPacket->data[0] = (naluHeader & NALU_NRI) | 28;
        rtpPacket->data[1] = naluHeader & NALU_TYPE;
        rtpPacket->data[1] |= 0x40; //E位是1 ->0100 0000
        //把NALU数据填充到RTP包数据段
        memcpy(rtpPacket->data+2,data+pos,remain+2);
        int ret= sendPacket(rtpPacket,remain+2);
        if(ret<0){
             return -1;
         }
    }

    free(rtpPacket);
    return seq;
}

int RTP::sendPacket(RtpPacket *rtpPacket, int dataSize)
{
    //将主机字节序变成网络字节序
    rtpPacket->header.seq = htons(rtpPacket->header.seq);
    rtpPacket->header.timestamp = htonl(rtpPacket->header.timestamp);
    rtpPacket->header.ssrc = htonl(rtpPacket->header.ssrc);

    int ret=m_network.sendn((void*) rtpPacket,dataSize);

    rtpPacket->header.seq = ntohs(rtpPacket->header.seq);
    rtpPacket->header.timestamp = ntohl(rtpPacket->header.timestamp);
    rtpPacket->header.ssrc = ntohl(rtpPacket->header.ssrc);

    return ret;
}

void RTP::sendFrameMin(char *data, int seq, int size)
{
    //创建rtp包
   struct RtpPacket* rtpPacket = (struct RtpPacket*)malloc(MAX_RTPPACKET_SIZE);
   //对rtp包的头进行初始化
   initRtpHeader(rtpPacket->header,
                 0,0,0,RTP_VER,
                 RTP_PAYLOAD_TYPE_H264,0,
                 seq,
                 0,
                 0);
   memcpy(rtpPacket->data, data, size);
   sendPacket(rtpPacket,size);
   free(rtpPacket);
}
