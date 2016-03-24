#ifndef SLEEPSIMULATOR_H
#define SLEEPSIMULATOR_H

#include <QWaitCondition>

class SleepSimulator
{
private:
     QMutex localMutex;
     QWaitCondition sleepSimulator;
public:
    SleepSimulator::SleepSimulator();
    void sleep(unsigned long sleepMS);
    void cancelSleep();
};

#endif