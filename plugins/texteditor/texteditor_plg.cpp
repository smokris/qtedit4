#include <QMainWindow>
#include <QUrl>
#include <QMessageBox>
#include <QAction>
#include <QStringList>

#include <qmdiserver.h>

#include "texteditor_plg.h"
#include "src/widgets/qmdieditor.h"


TextEditorPlugin::TextEditorPlugin()
{
	name = tr("Text editor plugin - based on QtSourceView");
	author = tr("Diego Iastrubni <elcuco@kde.org>");
	iVersion = 0;
	sVersion = "0.0.1";
	autoEnabled = true;
	alwaysEnabled = false;
	
	actionNewFile	= new QAction( tr("New blank file"), this  );
	actionNewCPP	= new QAction( tr("New source"), this );
	actionNewHeader	= new QAction( tr("New header"), this );
	myNewActions	= new QActionGroup(this);
	myNewActions->addAction( actionNewFile );
	myNewActions->addAction( actionNewCPP );
	myNewActions->addAction( actionNewHeader );

	connect( myNewActions, SIGNAL(triggered(QAction*)), this, SLOT(fileNew(QAction*)));
}

TextEditorPlugin::~TextEditorPlugin()
{
}

void	TextEditorPlugin::showAbout()
{
	QMessageBox::information( dynamic_cast<QMainWindow*>(mdiServer), "About", "This pluging gives a QtSourceView based text edito" );
}

QWidget*	TextEditorPlugin::getConfigDialog()
{
	return NULL;
}

QActionGroup*	TextEditorPlugin::newFileActions()
{
	return myNewActions;
}

QStringList	TextEditorPlugin::myExtensions()
{
	QStringList s;
	s << tr("Sources"	, "EditorPlugin::myExtensions")	+ " (*.c *.cpp *.cxx *.h *.hpp *.hxx *.inc)";
	s << tr("Headers"	, "EditorPlugin::myExtensions")	+ " (*.h *.hpp *.hxx *.inc)";
	s << tr("Text files"	, "EditorPlugin::myExtensions")	+ " (*.txt)";
		s << tr("Qt project"	, "EditorPlugin::myExtensions")	+ " (*.pro *.pri)";
	s << tr("All files"	, "EditorPlugin::myExtensions")	+ " (*.*)";
	
	return s;
}

int	TextEditorPlugin::canOpenFile( const QString fileName )
{
	QUrl u(fileName);

	// if the scheme is a single line, lets assume this is a windows drive
	if (u.scheme().length() != 1)
		if ( (u.scheme().toLower() != "file") && (!u.scheme().isEmpty()) )
			return -2;

	if (fileName.endsWith(".c", Qt::CaseInsensitive))
		return 5;
	else if (fileName.endsWith(".cpp", Qt::CaseInsensitive))
		return 5;
	else if (fileName.endsWith(".cxx", Qt::CaseInsensitive))
		return 5;
	else if (fileName.endsWith(".h", Qt::CaseInsensitive))
		return 5;
	else if (fileName.endsWith(".hpp", Qt::CaseInsensitive))
		return 5;
	else if (fileName.endsWith(".hxx", Qt::CaseInsensitive))
		return 5;
	else if (fileName.endsWith(".inc", Qt::CaseInsensitive))
		return 5;
	else if (fileName.endsWith(".pro", Qt::CaseInsensitive))
		return 5;
	else if (fileName.endsWith(".pri", Qt::CaseInsensitive))
		return 5;
	else return 1;
}

bool	TextEditorPlugin::openFile( const QString fileName, int x, int y, int z )
{
	qmdiEditor *editor = new qmdiEditor( fileName, dynamic_cast<QMainWindow*>(mdiServer) );
	mdiServer->addClient( editor );

	// TODO
	// 1) move the cursor as specified in the parameters
	// 2) return false if the was was not open for some reason
	return true;
	Q_UNUSED( x );
	Q_UNUSED( y );
	Q_UNUSED( z );
}

void	TextEditorPlugin::getData()
{
}

void	TextEditorPlugin::setData()
{
}

void TextEditorPlugin::fileNew( QAction * )
{
	qmdiEditor *editor = new qmdiEditor( tr("NO NAME"), dynamic_cast<QMainWindow*>(mdiServer) );
	mdiServer->addClient( editor );
}
