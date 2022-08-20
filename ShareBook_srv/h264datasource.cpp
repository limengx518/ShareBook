#include "h264datasource.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#define MAX_FRAME_LENGTH 100000

H264DataSource::H264DataSource(char *filePath)
{
    //以只读的方式打开文件
    m_fp=fopen(filePath,"r");
    assert(m_fp!=NULL);
}

int H264DataSource::getFrames(char *filePath, unsigned int *result)
{
    AVFormatContext *input_ctx = NULL;
    AVCodecContext *decoder_ctx = NULL;
    AVCodec *decoder = NULL;
    enum AVHWDeviceType type;
    const char *dec_name = "h264_qsv";
    int i=0;

    int video_stream = 0, ret;
    AVPacket packet;

    //打开文件
    if (avformat_open_input(&input_ctx, filePath, NULL, NULL) != 0) {
        fprintf(stderr, "Cannot open input file '\n");
        return -1;
    }

    //读取文件信息
    ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Cannot find a video stream in the input file\n");
        return -1;
    }

    while (ret >= 0)
    {
        if ( ( ret = av_read_frame(input_ctx,&packet) ) < 0 )//逐个包从文件读取数据
        {
            break;
        }

        if (video_stream == packet.stream_index)//打印帧大小
        {
            printf( " frame size:%d \n" , packet.size );
            result[i]=packet.size;
            i++;
        }

        av_packet_unref(&packet);
    }

    avformat_close_input(&input_ctx);

    return 0;
}

int H264DataSource::isStartCode3(char *buf , int len)
{
    if(len<3){
        printf("H264DataSource::isStartCode3,buf size is smaller than 3\n");
        return -1;
    }
    if(buf[0]==0 && buf[1]==0 && buf[2]==1){
        return 1;
    }
    return 0;

}

int H264DataSource::isStartCode4(char *buf , int len)
{
    if(len<4){
        printf("H264DataSource::isStartCode3,buf size is smaller than 3\n");
        return -1;
    }
    if(buf[0]==0 && buf[1]==0 && buf[2]==0 && buf[3]==1){
        return 1;
    }
    return 0;
}

char *H264DataSource::findStartCode(char *buf, int len)
{
    if(len<3){
        return NULL;
    }

    for(int i=0;i<len;i++){
        if(isStartCode3(buf,len)||isStartCode4(buf,len)){
            return buf;
        }
        ++buf;
    }

    return NULL;
}

int H264DataSource::getFrame(char **frame)
{
    int pos=0;
    char buf[MAX_FRAME_LENGTH];
    int length,i=0;
    while(1){
        //读取3个字节，查看初始字节码
        if(3 != fread(buf,1,3,m_fp)){
            return 0;
        }

        //如果不是0x000001
        if(!isStartCode3(buf,sizeof(buf))){
            //再读取一个字节
            if(1 != fread(buf+3,1,1,m_fp)){
                return 0;
            }
            if(!isStartCode4(buf,sizeof(buf))){
                return -1;
            }else{
                pos=4;
            }
        }else{
            pos=3;
        }

//        while(!startCodeFound){

//        }
    }
}
