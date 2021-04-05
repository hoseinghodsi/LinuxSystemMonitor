#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Finding out how many cpu (active cpus) the system has
int LinuxParser::cpuNumber() {
  std::string line;
  std::string key;
  std::string keyFilter = "processor";  
  int cpuNumber = 0;

  std::ifstream filestream(kProcDirectory + kCpuinfoFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stream(line);
      while (stream >> key) {
        // Counting the number of lines starting with keyword "processor"
        if (key == keyFilter) 
          cpuNumber++; 
      }    
    }
  }
  return cpuNumber;  
}

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  string unit;
  float MemTotal; 
  float MemFree; 
  float Buffers;
  float Cached;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  

  // parsing through /proc/meminfo
  if (filestream.is_open()) {
    while (std::getline(filestream, line)){
      std::istringstream stream(line);
      while (stream >> key >> value >> unit) {
        if (key == "MemTotal:") {
          MemTotal = std::stoi(value);
        }
        else if (key == "MemFree:") {
          MemFree = std::stoi(value);
        }
        else if (key == "Buffers:") {
          Buffers = std::stoi(value);
        } 
        else if (key == "Cached:") {
          Cached = std::stoi(value);
        }
      }
    }
  }

  // calculating memory utilization
  return (MemTotal - (MemFree + Buffers + Cached)) / MemTotal;
}

// Calculating system's overall uptime 
long LinuxParser::UpTime() { 
  std::string line;
  std::string sysUpTime = "";
  std::string sysIdleTime;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stream(line);
      while (stream >> sysUpTime) {
        return std::stol(sysUpTime);
      }
    }
  }
  return 0;
 }

long LinuxParser::Jiffies() { return 0; }
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }
long LinuxParser::ActiveJiffies() { return 0; }
long LinuxParser::IdleJiffies() { return 0; }

// Calculating CPU utilization for any given processes
float LinuxParser::CpuUtilization(int pid) {
  std::string line;
  std::string item;
  long utime;
  long stime;
  long cutime;
  long cstime;
  long starttime;

  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream stream(line);
        for (int i = 1; i <= nStarttime; i++) {
          stream >> item;
          if (i == nUtime) {
            utime = std::stol(item);
          }
          else if (i == nStime) {
            stime = std::stol(item);
          }
          else if (i == nCutime) {
            cutime = std::stol(item);
          }
          else if (i == nCstime) {
            cstime = std::stol(item);
          }
          else if (i == nStarttime) {
            starttime = std::stol(item);
          }
        }
      }
    }
    long hertz = sysconf(_SC_CLK_TCK);

    long totalTime = utime + stime + cutime + cstime;
    float seconds = LinuxParser::UpTime() - (starttime / hertz);
    float cpuUsage = ((totalTime / hertz) / seconds);
    return cpuUsage;

}

// Findind the required values for any desired CPU utilization
vector<std::string> LinuxParser::CpuUtilization(std::string cpuID) {
  std::vector<std::string> cpuUtilComp{};
  std::string line;
  std::string key;
  std::string user;
  std::string nice; 
  std::string system;
  std::string idle; 
  std::string iowait; 
  std::string irq; 
  std::string softirq;
  std::string steal;
  std::string guest; 
  std::string guest_niced;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stream(line);
      while (stream >> key >> user >> nice >> system >> idle >> iowait >> irq >>
             softirq >> steal >> guest >> guest_niced) {
        if (key == cpuID) {
          cpuUtilComp.push_back(user);
          cpuUtilComp.push_back(nice);
          cpuUtilComp.push_back(system);
          cpuUtilComp.push_back(idle);
          cpuUtilComp.push_back(iowait);
          cpuUtilComp.push_back(irq);
          cpuUtilComp.push_back(softirq);
          cpuUtilComp.push_back(steal);
          cpuUtilComp.push_back(guest);
          cpuUtilComp.push_back(guest_niced);
          return cpuUtilComp;                              
        }         
      }
    }
  }
  return cpuUtilComp;
}

// parsing through /proc/stat
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "processes") return std::stoi(value);
      }
    }
  }
  return 0;
}

// parsing through /proc/stat
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "procs_running") return std::stoi(value);
      }
    }
  }
  return 0;
}

// Finding the Command used to launch any desired processes
string LinuxParser::Command(int pid) {
  std::string command = "";
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, command);
    return command;
  }
  return command;
}

// Findind the occupied RAM by any desired processes
string LinuxParser::Ram(int pid) {
  std::string line;
  std::string k;
  std::string v = "";
  int ram;
  std::string ramString;

  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      while (stream >> k >> v) {
        if (k == "VmSize") {
          ram = std::stoi(v);
          ram /= 1024;
          ramString = std::to_string(ram);
          return ramString;
        }
      }
    }
  }
  return v;
}

// Finding the ID of the user owning any desired processes  
string LinuxParser::Uid(int pid) {
  std::string line;
  std::string key;
  std::string value = "";
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "Uid") { 
          return value;
        }
      }
    }
  }
  return value;
}

// Finding the user owning the process
string LinuxParser::User(int pid) {
  std::string uid = Uid(pid);
  std::string line;
  std::string user = "";
  std::string ID, etc;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      while (stream >> user >> etc >> ID) {
        if (ID == uid) {
          return user;
        } 
      }
    }
  }
  return user;
}

// Calculating the uptime for any desired processud
long LinuxParser::UpTime(int pid) {
  std::string line;
  std::string value;
  long upTimePid = 0;

  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stream(line);
      for (int i = 1; i <= nStarttime; i++) {
        stream >> value;
        if (i == nStarttime) {
          try {
            upTimePid = UpTime() - std::stol(value) / sysconf(_SC_CLK_TCK);
            return upTimePid;
          } catch (std::invalid_argument ) {
            return 0;
          }
        }
      }
    }
  }  
  return upTimePid;
}