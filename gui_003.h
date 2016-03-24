#ifndef GUI_003_H
#define GUI_003_H

#include <QtWidgets/QMainWindow>
#include "ui_gui_003.h"
#include "videoplayer.h"
#include "motorcommands.h"
#include "qgraphicsview_.h"
#include "inspectionwindow.h"
#include <QWheelEvent>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>

#define UP 1
#define DOWN -1
#define LEFT 2
#define RIGHT -2
#define FOCUS_UP 3
#define FOCUS_DOWN -3
#define SMALL 1
#define LARGE 10

class gui_003 : public QMainWindow
{
	Q_OBJECT

public:
	gui_003(QWidget *parent = 0);
	~gui_003();
	VideoPlayer* Player;
	MotorCommands* Motors;
	QGraphicsView_* GraphicsView;
	QGraphicsScene *Scene;
	QGraphicsPixmapItem *Item;
	InspectionWindow *HighResInspectionWindow;

signals:
	void moveFocus( int Direction);
	void moveX( int Direction);
	void moveY( int Direction);

public slots:
	void onFastScroll( int Distance);
	void onHighResolutionRegion( QRect SelectionRegion);
	void onSendFrame( QImage);

protected slots:
	void on_actionUp_triggered();
	void on_actionDown_triggered();
	void on_actionLeft_triggered();
	void on_actionRight_triggered();
	void on_actionSave_triggered();
	void on_actionRGB_toggled(bool type);
	void on_actionLab_toggled(bool type);
	void on_actionCrossCursor_toggled(bool cross);
	void wheelEvent( QWheelEvent *event);
	virtual void keyPressEvent(QKeyEvent *e);

private:
	Ui::gui_003Class ui;
};

#endif // GUI_003_H
