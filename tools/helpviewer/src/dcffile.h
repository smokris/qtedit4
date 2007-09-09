/**
 * \file dcffile.h
 * \brief Definition of the dcf file loader
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#ifndef DCFFILE_H
#define DCFFILE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QThread>

class QDomDocument;
class QDomNode;
class QFile;
class dcfFile;
class dcfModel;

struct dcfSection
{
	QString reference;
	QString title;
	QString keywordRef;
	QString keywordName;
	dcfFile	*file;
};

class dcfFile;

class dcfFileLoadThread: public QThread
{
public:
	dcfFileLoadThread( class dcfFile *f, QString fileName );
	virtual void run();
private:
	dcfFile *m_f;
	QString m_fileName;
};

class dcfFile: public QObject
{
	Q_OBJECT
public:
	dcfFile();
	void loadFile( QString fileName );
	void loadDocument( QDomDocument doc );
	void loadSection( QDomNode node );
	bool containsPage( QString s );

public slots:
	void fileLoaded();
	
signals:
	void newContentAvaialable();
	
private:
	QString reference;
	QString icon;
	QString imageDir;
	QString title;	
	QList<dcfSection> sections;
	dcfFileLoadThread *loadingThread;

	class dcfModel friend;
	class dcfFileLoadThread friend;
};

#endif
