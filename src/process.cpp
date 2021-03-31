#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return _pid; }

float Process::CpuUtilization() { 
    return LinuxParser::CpuUtilization(_pid);
}

string Process::Command() { return _command; }

string Process::Ram() { return _ram; }

string Process::User() { return _user; }

long int Process::UpTime() { return _pidUpTime; }

bool Process::operator<(Process const& a) const {
    if (_cpuUtil > a._cpuUtil)
        return true;
    else
        return false;
}

// Constructor definition
Process::Process(int pid) {
    _pid = pid;
    _user = LinuxParser::User(_pid);
    _cpuUtil = LinuxParser::CpuUtilization(_pid);
    _pidUpTime = LinuxParser::UpTime(_pid);
    _ram = LinuxParser::Ram(_pid);
    _command = LinuxParser::Command(_pid);
}