#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

float Processor::Utilization(std::string cpuID) {
    std::vector<std::string> CPU = LinuxParser::CpuUtilization(cpuID);
    try {
        long user = std::stol(CPU[0]);
        long nice = std::stol(CPU[1]);
        long system = std::stol(CPU[2]);
        long idle = std::stol(CPU[3]);
        long iowait = std::stol(CPU[4]);
        long irq = std::stol(CPU[5]);
        long softirq = std::stol(CPU[6]);
        long steal = std::stol(CPU[7]);

        long prevIdle = _prev_idle + _prev_iowait;
        long Idle = idle + iowait;
        long prevNoneIdle = _prev_user + _prev_nice + _prev_system +_prev_irq + _prev_softirq + _prev_steal;
        long noneIdle = user + nice + system + irq + softirq + steal;

        long prevTotal = prevIdle + prevNoneIdle;
        long total = Idle + noneIdle;

        float instTotal = total - prevTotal;
        float instIdel = Idle - prevIdle;

        _prev_user = user;
        _prev_nice = nice;
        _prev_system = system;
        _prev_idle = idle;
        _prev_iowait = iowait;
        _prev_irq = irq;
        _prev_softirq = softirq;
        _prev_steal = steal; 

    return (instTotal - instIdel) / instTotal;
    }
    catch (...) {
        return 0;
    }
}