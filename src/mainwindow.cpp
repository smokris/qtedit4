#include <QtGui>
#include <QtDebug>
#include <QUrl>

// #include "qexdilib/qextabwidget.h"
// #include "qmdilib/qmdihost.h"
#include "qmdilib/qmditabwidget.h"

#include "qecpphighlighter.h"
#include "qecodeeditor.h"
#include "mainwindow.h"
#include "textdisplay.h"
#include "helpdisplay.h"

#include "qelib/qtsourceview/qegtkhighlighter.h"
#include "qelib/qtsourceview/qegtklangdef.h"


/**
 * \file    mainwindow.cpp
 * \brief   Implementation of the main window class
 * \author  Diego Iastrubni (elcuco@kde.org)
 * \date    11-11-2005
 * 
 */


/**
 * \class MainWindow
 * \brief The main window of the application
 * 
 * This is the main window seen on screen. When this window
 * is closed, this means the application has been terminated.
 * 
 */


/**
 * \brief Constructor for the main window
 * 
 * This constructs a main window, and generates the actions, menus
 * and toolbars needed.
 * 
 * \todo This class also loads the default highlight mode,
 *       should be move to another class?
 * \todo Do we need to modify the title of the main window?
 */
MainWindow::MainWindow()
{
	defColors.load( "data/default/kate.xml" );
// 	mainTab = new QexTabWidget();
	mainTab = new qmdiTabWidget( this );
	setCentralWidget( mainTab );
	
	createActions();
	createMenus();
	createToolbar();
	
	addAction( actionSelectEditor );
	
	setWindowTitle( tr("QtEdit 4 [*]") );
	statusBar()->showMessage( tr("Welcome"), 5000 );
	loadStatus();

	optionsDialog = new OptionsDialog( this );
	optionsDialog->hide();
}


/**
 * \brief Called when the window is been closed
 *
 * This function is called when the main window is about to 
 * be closed. We use this to save the status of the application.
 * 
 * \warning This function changes the value of the event passed
 *          because otherwise, gcc complains and issues a warning.
 */
void MainWindow::closeEvent( QCloseEvent *event )
{
	saveStatus();

	// ugly hack to shut up warnings from gcc...
	event = 0;
}

/*
NOT USED, left as reference
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(actionCut);
    menu.addAction(actionCopy);
    menu.addAction(actionPaste);
    menu.exec(event->globalPos());
}
*/


/**
 * \brief set the focus on the editor (SLOT)
 *
 * This function restores the focus to the current editor,
 * to the editor in the mainTab.
 */
void MainWindow::selectEditor()
{
	QWidget *w = getCurrentEditor();
	
	if (w)
		w->setFocus();
}

void MainWindow::editorCursorPositionChanged()
{
// 	QPoint p = qobject_cast<TextDisplay*>(mainTab->currentWidget())->getCursorLocation();
// 	statusBar()->showMessage( QString("line: %1, culumn %2").arg(p.y()).arg(p.x() ) );
}


void MainWindow::fileNew()
{
	loadFile( "" );
}

void MainWindow::fileOpen()
{
	QString s = QFileDialog::getOpenFileName(
		this,
		"Choose a file to open",
		"",
		"C/C++ cources (*.c *.c++ *.cpp *.h *.moc);;"
		"C/C++ headers (*.h *.hxx);;"
		"Qt Project files (*.pro *.pri);;"
		"All files (*.*)"
	);
	
	if ( s.isEmpty() )
		return;

	loadFile( s );
}

bool MainWindow::fileSave()
{
	return fileSave( (QECodeEditor*) getCurrentEditor() );
}

bool MainWindow::fileSave( QTextEdit *edit )
{
	if (!edit)
		return false;

	QECodeEditor *e = qobject_cast<QECodeEditor*>(edit);
	if (!e)
		return fileSaveAs();
		
	QString fileName = e->getFileName();

	if (fileName.isEmpty())
		return fileSaveAs();
	
	e->saveFile( fileName );
	setWindowModified( false );
	
	return true;
}

bool MainWindow::fileSaveAs()
{
	return fileSaveAs( (QECodeEditor*) getCurrentEditor() );
}

bool MainWindow::fileSaveAs( QTextEdit *edit )
{
	if (!edit)
		return false;
		
	QECodeEditor *e = qobject_cast<QECodeEditor*>(edit);
	if (!e)
		return false;

	
	QString s = QFileDialog::getSaveFileName(
		this,
		"Choose a file to save",
		"",
		"C/C++ cources (*.c *.c++ *.cpp *.h *.moc);;"
		"C/C++ headers (*.h *.hxx);;"
		"Qt Project files (*.pro *.pri);;"
		"All files (*.*)"
	);
	
	if ( s.isEmpty() )
		return false;

	e->saveFile( s );
	setWindowModified( false );

	return true;
}

