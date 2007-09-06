#ifndef __QMDI_CLIENT_H__
#define __QMDI_CLIENT_H__

/**
 * \file qmdiclient.h
 * \brief Definition of the qmdi client class
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 * \see qmdiClient
 */
 
 // $Id$

#include "actiongrouplist.h"

class QString;
class qmdiServer;

class qmdiClient
{
public:
	qmdiClient();
	virtual ~qmdiClient();

	virtual bool closeClient();
	virtual bool canCloseClient();
	
	QString name;
	QString fileName;
	qmdiActionGroupList menus;
	qmdiActionGroupList toolbars;
// protected:
	qmdiServer *mdiServer;
};

#endif // __QMDI_CLIENT_H__