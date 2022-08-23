/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       ShareBook_srv
 * @brief      获取.h264格式数据的详细信息
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022-08-20
 * @history
 *****************************************************************************/
#ifndef H264DATASOURCE_H
#define H264DATASOURCE_H
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/hwcontext.h>
#include <libavutil/error.h>
#ifdef __cplusplus
}
#endif

#define MAX_FRAME_SIZE 1920*1080*3/8 //一帧数据大概的字节数

class H264DataSource
{
public:
    H264DataSource(char* filePath);
    int getFrames(uint8_t *result[]);
    //得到每一帧的数据
    int getFrame(char *frame[MAX_FRAME_SIZE], int *size);
    ~H264DataSource();

private:
    char* m_path;
    FILE* m_fp;

    //每一个帧前面带有0x00 00 00 01 或 0x00 00 01分隔符
    //判断是否是帧的起始字节
    int isStartCode3(char* buf);
    int isStartCode4(char* buf);

    int copyString(char* buf,char* frame,int len);

    //找到帧的起始点的位置
    char *findStartCode(char* buf);
};

#endif // H264DATASOURCE_H