void MainWindow::fileClose()
{
	if (!canCloseEditor(getCurrentEditor()) )
		return;
		
	QWidget *w = mainTab->currentWidget();
	mainTab->removeTab( mainTab->currentIndex() );

	delete w;
}


void MainWindow::optionsConfiguration()
{
	optionsDialog->show();
}

void MainWindow::helpBrowseQtDocs()
{
	QString helpFile = QLibraryInfo::location(QLibraryInfo::DocumentationPath) + QLatin1String("/html/index.html");
	
	HelpDisplay *t = new HelpDisplay( helpFile );	
	int i = mainTab->addTab( t, "Qt - doc" );
	mainTab->setCurrentWidget( t );
	mainTab->setCurrentIndex( i );
}

void MainWindow::helpBrowseQtEditDocs()
{
	QString helpFile = QApplication::applicationDirPath()  + QLatin1String("/../doc/html/index.html");
	
	HelpDisplay *t = new HelpDisplay( helpFile );
	int i = mainTab->addTab( t, "QtEdit - doc" );
	mainTab->setCurrentWidget( t );
	mainTab->setCurrentIndex( i );
}

void MainWindow::about()
{
	QMessageBox::about( this, tr("About QtEdit4"),
		tr("The 4rth version of my Qt editor.")
	);
}

void MainWindow::createActions()
{
	actionSelectEditor = new QAction( tr("Select editor"), this);
	actionSelectEditor->setShortcut( tr("Esc") );
	connect( actionSelectEditor, SIGNAL(triggered()), this, SLOT(selectEditor()) );
	
	// file menu
	actionNew = new QAction( QIcon(":images/filenew.png"), tr("&New"), this);
	actionNew->setShortcut(tr("Ctrl+N"));
	actionNew->setStatusTip(tr("Create a new file"));
	connect( actionNew, SIGNAL(triggered()), this, SLOT(fileNew()) );
	
	actionOpen = new QAction( QIcon(":images/fileopen.png"), tr("&Open..."), this);
	actionOpen->setShortcut(tr("Ctrl+O"));
	actionOpen->setStatusTip(tr("Open an existing file"));
	connect( actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()) );
	
	actionClose = new QAction( QIcon(":images/fileclose.png"), tr("C&lose"), this);
	actionClose->setShortcut(tr("Ctrl+W"));
	actionClose->setStatusTip(tr("Close the active tab"));
	connect( actionClose, SIGNAL(triggered()), this, SLOT(fileClose()) );
	
	actionExit = new QAction( QIcon(":images/exit.png"), tr("E&xit"), this);
	actionExit->setShortcut(tr("Ctrl+Q"));
	actionExit->setStatusTip(tr("Exit the application"));
	connect( actionExit, SIGNAL(triggered()), this, SLOT(close()) );

	// options menu
	actionConfig = new QAction( tr("Configuration"), this  );
	actionConfig->setStatusTip("Change the default behaviour of the editor");
	connect( actionConfig, SIGNAL(triggered()), this, SLOT(optionsConfiguration()) );
	
	// help menu
	actionBrowseQtDocs = new QAction(tr("&Browse Qt documentation"), this);
	connect(actionBrowseQtDocs, SIGNAL(triggered()), this, SLOT(helpBrowseQtDocs()));
	
	actionBrowseQtEditDocs = new QAction(tr("&Browse QtEdit documentation"), this);
	actionBrowseQtEditDocs->setShortcut(tr("F1"));
	connect(actionBrowseQtEditDocs, SIGNAL(triggered()), this, SLOT(helpBrowseQtEditDocs()));
	
	actionAbout = new QAction(tr("&About"), this);
	actionAbout->setStatusTip(tr("Show the application's About box"));
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	
	actionAboutQt = new QAction(tr("About &Qt"), this);
	actionAboutQt->setStatusTip(tr("Show the Qt library's About box"));
	connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
	// create own menus
	menus["&File"]->addAction( actionOpen );
	menus["&File"]->addAction( actionExit );
	
	menus["&Edit"];
	menus["&Search"];
	menus["&Project"];
	menus["&Tools"];
	menus["&Options"]->addAction( actionConfig );
	menus["&Settings"];
	
	menus["&Help"]->addAction( actionBrowseQtDocs );
	menus["&Help"]->addAction( actionBrowseQtEditDocs );
	menus["&Help"]->addSeparator();
	menus["&Help"]->addAction( actionAbout );
	menus["&Help"]->addAction( actionAboutQt );
// 	menus.makeMenuBar( menuBar() );
	menus.updateMenu( menuBar() );
}

