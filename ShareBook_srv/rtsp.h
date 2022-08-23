/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       ShareBook_srv
 * @brief      处理客户端的RTSP请求，回应消息的格式为：
 *             RTSP版本+状态码+状态码的解释+CR+LF
 *             消息类型+消息内容+CR+LF
 *             ...
 *             CR+LF
 *
 *             eg:  RTSP/1.0 401 Unauthorized\r\n
 *                  Cseq: 2\r\n
 *                  Data: Fri,Apr 10 2020 19:07:19 GMT\r\r
 *                  qq\r\n
 *
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022-08-23
 * @history
 *****************************************************************************/

#ifndef RTSP_H
#define RTSP_H

#include "rtp.h"
#include <map>


class RTSP
{
public:
    RTSP(int& fd,std::string ipaddr);
    int handleOptions();
    int handleDescribe();
    int handleSetup();
    int handlePlay(int &fd);
    int handlePause();
    int handleTearDown();
    char *GetLineFromBuf(char* buf, char* line);
    void start(int &fd);
    ~RTSP();

private:
    RTP * m_rtp;
    Network m_network;
    int m_cseq; //所接收的序列号
    std::string m_ipaddr;//发送方的ip信息
};

#endif // RTSP_H
