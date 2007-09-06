/**
 * \file dcffile.h
 * \brief Definition of the dcf file loader
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#ifndef DCFFILE_H
#define DCFFILE_H

#include <QList>
#include <QString>

class QDomDocument;
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

class dcfFile
{
public:
	dcfFile();

	void loadFile( QString fileName );
	void loadDocument( QDomDocument doc );
	
private:
	QString reference;
	QString icon;
	QString imageDir;
	QString title;	
	QList<dcfSection> sections;

	class dcfModel friend;
};

#endif
