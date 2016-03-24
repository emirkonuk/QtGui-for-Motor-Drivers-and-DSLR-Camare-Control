#ifndef INSPECTIONWINDOW_H
#define INSPECTIONWINDOW_H

#include <QMainWindow>
#include "ui_inspectionwindow.h"
#include "opencv2\opencv.hpp"
#include "qgraphicsview_.h"
#include <QGraphicsPixmapItem>
#include <QFileDialog>

class InspectionWindow : public QMainWindow
{
	Q_OBJECT

public:
	InspectionWindow(QWidget *parent = 0);
	~InspectionWindow();
	QGraphicsView_ *HighResInspectionView;
	QGraphicsScene *HighResInspectionScene;
	QGraphicsPixmapItem *HighResInspectionPixmapItem;

private:
	Ui::InspectionWindow ui;

public slots:
	void onSaveRegion( QRect);

protected slots:
	void on_actionSelectionCursor_toggled(bool Selection);
	void on_actionHandCursor_toggled(bool Selection);
};

#endif // INSPECTIONWINDOW_H
