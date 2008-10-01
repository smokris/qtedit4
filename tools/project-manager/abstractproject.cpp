#include "abstractproject.h"
#include <QDebug>

AbstractProject::AbstractProject( QObject *parent, QString fileName )
	: QObject(parent)
{
	m_fileName = fileName;
// 	loadProject( fileName );
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

bool AbstractProject::addFile( QString fileName, QString category )
{
	m_files[category] << fileName;
	return true;
}


bool AbstractProject::addFiles( QStringList fileNames, QString category )
{
	foreach( QString s, fileNames )
	{
		m_files[category] << s;
	}
	
	return true;
}

bool AbstractProject::removeFile( QString fileName )
{
	QHash<QString,QStringList>::const_iterator i = m_files.constBegin();
	while (i != m_files.constEnd()) 
	{
		int j = m_files[i.key()].indexOf(fileName);
		if (j == -1)
			return false;
		m_files[i.key()].removeAt(j);
		++i;
	}
	
	return true;
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

QStringList AbstractProject::getFiles(QString category)
{
	return m_files[category];
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
	m_files.clear();
	m_subProjects.clear();
}

void AbstractProject::dumpProject()
{
	QHash<QString,QStringList>::const_iterator i = m_files.constBegin();
	qDebug() << "NAME" << "->" << m_projectName;
	qDebug() << "TARGET" << "->" << m_targetName;
	
	// for each file category
	while (i != m_files.constEnd()) 
	{
		qDebug() << i.key() << "->" << i.value();
		++i;
	}
	
	
	// now dump, sub projects
}

// kate: space-indent off; tab-indent on; tab-width 8; indent-width 8;
// kate: show-tabs on; auto-brackets on; end-of-line: unix
// kate: syntax: c++; indent-mode cstyle;auto-insert-doxygen: on;
