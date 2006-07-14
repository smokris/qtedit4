#ifndef __KATE_ITEM_DATA_MANAGER__
#define __KATE_ITEM_DATA_MANAGER__

#include <QDomDocument>
#include <QString>
#include <QList>

#include "kateitemdata.h"

class QsvColorDefFactory
{
public:
	QsvColorDefFactory();
	QsvColorDefFactory( QDomDocument doc );
	QsvColorDefFactory( QString fileName );
	virtual ~QsvColorDefFactory();

	QsvColorDef getColorDef( QString name );
	bool load( QDomDocument doc );
	bool load( QString fileName );
private:
	QList<QsvColorDef> colorDefs;
};

//typedef QsvColorDefFactory kateItemDataManager;

#endif // __KATE_ITEM_DATA_MANAGER__
