#ifndef ABSTRACTPROJECT_H
#define ABSTRACTPROJECT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QMultiHash>
#include <QVariant>

class AbstractProject : public QObject
{
	Q_OBJECT
public:
	AbstractProject( QObject *parent=NULL, QString fileName=QString() );
	AbstractProject( QString fileName=QString(), QObject *parent=NULL );
	virtual ~AbstractProject();
	
	virtual bool newProject( QString newProjectName ) = 0;
	virtual bool loadProject( QString newFileName ) = 0;
	virtual bool saveProject( QString newFileName ) = 0;
	virtual bool isLoaded() = 0;

	virtual bool addFile( QString fileName, QString category=QString() ) = 0;
	virtual bool addFiles( QStringList fileNames, QString category=QString() );
	virtual bool removeFile( QString fileName ) = 0;
	virtual bool removeFiles( QStringList fileNames );
	virtual QStringList getFiles(QString category=QString()) = 0;
	virtual QStringList getCategoryList() = 0;
	
	bool addSubProject( AbstractProject *subProject );
	bool removeSubProject( AbstractProject *subProject );
	bool removeSubProject( QString fileName );
	bool removeSubProjects( QStringList fileNames );
	QList<AbstractProject*> getSubProjects();
	
	virtual QVariant getVariableValue( QString section ) = 0;
	virtual void setVariableValue( QString section, QVariant value ) = 0;
	virtual void addVariableValue( QString section, QVariant value ) = 0;
	virtual void delVariableValue( QString section, QVariant value ) = 0;
	virtual void delVariable( QString section ) = 0;
	
	QString getProjectName();
	void setProjectName( QString newProjectName );
	QString getTargetName();
	void setTargetName( QString newTargetName );
	void clear();
	
	void dumpProject( int depth=0 );
	
	
protected:
	// list of all sub-projects in this project
	// modified by addSubProject(), removeSubProject(), removeSubProjects()
	QList<AbstractProject*> m_subProjects;	// modified by add/remove sub projects
	
	// should be modified by load, save
	QString m_fileName;
	
	// modified by set/get project name
	QString m_projectName;
	
	// the exe/library name this project targets. not always defined
	// modified by setTargetName
	QString m_targetName;
};

#endif // ABSTRACTPROJECT_H

// kate: space-indent off; tab-indent on; tab-width 8; indent-width 8;
// kate: show-tabs on; auto-brackets on; end-of-line: unix
// kate: syntax: c++; indent-mode cstyle;auto-insert-doxygen: on;
