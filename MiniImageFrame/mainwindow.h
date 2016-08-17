#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
	class QAction;
	class QMenu;
	class ViewWidget;
	class QImage;
	class QPainter;
	class QRect;
	class ImageWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *e);
	void paintEvent(QPaintEvent *paintevent);

private slots:
	
private:
	void CreateActions();
	void CreateMenus();
	void CreateToolBars();
	void CreateStatusBar();

private:
	Ui::MainWindowClass ui;

	// Menus
	QMenu		*menu_file_;
	QMenu		*menu_edit_;
	QMenu		*menu_help_;
	QMenu		*menu_warping_;

	// ToolBars
	QToolBar	*toolbar_file_;
	QToolBar	*toolbar_edit_;

	// Actions
	QAction		*action_new_;
	QAction		*action_open_;
	QAction		*action_save_;
	QAction		*action_saveas_;

	QAction		*action_invert_;
	QAction		*action_mirror_;
	QAction		*action_gray_;
	QAction		*action_restore_;

	QAction		*action_controlpoints_;
	QAction		*action_warping_idw_;
	QAction		*action_warping_rbf_;

	ImageWidget		*imagewidget_;
};

#endif // MAINWINDOW_H
