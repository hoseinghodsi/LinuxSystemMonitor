#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();  
  int system_n_CPUs();                 
  std::vector<Processor>& CPUs();
  std::vector<Process>& Processes();  
  float MemoryUtilization();          
  long int UpTime();                  
  int TotalProcesses();              
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();    
  
  
  // Constructor
  System(); 

 private:
  Processor cpu_ = {};
  std::vector<Processor> CPUs_ = {};
  std::vector<Process> processes_ = {};
  float MemoryUtilization_ = 0;
  long UpTime_ = 0;
  int TotalProcesses_ = 0;
  int RunningProcesses_ = 0;
  int nCPU_ = 4;
 
  
  // initializing Kernel_ private member using LinuxParser Kernel function
  std::string Kernel_ = LinuxParser::Kernel();
  // initializing OS_ private member using LinuxParser OperatingSystem function
  std::string OS_ = LinuxParser::OperatingSystem();
};

#endif