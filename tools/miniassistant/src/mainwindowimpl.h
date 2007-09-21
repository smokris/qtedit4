/**
 * \file mainwindowimpl.h
 * \brief Definition of the main window
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include "ui_mainwindow.h"
#include "miniassistant.h"

class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );

private slots:
	void on_textEdit_cursorPositionChanged();
	void on_action_Quit_triggered();
	
	void on_action_Help_triggered();
	void on_action_Seach_Help_triggered();
	void on_actionShow_contents_triggered();

protected:
	void timerEvent(QTimerEvent *event);

private:
	int guessTimer;
	MiniAssistant *miniAssistant;
};

#endif

