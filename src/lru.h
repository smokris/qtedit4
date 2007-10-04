/**
 * \file lru.h
 * \brief Definition of the lru class used in the help browser
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#ifndef LRU_H
#define LRU_H

#include <QHash>
#include <QLinkedList>

class QString;
class QStringList;

namespace MiniAssistantInt
{

struct LRU_item
{
	QString name;
	int age;
	int value;
};

typedef QLinkedList<LRU_item> LRU_List;

class LRU  
{
public:
	LRU();
	void reset();
	void touchItem( QString name );
	LRU_List getTop( int n = 5 );
	void print_list();
private:
	void updateItemLocation( LRU_List::iterator i );
	LRU_List items;
};


} // end of namespace

#endif
