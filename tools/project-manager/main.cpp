#include "qmakeproject.h"
#include <QCoreApplication>
#include <QDebug>

void loadTest();
void saveTest();
void loadSaveTest();

// make this a shared project, this should not get fuckedup
QMakeProject p(NULL);

int main( int argc, char *argv[] )
{
	QCoreApplication app(argc,argv);

	loadTest();
	saveTest();
	loadSaveTest();
	return 0;
}

void loadTest()
{
	QStringList list;
	list	<< "tests/test1.pro"
		<< "tests/test2.pro"
		<< "tests/test3.pro"
		<< "tests/arora.pro"
	;

	qDebug("--------------");
	foreach( QString s, list)
	{
		qDebug() << "Test - loading" << s;
		p.loadProject(s);
		p.dumpProject();
		qDebug("--------------");
	}
}

void saveTest()
{
	QString fileName = "tests/test4.pro";
	qDebug() << "Test - saving" << fileName;

	p.clearData();
	p.addFile( "main.cpp", "SOURCES" );
	p.addFile( "foo.cpp", "SOURCES" );
	p.addFile( "foo.h", "HEADERS" );
	p.setProjectName("test4");
	p.setTargetName("test4");
	p.saveProject(fileName);
}

void loadSaveTest()
{
	qDebug() << "Loading tests/test1.pro, saving tests/test1_s.pro";
	p.clearData();
	p.loadProject("tests/test1.pro");
	p.saveProject("tests/test1_s.pro");
}


// kate: space-indent off; tab-indent on; tab-width 8; indent-width 8;
// kate: show-tabs on; auto-brackets on; end-of-line: unix
// kate: syntax: c++; indent-mode cstyle;auto-insert-doxygen: on;
