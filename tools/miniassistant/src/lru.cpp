#include "lru.h"
#include <QStringList>

namespace MiniAssistantInt {

/**
\struct LRU_item
\brief An entry in the LRU table

Each item in the LRU table represents a visit to a page. The visit has a location, a date, 
and the amount of "time" that passed since it was first created.

Note that time is represented here in "ticks", each tick happens when a new
item is added to the LRU table.

\todo add a title to each item. this will help maintain the links in display (you will see the title of the pages, and not the file)
*/


/**
\class LRU
\brief List of least recently used pages

The LRU table (least recently used) is a sorted list of pages that the user visited. You can "touch" to existing ones, 
the you can query for the top list of items.

The list is ordered in this way:
 - first by count: the most visited items will be higher on the list
 - then by age: if 2 items will have the same count, the newer will be hight on the list.

The rational for the second rule is: imagine you visit pageA 50 times, then go, and visit some random pages. 
Eventually you visit pageB 50 times as well. You would expect that pageB will be higher on the list
then pageB, since this is what you are looking for \b now.
*/

/**
\var LRU::items
\brief thge list of itmes.
The list of items available on the table. If an item is here, it means it was accessed once.

*/

/**
\brief Default constuctor

Constructs an LRU class.
*/
LRU::LRU()
{
	// TODO how to compile an empty constuctor
}

/**
\brief Reset the LRU

This method will remove all itmes from the list, and zero the statistics.
*/
void LRU::reset()
{
	items.clear();
}

/**
\brief mark  avisit to a page
\param name the name of the item which was visited

This will mark a visit to a page. If that page has not been visited, a new entry will be 
added to the list. If the page has been visited the count will be increase. and then the age 
of all visits is incremented.

If an item was available (the visit is now a new visit) it's location is updated 
(IE: it's pushed up as much as possible). At the end of this function, the list is
always ordered.

\see updateItemLocation()
*/
void LRU::touchItem( QString name )
{
	bool found = false;
	
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
}

/**
\brief Get the list of most visited pages
\param n the numer of items to get

This function will return the most visited pages. The \n n parameter
(which defaults to 5) controlls the list size.

If you asked for more items then are available (you asked 5 items, but only 3 pages have 
been listed) the returned list will contain only the amount of visites (in this example 
you will get a list of 3 pages) 
*/
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

/**
\brief Debug function - to be removed soon

This function prints the internal status of the LRU. To be removed soon.

\todo remove this function soon
*/
void LRU::print_list()
{
	LRU_List::iterator i;

	qDebug("list begin");
	for (i = items.begin(); i != items.end(); ++i)
	{
		qDebug( "\t%s - %d hits, age=%d", qPrintable((*i).name), (*i).value, (*i).age );
	}
	qDebug("list end");
}


/**
\brief Update the location of an item

This function will get the item \b i as up as possible in the list. It's called
when "touching" an item.
*/
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
