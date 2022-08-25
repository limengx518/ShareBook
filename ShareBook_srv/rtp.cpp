#include "rtp.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

RTP::RTP(char* videoPath, std::string ip, int port):
    m_video(videoPath),m_clientIp(ip),m_clientRtpPort(port)
{
    std::cout<<"客户端的地址为"<<m_clientIp<<":"<<m_clientRtpPort<<std::endl;
    m_timestamp=0;
    m_rtp.createUdpSocket();
    m_rtp.bindSocket(SERV_IP,RTP_PORT);
}

int RTP::sendFrames()
{
    printf("----------------start play-----------\n");
//    m_network.bindn(port);
    //对视频进行一帧一帧的解析然后发送
    uint8_t* datas[MAX_FRAME_NUM]={};
    for(int i=0;i<MAX_FRAME_NUM;i++){
        datas[i]=(uint8_t* )malloc(sizeof(uint8_t)*MAX_FRAME_SIZE);
    }
    int *size=(int* )malloc(sizeof(int)*MAX_FRAME_NUM);

    int frameNum=m_video.getFrame(datas,size);
    if(frameNum<0){
        return -1;
    }
    printf("共有%d帧数据\n",frameNum);

    int seq=0;
//    sendFrameMin(datas[0],++seq,size[0]+MAX_RTPHEADER_SIZE);

    //将数据进行循环发送
    for(int i=0;i<frameNum;i++){
        if(size[i]<=MAX_RTPPACKET_SIZE-MAX_RTPHEADER_SIZE){
            if(sendFrameMin(&datas[i][0],++seq,size[i])<=0){
                printf("send min error\n");
                return -1;
            }
        }else{
            if(sendFrameMax(&datas[i][0],size[i],++seq)<=0){
                printf("sendc max error\n");
                return -1;
            }
        }
        //控制播放帧率
        usleep(1000*1000/25);
    }

    printf("全部发送完成\n");
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

RTP::~RTP()
{

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

int RTP::sendFrameMax(uint8_t* data, int size, int& seq)
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

//    printf("%s: size=%d, n=%d\n",__func__,size,seq);
    //完整的RTP包的个数
    int n=size/MAX_RTPPACKET_SIZE;
    //剩余不完整包的大小
    int remain=size%MAX_RTPPACKET_SIZE;
    //NALU头
    uint8_t naluHeader=data[0];

    //创建rtp包
   struct RtpPacket* rtpPacket = (struct RtpPacket*)malloc(MAX_FRAME_SIZE);
   memset(rtpPacket,0,MAX_FRAME_SIZE);

    //对rtp包的头进行初始化
   initRtpHeader(rtpPacket->header,
                  0,0,0,RTP_VER,
                  RTP_PAYLOAD_TYPE_H264,0,
                  seq,
                  m_timestamp,
                  0x66668899);
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
       rtpPacket->header.timestamp=m_timestamp;
       m_timestamp+= 90000/25;
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
            printf("send error\n");
//            return -1;
         }else if(ret==0){
            printf("client close socket\n");
//            return -1;
        }
    }
    free(rtpPacket);
    return seq;
}

int RTP::sendPacket(RtpPacket *rtpPacket, int dataSize)
{
    rtsp_interleaved *rtsp=(struct rtsp_interleaved*)malloc(MAX_RTPPACKET_SIZE+MAX_RTSPINTER_SIZE);
    rtsp->magic='$';
    rtsp->channel=0;
    rtsp->rtp_len=dataSize;


    //将主机字节序变成网络字节序
    rtpPacket->header.seq = htons(rtpPacket->header.seq);
    rtpPacket->header.timestamp = htonl(rtpPacket->header.timestamp);
    rtpPacket->header.ssrc = htonl(rtpPacket->header.ssrc);

    rtsp->rtpPacket=*rtpPacket;
//    printf("dataSize=%d\n",dataSize);
    int ret=m_rtp.sendUdp((void*) rtpPacket,dataSize,m_clientIp.data(),m_clientRtpPort);
//    for(int i=0;i<dataSize-MAX_RTPHEADER_SIZE;i++){
//        printf("%X",rtpPacket->data[i]);
//    }
//    printf("\n");
//    int ret=m_rtp.sendn((void*)rtpPacket,dataSize);
    rtpPacket->header.seq = ntohs(rtpPacket->header.seq);
    rtpPacket->header.timestamp = ntohl(rtpPacket->header.timestamp);
    rtpPacket->header.ssrc = ntohl(rtpPacket->header.ssrc);

    free(rtsp);
    return ret;
}

int RTP::sendFrameMin(uint8_t* data, int seq, int size)
{
//    printf("%s: size=%d,n=%d\n",__func__,size,seq);
    //创建rtp包
   struct RtpPacket* rtpPacket = (struct RtpPacket*)malloc(MAX_RTPPACKET_SIZE);
   //对rtp包的头进行初始化
   initRtpHeader(rtpPacket->header,
                 0,0,0,RTP_VER,
                 RTP_PAYLOAD_TYPE_H264,0,
                 seq,
                 m_timestamp,
                 0x66668899);
   memcpy(rtpPacket->data, data, size);

//   for(int i=0;i<size;i++){
//       printf("%X",rtpPacket->data[i]);
//   }
//   printf("\n");

   int n=-1;
   n=sendPacket(rtpPacket,size+MAX_RTPHEADER_SIZE);
   m_timestamp+= 90000/25;
   free(rtpPacket);
   return n;
}
