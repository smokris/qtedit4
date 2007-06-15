#ifndef __DEBUG_CLASS_H__
#define __DEBUG_CLASS_H__

#ifndef NO_DEBUG_CLASS

class QString;
#include <QFile>

class DebugClass
{
public:
// static functions
	static DebugClass* instance();
	static void verbose( QString, int level=1 );
	static void debug( QString, int level=1 );
	
// non static functions
	void setDebug( int );
	void setVerbose( int );
	void log_verbose( QString s, int level=1 );
	void log_debug( QString, int level=1 );
	void set_log_file( QString s );
	void flush();
	
private:
	DebugClass();
	~DebugClass();

	static DebugClass *my_instance;
	int debug_level;
	int verbose_level;
	QFile debug_file;
	QTextStream stream;
};

#define DEBUG(s,i) \
	DebugClass::debug( QString("%1 %2 - %3").arg(__FILE__).arg(__LINE__).arg(s), i )
 
#define VERBOSE(s,i) \
	DebugClass::verbose( QString("%1 %2 - %3").arg(__FILE__).arg(__LINE__).arg(s), i )

#define DEBUG_LOG_FILE(s)\
	DebugClass::instance()->set_log_file(s)

#define DEBUG_SET_DEBUG(i)\
	DebugClass::instance()->setDebug(i)
	
#define DEBUG_SET_VERBOSE(i)\
	DebugClass::instance()->setVerbose(i)

#define DEBUG_FLUSH() \
	DebugClass::instance()->flush()

#else 	// NO_DEBUG_CLASS

// stub functions, for code compability
// when compiled using -DNO_DEBUG_CLASS
#define DEBUG(s,i)
#define VERBOSE(s,i) 
#define DEBUG_LOG_FILE(s)
#define DEBUG_SET_DEBUG(i)	
#define DEBUG_SET_VERBOSE(i)
#define DEBUG_FLUSH()

#endif	// NO_DEBUG_CLASS

#endif // __DEBUG_CLASS_H__
