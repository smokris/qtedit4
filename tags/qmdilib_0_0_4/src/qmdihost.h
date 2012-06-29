#ifndef __QMDI_HOST_H__
#define __QMDI_HOST_H__

#include "actiongrouplist.h"

/**
 * \file qmdihost.h
 * \brief Definition of the qmdi host class
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 * \see qmdiHost
 */
 
// $Id$ 

class QMainWindow;
class qmdiClient;

class qmdiHost
{
public:
	qmdiHost();
	virtual ~qmdiHost();
	
	qmdiActionGroupList menus;
	qmdiActionGroupList toolbars;

	void updateGUI( QMainWindow *window = NULL );
	void mergeClient( qmdiClient *client );
	void unmergeClient( qmdiClient *client );
	
	QList<QToolBar*>* toolBarList;
};

#endif // __QMDI_CLIENT_H__