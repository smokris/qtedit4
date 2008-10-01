#ifndef ABSTRACTPROJECT_H
#define ABSTRACTPROJECT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>

class AbstractProject : public QObject
{
	Q_OBJECT
public:
	AbstractProject( QObject *parent=NULL, QString fileName=QString() );
	AbstractProject( QString fileName=QString(), QObject *parent=NULL );
	virtual ~AbstractProject();
	
	bool addFile( QString fileName, QString category=QString() );
	bool addFiles( QStringList fileNames, QString category=QString() );
	bool removeFile( QString fileName );
	bool removeFiles( QStringList fileNames );
	QStringList getFiles(QString category=QString());
	
	bool addSubProject( AbstractProject *subProject );
	bool removeSubProject( AbstractProject *subProject );
	bool removeSubProject( QString fileName );
	bool removeSubProjects( QStringList fileNames );
	QList<AbstractProject*> getSubProjects();
	
	QString getProjectName();
	void setProjectName( QString newProjectName );
	QString getTargetName();
	void setTargetName( QString newTargetName );
	void clear();
	
	void dumpProject();
	
	virtual bool newProject( QString newProjectName ) = 0;
	virtual bool loadProject( QString newFileName ) = 0;
	virtual bool saveProject( QString newFileName ) = 0;
	virtual bool isLoaded() = 0;
	
protected:
	QHash<QString,QStringList> m_files;	// modified by add/remove file
	QList<AbstractProject*> m_subProjects;	// modified by add/remove sub projects
	QString m_fileName;			// should be modified by load, save
	QString m_projectName;			// modified by set/get project name
	QString m_targetName;
};

#endif // ABSTRACTPROJECT_H

// kate: space-indent off; tab-indent on; tab-width 8; indent-width 8;
// kate: show-tabs on; auto-brackets on; end-of-line: unix
// kate: syntax: c++; indent-mode cstyle;auto-insert-doxygen: on;
