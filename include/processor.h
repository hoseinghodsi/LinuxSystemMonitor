#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
    float Utilization();  // TODO: See src/processor.cpp
    //Processor();

  // TODO: Declare any necessary private members
 private:
    long _prev_user {0};
    long _prev_nice {0};
    long _prev_system {0};
    long _prev_idle {0};
    long _prev_iowait {0};
    long _prev_irq {0};
    long _prev_softirq {0};
    long _prev_steal {0};
};

#endif