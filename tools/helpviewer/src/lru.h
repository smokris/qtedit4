#ifndef LRU_H
#define LRU_H

#include <QHash>
#include <QLinkedList>

class QString;
class QStringList;

namespace HelpViewer
{

struct LRU_item
{
	QString name;
	int age;
	int value;
};

typedef QLinkedList<LRU_item> LRU_List;
//typedef 

class LRU  
{
public:
	LRU();
	void touchItem( QString name );
	LRU_List getTop( int n = 5 );
	void print_list();
private:
	LRU_List items;
};


} // end of namespace

#endif
