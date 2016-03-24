#ifndef MOTORCOMMANDS_H
#define MOTORCOMMANDS_H

#include <QThread>
#include "sleepsimulator.h"
#include "motorcontrol.h"

class MotorCommands : public QThread
{
	Q_OBJECT

public:
	MotorCommands(QObject *parent);
	~MotorCommands();
	bool Stop;
	int Distance;
	void run();

public slots:
	void onMoveFocus( int Direction);
	void onMoveX( int Direction);
	void onMoveY( int Direction);

private:
	SleepSimulator Sleeper;
	
};

#endif // MOTORCOMMANDS_H
