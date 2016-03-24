#include "videoplayer.h"

VideoPlayer::VideoPlayer(QObject *parent)
	: QThread(parent)
{
	QMutex mutex;
	mutex.lock();
	Stop = false;
	mutex.unlock();
	remove( "temp.jpg");
}

VideoPlayer::~VideoPlayer()
{
	QMutex mutex;
	mutex.lock();
	Stop = true;
	finalizeCamera();
	mutex.unlock();
}

void VideoPlayer::run()
{
	cv::Mat InputImage;
	cv::Mat Image;
	QImage ImageView;

	initializeCamera();

	while( true)
	{
		if( this->Stop) break;
		
		getCapture( 1, "temp.jpg");
		InputImage = cv::imread( "temp.jpg", 1);
		remove("temp.jpg");

		if( LabImage)
			cv::cvtColor( InputImage, Image, cv::COLOR_BGR2Lab, 0);
		else
			InputImage.copyTo( Image);

		ImageView = QImage( (const unsigned char*)( Image.data), Image.cols, Image.rows,QImage::Format_RGB888).rgbSwapped();
		
		emit sendFrame( ImageView);
		Sleeper.sleep( 10);
	}
}
