#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// finding currently running processes
// Creating a vector of processes and sorting them based on their CPU usage
vector<Process>& System::Processes() {
    vector<Process> runningProcesses{};
    vector<int> runningProcessesIDs = LinuxParser::Pids();
    for (int idx : runningProcessesIDs) {
        Process process{idx};
        runningProcesses.push_back(process);
    }

    // sorting
    sort(runningProcesses.begin(), runningProcesses.end(), 
                        [](const Process& A, const Process& B) {
                            return (A < B); 
                        });
    processes_ = runningProcesses;

    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return Kernel_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return OS_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }