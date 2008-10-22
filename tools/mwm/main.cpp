#include <QApplication>
#include <MyMainWindow.h>

int main( int argc, char *argv[] )
{
	QApplication app(argc,argv);
	MyMainWindow window;
	
	return app.exec();
}
