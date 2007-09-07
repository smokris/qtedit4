#include "lru.h"
#include <QStringList>

namespace HelpViewer{

LRU::LRU()
{
	// TODO
}

void LRU::touchItem( QString name )
{
	bool found = false;
	//int location = 0;
	
	// search for the item
	LRU_List::iterator i;
	for (i = items.begin(); i != items.end(); i++)
	{
		if ((*i).name == name)
		{
			(*i).value ++;
			found = true;
			updateItemLocation( i );
			break;
		}
	}
	
	
	// if not found, just push it to the back
	if (!found)
	{

		LRU_item item;
		item.name = name;
		item.age = 1;
		item.value = 1;
		items.push_back(item);
		
		// increase the age of all other items
		for ( i = items.begin(); i != items.end(); i++)
			(*i).age++;
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
		qDebug( "\t%s - %d hits, age=%d", qPrintable((*i).name), (*i).value, (*i).age );
	}
	qDebug("list end");
}

void LRU::updateItemLocation( LRU_List::iterator i )
{
	
	LRU_List::iterator	p;
	LRU_item		item;

	while (i != items.begin()) 
	{
		p = i - 1;
		
		// easier to maintain this inside the loop that
		// in the condition of the loop, i know sad programming :)
		if ( (*i).value < (*p).value )
			break;
		else if ( (*i).value == (*p).value )
			if ( (*i).age >= (*p).age )
				break;
		
		// do the dirty job
		item = (*p);
		i = items.erase(p);
		i = items.insert( i+1, item );
		i --;
	}
}

} // end of namespace
