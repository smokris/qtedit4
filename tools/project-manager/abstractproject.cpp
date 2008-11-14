#include "abstractproject.h"
#include <QDebug>

AbstractProject::AbstractProject( QObject *parent, QString fileName )
	: QObject(parent)
{
	m_fileName = fileName;
//	loadProject( fileName );
}

AbstractProject::AbstractProject( QString fileName, QObject *parent )
	: QObject(parent)
{
	m_fileName = fileName;
}

AbstractProject::~AbstractProject()
{
	// nothing to do
}

bool AbstractProject::addFiles( QStringList fileNames, QString category )
{
	bool b = true;
	foreach( QString s, fileNames )
	{
		b &= addFile( s, category );
	}
	
	return b;
}

bool AbstractProject::removeFiles( QStringList fileNames )
{
	bool b = true;
	foreach( QString s, fileNames )
	{
		b &= removeFile( s );
	}
	
	return b;
}

bool AbstractProject::addSubProject( AbstractProject *subProject )
{
	if (!subProject)
		return false;
	
	m_subProjects << subProject;
	return true;
}

bool AbstractProject::removeSubProject( AbstractProject *subProject )
{
	int i = m_subProjects.indexOf(subProject);
	if (i == -1)
		return false;
	m_subProjects.removeAt(i);
	delete subProject;
	return true;
}

bool AbstractProject::removeSubProject( QString fileName )
{
	foreach(AbstractProject *p, m_subProjects)
	{
		if (p->m_fileName != fileName)
			continue;
		
		m_subProjects.removeAll(p);
		delete p;
	}
	
	return false;
}

bool AbstractProject::removeSubProjects( QStringList fileNames )
{
	bool b = true;
	foreach( QString s, fileNames )
	{
		b &= removeSubProject( s );
	}
	return b;
}

QList<AbstractProject*> AbstractProject::getSubProjects()
{
	return m_subProjects;
}

QString AbstractProject::getProjectName()
{
	return m_projectName;
}

void AbstractProject::setProjectName( QString newProjectName )
{
	m_projectName = newProjectName;
}

QString AbstractProject::getTargetName()
{
	return m_targetName;
}

void AbstractProject::setTargetName( QString newTargetName )
{
	m_targetName = newTargetName;
}

void AbstractProject::clear()
{
	m_subProjects.clear();
}

void AbstractProject::dumpProject( int depth )
{
	QString filler;
	filler.fill( '\t', depth );

	qDebug( "\n%sDumping project", qPrintable(filler) );
	qDebug( "%s\tname\t%s", qPrintable(filler), qPrintable(m_projectName) );
	qDebug( "%s\ttarget\t%s", qPrintable(filler), qPrintable(m_targetName) );
	
	foreach( QString category, getCategoryList() )
	{
		if (category=="NAME") continue;
		if (category=="TARGET") continue;

		qDebug( "\t%s%s", qPrintable(filler), qPrintable(category) );
		foreach( QString fileName, getFiles(category) )
			qDebug( "%s\t\t%s", qPrintable(filler), qPrintable(fileName) );
	}
	
	// now dump, sub projects
	foreach( AbstractProject *project, m_subProjects )
	{
		project->dumpProject( depth + 1 );
	}
}

// kate: space-indent off; tab-indent on; tab-width 8; indent-width 8;
// kate: show-tabs on; auto-brackets on; end-of-line: unix
// kate: syntax: c++; indent-mode cstyle;auto-insert-doxygen: on;
