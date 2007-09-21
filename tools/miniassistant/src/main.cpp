/**
 * \file main.cpp
 * \brief Application entry
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#include <QApplication>
#include "mainwindowimpl.h"


/// main entry point
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	MainWindowImpl win;
	win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
