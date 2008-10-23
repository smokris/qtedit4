#include <MyMainWindow.h>

#include <QResizeEvent>
#include <QSettings>
#include <QTabWidget>
#include <QWidget>
#include <QToolButton>
#include <QBoxLayout>
#include <QTextEdit>
#include <QDebug>
#include <QTimer>
#include <QMenuBar>
#include <QKeySequence>
#include <QToolBar>

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
	
	// TODO this does not really work
// 	QTimer::singleShot( 0, this, SLOT(updateFrame()));
}

void MyMainWindow::initGUI()
{
	// http://wiki.qtcentre.org/index.php?title=Embedded_resources
	QIcon	closeIcon(":/trolltech/styles/commonstyle/images/standardbutton-close-16.png");
	QIcon	maxIcon(":/trolltech/styles/commonstyle/images/up-16.png");
	QIcon	minIcon(":/trolltech/styles/commonstyle/images/down-16.png");
	QIcon	newIcon(":/trolltech/styles/commonstyle/images/newdirectory-16.png");
	QIcon	openIcon(":/trolltech/styles/commonstyle/images/standardbutton-open-16.png");
	QIcon	saveIcon(":/trolltech/styles/commonstyle/images/standardbutton-save-16.png");
	
	tabWidget = new QTabWidget;
	QBoxLayout *l;
	
// buttons on the left
	leftButtons = new QWidget(tabWidget);
	leftButtons->hide();
	l = new QBoxLayout(QBoxLayout::LeftToRight);
	QToolButton	*newButton = new QToolButton;
	QToolButton	*openButton = new QToolButton;
	QToolButton	*saveButton = new QToolButton;
	
	newButton->setObjectName("newButton");
	newButton->setAutoRaise( true );
	newButton->setFixedSize(QSize(16,16));
	newButton->setIcon( newIcon );
	
	saveButton->setObjectName("saveButton");
	saveButton->setAutoRaise( true );
	saveButton->setFixedSize(QSize(16,16));
	saveButton->setIcon( saveIcon );
	
	openButton->setObjectName("openButton");
	openButton->setAutoRaise( true );
	openButton->setFixedSize(QSize(16,16));
	openButton->setIcon( openIcon );
	
	l->setSpacing(0);
	l->setContentsMargins(0,0,0,0);
	l->addWidget( newButton );
	l->addWidget( openButton );
	l->addWidget( saveButton );
	leftButtons->setLayout( l );
	
// buttons on the right	
	rightButtons = new QWidget(tabWidget);
	rightButtons->hide();
	l = new QBoxLayout(QBoxLayout::LeftToRight);
	QToolButton	*minButton = new QToolButton(this);
	QToolButton	*maxButton = new QToolButton(this);
	QToolButton	*closeButton = new QToolButton(this);
	
	closeButton->setObjectName("closeButton");
	closeButton->setAutoRaise( true );
	closeButton->setIcon( closeIcon );
	closeButton->setFixedSize(QSize(16,16));
	connect( closeButton, SIGNAL(clicked()), this, SLOT(on_closeButton_clicked()));
	
	minButton->setObjectName("minimizeButton");
	minButton->setAutoRaise( true );
	minButton->setIcon( minIcon );
	minButton->setFixedSize(QSize(16,16));
	connect( minButton, SIGNAL(clicked()), this, SLOT(on_minimizeButton_clicked()));
	
	maxButton->setObjectName("maximizeButton");
	maxButton->setAutoRaise( true );
	maxButton->setIcon( maxIcon );
	maxButton->setFixedSize(QSize(16,16));
	connect( maxButton, SIGNAL(clicked()), this, SLOT(on_maximizeButton_clicked()));
	
	l->setSpacing(0);
	l->setContentsMargins(0,0,0,0);
	l->addWidget( minButton );
	l->addWidget( maxButton );
	l->addWidget( closeButton );
	rightButtons->setLayout( l );
	
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

	initMenu();
	
	metaObject()->connectSlotsByName(this);
}

