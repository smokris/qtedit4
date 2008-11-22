#include "qmakeproject.h"
#include <QCoreApplication>
#include <QDebug>

void loadTest();
void saveTest();
void loadSaveTest();

// make this a shared project, this should not get fuckedup
//QMakeProject p(NULL);
AbstractProject *p = NULL;

int main( int argc, char *argv[] )
{
	QCoreApplication app(argc,argv);
	p = new QMakeProject(NULL);

	loadTest();
	saveTest();
	loadSaveTest();

	qDebug("-------------- project should be clean");
	p->clear();
	p->dumpProject();
	return 0;
}

void loadTest()
{
	QStringList list;
	list
		<< "tests/test1.pro"	// tests = assignment
		<< "tests/test2.pro"	// tests +=
		<< "tests/test3.pro"	// tests -=
		<< "tests/test5.pro"	// tests subdirs
		<< "tests/arora.pro"
	;

	foreach( QString s, list)
	{
		qDebug("--------------");
		qDebug() << "Test - loading" << s;
		p->loadProject(s);
		p->dumpProject();
	}
}

void saveTest()
{
	QString fileName = "tests/test4.pro";
	qDebug() << "Test - saving" << fileName;

	p->clear();
	p->addFile( "main.cpp", "SOURCES" );
	p->addFile( "foo.cpp", "SOURCES" );
	p->addFile( "foo.h", "HEADERS" );
	p->setProjectName("test4");
	p->setTargetName("test4");
	p->saveProject(fileName);
}

void loadSaveTest()
{
	qDebug() << "Loading tests/test1.pro, saving tests/test1_s.pro";
	p->clear();
	p->loadProject("tests/test1.pro");
	p->saveProject("tests/test1_s.pro");
}


// kate: space-indent off; tab-indent on; tab-width 8; indent-width 8;
// kate: show-tabs on; auto-brackets on; end-of-line: unix
// kate: syntax: c++; indent-mode cstyle;auto-insert-doxygen: on;
