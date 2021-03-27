#include <string>

#include "format.h"

std::string Format::ElapsedTime(long seconds) {
    std::string sysUpTime = "00:00:00";
    long hrs = 0;
    long mins = 0;

    if (seconds > 0) {
        hrs = seconds / 3600;
        sysUpTime = Format::SysTimeToString(hrs) + ":";
        mins = seconds / 60;
        sysUpTime += Format::SysTimeToString(mins % 60) + ":";
        sysUpTime += Format::SysTimeToString(seconds % 60);
    } 
    return sysUpTime;
}

std::string Format::SysTimeToString(long timeComp) {
    if (timeComp < 10)
        return "0" + std::to_string(timeComp);
    else
        return std::to_string(timeComp);
}