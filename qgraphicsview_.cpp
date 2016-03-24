#include "qgraphicsview_.h"

QGraphicsView_::QGraphicsView_(QWidget *parent)
	: QGraphicsView(parent)
{
	rubberBandExists = false;
	isHighResolution = false;
	isScrollPressed = false;
}

QGraphicsView_::~QGraphicsView_()
{

}

void QGraphicsView_::mousePressEvent(QMouseEvent *event)
{
	if( //event->button() == Qt::LeftButton && 
		this->cursor().shape() == Qt::CrossCursor)
	{
		Origin = event->pos();
		if ( !rubberBandExists)
		{
			RubberBand = new QRubberBand(QRubberBand::Rectangle, this);
			rubberBandExists = true;
		}
		RubberBand->setGeometry(QRect( Origin, QSize()));
		RubberBand->show();
				
		if( isHighResolution)
			SelectionRect.setTopLeft( this->mapToScene( this->mapFromGlobal( event->globalPos())).toPoint());
		else
			SelectionRect.setTopLeft( this->mapFromGlobal( event->globalPos()));
	}
	else if( event->button() == Qt::MidButton && !isHighResolution)
	{
		isScrollPressed = true;
		Origin = event->pos();
	}
	else if( isHighResolution)
		QGraphicsView::mousePressEvent( event);
}

void QGraphicsView_::mouseMoveEvent(QMouseEvent *event)
{
	if( this->cursor().shape() == Qt::CrossCursor && rubberBandExists)
		RubberBand->setGeometry(QRect( Origin, event->pos()).normalized());
	else if( !isHighResolution && isScrollPressed)
		emit fastScroll( Origin.y() - event->pos().y());
	else if( isHighResolution)
		QGraphicsView::mouseMoveEvent( event);
}

void QGraphicsView_::mouseReleaseEvent(QMouseEvent *event)
{
	if( this->cursor().shape() == Qt::CrossCursor && rubberBandExists)
	{
		RubberBand->hide();
		SelectionRect.setBottomRight( this->mapFromGlobal( event->globalPos()));
		SelectionRect = SelectionRect.normalized();

		emit highResolutionRegion( SelectionRect);

		if( isHighResolution)
		{	
			SelectionRect.setBottomRight( this->mapToScene( this->mapFromGlobal( event->globalPos())).toPoint());
			SelectionRect = SelectionRect.normalized();
			emit saveRegion( SelectionRect);
		}
		
		RubberBand->~QRubberBand();
		rubberBandExists = false;
	}
	else if( event->button() == Qt::MidButton && !isHighResolution && isScrollPressed)
		emit fastScroll( 0);
	else if( isHighResolution)
		QGraphicsView::mouseReleaseEvent( event);
}