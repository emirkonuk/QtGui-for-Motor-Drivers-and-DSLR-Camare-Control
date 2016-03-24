#include "gui_003.h"

gui_003::gui_003(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	GraphicsView = new QGraphicsView_( this);
	Scene = new QGraphicsScene( GraphicsView);
	Item = new QGraphicsPixmapItem();
	Player = new VideoPlayer( this);
	Motors = new MotorCommands( this);

	connect( Player, SIGNAL( sendFrame( QImage)), this, SLOT( onSendFrame( QImage)));
	connect( GraphicsView, SIGNAL( fastScroll( int)), this, SLOT( onFastScroll( int)));
	connect( GraphicsView, SIGNAL( highResolutionRegion( QRect)), this, SLOT( onHighResolutionRegion( QRect)));
	connect( this, SIGNAL( moveFocus( int)), Motors, SLOT( onMoveFocus( int)));
	connect( this, SIGNAL( moveX( int)), Motors, SLOT( onMoveX( int)));
	connect( this, SIGNAL( moveY( int)), Motors, SLOT( onMoveY( int)));
	
	this->setFixedSize(960, 700);
	ui.toolBar->setMovable( false);
	ui.toolBar_2->setMovable( false);
	ui.toolBar_3->setMovable( false);

	
	gui_003::setCentralWidget( GraphicsView);
	GraphicsView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
	GraphicsView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
	
	ui.actionRGB->setCheckable( true);
	ui.actionLab->setCheckable( true);
	ui.actionCrossCursor->setCheckable( true);
	ui.actionRGB->setIcon( QIcon( "Resources/rgb.png"));
	ui.actionLab->setIcon( QIcon( "Resources/lab.png"));
	ui.actionCrossCursor->setIcon( QIcon( "Resources/cross.png"));
	ui.actionSave->setIcon( QIcon( "Resources/Save-icon.png"));
	ui.actionUp->setIcon( QIcon( "Resources/up.png"));
	ui.actionDown->setIcon( QIcon( "Resources/down.png"));
	ui.actionLeft->setIcon( QIcon( "Resources/reverse.png"));
	ui.actionRight->setIcon( QIcon( "Resources/forward.png"));

	QActionGroup* filter_group = new QActionGroup( this );
	ui.actionLab->setActionGroup( filter_group);
	ui.actionRGB->setActionGroup( filter_group);

	//position the horizontal movement controls
	QWidget* HorizontalSpacer = new QWidget();
	HorizontalSpacer->setFixedWidth( 330);
	ui.toolBar_3->addWidget( HorizontalSpacer);
	ui.toolBar_3->addAction( ui.actionLeft);
	ui.toolBar_3->addSeparator();
	ui.toolBar_3->addAction( ui.actionRight);

	//position the vertical movement controls
	QWidget* VerticalSpacer = new QWidget();
	VerticalSpacer->setFixedHeight( 210);
	ui.toolBar_2->addWidget( VerticalSpacer);
	ui.toolBar_2->addAction( ui.actionUp);	
	ui.toolBar_2->addSeparator();
	ui.toolBar_2->addAction( ui.actionDown);

	//position the main toolbar controls
	ui.toolBar->addAction( ui.actionSave);
	ui.toolBar->addAction( ui.actionCrossCursor);
	QWidget* maintoolbar_spacer = new QWidget();
	maintoolbar_spacer->setFixedWidth( 270);
	ui.toolBar->addWidget( maintoolbar_spacer);
	ui.toolBar->addAction( ui.actionRGB);
	ui.toolBar->addSeparator();
	ui.toolBar->addAction( ui.actionLab);

	//set cursor type over the main window parts
	ui.toolBar->setCursor( Qt::PointingHandCursor);
	ui.toolBar_2->setCursor( Qt::PointingHandCursor);
	ui.toolBar_3->setCursor( Qt::PointingHandCursor);

	//color scheme is default
	Player->LabImage = false;
	ui.actionRGB->setChecked( true);
	Player->start();

	//motors
	initializeMotor();

	//focus
	this->setFocus();
}

gui_003::~gui_003()
{

}

