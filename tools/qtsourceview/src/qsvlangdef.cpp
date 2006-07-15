/**
 * \file qsvlangdef.cpp
 * \brief Implementation of the language definition, and support structs
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 * \see qmdiActionGroup
 */

#include <QString>
#include <QStringList>
#include <QDomDocument>
#include <QDomNode>
#include <QFile>

#include <QtDebug>

#include "qsvlangdef.h"


/**
 * \file    qegtklangdef.cpp
 * \brief   Implementation of the QeGtkSourceViewLangDef
 * \author  Diego Iastrubni (elcuco@kde.org)
 * \date    6-1-2006
 * \
 */


/**
 * \class QeGtkSourceViewLangDef
 * \brief A container class for GtkSourceView syntax definitions
 * 
 * GtkSourceView is a text widget that extends the standard gtk+ 2.x text widget:
 * http://gtksourceview.sourceforge.net/
 * 
 * The syntax definitions used by that library are stored as XML files.
 * This class provides an OO way of loading those definitions, and later on
 * use them for syntax highlighter classes.
 * 
 */


/**
 * \brief Constructor for the QeGtkSourceViewLangDef class
 * 
 */
QsvLangDef::QsvLangDef( QDomDocument doc )
{
	load( doc );
};


/**
 * \brief Constructor for the QeGtkSourceViewLangDef class
 * 
 */
QsvLangDef::QsvLangDef( QString fileName )
{
	load( fileName );
};


/**
 * \brief Destructor for the QeGtkSourceViewLangDef class
 * 
 */
QsvLangDef::~QsvLangDef()
{
};


/**
 * \brief Loads a language definition from an XML file stored on the disk
 * \param fileName The file from which the synatx should be loads
 * \return true on sucess, false on any error
 * 
 * 
 */
bool QsvLangDef::load( QString fileName )
{
	QDomDocument doc("language");
	QFile file(fileName);
	QString s;

	if (!file.open(QIODevice::ReadOnly))
	{
#ifdef	__DEBUG_LANG_DEF_LOAD__
		qDebug( "%s %d : Could not open %s", __FILE__, __LINE__, qPrintable(fileName) );
#endif		
		return false;
	}

	if (!doc.setContent(&file))
	{
		file.close();
#ifdef	__DEBUG_LANG_DEG_LOAD__
		qDebug( "%s %d : Could not open %s", __FILE__, __LINE__, qPrintable(fileName) );
#endif
		return false;
	}
	file.close();

	return load( doc );
}

bool QsvLangDef::load( QDomDocument doc )
{
	QDomNodeList list, l;
	QDomNode n,m;

	// read information about this syntax highlight
	list = doc.elementsByTagName("language");
	n = list.item(0);

	// we assume that if the fist line is well decalred, everything else is cool
	if (! n.hasAttributes() ) 
		return false;

	name		= n.attributes().namedItem("_name").nodeValue();
	version		= n.attributes().namedItem("_version").nodeValue();
	section		= n.attributes().namedItem("_section").nodeValue();
	mimeTypes	= n.attributes().namedItem("mimetypes").nodeValue().split(QRegExp("[;,]"));
	extensions	= n.attributes().namedItem("extensions").nodeValue().split(";");

	// read the entities which define this language/syntax
	list = doc.elementsByTagName("escape-char");
	escapeChar = list.item(0).nodeValue();

	if (!loadLineComments( doc.elementsByTagName("line-comment") ))	
		return false;

	if (!loadStrings( doc.elementsByTagName("string") ))
		return false;

	if (!loadPatternItems( doc.elementsByTagName("pattern-item")))
		return false;

	if (!loadBlockComments( doc.elementsByTagName("block-comment"), blockCommentsDefs ))
		return false;

	if (!loadKeywordList( doc.elementsByTagName("keyword-list") ))
		return false;

	if (!loadBlockComments( doc.elementsByTagName("syntax-item"), syntaxItemDefs ))
		return false;

	return true;
};

bool	QsvLangDef::isTrue( QString s )
{
	bool b = false;

	s  = s.toLower();
	if (s == "true")	b = true;
	if (s == "yes")		b = true;
	if (s == "1")		b = true;
	
	return b;
}

