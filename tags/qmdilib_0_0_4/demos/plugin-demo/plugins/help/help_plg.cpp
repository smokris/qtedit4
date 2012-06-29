/**
 * \file help_plg.cpp
 * \brief Implementation of the HelpPlugin class
 * \author Diego Iastrubni (elcuco@kde.org)
 *  License LGPL
 * \see HelpPlugin
 */

// $Id$

#include <QAction>
#include <QProcess>
#include <QApplication>
#include <QFile>
#include <QUrl>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QMainWindow>
#include <QStatusBar>

#include "iplugin.h"
#include "qmdiserver.h"
#include "help_plg.h"
#include "helpbrowse.h"


HelpPlugin::HelpPlugin()
	: IPlugin()
{
	actionAbout	= new_action( QIcon(), tr("&About"), this, "", tr("XXXXX"), SLOT(showAboutApp()) );
	actionAboutQt	= new_action( QIcon(), tr("&About Qt"), this, "", tr("XXXXX"), SLOT(showAboutQt()) );
	actionShowQtHelp= new_action( QIcon(), tr("&Qt help"), this, "Ctrl+F1", tr("XXXXX"), SLOT(showQtHelp()) );

	name = "Help plugin";
	author = "Diego Iastrubni <elcuco@kde.org>";
	iVersion = 0;
	sVersion = "0.0.1";
	autoEnabled = true;
	alwaysEnabled = true;

	menus["&Help"]->addAction( actionShowQtHelp );
	menus["&Help"]->setMergePoint();
	menus["&Help"]->addSeparator();
	menus["&Help"]->addAction( actionAbout );
	menus["&Help"]->addAction( actionAboutQt );

// 	externalBrowser = "/opt/kde3/bin/konqueror";
}

HelpPlugin::~HelpPlugin()
{
	delete actionAbout;
	delete actionAboutQt;
	delete actionShowQtHelp;
}

void HelpPlugin::showAbout()
{
}

void HelpPlugin::showAboutApp()
{
	QMessageBox::about( 0, "qmdilib demo", "A small demo for loading plugins<br>Diego Iastrubni (elcuco@kde.org) - lgpl)" );
}

void HelpPlugin::showAboutQt()
{
	QApplication::aboutQt();
}

void HelpPlugin::showQtHelp()
{
	QString helpFile = QLibraryInfo::location(QLibraryInfo::DocumentationPath) + QLatin1String("/html/index.html");
	loadHTML( helpFile );
}

void HelpPlugin::on_browser_sourceChanged ( const QUrl & src )
{
	if (externalBrowser.isEmpty())
		return;

	if ((src.scheme() == "file") || (src.scheme().isEmpty()))
		return;

	if (!QProcess::startDetached( externalBrowser, QStringList(src.toString()) ))
	{
		QWidget *w = dynamic_cast<QWidget*>(mdiServer);
		if (w == NULL)
			return;

		QMainWindow *ww = dynamic_cast<QMainWindow*>(w->window());
		if (ww)
			ww->statusBar()->showMessage("Error: could not start external browser", 5000);
	}
}

int HelpPlugin::canOpenFile( const QString fileName )
{
	QUrl u(fileName);

	if (u.scheme().toLower() != "help")
		return -1;

	// now, lets assume this is a class name
	QString className = u.path().toLower();
	if (QFile::exists( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + 
		   QLatin1String("/html/") + className + QLatin1String(".html") ))
		return 1;
	else	
		return -1;
}

bool HelpPlugin::openFile( const QString fileName, int x, int y, int z )
{
	QUrl u(fileName);
	bool b = canOpenFile(fileName) == 1;

	if (b)
		return loadHTML( QLibraryInfo::location(QLibraryInfo::DocumentationPath) +  QLatin1String("/html/") + u.path().toLower() + QLatin1String(".html"),
			 x, y, z );
	else
		return false;
}

bool HelpPlugin::loadHTML( QString fileName, int x, int y, int z  )
{
	QexHelpBrowser *browser = new QexHelpBrowser( QUrl("file:" + fileName), true );
	browser->hide();
	browser->name = "Qt help";
	browser->setObjectName( browser->name );
	connect( browser, SIGNAL(sourceChanged(QUrl)), this, SLOT(on_browser_sourceChanged(QUrl)));

	mdiServer->addClient( browser );
	
	return true;
}