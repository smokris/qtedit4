#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QToolButton>
#include <QBoxLayout>
#include <QTextEdit>

/*
To compile:
qmake -project
qmake
make

Bug:
When maximizing the window, the window will toggle between 3 states, not 2
 1) normal
 2) maximized without border
 3) pseudo full screen
 
This is triggeered by the resize() call, if commented out, the maximize/unmiximize 
works as expected with 2 states
*/
 
class MyMainWindow: public QMainWindow
{
	Q_OBJECT
public:
	MyMainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
	
protected:
	void resizeEvent( QResizeEvent * event );
	
public slots:
	void removeFrame();
	
	void on_maximizeButton_clicked();
	void on_minimizeButton_clicked();
	void on_closeButton_clicked();
};

int main( int argc, char *argv[] )
{
	QApplication app(argc,argv);
	MyMainWindow window;
	
	window.show();
	
	return app.exec();
}

#include "remove-frame-resize.moc"

MyMainWindow::MyMainWindow( QWidget * parent, Qt::WindowFlags flags )
	: QMainWindow( parent, flags )
{
	QTabWidget *tabWidget = new QTabWidget;
	QWidget *rightButtons = new QWidget;
	
// buttons on the right
	QBoxLayout	*l = new QBoxLayout(QBoxLayout::LeftToRight);
	QToolButton	*minButton = new QToolButton;
	QToolButton	*maxButton = new QToolButton;
	QToolButton	*closeButton = new QToolButton;
	
	closeButton->setFixedSize(QSize(16,16));
	closeButton->setText("x");
	minButton->setFixedSize(QSize(16,16));
	minButton->setText(".");
	maxButton->setFixedSize(QSize(16,16));
	maxButton->setText("^");
	connect( closeButton, SIGNAL(clicked()), this, SLOT(on_closeButton_clicked()));
	connect( minButton, SIGNAL(clicked()), this, SLOT(on_minimizeButton_clicked()));
	connect( maxButton, SIGNAL(clicked()), this, SLOT(on_maximizeButton_clicked()));
	
	l->addWidget( minButton );
	l->addWidget( maxButton );
	l->addWidget( closeButton );
	rightButtons->setLayout( l );
	tabWidget->setCornerWidget( rightButtons, Qt::TopRightCorner );
	
// buttons on the left
	QWidget *leftButtons = new QWidget;
	l = new QBoxLayout(QBoxLayout::LeftToRight);
	QToolButton	*newButton = new QToolButton;
	QToolButton	*openButton = new QToolButton;
	QToolButton	*saveButton = new QToolButton;
	
	newButton->setFixedSize(QSize(16,16));
	newButton->setText("n");
	saveButton->setFixedSize(QSize(16,16));
	saveButton->setText("s");
	openButton->setFixedSize(QSize(16,16));
	openButton->setText("o");
	
	l->addWidget( newButton );
	l->addWidget( openButton );
	l->addWidget( saveButton );
	leftButtons->setLayout( l );
	tabWidget->setCornerWidget( leftButtons, Qt::TopLeftCorner );


	setCentralWidget( tabWidget );
	layout()->setSpacing( 0 );

	// dummy widgets for the test
	QTextEdit *e = new QTextEdit;
	e->setFrameShape(QFrame::NoFrame);
	tabWidget->addTab( e, "Editor 1" );
	
	e = new QTextEdit;
	e->setFrameShape(QFrame::NoFrame);
	tabWidget->addTab( e, "Editor 2" );

	move( QPoint(100,100) );

	// BUG: uncomment this to see the borked!
	resize( QSize(500,300) );
}

void MyMainWindow::resizeEvent( QResizeEvent * event )
{
	if (isMaximized())
	{
		removeFrame();
		return;
	}
	
	QMainWindow::resizeEvent( event );
	
	Qt::WindowFlags flags = windowFlags();
	if (!flags.testFlag(Qt::FramelessWindowHint))
		return;
		
	flags &= ~Qt::FramelessWindowHint;
	setWindowFlags( flags );
	show();
}

void MyMainWindow::removeFrame()
{
	Qt::WindowFlags flags = windowFlags();
	if (flags.testFlag(Qt::FramelessWindowHint))
		return;
	
	flags |= Qt::FramelessWindowHint;
	setWindowFlags( flags );
	show();
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
