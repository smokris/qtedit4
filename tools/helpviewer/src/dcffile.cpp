/**
 * \file dcffile.cpp
 * \brief Implementation of the dcf file loader
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#include "dcffile.h"
#include <QDebug>

#include <QDomDocument>
#include <QFile>
#include <QString>

//
dcfFileLoadThread::dcfFileLoadThread( class dcfFile *f, QString fileName  )
{
	m_f = f;
	m_fileName = fileName;
}

void dcfFileLoadThread::run()
{
	if (!m_f)
		return;
	
	QDomDocument doc("language");
	
	QFile file(m_fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug( "%s %d : Could not open %s", __FILE__, __LINE__, qPrintable(m_fileName) );
		return;
	}

	//QString s = file.readAll();
	//if (!doc.setContent(s))
	if (!doc.setContent(&file))
	{
		file.close();
		qDebug( "%s %d : Could not open %s", __FILE__, __LINE__, qPrintable(m_fileName) );
		return;
	}
	file.close();

	m_f->loadDocument( doc );
}

//
dcfFile::dcfFile(  )
{
	
}

void dcfFile::loadFile( QString  fileName )
{
#if 1
	loadingThread = new dcfFileLoadThread( this, fileName );
	connect( loadingThread, SIGNAL(finished()), this, SLOT(fileLoaded()));
	loadingThread->start();
#else
	//qDebug("load file start");

	QDomDocument doc("language");
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug( "%s %d : Could not open %s", __FILE__, __LINE__, qPrintable(fileName) );
		return;
	}

	//QString s = file.readAll();
	//qDebug("load file - setting contents");
	if (!doc.setContent(&file))
	//if (!doc.setContent(s))
	{
		file.close();
		qDebug( "%s %d : Invalid XML %s", __FILE__, __LINE__, qPrintable(fileName) );
		return;
	}
	//qDebug("load file end");
	file.close();
	
	loadDocument( doc );
	emit newContentAvaialable();
#endif
}

void dcfFile::loadDocument( QDomDocument doc )
{
	QDomNode dcf_head;

	dcf_head	= doc.elementsByTagName("DCF").item(0);
	reference	= dcf_head.attributes().namedItem("ref").nodeValue();
	icon		= dcf_head.attributes().namedItem("icon").nodeValue();
	imageDir	= dcf_head.attributes().namedItem("imagedir").nodeValue();
	title		= dcf_head.attributes().namedItem("title").nodeValue();

	for( uint i=0; i< dcf_head.childNodes().length(); i++ )
		loadSection( dcf_head.childNodes().item(i) );
}

void dcfFile::loadSection( QDomNode node )
{
	QDomNode  n2 = node.firstChildElement("section");
	
	while (!n2.isNull())
	{
		dcfSection section;
		section.reference	= n2.attributes().namedItem("ref").nodeValue();
		section.title		= n2.attributes().namedItem("title").nodeValue();
		section.file		= this;
		
		//qDebug() << "new section" << section.reference << ":" << section.title;
		sections << section;
		n2 = n2.nextSiblingElement("section");
	}
}

bool dcfFile::containsPage( QString s )
{
	foreach( dcfSection section, sections )
	{
		if (section.reference == s)
			return true;
	}
	return false;
}

void dcfFile::fileLoaded()
{
	delete loadingThread;
	loadingThread = NULL;
	emit newContentAvaialable();
}
