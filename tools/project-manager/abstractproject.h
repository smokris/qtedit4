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
	AbstractProject( QObject *parent=NULL, const QString fileName=QString() );
	AbstractProject( const QString fileName=QString(), QObject *parent=NULL );
	virtual ~AbstractProject();
	
	virtual bool newProject( const QString newProjectName ) = 0;
	virtual bool loadProject( const QString newFileName ) = 0;
	virtual bool saveProject( const QString newFileName ) = 0;
	virtual bool isLoaded() = 0;

	virtual bool addFile( const QString fileName, QString category=QString() ) = 0;
	virtual bool addFiles( const QStringList fileNames, QString category=QString() );
	virtual bool removeFile( const QString fileName ) = 0;
	virtual bool removeFiles( const QStringList fileNames );
	virtual QStringList getFiles(const QString category=QString()) = 0;
	virtual QStringList getCategoryList() = 0;
	
	bool addSubProject( AbstractProject *subProject );
	bool removeSubProject( AbstractProject *subProject );
	bool removeSubProject( const QString fileName );
	bool removeSubProjects( const QStringList fileNames );
	QList<AbstractProject*> getSubProjects();
	
	virtual QVariant getVariableValue( QString section ) = 0;
	virtual void setVariableValue( const QString section, QVariant value ) = 0;
	virtual void addVariableValue( const QString section, QVariant value ) = 0;
	virtual void delVariableValue( const QString section, QVariant value ) = 0;
	virtual void delVariable( const QString section ) = 0;
	
	QString getProjectName();
	void setProjectName( const QString newProjectName );
	QString getTargetName();
	void setTargetName( const QString newTargetName );
	virtual void clear();

	virtual bool isSectionList( const QString section );
	
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
