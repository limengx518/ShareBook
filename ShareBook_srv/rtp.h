#ifndef RTP_H
#define RTP_H

#include <cstdint>
#include "h264datasource.h"
#include "aacdatasource.h"
#include "network.h"

/*
 *   RTP包的头部结构
 *
 *    0                   1                   2                    3
 *    7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |V=2|P|X|  CC   |M|     PT      |       sequence number         |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                           timestamp                           |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |           synchronization source (SSRC) identifier            |
 *   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *   |            contributing source (CSRC) identifiers             |
 *   :                             ....                              :
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *   1. M字段用于表示当前RTP包中的载荷是否本身是一个完整的视频帧，或者是否与前一个RTP包中的载荷属于一个视频帧，并且是该帧中的最后一个分片
 *   2. PT字段表示RTP载荷的类型，客户端解析接收到的RTP包后，可以利用该字段选择合适的解码器对媒体数据解码
 *   3. sequence number字段在每个RTP包中以此递增，也就是说每发送一个RTP包就加一，如果客户端接收到RTP包时发生乱序或者丢包，可以根据该字段在应用程序中进行
 *      乱序重组或者请求重传
 *   4. timestamp字段表示该RTP包载荷首字节的采集时间戳，可以据其对声音和画面同步去抖。比较特殊的是，时间戳的单位不是秒，而是将采样的频率作为基本单位，使得
 *      时间戳单位精确度更高，如视频采样率是90000HZ,则时间戳的单位就为1/90000
 *   5.SSRC字段即同步源，用它区分不同分RTP包，实现时需要保证同意RTP会话中不能有相同的SSRC的值
 *
 */

#define MAX_FRAME_SIZE 1920*1080*3/8 //一帧数据大概的字节数
#define MAX_FRAME_NUM  3*60*60*60 //3分钟视频的最大帧数

struct RtpHeader
{
    /* byte 0 */
    //CC(CSRC count): 4bits，表示头部之后contributing sources identifiers的个数。
    uint8_t csrcLen:4;
    //X: 1bit, 表示是否支持Rtp头扩展，置为1的时候，RtpHeader之后会跟1个header extension
    uint8_t extension:1;
    //P: 1bit，表示是否支持填充，为1的时表示在packet的末尾进行填充，方便一些针对固定长度算法的封装。
    uint8_t padding:1;
    //V: 占用2bits，表示版本号。
    uint8_t version:2;

    /* byte 1 */
    //PT: 7bits，playload type表示传输的多媒体类型。
    uint8_t payloadType:7;
    //M: 1bit;对于视频，标记一帧的结束；对于音频，标记会话的开始。
    uint8_t marker:1;

    /* bytes 2,3 */
    //sequence number:16bits(2字节)，表示RTP包序号
    uint16_t seq;

    /* bytes 4-7 */
    //timestamp:32bits（4字节），表示时间戳, 必须使用90 kHz 时钟频率
    uint32_t timestamp;

    //SSRC:32bits(4字节)，用于标识同步信源
    /* bytes 8-11 */
    uint32_t ssrc;
};

class RTP
{
public:
    RTP(char* videoPath,int &fd);
    int sendFrames();
private:
    H264DataSource m_video;
    Network m_network;

};

#endif // RTP_H
