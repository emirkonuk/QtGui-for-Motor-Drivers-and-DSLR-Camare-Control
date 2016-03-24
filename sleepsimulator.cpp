#include "sleepsimulator.h"

SleepSimulator::SleepSimulator()
{
	localMutex.lock();
}

void SleepSimulator::sleep(unsigned long sleepMS)
{
	sleepSimulator.wait(&localMutex, sleepMS);
}

void SleepSimulator::cancelSleep()
{
	sleepSimulator.wakeAll();
}