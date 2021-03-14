#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  // initializing Kernel_ private member using LinuxParser Kernel function
  std::string Kernel_ = LinuxParser::Kernel();

  // initializing OS_ private member using LinuxParser OperatingSystem function
  std::string OS_ = LinuxParser::OperatingSystem();
};

#endif