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

namespace HelpViewer {

#define NEW_DCF_LOAD
#define ASYNC_FILE_LOADING

/**
\struct dcfReference
\brief Abstract definition of a reference node in a DCF file

This struct defines a single "reference" node in a DCF file.

\todo examples + documentation for dcfReference
*/


/** 
\class dcfFileLoadThread
\brief Worker thread for loading a DCF file

The dcfFile class can be compiled with asynchronous loading. The real load is done 
in a separate thread which is impemented as dcfFileLoadThread, and is executed when the run() method
is called.
*/

/**
\var dcfFileLoadThread::m_file
\brief The file to be loaded

A reference to the file to be loaded.
*/

/**
\var dcfFileLoadThread::m_fileName
\brief The file name of the DCF file to be loaded

A string representing the file name to be loaded. This must be
valid for a QFile to load it.
*/

/**
\brief Constructs the thread
\param f reference to the dcfFile class to load
\param fileName the dcf file to load 

This will setup the class for loading the file specified by \b fileName into the
class \b f class.

Note that the file is not opened in the constructor, but only when then
thread is being run.

\see dcfFileLoadThread::run()
*/
dcfFileLoadThread::dcfFileLoadThread( class dcfFile *f, QString fileName  )
{
	m_file = f;
	m_fileName = fileName;
}

/** 
\brief the thread implementation

When called, the fie is loaded from the disk. The contents is set to a
QDomDocument, and then passed to the m_file, to be precessed.

This function will display a warning using qDebug if it could not load the
file, or it's not a valid XML.

\see dcfFile::loadDocument()
*/
void dcfFileLoadThread::run()
{
	if (!m_file)
		return;
	
	QDomDocument doc("language");
	
	QFile file(m_fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug( "%s %d : Could not open %s", __FILE__, __LINE__, qPrintable(m_fileName) );
		return;
	}

#if 0
	QString s = file.readAll();
	if (!doc.setContent(s))
#else	
	if (!doc.setContent(&file))
#endif	
	{
		file.close();
		qDebug( "%s %d : Could not open %s", __FILE__, __LINE__, qPrintable(m_fileName) );
		return;
	}
	file.close();

	m_file->loadDocument( doc );
}


/**
\class dcfFile
\brief An abstract representation of a DCF file

A DCF file (abbrebiation of Document Content Format), is a XML structured file which contains definitions of 
help to be displatyed on Qt Assistant. This class can load the file and have it displayed on a QListView
by using a dcfModel.

For a complete documentation of this format see the "Customizing Qt Assistant" chapter in the Qt4 documentation,
which is available also in this link: http://doc.trolltech.com/4.3/assistant-manual.html#customizing-qt-assistant

\see dcfModel
*/

/**
\var dcfFile::reference
\brief The refernce of the top level node
*/

/**
\var dcfFile::icon
\brief The icon of which represents this file
*/

/**
\var dcfFile::imageDir
\brief The directory for searching images
*/

/**
\var dcfFile::title
\brief The title of this DCF file
*/

/**
\var dcfFile::references
\brief list of references available in this DCF file

A QList of dcfReference which contains all the references available in this 
dcfFile. This is used by the model,
*/

/**
\var dcfFile::loadingThread
\brief The reference to the working thread.

If the class is compiled with asynchronous loading, this class will be
used for doing the load of the DCF file.

\see dcfFile::loadFile()
\see dcfFileLoadThread
*/


/**
\brief Default constructor

Constructs a dcfFile class.
*/
dcfFile::dcfFile(  )
{
	
}

/**
\brief Load a file from the disk
\param fileName the file name to be loaded

This function will load the file passed as an argument from the disk. The loading
can be done in asynchronous mode (default), and synchronous mode.

When the file is compiled for asynchronous a new thread is created which will load
the file, and when the thread is over, a signal is emited which can be used to invalidate
the model (and update the display). 

Please note that if the code is not using the asynchronous loading the GUI of the application
might become frozen for several seconds.

To modify the loading mode, there is a define at the top of dcffile.cpp which controlls
this behaviour. The default is asynchronous loading, and you are \n not encouraged to modify
this default.

\todo Should this signal be emited aswell when the synchronous loading is defined...?

\see loadDocument()
\see dcfFileLoadThread
*/
void dcfFile::loadFile( QString fileName )
{
#ifdef ASYNC_FILE_LOADING
	loadingThread = new dcfFileLoadThread( this, fileName );
	connect( loadingThread, SIGNAL(finished()), this, SLOT(fileLoaded()));
	loadingThread->start();
#else
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
		qDebug( "%s %d : Invalid XML %s", __FILE__, __LINE__, qPrintable(fileName) );
		return;
	}
	file.close();
	
