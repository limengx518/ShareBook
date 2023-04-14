#ifndef SNOWFLAKE_NONCOPYABLE_H
#define SNOWFLAKE_NONCOPYABLE_H

class Noncopyable
{
protected:
    Noncopyable() = default;
    ~Noncopyable() = default;

    Noncopyable(const Noncopyable&) = delete;
    Noncopyable(const Noncopyable&&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&&) = delete;
};

#endif // SNOWFLAKE_NONCOPYABLE_H
