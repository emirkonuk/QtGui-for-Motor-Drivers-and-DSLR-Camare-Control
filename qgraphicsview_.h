#ifndef QGRAPHICSVIEW__H
#define QGRAPHICSVIEW__H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QRubberBand>

class QGraphicsView_ : public QGraphicsView
{
	Q_OBJECT

public:
	QGraphicsView_(QWidget *parent);
	~QGraphicsView_();

	QPoint Origin;
	bool isHighResolution;

signals:
	void fastScroll( int);
	void highResolutionRegion( QRect);
	void saveRegion( QRect);

protected:
	void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
	bool rubberBandExists;
	bool isScrollPressed;
	QRubberBand* RubberBand;
	QRect SelectionRect;
};

#endif // QGRAPHICSVIEW__H
