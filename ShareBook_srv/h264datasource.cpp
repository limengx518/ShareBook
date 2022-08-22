#include "h264datasource.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>

H264DataSource::H264DataSource(char *filePath):
    m_path(filePath)
{
    //以只读的方式打开文件
    m_fp=fopen(filePath,"r");
    assert(m_fp!=NULL);
}

int H264DataSource::getFrames(uint8_t *result[])
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
    if (avformat_open_input(&input_ctx, m_path, NULL, NULL) != 0) {
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

//        if (video_stream == packet.stream_index)//打印帧大小
//        {
            printf( " frame size:%d \n" , packet.size );
            result[i]=packet.data;//将存储的帧的数据装入返回数组中
//            printf("%s\n",packet.data);
            std::cout<<packet.data;
            i++;
//        }

        av_packet_unref(&packet);
    }

    avformat_close_input(&input_ctx);
    return i;
}

int H264DataSource::isStartCode3(char *buf)
{
    if(sizeof(buf)<3){
        printf("H264DataSource::isStartCode3,buf size is %lu smaller than 3\n",sizeof(*buf));
        return -1;
    }
    if(buf[0]==0 && buf[1]==0 && buf[2]==1){
        return 1;
    }
    return 0;

}

int H264DataSource::isStartCode4(char *buf)
{
    if(sizeof(buf)<4){
        printf("H264DataSource::isStartCode3,buf size is smaller than 3\n");
        return -1;
    }
    if(buf[0]==0 && buf[1]==0 && buf[2]==0 && buf[3]==1){
        return 1;
    }
    return 0;
}

int H264DataSource::copyString(char *buf, char *frame, int len)
{
    for(int i=0;i<len;i++){
        frame[i]=buf[i];
    }
    return 1;
}

char *H264DataSource::findStartCode(char *buf)
{
    if(sizeof(buf)<3){
        return NULL;
    }

    for(int i=0;i<sizeof(buf);i++){
        if(isStartCode3(buf)||isStartCode4(buf)){
            return buf;
        }
        ++buf;
    }

    return NULL;
}

int H264DataSource::getFrame(char *frame[],int* size)
{
//    FILE *p1=fopen("/root/test/test.txt","r+");
//    assert(p1!=NULL);

    char *buf=(char* )malloc(sizeof(char)*MAX_FRAME_SIZE);
    int frameNums=0;
    while(1){
        int pos=0,StartCodeFound = 0, info3 = 0,info4 = 0,rewind=0;
        memset(buf,0,MAX_FRAME_SIZE);

        //读取3个字节，查看初始字节码
        if(3 != fread(buf,1,3,m_fp)){
            printf("H264DataSource::getFrames, fread 3 error\n");
            return 0;
        }

        //如果不是0x000001
        if(!isStartCode3(buf)){
            //再读取一个字节
            if(1 != fread(buf+3,1,1,m_fp)){
                return 0;
            }
            if(!isStartCode4(buf)){
                printf("H264DataSource::getFrames, fread 4 error\n");
                return -1;
            }else{
                pos=4;
            }
        }else{
            pos=3;
        }

        while (!StartCodeFound){
            //如果已经读取到文件末尾
            if (feof (m_fp)){
                printf("H264DataSource::getFrames, read the end\n");
//                fclose(p1);
                free(buf);
                return frameNums+1;
            }
            //读取一个字符并将光标向后移动
            buf[pos++] = fgetc (m_fp);
            //判断是否是帧的起始符号
            info4 = isStartCode4(&buf[pos-4]);
            if(info4 != 1)
                info3 = isStartCode3(&buf[pos-3]);
            StartCodeFound = (info3 == 1 || info4 == 1);
        }
        int n=info3==1?3:4;
        copyString(buf,frame[frameNums],pos-n);
        size[frameNums]=pos-n;
        frameNums++;

        // Here, we have found another start code (and read length of startcode bytes more than we should
        // have.  Hence, go back in the file
        // 这里，要将文件指针的位置重新向前移动
        rewind = (info4 == 1)? -4 : -3;

        if (0 != fseek (m_fp, rewind, SEEK_CUR)){
            free(buf);
            printf("H264DataSource::getFrames, Cannot fseek in the bit stream file\n");
            return -1;
        }

    }

}

H264DataSource::~H264DataSource()
{
    fclose(m_fp);
}
