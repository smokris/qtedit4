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

dcfFile::dcfFile(  )
{
	
}

void dcfFile::loadFile( QString  fileName )
{
	QDomDocument doc("language");
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug( "%s %d : Could not open %s", __FILE__, __LINE__, qPrintable(fileName) );
		return;
	}

	if (!doc.setContent(&file))
	{
		file.close();
		qDebug( "%s %d : Could not open %s", __FILE__, __LINE__, qPrintable(fileName) );
		return;
	}
	file.close();
	
	loadDocument( doc );
}

void dcfFile::loadDocument( QDomDocument doc )
{
#if 0
	QDomNodeList list, l, l2;
	QDomNode n;

	list		= doc.elementsByTagName("DCF");
	n		= list.item(0);
	reference	= n.attributes().namedItem("ref").nodeValue();
	icon		= n.attributes().namedItem("icon").nodeValue();
	imageDir	= n.attributes().namedItem("imagedir").nodeValue();
	title		= n.attributes().namedItem("title").nodeValue();

	list = doc.elementsByTagName("section");
	int size = list.size();
	for( int i=0; i<size; i++ )
	{
		dcfSection section;
		n = list.item(i);
		section.reference	= n.attributes().namedItem("ref").nodeValue();
		section.title		= n.attributes().namedItem("title").nodeValue();
		section.file		= this;
		
		//qDebug() << "new section" << section.reference << ":" << section.title;
		sections << section;
	}
#else
	QDomNodeList list, l, l2;
	QDomNode dcf_head,n,n2;

	list		= doc.elementsByTagName("DCF");
	dcf_head	= list.item(0);
	
	n		= dcf_head.childNodes().item(0);
	if (n.attributes().namedItem("title").nodeValue() != "Classes" )
	{
		qDebug("This dcf file does not denote the classes documentation");
		return;
	}
		
	reference	= dcf_head.attributes().namedItem("ref").nodeValue();
	icon		= dcf_head.attributes().namedItem("icon").nodeValue();
	imageDir	= dcf_head.attributes().namedItem("imagedir").nodeValue();
	title		= dcf_head.attributes().namedItem("title").nodeValue();

	n2		= 	n.firstChildElement("section");
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
#endif
}
