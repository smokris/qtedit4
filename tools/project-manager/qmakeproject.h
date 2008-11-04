#ifndef QMAKEPROJECT_H
#define QMAKEPROJECT_H

#include "abstractproject.h"
#include <QHash>

class QMakeProject: public AbstractProject
{
public:
	QMakeProject( QObject *parent=NULL, QString fileName=QString() );
	QMakeProject( QString fileName=QString(), QObject *parent=NULL );
	
	bool newProject( QString newProjectName );
	bool loadProject( QString newFileName );
	bool saveProject( QString newFileName );
	bool isLoaded();
	void clearData();

	bool addFile( QString fileName, QString category=QString() );
	bool removeFile( QString fileName );
	QStringList getFiles(QString category=QString());
	QStringList getCategoryList();

	QVariant getVariableValue( QString section );
	void setVariableValue( QString section, QVariant value );
	void addVariableValue( QString section, QVariant value );
	void delVariableValue( QString section, QVariant value );
	void delVariable( QString section );

private:
	void setTokens(QStringList tokens);
	void appendTokens(QStringList tokens);
	void removeTokens(QStringList tokens);
	
	QHash<QString,QStringList> m_data;
	QList<QString> m_secctions;
};

#endif // QMAKEPROJECT_H

// kate: space-indent off; tab-indent on; tab-width 8; indent-width 8;
// kate: show-tabs on; auto-brackets on; end-of-line: unix
// kate: syntax: c++; indent-mode cstyle;auto-insert-doxygen: on;
