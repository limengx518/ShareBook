#ifndef SNOWFLAKEIDWORKER_H
#define SNOWFLAKEIDWORKER_H

#include <sstream>
#include <mutex>
#include "snowflake_noncopyable.h"
#include "snowflake_singleton.h"

class SnowflakeIdWorker :private Noncopyable
{
    // 实现单例
    friend class Singleton<SnowflakeIdWorker>;
public:
    typedef unsigned int UInt;
    typedef unsigned long long int UInt64;

#ifdef SNOWFLAKE_ID_WORKER_NO_LOCK
    typedef std::atomic<UInt> AtomicUInt;
    typedef std::atomic<UInt64> AtomicUInt64;
#else
    typedef UInt AtomicUInt;
    typedef UInt64 AtomicUInt64;
#endif
    void setWorkerId(UInt workerId);
    void setDatacenterId(UInt datacenterId);
    UInt64 getId();

    /**
     * 获得下一个ID (该方法是线程安全的
     *
     * @return SnowflakeId
     */
     UInt64 nextId();

protected:
     SnowflakeIdWorker();

     /**
      * 返回以毫秒为单位的当前时间
      *
      * @return 当前时间(毫秒)
      */
     UInt64 timeGen() const;

     /**
      * 阻塞到下一个毫秒，直到获得新的时间戳
      *
      * @param lastTimestamp 上次生成ID的时间截
      * @return 当前时间戳
      */
     UInt64 tilNextMillis(UInt64 lastTimestamp) const;

private:

#ifndef SNOWFLAKE_ID_WORKER_NO_LOCK
     std::mutex mutex;
#endif

     /**
      * 开始时间截 (2021-01-01 00:00:00.000)
      */
     const UInt64 twepoch = 1514736000000;

     /**
      * 机器id所占的位数
      */
     const UInt workerIdBits = 5;

     /**
      * 数据中心id所占的位数
      */
     const UInt datacenterIdBits = 5;

     /**
      * 序列所占的位数
      */
     const UInt sequenceBits = 12;

     /**
      * 机器ID向左移12位
      */
     const UInt workerIdShift = sequenceBits;

     /**
      * 数据标识id向左移17位
      */
     const UInt datacenterIdShift = workerIdShift + workerIdBits;

     /**
      * 时间截向左移22位
      */
     const UInt timestampLeftShift = datacenterIdShift + datacenterIdBits;

     /**
      * 支持的最大机器id，结果是31
      */
     const UInt maxWorkerId = -1 ^ (-1 << workerIdBits);

     /**
      * 支持的最大数据中心id，结果是31
      */
     const UInt maxDatacenterId = -1 ^ (-1 << datacenterIdBits);

     /**
      * 生成序列的掩码，这里为4095
      */
     const UInt sequenceMask = -1 ^ (-1 << sequenceBits);

     /**
      * 工作机器id(0~31)
      */
     UInt workerId=5;

     /**
      * 数据中心id(0~31)
      */
     UInt datacenterId=12;

     /**
      * 毫秒内序列(0~4095)
      */
     AtomicUInt sequence{ 0 };

     /**
      * 上次生成ID的时间截
      */
     AtomicUInt64 lastTimestamp{ 0 };
};

typedef SnowflakeIdWorker IdWorker;

#endif // SNOWFLAKEIDWORKER_H
