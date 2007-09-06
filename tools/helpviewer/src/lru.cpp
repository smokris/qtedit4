#include "lru.h"
#include <QStringList>

namespace HelpViewer{

LRU::LRU()
{
	// TODO
}

void LRU::touchItem( QString name )
{
	//qDebug("touching %s", qPrintable(name) );
	bool found = false;
	int location = 0;
	
	// search for the item
	LRU_List::iterator i;
	for (i = items.begin(); i != items.end(); ++i)
	{
		if ((*i).name == name)
		{
			LRU_List::iterator p = i - 1;
			(*i).value ++;
			found = true;
			
			// should be push back?
			if ( (*i).value > (*p).value )
			{
				LRU_item item = (*p);
				i = items.erase(p);
				//if (item.value != 3)
				items.insert( i+1, item );
			}
			
			break;
		}
		location++;
	}
	
	// if not found, just push it to the back
	if (!found)
	{
		LRU_item item;
		item.name = name;
		item.age = 1;
		item.value = 1;
		items.push_back(item);
	}
	
	//print_list();
}

LRU_List LRU::getTop( int n )
{
	LRU_List l;
	int count = 0;
	LRU_List::iterator i;
	
	for (i = items.begin(); (i != items.end()) && (count < n) ; ++i)
	{
		l << (*i);
		count ++;
	}
	
	return l;
}

void LRU::print_list()
{
	HelpViewer::LRU_List::iterator i;

	qDebug("list begin");
	for (i = items.begin(); i != items.end(); ++i)
	{
		qDebug( "\t%s - %d hits", qPrintable((*i).name), (*i).value );
	}
	qDebug("list end");
}

}