void MyMainWindow::initMenu()
{
	fileMenu = new QMenu( tr("&File") );
	editMenu = new QMenu( tr("&Edit") );
	settingsMenu = new QMenu( tr("&Settings") );
	helpMenu = new QMenu( tr("&Help") );
	
	actionNew = new QAction( tr("New"), this );
	actionNew->setObjectName("actionNew");
	actionNew->setIcon(QIcon(":/trolltech/styles/commonstyle/images/newdirectory-32.png"));
	
	actionOpen = new QAction( tr("Open..."), this );
	actionOpen->setObjectName("actionOpen");
	actionOpen->setIcon(QIcon(":/trolltech/styles/commonstyle/images/standardbutton-open-32.png"));
	
	actionSave = new QAction( tr("Save"), this );
	actionSave->setObjectName("actionSave");
	actionSave->setIcon(QIcon(":/trolltech/styles/commonstyle/images/standardbutton-save-32.png"));
	
	actionSaveAs = new QAction( tr("Save as.."), this );
	actionSaveAs->setObjectName("actionSaveAs");
	
	actionToggleGUI = new QAction( tr("Toggle GUI"), this );
	actionToggleGUI->setObjectName("actionToggleGUI");
	actionToggleGUI->setCheckable(true);
	actionToggleGUI->setShortcut(tr("Ctrl+M"));
	addAction(actionToggleGUI);
	
	fileMenu->addAction( actionNew );
	fileMenu->addAction( actionOpen );
	fileMenu->addAction( actionSave );
	fileMenu->addAction( actionSaveAs );
	
	settingsMenu->addAction( actionToggleGUI );

	menuBar()->addMenu( fileMenu );
	menuBar()->addMenu( editMenu );
	menuBar()->addMenu( settingsMenu );
	menuBar()->addMenu( helpMenu );
	
	QToolBar *b = addToolBar("Main");
	b->setObjectName("MainToolBar");
	b->addAction( actionNew );
	b->addAction( actionOpen );
	b->addAction( actionSave );
	b->setMovable( false );
}

void MyMainWindow::restoreSettings()
{
	QSettings settingsManager( "mwm.ini", QSettings::IniFormat );
	
	if (settingsManager.contains("state"))
		restoreState(settingsManager.value("state", saveState()).toByteArray());
	if (settingsManager.contains("geometry"))
		restoreGeometry(settingsManager.value("geometry", saveGeometry()).toByteArray());
	if (settingsManager.contains("size"))
		resize( settingsManager.value("size",size()).toSize() );
	if (settingsManager.contains("location"))
		move( settingsManager.value("location",pos()).toPoint() );
	if (settingsManager.value("maximized",false).toBool())
		setWindowState(windowState() | Qt::WindowMaximized);
	
	show();
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

void MyMainWindow::updateFrame()
{
	if (isMaximized() && actionToggleGUI->isChecked())
	{
		QTimer::singleShot( 0, this, SLOT(removeFrame()));
		return;
	}
	
	Qt::WindowFlags flags = windowFlags();
	if (!flags.testFlag(Qt::FramelessWindowHint) && !actionToggleGUI->isChecked())
	{
		// if the window already has a frame, nothing to do
		return;
	}
	
	flags &= ~Qt::FramelessWindowHint;
	setWindowFlags( flags );
	show();
}

void MyMainWindow::removeFrame()
{
	Qt::WindowFlags flags = windowFlags();
	if (flags.testFlag(Qt::FramelessWindowHint))
	{
		// if the window already has no frame, nothing to do
		return;
	}
	
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

void MyMainWindow::on_actionToggleGUI_triggered()
{
	bool display = !actionToggleGUI->isChecked();
	menuBar()->setVisible( display );
	
	leftButtons->setVisible( !display );
	rightButtons->setVisible( !display );
	if (display)
	{
		tabWidget->setCornerWidget( NULL, Qt::TopLeftCorner );
		tabWidget->setCornerWidget( NULL, Qt::TopRightCorner );
	}
	else
	{
		tabWidget->setCornerWidget( leftButtons, Qt::TopLeftCorner );
		tabWidget->setCornerWidget( rightButtons, Qt::TopRightCorner );
	}
	leftButtons->setVisible( !display );
	rightButtons->setVisible( !display );
	
	foreach( QToolBar *b, findChildren<QToolBar*>() )
	{
		b->setVisible( display );
	}
	
	// TODO this does not really work
	//QTimer::singleShot( 0, this, SLOT(updateFrame()));
}
