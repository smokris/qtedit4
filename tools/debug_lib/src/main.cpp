#include <IOStream>

#include <QDebug>
#include <QString>

#include "debug_class.h"

using namespace std;

float factorial( float );

int main()
{
	//DEBUG_LOG_FILE("test.log");
	DEBUG_SET_DEBUG( 2 );
	DEBUG_SET_VERBOSE( 2 );
	
	cout << "Factorial of 17 is " << factorial(10);
	//DebugClass::instance()->flush();
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