void gui_003::onSendFrame( QImage ImageView)
{
	QPixmap Pixmap = QPixmap::fromImage(ImageView).scaled( GraphicsView->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
	Item->setPixmap( Pixmap);
	Scene->addItem( Item);
	GraphicsView->setScene( Scene);
	GraphicsView->show();
}

void gui_003::onFastScroll( int Distance)
{
	if( Distance)
	{
		Motors->Stop = false;
		Motors->Distance = Distance;
		Motors->start();
	}
	else if ( !Distance)
	{
		Motors->Stop = true;
	}
	this->setFocus();
}

void gui_003::onHighResolutionRegion( QRect SelectionRect)
{
	remove( "temp_highresolution.jpg");
	Player->Stop = true;
	getCapture( 0, "temp_highresolution.jpg");

	HighResInspectionWindow = new InspectionWindow();
	HighResInspectionWindow->HighResInspectionView->centerOn( 2464 * (double)SelectionRect.center().x() / GraphicsView->rect().width(),
															  1632 * (double)SelectionRect.center().y() / GraphicsView->rect().height());
	HighResInspectionWindow->HighResInspectionView->setMinimumSize( (int)( SelectionRect.width() * 2464 / (double) 640),
																	(int)( SelectionRect.height()* 1632 / (double) 426));
	HighResInspectionWindow->HighResInspectionView->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum);	
	HighResInspectionWindow->setCentralWidget( HighResInspectionWindow->HighResInspectionView);
	HighResInspectionWindow->show();

	finalizeCamera();
	Player->Stop = false;
	Player->start();
}

void gui_003::on_actionUp_triggered()
{
	ui.statusBar->showMessage( "Going Up!");
	moveTo( UP, SMALL);
}

void gui_003::on_actionDown_triggered()
{
	ui.statusBar->showMessage( "Going Down!");
	moveTo( DOWN, SMALL);
}

void gui_003::on_actionLeft_triggered()
{
	ui.statusBar->showMessage( "Going Left!");
	moveTo( LEFT, SMALL);
}

void gui_003::on_actionRight_triggered()
{
	ui.statusBar->showMessage( "Going Right!");
	moveTo( RIGHT, SMALL);
}

void gui_003::wheelEvent( QWheelEvent *event)
{
	if(event->delta() > 0){
		moveTo( FOCUS_UP, SMALL);
	}
	else {
		moveTo( FOCUS_DOWN, SMALL);
	}		
}

void gui_003::keyPressEvent( QKeyEvent *e)
{
	switch( e->key()){
		case Qt::Key_A:
			moveTo( LEFT, SMALL);
			ui.statusBar->showMessage( "Going Left!");
			break;
		case Qt::Key_D:
			moveTo( RIGHT, SMALL);
			ui.statusBar->showMessage( "Going Right!");
			break;
		case Qt::Key_W:
			moveTo( UP, SMALL);
			ui.statusBar->showMessage( "Going Up!");
			break;
		case Qt::Key_S:
			moveTo( DOWN, SMALL);
			ui.statusBar->showMessage( "Going Down!");
			break;
		case Qt::Key_Plus:
			moveTo( FOCUS_UP, SMALL);
			ui.statusBar->showMessage( "Focus Up!");
			break;
		case Qt::Key_Minus:
			moveTo( FOCUS_DOWN, SMALL);
			ui.statusBar->showMessage( "Focus Down!");
			break;
		default:
			ui.statusBar->showMessage( "Use the arrow keys");
			break;

	}
	

}


void gui_003::on_actionRGB_toggled(bool type)
{
	if( type)
	{
		ui.statusBar->showMessage( "RGB image!");
		Player->LabImage = false;
	}
}

void gui_003::on_actionLab_toggled(bool type)
{
	if( type)
	{
		ui.statusBar->showMessage( "Lab image!");
		Player->LabImage = true;
	}
}

void gui_003::on_actionCrossCursor_toggled(bool cross)
{
	if( cross)
	{
		GraphicsView->setCursor( Qt::CrossCursor);
		ui.actionCrossCursor->setIconText("arrow cursor");
	}
	else
	{
		GraphicsView->setCursor( Qt::ArrowCursor);
		ui.actionCrossCursor->setIconText("arrow cursor");
	}
}

void gui_003::on_actionSave_triggered()
{
	remove( "temp_highresolution.jpg");
	Player->Stop = true;
	getCapture( 0, "temp_highresolution.jpg");
	cv::Mat highresimage;
	highresimage = cv::imread( "temp_highresolution.jpg");
	QImage save_image = QImage((const unsigned char*)(highresimage.data), highresimage.cols,highresimage.rows,QImage::Format_RGB888).rgbSwapped();
	QString fileName = QFileDialog::getSaveFileName( this, tr("Save File"), "C:/Users/Emir/Desktop", tr("Image Files (*.png *.jpg *.bmp)"));
	QPixmap::fromImage( save_image).copy().save( fileName);
	finalizeCamera();
	Player->Stop = false;
	Player->start();
}