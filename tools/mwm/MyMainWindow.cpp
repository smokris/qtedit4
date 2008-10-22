#include <MyMainWindow.h>

#include <QSettings>
#include <QTabWidget>
#include <QWidget>
#include <QToolButton>
#include <QBoxLayout>
#include <QTextEdit>
#include <QDebug>
#include <QTimer>
#include <QMenuBar>

MyMainWindow::MyMainWindow( QWidget * parent, Qt::WindowFlags flags )
	: QMainWindow( parent, flags )
{
	QTimer::singleShot( 0, this, SLOT(initGUI()) );
	QTimer::singleShot( 0, this, SLOT(restoreSettings()) );
}

MyMainWindow::~MyMainWindow()
{
	saveSettings();
}


void MyMainWindow::resizeEvent( QResizeEvent * event )
{
	QMainWindow::resizeEvent( event );
	
	if (isMaximized())
	{
// 		qDebug( "resizeEvent, maximized, calling removeFrame()");
		QTimer::singleShot( 0, this, SLOT(removeFrame()));
		return;
	}
	
	Qt::WindowFlags flags = windowFlags();
	if (!flags.testFlag(Qt::FramelessWindowHint))
	{
// 		qDebug( "resizeEvent, not maximized");
		// if the window already has a frame, nothing to do
		return;
	}
	
// 	qDebug( "resizeEvent, not maximized, adding frame");
	flags &= ~Qt::FramelessWindowHint;
	setWindowFlags( flags );
	show();
}

void MyMainWindow::initGUI()
{
	// http://wiki.qtcentre.org/index.php?title=Embedded_resources
	QIcon		closeIcon(":/trolltech/styles/commonstyle/images/standardbutton-close-16.png");
	QIcon		maxIcon(":/trolltech/styles/commonstyle/images/up-16.png");
	QIcon		minIcon(":/trolltech/styles/commonstyle/images/down-16.png");
	QIcon		newIcon(":/trolltech/styles/commonstyle/images/down-16.png");
	QIcon		openIcon(":/trolltech/styles/commonstyle/images/down-16.png");
	QIcon		saveIcon(":/trolltech/styles/commonstyle/images/down-16.png");
	
	tabWidget = new QTabWidget;
	
	rightButtons = new QWidget;
	QBoxLayout	*l = new QBoxLayout(QBoxLayout::LeftToRight);
	QToolButton	*minButton = new QToolButton;
	QToolButton	*maxButton = new QToolButton;
	QToolButton	*closeButton = new QToolButton;
	
	closeButton->setAutoRaise( true );
	closeButton->setIcon( closeIcon );
	closeButton->setFixedSize(QSize(16,16));
	connect( closeButton, SIGNAL(clicked()), this, SLOT(on_closeButton_clicked()));
	
	minButton->setAutoRaise( true );
	minButton->setIcon( minIcon );
	minButton->setFixedSize(QSize(16,16));
	connect( minButton, SIGNAL(clicked()), this, SLOT(on_minimizeButton_clicked()));
	
	maxButton->setAutoRaise( true );
	maxButton->setIcon( maxIcon );
	maxButton->setFixedSize(QSize(16,16));
	connect( maxButton, SIGNAL(clicked()), this, SLOT(on_maximizeButton_clicked()));
	
	l->addWidget( minButton );
	l->addWidget( maxButton );
	l->addWidget( closeButton );
	rightButtons->setLayout( l );
	tabWidget->setCornerWidget( rightButtons, Qt::TopRightCorner );
	
	setCentralWidget( tabWidget );
	layout()->setSpacing( 0 );

	// dummy widgets for the test
	QTextEdit *e = new QTextEdit;
	e->setFrameShape(QFrame::NoFrame);
	tabWidget->addTab( e, "Editor 1" );
	
	e = new QTextEdit;
	e->setFrameShape(QFrame::NoFrame);
	tabWidget->addTab( e, "Editor 2" );
	e->setFocus();

	Qt::WindowFlags flags = windowFlags();
	if (isMaximized())
		flags |=  Qt::FramelessWindowHint;
	else
		flags &= ~Qt::FramelessWindowHint;
	
	setWindowFlags( flags );
	menuBar()->hide();
}

void MyMainWindow::restoreSettings()
{
	QSettings settingsManager( "mwm.ini", QSettings::IniFormat );
	
// 	if (settingsManager.contains("state"))
// 		restoreState(settingsManager.value("state", saveState()).toByteArray());
// 	if (settingsManager.contains("geometry"))
// 		restoreGeometry(settingsManager.value("geometry", saveGeometry()).toByteArray());
// 	if (settingsManager.contains("location"))
// 		move( settingsManager.value("location",pos()).toPoint() );
// 	if (settingsManager.contains("size"))
// 		resize( settingsManager.value("size",size()).toSize() );
	if (settingsManager.value("maximized",false).toBool())
		setWindowState(windowState() | Qt::WindowMaximized);
}

void MyMainWindow::saveSettings()
{
	QSettings settingsManager( "mwm.ini", QSettings::IniFormat );
	
	settingsManager.setValue( "size", size() );
	settingsManager.setValue( "location", pos() );
	settingsManager.setValue( "maximized", isMaximized() );
	settingsManager.setValue( "state", saveState() );
	settingsManager.setValue( "geometry", saveGeometry() );
}

void MyMainWindow::removeFrame()
{
	Qt::WindowFlags flags = windowFlags();
	if (flags.testFlag(Qt::FramelessWindowHint))
	{
// 		qDebug( "removeFrame, already has no frame");
		// if the window already has no frame, nothing to do
		return;
	}
	
// 	qDebug( "removeFrame, removing frame");
	flags |= Qt::FramelessWindowHint;
	setWindowFlags( flags );
	showMaximized();
}

void MyMainWindow::on_maximizeButton_clicked()
{
	if (isMaximized())
		showNormal();
	else
		showMaximized();
}

void MyMainWindow::on_minimizeButton_clicked()
{
	showMinimized();
}

void MyMainWindow::on_closeButton_clicked()
{
	close();
}
