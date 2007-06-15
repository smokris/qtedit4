#ifndef NO_DEBUG_CLASS

#include <QString>
#include <QFile>
#include <QtDebug>

#include "debug_class.h"

// public + static parts
DebugClass* DebugClass::instance()
{
	if (!my_instance)
		my_instance = new DebugClass;
		
	return my_instance;
}

void DebugClass::verbose( QString s, int level )
{
	DebugClass::instance()->log_verbose( s,level );
}

void DebugClass::debug( QString s, int level )
{
	DebugClass::instance()->log_debug( s,level );
}

void DebugClass::setDebug( int n )
{
	debug_level = n;
}

void DebugClass::setVerbose( int n )
{
	verbose_level = n;
}
 
void DebugClass::log_verbose( QString s, int level )
{
	if (level>verbose_level)
		return;
		
	QString s1;
	s1 = QString("verbose (%1)\t%2").arg(level).arg(s);
	
	if (!debug_file.isOpen())
		qDebug( qPrintable(s1) );	
	else
		stream << s1 << "\n";
}

void DebugClass::log_debug( QString s, int level  )
{
	if (level>debug_level)
		return;

	QString s1;
	s1 = QString("debug   (%1)\t%2").arg(level).arg(s);
	
	if (!debug_file.isOpen())
		qDebug( qPrintable(s1) );	
	else
		stream << s1 << "\n";
}

void DebugClass::set_log_file( QString s )
{
	if (debug_file.isOpen())
		debug_file.close();
	
	if (!s.isEmpty())
	{
		debug_file.setFileName( s );
		debug_file.open( QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text );		
		if (!debug_file.isOpen())
			qDebug() << __FILE__ << ":" << __LINE__ << ": could not open file " << s;
		stream.setDevice( &debug_file );
	}
}

void DebugClass::flush()
{
	if ( !debug_file.isOpen() )
		return;
		
	debug_file.flush();
	stream.flush();
}

// private parts
DebugClass* DebugClass::my_instance = 0;

DebugClass::DebugClass()
{
	debug_level	= 0;
	verbose_level	= 0;
	stream.setDevice( &debug_file );
}

DebugClass::~DebugClass()
{
	qDebug("sssssss");
	debug_file.flush();
	stream.flush();
	debug_file.close();
}

#endif // NO_DEBUG_CLASS
