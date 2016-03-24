#include "motorcommands.h"

MotorCommands::MotorCommands(QObject *parent)
	: QThread(parent)
{
	QMutex mutex;
	mutex.lock();
	Stop = true;
	mutex.unlock();
}

MotorCommands::~MotorCommands()
{
	QMutex mutex;
	mutex.lock();
	Stop = true;
	mutex.unlock();
}

void MotorCommands::run()
{
	while( !Stop)
		if( abs(Distance ) < 10)
			continue;
		else if( Distance > 0)
		{
			if( abs(Distance) < 250)
			{
				int sleepytime;
				sleepytime = 100 / abs(Distance) + 10;
				Sleeper.sleep( sleepytime);
			}

			onMoveFocus( 1);
		}
		else if( Distance < 0)
		{
			if( abs(Distance) < 250)
			{
				int sleepytime;
				sleepytime = 100 / abs(Distance) + 10;
				Sleeper.sleep( sleepytime);
			}

			onMoveFocus( -1);
		}
}

void MotorCommands::onMoveFocus( int Direction)
{
	if( Direction > 0)
		moveTo( 1, 1);
	else if( Direction < 0)
		moveTo( -1, 1);
}

void MotorCommands::onMoveX( int Direction)
{
	if( Direction > 0)
		moveTo( 2, 1);
	else if( Direction < 0)
		moveTo( -2, 1);
}

void MotorCommands::onMoveY( int Direction)
{
	if( Direction > 0)
		moveTo( 1, 1);
	else if( Direction < 0)
		moveTo( -1, 1);
}