void MainWindow::createToolbar()
{
/*	fileToolBar = addToolBar( tr("File") );
	fileToolBar->setObjectName( "File" );
	fileToolBar->addAction(actionNew);
	fileToolBar->addAction(actionOpen);
	fileToolBar->addAction(actionClose);*/
	toolbars["File"]->addAction(actionNew);
	toolbars["File"]->addAction(actionOpen);
	toolbars["File"]->addAction(actionClose);
	toolBarList = toolbars.updateToolBar( toolBarList, this );
}


QString MainWindow::getFileName( QString fileName )
{
	int i = fileName.lastIndexOf( "/" );

        // if not found, try windows path
	if ( i == -1 )
		i = fileName.lastIndexOf( "\\" );

	if (i > 0 )
		fileName = fileName.remove( 0, i+1 );

	return fileName;
}

void MainWindow::loadFile( QString fileName )
{

#define _OLD_SH

#ifdef _OLD_SH_
	QECPPHighlighter *h = new QECPPHighlighter( &defColors );
	QECodeEditor *edit = new QECodeEditor( NULL, h );
#else
	QECodeEditor *edit = new QECodeEditor( NULL, NULL );
	QeGtkSourceViewLangDef *langC = new QeGtkSourceViewLangDef( "data/gtksourceview/cpp.lang" );
	QeGTK_Highlighter *highlight = new QeGTK_Highlighter( edit, &defColors );
	highlight->setHighlight( langC );
#endif
	QString tabName;

	if (!fileName.isEmpty())
	{
		edit->loadFile( fileName );
		tabName = getFileName( fileName );
	}
	else
		tabName = tr("NONAME");

	TextDisplay *t = new TextDisplay( edit );
	int i = mainTab->addTab( t, tabName );
	mainTab->setCurrentWidget( t );
	mainTab->setCurrentIndex( i );
	edit->setFocus();
	
	if ( !fileName.isEmpty() )
		statusBar()->showMessage( tr("File \"%1\" loaded").arg(fileName), 5000 );
}

QTextEdit* MainWindow::getCurrentEditor()
{
	QWidget *w = mainTab->currentWidget();

	if (!w)
		return NULL;

	if (!w->inherits("TextDisplay"))
		return NULL;

	w = qobject_cast<TextDisplay*>(w)->getEditor();

	if (!w)
		return NULL;
	else
		if (w->inherits("QTextEdit") )
			return (QTextEdit*)w;
		else
			return NULL;
}


void MainWindow::saveStatus()
{
	QSettings settings;
	QString openFiles = "";
	
	settings.setValue( "main/state", saveState(0) );
	settings.setValue( "main/position", pos() );
	settings.setValue( "main/size", size() );
	
	for (int i=0; i<mainTab->count(); i++)
	{
		if (mainTab->widget(i)->inherits( "TextDisplay" ) )
		{
			//if (dynamic_cast<const TextDisplay*>( mainTab->widget(i) ) == NULL)
			if (!mainTab->widget(i)->inherits("TextDisplay" ))
				continue;
			
			TextDisplay *e = (TextDisplay*) mainTab->widget(i);
			
			if (dynamic_cast<const QECodeEditor*>(e->getEditor()) == NULL )
			//if (!e->getEditor()->inherits("QECodeEditor" ))
				continue;
				
			QECodeEditor* ee = (QECodeEditor*)( e->getEditor() );
			openFiles += ee->getFileName() + ";";
		}
	}
	openFiles.remove( openFiles.size()-1, 1 );
 	settings.setValue( "editor/files", openFiles );
}

void MainWindow::loadStatus()
{
	QSettings settings;
	
	move( settings.value("main/position", QPoint(200, 200)).toPoint() );
	resize( settings.value("main/size", QSize(400, 400)).toSize() );

	QStringList sl = settings.value("editor/files").toString().split( ';' );
	for ( int i=0; i<sl.count(); i++)
		if (! sl[i].isEmpty() )
			loadFile( sl[i] );

	restoreState( settings.value("main/state").toByteArray() );
}

bool MainWindow::canCloseEditor( QTextEdit *e )
{
	if (!e)
		return true;
		
	if ( !e->document()->isModified() )
		return true;

	switch(QMessageBox::information(this, "QtEdit4",
		tr("The document contains unsaved changes\n"
		"Do you want to save the changes before exiting?" ),
		tr("&Save"), tr("&Don't save"), tr("Cancel"),
		0,    // Enter == button 0
		2)
		)  // Escape == button 2
	{
		case 0: // Save clicked or Alt+S pressed or Enter pressed.
			return fileSave();
			break;

		case 1: // Discard clicked or Alt+D pressed don't save but exit
			return true;

		case 2: // Cancel clicked or Escape pressed, don't exit
			return false;
	}

	return false;
}