	loadDocument( doc );

// 	TODO: emit as well...?
// 	emit newContentAvaialable();
#endif
}

/**
\brief Load the references from the XML document
\param doc the XML document

When a file is loaded from the disk, the XML content is passed to this function,
which searches for the defined references in that XML. Each reference is stored
and can be displayed later on by the model.

\see dcfModel
\see loadFile()
*/
void dcfFile::loadDocument( QDomDocument doc )
{
	QDomNode dcf_head;

	dcf_head	= doc.elementsByTagName("DCF").item(0);
	reference	= dcf_head.attributes().namedItem("ref").nodeValue();
	icon		= dcf_head.attributes().namedItem("icon").nodeValue();
	imageDir	= dcf_head.attributes().namedItem("imagedir").nodeValue();
	title		= dcf_head.attributes().namedItem("title").nodeValue();

#ifdef NEW_DCF_LOAD
	loadSection( dcf_head );
#else
	for( uint i=0; i< dcf_head.childNodes().length(); i++ )
		loadSection( dcf_head.childNodes().item(i) );
#endif
}

/**
\brief Load a section of the documentation
\param node the node representing the section

When loading the document, each section is parsed by this method. The method 
will also call itseld recursively if it finds new sub-sections.
*/
#ifdef NEW_DCF_LOAD
void dcfFile::loadSection( QDomNode node )
{
	QDomNode n = node.firstChildElement("section");
	QDomNode n2;
	dcfReference section;
	
	while (!n.isNull())
	{
		section.reference	= n.attributes().namedItem("ref").nodeValue();
		section.title		= n.attributes().namedItem("title").nodeValue();
		section.file		= this;
		//references << section;

		n2 = n.firstChildElement("keyword");
		while (!n2.isNull())
		{
			section.reference	= n2.attributes().namedItem("ref").nodeValue();
			section.title		= n2.toElement().text();
			section.file		= this;
			
			if ( (!section.title.isEmpty()) && (!section.reference.isEmpty()) )
				references << section;
			
			n2 = n2.nextSiblingElement("keyword");
		}

		loadSection( n );
		n = n.nextSiblingElement("section");
	}
}
#else
void dcfFile::loadSection( QDomNode node )
{
	QDomNode  n2 = node.firstChildElement("section");
	QDomNode n;
	
	while (!n2.isNull())
	{
		dcfReference section;
		section.reference	= n2.attributes().namedItem("ref").nodeValue();
		section.title		= n2.attributes().namedItem("title").nodeValue();
		section.file		= this;
		references << section;

		n2 = n2.nextSiblingElement("section");
	}
}
#endif

/**
\brief Will return true if the page is available as a reference 
\param pageName the page to be tested.

The function will return true if there is a reference to this
page in the document loaded. 
*/
bool dcfFile::containsPage( QString pageName )
{
	foreach( dcfReference section, references )
	{
		if (section.reference == pageName)
			return true;
	}
	return false;
}

/**
\brief Called when the working thread ended

When loading the file in asynchronous mode, this slot will be called
when the loading thread is over. This slot will emit a new signal 
which will tell to the model that new data is available and it
should invalidate the display and display the new data.

\see loadFile()
\see newContentAvaialable
\see dcfFileLoadThread
*/
void dcfFile::fileLoaded()
{
	disconnect( loadingThread, SIGNAL(finished()), this, SLOT(fileLoaded()));
	delete loadingThread;
	loadingThread = NULL;
	emit newContentAvaialable();
}

/**
\fn dcfFile::newContentAvaialable()
\brief Emitted when new content is available

When loading the file in asynchronous mode, this signal is emited when there is new content
avaialable. 
*/

} // end of namespace
