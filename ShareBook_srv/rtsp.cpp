#include "rtsp.h"

//Rtsp的状态码 应该是const的
//std::map<int,std::string> RtspStateCodes{
//    { 200, "OK" },
//    { 302, "Moved Temporarily" },
//    { 400, "Bad Request" },
//    { 401, "Unauthorized" },
//    { 403, "Forbidden" },
//    { 404, "Not Found" },
//    { 405, "Method Not Allowed" },
//    { 454, "Session Not Found" },
//    { 457, "Invalid Range" },
//    { 461, "Unsupported transport" },
//    { 500, "Internal Server Error" },
//    { 503, "Service Unavailable" },
//    { 505, "RTSP Version not supported" },
//    { 551, "Option not supported" },
//    { 0, NULL }
//};



RTSP::RTSP(int& fd,std::string ipaddr):
    m_network(fd),
    m_ipaddr(ipaddr)
{

}

int RTSP::handleOptions()
{
    //服务器收到来自客户端的OPTIONS请求后，直接返回一个代码中预先定义好的字符串变量，该字符串包含了所有RTSP服务端所支持的方法
    char result[500];
    sprintf(result, "RTSP/1.0 200 OK\r\n"
                    "CSeq: %d\r\n"
                    "Public: OPTIONS, DESCRIBE, SETUP, PLAY\r\n"
                    "\r\n",
                    m_cseq);
    std::cout<<"---------------S->C--------------\n";
    std::cout<<result<<std::endl;

    //发送响应给客户端
    return m_network.sendn(result,strlen(result));

}

int RTSP::handleDescribe()
{
    //根据SDP协议返回指定媒体流的描述信息
    //会话描述协议（SDP）为会话通知、会话邀请和其它形式的多媒体会话初始化等目的提供了多媒体会话描述。
    //sdp信息由多行"<type>=<value>"组成，其中<type>是一个字符串，<value>是一个字符串，type表示类型，value的格式视type而定，整个协议区分大小写，"="两侧不允许有空格！
    char sdp[500];
    char result[1000];
    sprintf(sdp, "v=0\r\n"                                      //表示sdp的版本号，不包含次版本号
                 "o=- 9%ld 1 IN IP4 %s\r\n"     //对会话的发起者进行了描述<username><session id><networktype><addresstype><address>
                 "t=0 0\r\n"                    //t=<start time> <stop time> 表示会话的开始时间和结束时间，如果数值均为0，表示一个持久的会话
                 "a=control:*\r\n"              //a=<*>，（可选）表示一个会话级别或者媒体级别下的0个或多个属性
                 "m=video 0 RTP/AVP 96\r\n"     //m=<media><port><transport type><fmt list> ,media表示媒体类型“audio,video...”
                 "a=rtpmap:96 H264/90000\r\n"   //a=rtpmap:<payload typee> <encoding name>/<clock rate> payload type表示动态负载类型，如 98表示h264 encoding name表示编码名称，如H.264
                 "a=control:track0\r\n",
                 time(NULL),m_ipaddr.data());

    sprintf(result, "RTSP/1.0 200 OK\r\n"
                    "CSeq: %d\r\n"
                    "Content-type: application/sdp\r\n"     //回复内容类型
                    "Content-Base: %s\r\n"                  //回复内容基础，一般是url
                    "Content-length: %zu\r\n\r\n"           //回复内容长度，表示sdp信息的长度
                    "%s",
                    m_cseq,
                    m_ipaddr.data(),
                    strlen(sdp),
                    sdp);
    std::cout<<"---------------S->C--------------\n";
    std::cout<<result<<std::endl;
    //发送响应给客户端
    return m_network.sendn(result,strlen(result));

}

int RTSP::handleSetup()
{
    //用于商定两端之间媒体流的传输方式
    char result[512];
//    char sendBuf[512];
    snprintf(result, strlen(result)*sizeof(char),
             "Transport: RTP/AVP/TCP;unicast;interleaved=0-1");
//    strcat(sendBuf,result);
//    return m_network.sendMessage(result,strlen(result));
}

int RTSP::handlePlay(int & fd)
{
    m_rtp=new RTP("/root/test/test.h264",fd);
    return m_rtp->sendFrames();
}

int RTSP::handlePause()
{

}

int RTSP::handleTearDown()
{

}

void RTSP::start(int& fd)
{
    char method[40];
    char url[100];
    char version[40];
    char *bufPtr;
    char* rBuf = (char*)malloc(1500);
    char line[400];
    int heartbeat = 0;
    int heartbeatcount = 0;
    while(1)
    {
        int recvLen;
        memset(rBuf,0,1500);
        recvLen = m_network.receiven(rBuf,sizeof(char)*1500);
        if(recvLen <= 0){
            printf("%s recvLen <=0 \n",__func__);
            break;
        }

        rBuf[recvLen] = '\0';
        printf("---------------C->S--------------\n");
        printf("%s", rBuf);

        /* 解析方法 */
        bufPtr = GetLineFromBuf(rBuf, line);
        if(sscanf(line, "%s %s %s\r\n", method, url, version) != 3)
        {
            printf("%s  parse err 1 \n",__func__);
            break;
        }

        /* 解析序列号 */
        bufPtr = GetLineFromBuf(bufPtr, line);
        if(sscanf(line, "CSeq: %d\r\n", &m_cseq) != 1)
        {
            printf("%s  parse err 2 \n",__func__);
            break;
        }

        /* 如果是SETUP，那么就再解析client_port */
        if(!strcmp(method, "SETUP")){
            printf("handleSetup\n");
            handleSetup();
        }

        if(!strcmp(method, "OPTIONS")){
            printf("handleOptions\n");
            if(handleOptions()<=0){
                printf("Send options error\n");
            }
        }

        if(!strcmp(method, "DESCRIBE")){
            printf("handleDescribe\n");
            handleDescribe();
        }

        if(!strcmp(method, "PLAY")){
            printf("handlePlay\n");
            handlePlay(fd);
        }

    }

    free(rBuf);
}

RTSP::~RTSP()
{
//    delete m_rtp;
}

char*  RTSP:: GetLineFromBuf(char* buf, char* line)
{
    while(*buf != '\n')
    {
        *line = *buf;
        line++;
        buf++;
    }

    *line = '\n';
    line++;
    *line = '\0';

    buf++;
    return buf;
}
