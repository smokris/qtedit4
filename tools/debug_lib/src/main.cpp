/**
 * \file main.cpp
 * \brief Demostration for using DebugClass
 * \author Diego Iastrubni (elcuco@kde.org)
 * license Modified BSD license
 */
 
#include <iostream>
#include <stdlib.h>

#include <QDebug>
#include <QString>
#include <QProcess>

#include "debug_class.h"

using namespace std;

float factorial( float );

int main()
{
/*	use system environment to read debug/verbose settings */
	bool b = false;
	int i = 0;
	QString s;

	i = QString(getenv("DEBUG_LEVEL")).toInt( &b );
	if (b) 
		DEBUG_SET_DEBUG( i );
	i = QString(getenv("VERBOSE_LEVEL")).toInt( &b );
	if (b) 
		DEBUG_SET_VERBOSE( i );
	
	DEBUG_LOG_FILE( getenv("DEBUG_FILE") );

/*	otherwise, use hardcoded defaults */
#if 0
	DEBUG_LOG_FILE("test.log");
	DEBUG_SET_DEBUG( 2 );
	DEBUG_SET_VERBOSE( 2 );
#endif	

/*	real functional code */
	cout << "Factorial of 17 is " << factorial(10) << endl;
	DEBUG_FLUSH();
	return 0;
}

float factorial( float f )
{
	DEBUG("entering", 1);
	
	if (f==0) {		
		VERBOSE("called with 0", 1);
		return 1;
	}
	else {
		VERBOSE( QString("recursive call with value %1").arg(f), 1);
		float d = f*factorial(f-1);
		DEBUG( QString("this iteration value: %1").arg(d), 2);
		return d;
	}
}
