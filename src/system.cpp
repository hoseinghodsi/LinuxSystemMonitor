#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor &System::Cpu() { return cpu_; }

std::vector<Processor> &System::CPUs() {
  std::vector<Processor> listCPUs{};
  for (int i = 0; i < cpuNumber_; i++) {
    Processor cpu;
    listCPUs.push_back(cpu);
  }
  CPUs_ = listCPUs;

  return CPUs_;
}

// getter funtion to access the number of cpus
int System::numberCpu() { return cpuNumber_; }

// finding currently running processes
// Creating a vector of processes and sorting them based on their CPU usage
vector<Process> &System::Processes() {
  vector<Process> runningProcesses{};
  vector<int> runningProcessesIDs = LinuxParser::Pids();
  for (int idx : runningProcessesIDs) {
    Process process{idx};
    runningProcesses.push_back(process);
  }
  // sorting
  sort(runningProcesses.begin(), runningProcesses.end(),
       [](const Process &A, const Process &B) { return (A < B); });
  processes_ = runningProcesses;

  return processes_;
}

std::string System::Kernel() { return Kernel_; }

float System::MemoryUtilization() { return MemoryUtilization_; }

std::string System::OperatingSystem() { return OS_; }

int System::RunningProcesses() { return RunningProcesses_; }

int System::TotalProcesses() { return TotalProcesses_; }

long int System::UpTime() { return UpTime_; }

// Constructor definition
System::System() {
  MemoryUtilization_ = LinuxParser::MemoryUtilization();
  RunningProcesses_ = LinuxParser::RunningProcesses();
  TotalProcesses_ = LinuxParser::TotalProcesses();
  UpTime_ = LinuxParser::UpTime();
  cpuNumber_ = LinuxParser::cpuNumber();
}
