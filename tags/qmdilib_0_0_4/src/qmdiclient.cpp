/**
 * \file qmdiclient.cpp
 * \brief Implementation of the qmdi client class
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 * \see qmdiClient
 */

// $Id$ 

#include <QObject>
#include <QWidget>
#include "qmdiclient.h"
#include "qmdiserver.h"

/**
 * \class qmdiClient
 * \brief Interface for menus and toolbars
 *
 * If you want to use qmdilib, every one of your widgets must inherit
 * qmdiClient and define their own menus using this class. Any widget
 * which will be inserted into a qmdiServer and does not inherit qmdiClient
 * will not show special menus and toolbars.
 * 
 * On the documentation of qmdilib, any widget inserted into a qmdiServer
 * that inherits qmdiClient will be called \b MDI \b client. In the following
 * example the class \b newclient is a \b MDI \b client.
 * 
 * \code
 * class newclient: public QTextEdit, public qmdiClient
 * {
 *     ...
 * };
 * \endcode
 * 
 * Defining menus and toolbars is easy:
 * 
 * \code
 * newclient::newclient()
 * {
 * 	// some code ...
 * 
 *	menus["&File"]->addAction( actionNew );
 *	menus["&File"]->addSeparator();
 *	menus["&File"]->addAction( actionQuit );
 * 
 * 	toolbars["General"]->addAction( actionNew );
 * }
 *
 * \endcode
 * 
 * You will have also to insert this MDI client into an MDI server
 * (for example qmdiTabWidget). The menus and toolbars will be merged 
 * automatically for you every time you widget is selected on the qmdiServer.
 * 
 * \see qmdiActionGroupList
 * \see qmdiTabWidget
 */

qmdiClient::qmdiClient()
{
	mdiServer = NULL;
}

qmdiClient::~qmdiClient()
{
	if (mdiServer != NULL)
		mdiServer->deleteClient( this );

	mdiServer = NULL;
}

/**
 * \var qmdiClient::name
 * \brief The name of the MDI client
 * 
 * This property defines the name of the MDI client. The \b name
 * is used by qmdiTabWidget for setting the tab name, when inserting 
 * the widget into the tab widget.
 */


/**
 * \var qmdiClient::fileName
 * \brief The file opened by this MDI client
 * 
 * This property defines the file name which is opened by the MDI client.
 * The file name is not really used inside \b qmdilib , but it's left 
 * for usage outside of library. The \b name property should reflect
 * this property, but it's not really needed.
 */

/**
 * \var qmdiClient::menus
 * \brief The list of menus defined by this MDI client
 * 
 * The list of menus this MDI client defines, and will be merged
 * each time the MDI client is showed on screen.
 * 
 * The syntax for defining menus is:
 * 
 * \code
 * menus->addAction( actionNew );
 * menus->addAction( actionOpen );
 * menus->addAction( actionSave );
 * menus->addSeparator();
 * menus->addAction( actionQuit );
 * \endcode
 * 
 * \see qmdiClient::toolbars
 */

/**
 * \var qmdiClient::toolbars
 * \brief The list of toolbars defined by this MDI client
 * 
 * The list of toolbars this MDI client defines, and will be merged
 * each time the MDI client is showed on screen.
 * 
 * The syntax for defining toolbars is:
 * 
 * \code
 * toolbars->addAction( actionNew );
 * toolbars->addSeparator();
 * toolbars->addAction( actionOpen );
 * toolbars->addAction( actionSave );
 * \endcode
 * 
 * \see qmdiClient::menus
 */

/**
 * \var qmdiClient::mdiServer
 * \brief A pointer to the MDI server in which this client is inserted.
 * 
 * When the MDI client is inserted into a MDI server, this property
 * is set to point to the corresponding MDI server. This property is
 * read only, and you should not assign value to it.
 * 
 * You can use this property to insert new clients into the server,
 * from this client.
 * 
 * \see qmdiServer
 */

/**
 * \brief close the MDI client
 * \return true if the widget is closed after this call
 * 
 * If you want to delete MDI clients from your MDI server, sometimes
 * just deleting this client is not the smart move. For example if the
 * client contains an user editable document, the user should be asked
 * to save his changes, and maybe even abort.
 *
 * By default this function does:
 *  - calls canCloseClient to find out if it's OK to closeClient()
 *  - if the function returns true, it will try to cast the MDI client to a
 *    QObject, calls deleteLater() and returns true and the object 
 *    will be deleted afterwards.
 *  - if cast failed or canCloseClient() returned false, a negative value
 *    is returned, and the object is not destructed.
 * 
 * This means that for read only clients you can leave the default. On R/W
 * clients which derive QObject, you will have to override canCloseClient().
 * If your derived class does not derive QObject, you will need to overide this
 * function as well.
 *
 * TODO update documentation
 *
 * \see canCloseClient()
 * \see QObject::deleteLater()
 */
bool qmdiClient::closeClient()
{
	if (canCloseClient())
	{
		QObject *o = dynamic_cast<QObject*>(this);

		if (o)
		{
			QWidget *w = qobject_cast<QWidget*>(o);
			
			if (w)
				w->hide();
			o->deleteLater();
		}
		return true;
	}
	else
		return false;
}

/**
 * \brief check if the MDI client is valid for closing
 * \return true is it is OK to destruct the MDI client
 *
 * This function should return true if the MDI client is safe for
 * removal from the MDI client. Clients are valid for removal usually
 * if no changes have been made, or the user accepted to save then.
 *
 * You can define your own logic by re-implementing this function
 * in your derived classes.
 * 
 * \see closeClient()
 */
bool qmdiClient::canCloseClient()
{
	return true;
}