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
	
private:
	void setTokens(QStringList tokens);
	void appendTokens(QStringList tokens);
	void removeTokens(QStringList tokens);
	
	QHash<QString,QStringList> m_data;
};

#endif // QMAKEPROJECT_H

// kate: space-indent off; tab-indent on; tab-width 8; indent-width 8;
// kate: show-tabs on; auto-brackets on; end-of-line: unix
// kate: syntax: c++; indent-mode cstyle;auto-insert-doxygen: on;
