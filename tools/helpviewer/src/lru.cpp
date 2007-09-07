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
	int location = 0;
	
	// search for the item
	LRU_List::iterator i;
	for (i = items.begin(); i != items.end(); ++i)
	{
		if ((*i).name == name)
		{
			(*i).value ++;
			found = true;
			updateItemLocation( i );
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

void LRU::updateItemLocation( LRU_List::iterator i )
{
	LRU_List::iterator p = i - 1;
	LRU_item item = (*p);
	while ( ((*i).value >= (*p).value) && (i != items.begin()) )
	{
		/*
		if ( (*p).age > ( (*i).age ) )
			continue;*/
		item = (*p);
		
		i = items.erase(p);
		i = items.insert( i+1, item );
		print_list();

		p = i - 1;
	}
}

} // end of namespace