bool	QsvLangDef::loadEntity(QDomNode node, QsvEntityDef &entity )
{
	try
	{
		entity.name	= node.attributes().namedItem("_name").nodeValue();
		entity.style	= node.attributes().namedItem("style").nodeValue();
		entity.type	= node.toElement().tagName();
	}
	catch( ... )
	{
		return false;
	}
	
	return true;
}

bool	QsvLangDef::loadLineComments( QDomNodeList nodes )
{
	QDomNode node;
	int i, size = nodes.size();

	for( i=0; i<size; i++ )
	{
		QsvEntityLineComment e;
		node = nodes.at( i );

		if (!loadEntity( node, e )) return false;
		e.start = node.toElement().elementsByTagName("start-regex").item(0).toElement().text();

		// WTF???
		e.start.replace( "\\\\", "\\" );
		
		lineCommentsDefs << e;
	}

	return true;
}

bool	QsvLangDef::loadStrings( QDomNodeList nodes )
{
	QDomNode node;
	QString s;
	int i, size = nodes.size();

	for( i=0; i<size; i++ )
	{
		QsvEntityString e;
		node = nodes.at( i );

		if (!loadEntity( node, e )) return false;

		e.atEOL      = isTrue( node.attributes().namedItem("end-at-line-end").nodeValue() );
		e.startRegex = node.toElement().elementsByTagName("start-regex").item(0).toElement().text();
		e.endRegex   = node.toElement().elementsByTagName("end-regex").item(0).toElement().text();

		// WTF???
		e.startRegex.replace( "\\\\", "\\" );
		e.endRegex.replace( "\\\\", "\\" );
		
		stringsDefs << e;
	}

	return true;
}

bool	QsvLangDef::loadPatternItems( QDomNodeList nodes )
{
	QDomNode node;
	int i, size = nodes.size();

	i = patternItems.size();

	for( i=0; i<size; i++ )
	{
		node = nodes.at( i );

		QsvEntityPatternItem e;
		if (!loadEntity( node, e )) return false;
		e.regex = node.toElement().elementsByTagName("regex").item(0).toElement().text();

		// WTF???
// 		e.regex.replace( "\\\\", "\\" );
		e.regex.replace( "\\n", "$" );
		
		patternItems << e;
	}

	return true;
}

bool	QsvLangDef::loadBlockComments( QDomNodeList nodes, QList<QsvEntityBlockComment> &list )
{
	QDomNode node;
	int i, size = nodes.size();

	i = list.size();

	for( i=0; i<size; i++ )
	{
		node = nodes.at( i );

		QsvEntityBlockComment e;
		if (!loadEntity( node, e )) return false;
		e.startRegex = node.toElement().elementsByTagName("start-regex").item(0).toElement().text();
		e.endRegex   = node.toElement().elementsByTagName("end-regex").item(0).toElement().text();

		// WTF???
		e.startRegex.replace( "\\\\", "\\" );
		e.endRegex.replace( "\\\\", "\\" );
		
		list << e;
	}

	return true;
}

bool	QsvLangDef::loadKeywordList( QDomNodeList nodes )
{
	QDomNodeList strs;
	QDomNode str;
	QString s;

	int i, size = nodes.size();
	int j;
	
	for( i=0; i<size; i++ )
	{
		QsvEntityKeywordList e;
		QDomNode node = nodes.at( i );
		
		if (!loadEntity( node, e )) return false;
		e.list.clear();

		e.caseSensitive               = isTrue( node.attributes().namedItem("case-sensitive").nodeValue() );
		e.matchEmptyStringAtBeginning = isTrue( node.attributes().namedItem("match-empty-string-at-beginning").nodeValue() );
		e.matchEmptyStringAtEnd       = isTrue( node.attributes().namedItem("match-empty-string-at-end").nodeValue() );
		e.startRegex                  = node.attributes().namedItem("beginning-regex").nodeValue();
		e.endRegex                    = node.attributes().namedItem("end-regex").nodeValue();

		// WTF???
		e.startRegex.replace( "\\\\", "\\" );
		e.endRegex.replace( "\\\\", "\\" );
		
		// read strings
		strs = node.toElement().elementsByTagName("keyword");
		for( j=0; j<strs.size(); j++ )
		{
			str = strs.item( j );
			e.list << str.toElement().text();
		}
		
		keywordListDefs << e;
	}	

	return true;
}
