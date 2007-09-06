/**
 * \file mainwindowimpl.h
 * \brief Definition of the main window
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include "ui_mainwindow.h"
#include "helpviewerimpl.h"

//using namespace 
class HelpViewerImpl;
	
class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );

private slots:
	void on_action_Help_triggered();
	void on_action_Seach_Help_triggered();
	void on_action_Quit_triggered();

private:
	HelpViewerImpl *helpViewer;
};

#endif
