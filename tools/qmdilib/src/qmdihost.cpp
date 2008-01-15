/**
 * \file qmdihost.cpp
 * \brief Implementation of the qmdi host class
 * \author Diego Iastrubni (diegoiast@gmail.com)
 * License LGPL
 * \see qmdiHost
 */
 
// $Id$ 

#include <QMainWindow>
#include "qmdihost.h"
#include "qmdiclient.h"

/**
 * \class qmdiHost 
 * \brief The class which contain the menus and toolbars defined by qmdiClient
 *
 * Every time a user selects a new widget in the MDI server (for example qmdiTabWidget), the
 * server will try and ask the host to modify the menus. The interface in which the QTabWidget
 * and QMainWindow talk is this class.
 * 
 * Generally speaking, you don't have to know much of this class, only inherit it in your
 * main windows.
 */

/**
 * \var qmdiHost::menus
 * \brief the default menus defined in this MDI host
 *
 * When you construct a window which derives this class,
 * you cannot define the menus the way which is dictated by
 * Qt. Instead you must follow the code guidelines provided by
 * this library.
 *
 * The menus defined in this class, will be displayed by default
 * on the MDI host. It is a wise idea to define the order of the
 * menus on your application on the initialization of the application,
 * since there is no way of changing the order of the menus later
 * on.
 *
 * \code
 * menus["&File"];
 * menus["&Edit"];
 * menus["&Help"];
 * \endcode
 *
 * \see \ref small_tutor
 * \see qmdiServer
 * \see qmdiActionGroupList
 */

/**
 * \var qmdiHost::toolbars
 * \brief the default toolbars defined in this MDI host
 *
 * When you construct a window which derives this class,
 * you cannot define the toolbars the way which is dictated by
 * Qt. Instead you must follow the code guidelines provided by
 * this library.
 *
 * The toolbars defined in this class, will be displayed by default
 * on the MDI host.
 *
 * \see \ref small_tutor
 * \see qmdiServer
 * \see qmdiActionGroupList
 */

/**
 * \var qmdiHost::toolBarList
 * \brief internal list of available toolbars
 *
 * As Qt4 does not provide an interface for listing the available
 * toolbars, the list is maintained as a separate list.
 *
 * You should usually not use this list directly.
 */
 
qmdiHost::qmdiHost()
{
	toolBarList = NULL;
}

qmdiHost::~qmdiHost()
{
	delete toolBarList;
}

/**
 * \brief update the toolbars and menus
 * \param window the window to update
 * 
 * This function generates the menubar and toolbars from
 * the toolbar and menus definitions. 
 * 
 * You should call this method after every time you modify the menus
 * or structures.
 * 
 * The parameter window should be generally \b this , it's passed on
 * as a parameter, since qmdiHost cannot dynamic_cast it self to an 
 * QObject (this just does not work). On the other hand, this can give
 * you more freedom, as you do not have to derive the main window 
 * also from qmdiHost, and the host can be a separate object.
 * 
 * Since version 0.0.4, the \b window parameter is optional. This method
 * will try and see it \b this is a QMainWindow, and then update itself.
 * 
 * \see qmdiActionGroupList
 */
void qmdiHost::updateGUI( QMainWindow *window )
{
	// if passed NULL, lets try another sanity check
	if (window == NULL)
		window = dynamic_cast<QMainWindow*>(this);

	if (window == NULL)
	{
		qDebug("%s - warning, no QMainWindow specified", __FUNCTION__ );
		return;
	}
		
	window->setUpdatesEnabled(false);
	toolBarList = toolbars.updateToolBar( toolBarList, window );
	menus.updateMenu( window->menuBar() );
	window->setUpdatesEnabled(true);
}

/**
 * \brief merge the toolbars and menus of another MDI client
 * \param client the client to be merged
 *
 * This function is used to merge the toolbars and contents
 * of the MDI client to be merged into this client. The menus
 * and toolbars of the host will be appended to the end
 * of the menus and toolbars of this MDI host.
 *
 * After a call to this function, you should manually call
 * updateGUI
 *
 * \see updateGUI
 * \see unmergeClient
 */
void qmdiHost::mergeClient( qmdiClient *client )
{
	if (client == NULL)
		return;
		
	menus.mergeGroupList( &client->menus );
	toolbars.mergeGroupList( &client->toolbars );
}

/**
 * \brief merge the toolbars and menus of another MDI client
 * \param client the client to be merged
 *
 * This function is used to un-merge the toolbars and contents
 * of the MDI client to be un-merged into this client. The menus
 * and toolbars of host will be updated, and all the entries
 * defined in the client will be removed.
 *
 * After a call to this function, you should manually call
 * updateGUI
 *
 * \see updateGUI
 * \see mergeClient
 */
void qmdiHost::unmergeClient( qmdiClient *client )
{
	if (client == NULL)
		return;
		
	menus.unmergeGroupList( &client->menus );
	toolbars.unmergeGroupList( &client->toolbars );
}
