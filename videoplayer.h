#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include "sleepsimulator.h"
#include "cameracontrol.h"
#include <opencv2\opencv.hpp>

class VideoPlayer : public QThread
{
	Q_OBJECT

public:
	VideoPlayer(QObject *parent);
	~VideoPlayer();

	bool Stop;
	bool LabImage;
	void run();

signals:
	void sendFrame( QImage);

private:
	SleepSimulator Sleeper;
	
};

#endif // VIDEOPLAYER_H
