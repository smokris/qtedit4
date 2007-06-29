/**
 * \file debug_class.cpp
 * \brief implementation of DebugClass and several helper functions
 * \author Diego Iastrubni (elcuco@kde.org)
 * license Modified BSD license
 */

#ifndef NO_DEBUG_CLASS

#include <QString>
#include <QFile>
#include <QtDebug>

#include "debug_class.h"

/**
 * \class DebugClass
 * \brief a class with debug facilities
 *
 * This class can be used for debugging. It contains the concept of channels (currently 2: debug and verbose)
 * and output levels. A program can ask the class to send information trough a channel, and in runtime
 * the output level of that channel will be tested.
 *
 * The idea is that when the developer is writing the application, he will put in strategic places
 * of his application marks, with debug information, or write more detailed information (verbosity).
 * Then on runtime, the user can change the output level for each one of the channels, individually,
 * and only if the output level if equal or lower, the text will be sent to the channel.
 * 
 * The output by default is done via qDebug, however if you specify a file, the output will be sent
 * that file and not stderr, which is not visible on graphical operating systems (like Windows).
 * 
 * The class is designed as a singleton, and you will have to query for it's instance before doing anything 
 * with it. However, there are wrappers for that, and you are encouraged to use those.
 */

/**
 * \brief get a pointer to the instance of this class
 *
 * This method will return an instance to the class. 
 * If no instance is created yet, it will create one.
 */
DebugClass* DebugClass::instance()
{
	if (!my_instance)
		my_instance = new DebugClass;
		
	return my_instance;
}

/**
 * \brief display a string to the verbose channel, using a specific output level
 * \param s the string to send to the channel
 * \param level the level to use for output
 *
 * Sends a string to the verbose channel, using a specific output level.
 * 
 * This is a static function, to ease the usage of the class, and will actually
 * ask the instance of the class to do the work.
 * 
 * \see log_verbose
 * \see set_log_file
 */
void DebugClass::verbose( QString s, int level )
{
	DebugClass::instance()->log_verbose( s,level );
}

/**
 * \brief display a string to the debug channel, using a specific output level
 * \param s the string to send to the channel
 * \param level the level to use for output
 *
 * Sends a string to the debug channel, using a specific output level.
 * 
 * This is a static function, to ease the usage of the class, and will actually
 * ask the instance of the class to do the work.
 * 
 * \see log_debug
 * \see set_log_file
 */
void DebugClass::debug( QString s, int level )
{
	DebugClass::instance()->log_debug( s,level );
}

/**
 * \brief set the default output level of the debug channel
 * \param n the new output level
 *
 * Set the output level of the debug channel to a new value. This affects
 * any new call to log_debug(): if the value is lower then the value of the 
 * level passed on that method, is lower then the one passed on this
 * method the string will not be logged.
 * 
 * \see log_debug
 * \see set_log_file
 */
void DebugClass::setDebug( int n )
{
	debug_level = n;
}

/**
 * \brief set the default output level of the verbose channel
 * \param n the new output level
 *
 * Set the output level of the verbose channel to a new value. This affects
 * any new call to log_verbose(): if the value is lower then the value of the 
 * level passed on that method, is lower then the one passed on this
 * method the string will not be logged.
 * 
 * \see log_verbose
 * \see set_log_file
 */
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
	debug_file.flush();
	stream.flush();
	debug_file.close();
}


/**
\def DEBUG (s, i)
\param s the text to send
\param i the output level
A simple wrapper to DebugClass::debug(), which will also display of the debug channel 
the file from which the call originates and the line number.

\def VERBOSE(s, i)
\param s the text to send
\param i the output level
A simple wrapper to DebugClass::verbose(), which will also display of the debug channel 
the file from which the call originates and the line number.

\def DEBUG_LOG_FILE(s)
\param s the new file name for the logs
This is a simple wrapper to DebugClass::instance()->set_log_file()

\def DEBUG_SET_DEBUG(i)	
\def DEBUG_SET_VERBOSE(i)

\def DEBUG_FLUSH()
This is a simple wrapper to DebugClass::instance()->flush()
*/

#endif // NO_DEBUG_CLASS
