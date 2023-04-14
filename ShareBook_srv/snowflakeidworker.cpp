#include "snowflakeidworker.h"
#include <atomic>
#include <chrono>
#include <exception>

void SnowflakeIdWorker::setWorkerId(UInt workerId)
{
    this->workerId = workerId;
}

void SnowflakeIdWorker::setDatacenterId(UInt datacenterId)
{
    this->datacenterId = datacenterId;
}

SnowflakeIdWorker::UInt64 SnowflakeIdWorker::getId()
{
    return nextId();
}

SnowflakeIdWorker::UInt64 SnowflakeIdWorker::nextId()
{
#ifndef SNOWFLAKE_ID_WORKER_NO_LOCK
    std::unique_lock<std::mutex> lock{ mutex };
    AtomicUInt64 timestamp{ 0 };
#else
    static AtomicUInt64 timestamp{ 0 };
#endif
    timestamp = timeGen();

    // 如果当前时间小于上一次ID生成的时间戳，说明系统时钟回退过这个时候应当抛出异常
    if (timestamp < lastTimestamp) {
        std::ostringstream s;
        s << "clock moved backwards.  Refusing to generate id for " << lastTimestamp - timestamp << " milliseconds";
        throw std::exception(std::runtime_error(s.str()));
    }

    if (lastTimestamp == timestamp) {
        // 如果是同一时间生成的，则进行毫秒内序列
        sequence = (sequence + 1) & sequenceMask;
        if (0 == sequence) {
            // 毫秒内序列溢出, 阻塞到下一个毫秒,获得新的时间戳
            timestamp = tilNextMillis(lastTimestamp);
        }
    } else {
        sequence = 0;
    }

#ifndef SNOWFLAKE_ID_WORKER_NO_LOCK
    lastTimestamp = timestamp;
#else
    lastTimestamp = timestamp.load();
#endif

    // 移位并通过或运算拼到一起组成64位的ID
    return ((timestamp - twepoch) << timestampLeftShift)
    | (datacenterId << datacenterIdShift)
    | (workerId << workerIdShift)
    | sequence;
}

SnowflakeIdWorker::SnowflakeIdWorker():
    workerId(0),
    datacenterId(0),
    sequence(0),
    lastTimestamp(0)
{

}

SnowflakeIdWorker::UInt64 SnowflakeIdWorker::timeGen() const
{
    auto t = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
    return t.time_since_epoch().count();
}

SnowflakeIdWorker::UInt64 SnowflakeIdWorker::tilNextMillis(UInt64 lastTimestamp) const
{
    UInt64 timestamp = timeGen();
    while (timestamp <= lastTimestamp) {
        timestamp = timeGen();
    }
    return timestamp;
}
