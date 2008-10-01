#include "qmakeproject.h"
#include <QCoreApplication>
#include <QDebug>

int main( int argc, char *argv[] )
{
	QCoreApplication app(argc,argv);

	QStringList list;
	list	<< "tests/test1.pro" 
		<< "tests/test2.pro"
		<< "tests/test3.pro"
		<< "tests/arora.pro"
	;

	QMakeProject p(NULL);
	qDebug("--------------");
	foreach( QString s, list)
	{
		p.loadProject(s);
		p.dumpProject();
		qDebug("--------------");
	}

	p.clearData();
	p.addFile( "main.cpp", "SOURCES" );
	p.addFile( "foo.cpp", "SOURCES" );
	p.addFile( "foo.h", "HEADERS" );
	p.setProjectName("test4");
	p.setTargetName("test4");
	p.saveProject("tests/test4.pro");
	return 0;
}

// kate: space-indent off; tab-indent on; tab-width 8; indent-width 8;
// kate: show-tabs on; auto-brackets on; end-of-line: unix
// kate: syntax: c++; indent-mode cstyle;auto-insert-doxygen: on;
