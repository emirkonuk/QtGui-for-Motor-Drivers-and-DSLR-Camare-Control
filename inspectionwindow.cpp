#include "inspectionwindow.h"

InspectionWindow::InspectionWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	HighResInspectionView = new QGraphicsView_(this);
	HighResInspectionScene = new QGraphicsScene( HighResInspectionView);
	
	cv::Mat HighResCVImage;
	HighResCVImage = cv::imread( "temp_highresolution.jpg");
	QImage HighResQImage = QImage((const unsigned char*)( HighResCVImage.data), HighResCVImage.cols, HighResCVImage.rows,
									QImage::Format_RGB888).rgbSwapped();
	HighResInspectionPixmapItem = new QGraphicsPixmapItem( QPixmap::fromImage( HighResQImage));
	
	HighResInspectionScene->addItem( HighResInspectionPixmapItem);
	HighResInspectionView->setScene( HighResInspectionScene);
	HighResInspectionView->setDragMode( QGraphicsView::ScrollHandDrag);
	HighResInspectionView->isHighResolution = true;
	HighResInspectionView->show();

	ui.mainToolBar->addAction( ui.actionHandCursor);
	ui.mainToolBar->addAction( ui.actionSelectionCursor);
	ui.actionSelectionCursor->setText( "Selection Tool");
	ui.actionSelectionCursor->setIcon( QIcon( "Resources/cross.png"));
	ui.actionHandCursor->setText( "Hand Tool");
	ui.actionHandCursor->setIcon( QIcon( "Resources/openhand.png"));
	ui.actionSelectionCursor->setCheckable( true);
	ui.actionHandCursor->setCheckable( true);
	QActionGroup* cursor_group = new QActionGroup( this );
	ui.actionHandCursor->setActionGroup( cursor_group);
	ui.actionSelectionCursor->setActionGroup( cursor_group);
	ui.actionHandCursor->setChecked( true);

	connect( HighResInspectionView,SIGNAL( saveRegion( QRect)), this, SLOT( onSaveRegion( QRect)));
}

InspectionWindow::~InspectionWindow()
{

}

void InspectionWindow::onSaveRegion( QRect SelectionRect)
{
	QString fileName = QFileDialog::getSaveFileName( this, tr("Save File"), "C:/Users/Emir/Desktop", tr("Image Files (*.png *.jpg *.bmp)"));
	HighResInspectionPixmapItem->pixmap().copy( SelectionRect).save( fileName);
}

void InspectionWindow::on_actionHandCursor_toggled( bool Selection)
{
	if( Selection)
	{
		HighResInspectionView->setCursor( Qt::OpenHandCursor);
		HighResInspectionView->setDragMode( QGraphicsView::ScrollHandDrag);
	}
}

void InspectionWindow::on_actionSelectionCursor_toggled( bool Selection)
{
	if( Selection)
	{
		HighResInspectionView->setDragMode( QGraphicsView::NoDrag);
		HighResInspectionView->setCursor( Qt::CrossCursor);
	}
}