#ifndef TIMEINFO_H
#define TIMEINFO_H

#include <iomanip>
#include <string>
#include <chrono>
#include <sstream>

std::string getTime(){
    auto now = std::chrono::system_clock::now();
    auto t_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss<<std::put_time(std::localtime(&t_c), "%F %T");
    std::string time_string=ss.str();

    return time_string;
}

#endif // TIMEINFO_H
