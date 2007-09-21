/**
 * \file helpviewerimpl.cpp
 * \brief Implementation of the help viewer object
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#include <QWidget>
#include <QDockWidget>
#include <QApplication>
#include <QMainWindow>
#include <QLibraryInfo>
#include <QStringList>
#include <QUrl>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QKeyEvent>
#include <QCompleter>

#include <QDebug>

#include "lru.h"
#include "helpviewerimpl.h"
#include "dcffile.h"
#include "dcfmodel.h"

/**
\namespace HelpViewer
\brief Internal namespace

Internal namepace which ontains all the needed classes
needed by the help viewer. They are all considered to be
\b internal and you should not use them directly. 
*/

/**
\class HelpViewerImpl
\brief A mini-assistant to be emmbeded in applications

The mini-assistant is a small widget which will give you 
an experiece close to running a full assistant, but in a small 
embbeded widget, a dock window which contains a tab widget, with 2 tabs: 
 - a index which contains, a searchable list of items, a label
   which displays the mostly used items in the help browser (based uppn the 
   LRU class), and a list of recommended links 
 - a small help browser. 
 
This is the  only class that needs to be used in applications and all other 
classes are classified as internal, and are part of the namespace 'HelpViewer'.

To use this class, all you need to do is create in instace of it
\code
helpViewer = new HelpViewerImpl(this);
helpViewer->showContents();

// In this example we take the word under a QTextEdit and feed it to the browser
QTextCursor cursor = textEdit->textCursor();
cursor.select(QTextCursor::WordUnderCursor);
helpViewer->displayKeyword( cursor.selectedText() );
\endcode

The filtering in the index is done via a timer. Each time a user presses
a key, a timer is triggered, and when that timer times out, only then
the text is filtered and displayed on screen. This way if the user types
"too fast" on the keyboard, the list will be generated 2-3 times per second
instead of beeing re-generated on every keyboard click. This also makes the 
GUI more responsive as it gets frozen only maximum times per second.

\todo Class name needs to be modified to MiniAssistant
\todo currently the dcfFile which is loaded is hardcoded to $QTDIR/doc/html/qt.dcf, make this configurable
\todo HelpViewer namespace should be modified to MiniAssistantInternal

\see on_indexEditTimer_timeout()
\see loadFile()

*/

/**
\var HelpViewerImpl::m_showMaxTop
\brief The number or most viewed pages to be displayed
When updating the popular links this parameter defines
how much items to display (defaults is 5 items).

If you modify this parameter please call updatePopularLinks() manually.

\todo this is private, maybe make it public...? or add a function to modify this value?
\see updatePopularLinks()
*/

/**
\var HelpViewerImpl::m_dock_widget
\brief The widget seen on the dock widget

This is what you see inside the dock widget. This is a place holder for the GUI, which is 
designed in the designer. This widget will be inserted into the dock widget.

\see m_dock
*/

/**
\var HelpViewerImpl::m_dock
\brief The dock widget used for containing the main help viewer

The HelpViewerImpl is not a widget, it's a class which controlls the main
widget to be displayed o nscreen. This widget is currently a QDockWidget, 
which is represented by this variable.

The m_dock will contain the m_dock_widget.

\see m_dock_widget
*/

/**
\var HelpViewerImpl::m_locationCompleter
\brief The compltetion for the URL like browsing.

The help browser contains a small input line, for writing manually 
the classes you want to browse. This is the completer for that 
widget.

It uses the model for completition.

\see m_dcfModel
*/

/**
\var HelpViewerImpl::m_indexEditTimer
\brief Use to help filtering text

When the user presses the keyboard on the indexEdit
this timer will be triggered. The full documentation 
can be found in on_indexEdit_textEdited.

\todo remove this variable
\see on_indexEditTimer_timeout()
\see on_indexEdit_textEdited()
*/

/**
\var HelpViewerImpl::m_filterModel
\brief Filter the results on the indexListView

When the user wants to filter the list of classes, this class
will be used to filter the display in the indexListView.

\see m_dcfModel
*/

/**
\var HelpViewerImpl::m_dcfFile
\brief The help topics are loaded from this file

This variable contains all the help topics the user can choose to view.

\todo move to a list of dcf files instead of a single dcf file
\see m_dcfModel
\see HelpViewer::dcfFile
*/

/**
\var HelpViewerImpl::m_dcfModel
\brief Display the list of help topics available

This class will display on the list view the available topics.

\todo move to a model which knows how to deal with a list of DCF files

\see HelpViewer::dcfModel
\see HelpViewer::dcfFile
*/

/**
\var HelpViewerImpl::m_classesLRU
\brief The least recently used help pages

This contains the list of recently viewed pages.

\see updatePopularLinks()
\see HelpViewer::LRU
*/

/**
\brief Default constructor
\param parent the main window to attach the dock widget to

Will create the main GUI, and issue the connection of the slots
(slots are not autoconnected, since HelpViewerImpl is not a QWidget).

The constructor will also add some defaults to the LRU
- qabstractbutton.h
- qmenubar.h
- qtextedit.h
- qstring.h
- qwidget.h

The default size of the dock widget will be 1/6th of the parent's width
if it has a parent. The help browser is zoomed out once.

The \b parent parameter is very important, and should be set to the QMainWindow
in which you want the dock window to be attached. Even tough it defaults to "0"
you should \b never pass it "0", but a valid QMainWindow (and \b a QWidget)
*/
HelpViewerImpl::HelpViewerImpl( QWidget * parent ) 
	: QObject(parent)
{
	m_classesLRU = new HelpViewer::LRU;
	m_dock = new QDockWidget;
	m_dock_widget = new QWidget(m_dock);
	m_indexEditTimer = new QTimer;
	
	setupUi(m_dock_widget);
	connect( indexEdit, SIGNAL(textEdited(QString)), this, SLOT(on_indexEdit_textEdited(QString)));
	connect( helpBrowser, SIGNAL(sourceChanged(QUrl)), this, SLOT(on_helpBrowser_sourceChanged(QUrl)));
	connect( mainTab, SIGNAL(currentChanged(int)), this, SLOT(on_mainTab_currentChanged(int)));
	connect( helpSuggestions, SIGNAL(linkActivated(QString)), this, SLOT(on_helpSuggestions_linkActivated(QString)));
	connect( popularPages, SIGNAL(linkActivated(QString)), this, SLOT(on_popularPages_linkActivated(QString)));
	connect( indexListView, SIGNAL(activated(QModelIndex )), this, SLOT(on_indexListView_activated(QModelIndex)));
	connect( indexEdit, SIGNAL(returnPressed()), this, SLOT(on_indexEdit_returnPressed()));
	connect( locationBar, SIGNAL(returnPressed()), this, SLOT(on_locationBar_returnPressed()));
	connect( btnShowPage, SIGNAL(clicked(bool)), this, SLOT(on_btnShowPage_clicked(bool)));
	connect( m_indexEditTimer, SIGNAL(timeout()), this, SLOT(on_indexEditTimer_timeout()));
	
	m_dock->setWidget( m_dock_widget );
	if (parent)
		m_dock->resize( parent->width() / 6, parent->height() );
	updateWindowTitle();
	
	helpBrowser->setSearchPaths( QStringList( QLibraryInfo::location ( QLibraryInfo::DocumentationPath ) + "/html/" ) );
	helpBrowser->setSource( QUrl("index.html") );
	helpBrowser->zoomOut( 1 );
	
	indexEdit->installEventFilter( this );
	indexListView->setAlternatingRowColors( true );
	locationBar->hide();
	QTimer::singleShot( 0, this, SLOT(loadFile()));
	
	// default "recently used" items
	for( int i=0; i<2; i++ )
	{
		m_classesLRU->touchItem( "qabstractbutton.html" );
		m_classesLRU->touchItem( "qmenubar.html" );
		m_classesLRU->touchItem( "qtextedit.html" );
		m_classesLRU->touchItem( "qstring.html" );
		m_classesLRU->touchItem( "qwidget.html" );
	}
	m_showMaxTop = 5;
	updatePopularLinks();
}

/**
\brief Toggle the help dock window

Call this function if you want to display of hide the dock window:
 - If called when the help dock is displayed it will hide the help dock window
 - If the dock window is hidden, it will re-parent the dock (if needed) and display the dock window

This function will also focus the first tab and the search bar.

\see on_mainTab_currentChanged
*/
void HelpViewerImpl::toggleDock()
{
	if (m_dock->isVisible())
		m_dock->hide();
	else
	{
		if (m_dock->parent() == NULL)
		{
			QMainWindow *w = qobject_cast<QMainWindow*>(parent());
			m_dock->setParent( w );
			w->addDockWidget(Qt::RightDockWidgetArea, m_dock);
		}
		m_dock->show();
		m_dock->activateWindow();
		
		mainTab->setCurrentIndex( 0 );
		indexEdit->setFocus();
		indexEdit->activateWindow();
	}
}

/**
\brief Display the help contents, simply loads "index.html"

\todo should we remove it?
\todo make sure that the dock is visible
\todo load the main document of the DCF file
*/
void HelpViewerImpl::showContents()
{
	showPage("index.html");
}

/**
\brief Opens a page on the help browser.

This function does not try to detect if the file is available, 
and it does not add a "html" extension. It will just pass the URL
to the help browser.

This function will also ensure that the dock is visible.

\see toggleDock
*/
void HelpViewerImpl::showPage( QString page )
{
	if (!m_dock->isVisible())
		toggleDock();
	
	helpBrowser->setSource( QUrl(page) );
}

/**
\brief Open a documentation page which corresponds to a keyword
\param keyword a keyword to show on the help browser
\return true if the keyword is displayed

Will try and open a page, which corresponds to the
\b keyword specified in the arguments. 

The URL is constructed by making that string lower 
case and appending the postfix ".html".
If the page was contained in the dcfFile documentation
the function will return true, on any other occasion
it will return false
*/
bool HelpViewerImpl::displayKeyword( QString keyword )
{
	keyword = keyword.toLower() + ".html";
	if (m_dcfFile->containsPage(keyword))
	{
		helpBrowser->setSource( QUrl(keyword) );

		if (!m_dock->isVisible())
			toggleDock();
		mainTab->setCurrentIndex( 1 );
		helpBrowser->setFocus();
		m_dock->activateWindow();
		return true;
	}
	return false;
}

/**
\brief Update the window title of the dock window

Ensure that the dock window contains the correct title. The title
needs to be udpated when the user navigates to a new page in the 
help browser or when a new page is displayed on the tab widget.

The title is "Help Browser", and when the help browser is displayed
the document title of the current page is appended to the window
title.
*/
void HelpViewerImpl::updateWindowTitle()
{
	QString s = tr("Help Browser");
	switch (mainTab->currentIndex())
	{
		case 0:
			m_dock->setWindowTitle( s );
			break;
		case 1:
			m_dock->setWindowTitle( s + " - " + helpBrowser->documentTitle() );
			break;
	}
}

/**
\brief Update the content of the popular links label

When the user browses a new page, the values of the LRU change, and the
top 5 times to be displayed need to be re-calculated and the display
must be updated. This function re-populates the label, and adds
links to the top 5 visited documents by the user.

The number of popular pages can be modified by setting a value to m_showMaxTop
which defaults to 5

\see m_showMaxTop
*/
void HelpViewerImpl::updatePopularLinks()
{
	QString s = tr("Popular pages (based on previous searches):") + "<ul>";
	HelpViewer::LRU_List l = m_classesLRU->getTop(m_showMaxTop);
	HelpViewer::LRU_List::iterator i;

	for (i = l.begin(); i != l.end(); ++i)
	{
		// TODO modify the popular links text for release
#if 0		
		s += QString("<li><a href='%1'>%2</a></li>").arg((*i).name).arg((*i).name);
#else
		s +=QString( "<li><a href='%1'>%2 (%3 hits)</a></li>").arg((*i).name).arg((*i).name).arg((*i).value );
#endif
	}
	s+= "</ul><br>";
	popularPages->setText( s );
}

/**
\brief Called when the btnShowPage button is clicked.

The button is location on top of the help browser, and will display or hide
a line text edit which will give the user a way to input URLs to browse.
*/
void HelpViewerImpl::on_btnShowPage_clicked(bool b)
{
	locationBar->setVisible( b );
	if (b)
	{
		locationBar->setFocus();
		locationBar->selectAll();
	}
}

/**
\brief Called when "enter" is pressed on the location bar.

This function will try and load the page written in the location bar
and set the focus to the help browser.
*/
void HelpViewerImpl::on_locationBar_returnPressed()
{
	helpBrowser->setSource( locationBar->text() );
	helpBrowser->setFocus();
}

/**
\brief Called when "enter" is pressed on the index edit

This function will open the page listed on the list box of the
Index tab in the help browser, and then set the current page
the browser tab and enforce the focus on the help browser.
*/
void HelpViewerImpl::on_indexEdit_returnPressed()
{
	QModelIndex index = indexListView->currentIndex();
	
	if (!index.isValid())
		return;
	
	mainTab->setCurrentIndex( 1 );
	helpBrowser->setSource( QUrl( index.data(Qt::StatusTipRole).toString() ) );
	helpBrowser->setFocus();
}

/**
\brief  Called when the user modifies the text of the index edit

Update the filter proxy when the user presses the keyboard and starts searching something. 
It will issue a timer in 450msec to do the actualy change. This way you can safely type
as fast as you want and the filtering will be done 2-3 times per second. Most programmers
can type 10 chars in less then a second, and this technique will update the filter only one
while typing and once when the class name has been written.

\todo the value of 450ms is hardcoded. Maybe a member variable?
\todo should the timer be stopped if runnning? this way the filter is done only when the user finishes typing
\todo is it necesaty to use a special timer function? QObject has enough support for setting timers
\todo code cleanup, no need to test for m_indexEditTimer, it's always available

\see on_indexEditTimer_timeout
*/
void HelpViewerImpl::on_indexEdit_textEdited(QString s)
{
	if (m_indexEditTimer)
	{
		if (m_indexEditTimer->isActive())
			//m_indexEditTimer->stop();
			return;
		m_indexEditTimer->start( 450 );
	}
	else
		m_filterModel->setFilterRegExp( s );
}

/**
\brief Called when the keypress timer is dome

When the user modifies the indexEdit a timer is issued, and only when the timer
ends this callback is generated. This function will update the filter regex for the
proxy model, and will update the list of calsses seen on screen.

\see on_indexEdit_textEdited
*/
void HelpViewerImpl::on_indexEditTimer_timeout()
{
	m_filterModel->setFilterRegExp( indexEdit->text() );
}


/**
\brief Called when the user double clicks an item in the list view

When the user clicks an item on the list view (the list of classes in the "Index" tab)
this callback is triggered. It will change the tab widget to the browser and display the
page which is described on that item.

\see dcfModel
\see dcfFile
*/
void HelpViewerImpl::on_indexListView_activated(QModelIndex index )
{
	mainTab->setCurrentIndex( 1 );
	helpBrowser->setSource( QUrl(index.data(Qt::StatusTipRole).toString() ) );
}

/**
\brief Called when the user pressed a link in the help suggestions label

The help suggestions label contains links to different pages in the documentation.
When the user clicks one of those, this function is triggered and the corresponding
page will be opened. The browser tab will be displayed and the focus will be set
to the help browser.
*/
void HelpViewerImpl::on_helpSuggestions_linkActivated(QString link)
{
	mainTab->setCurrentIndex( 1 );
	helpBrowser->setSource( QUrl(link) );
	helpBrowser->setFocus();
}

/**
\brief Called when the user pressed a link in the popular pages label

The help suggestions label contains links to different pages in the documentation.
When the user clicks one of those, this function is triggered and the corresponding
page will be opened. The browser tab will be displayed and the focus will be set
to the help browser.
*/
void HelpViewerImpl::on_popularPages_linkActivated(QString link)
{
	mainTab->setCurrentIndex( 1 );
	helpBrowser->setSource( QUrl(link) );
	helpBrowser->setFocus();
}

/**
\brief Called when the tab widget is changed

This function will update the title of the dock window. 

When the tab index is "1", the second tab page, the help browser will be focused,
unless the location bar is visible, in which case it will be focused.

When the tab index is "0", the first tab page, the popular links label will be 
updated and also set the focus to the index edit.

\see updatePopularLinks()
\see updateWindowTitle()
*/
void HelpViewerImpl::on_mainTab_currentChanged(int index)
{
	updateWindowTitle();
	
	// handle only when the index tab is showed
	if (index == 1)
	{
		if (locationBar->isVisible() )
			locationBar->setFocus();
		else
			helpBrowser->setFocus();
		return;
	}
		
	indexEdit->setFocus();
// 	indexEdit->activateWindow();
	updatePopularLinks();
}

/**
\brief Called when a new page is displayed on the help browser

This function will update the title of the dock window, update the LRU with
the new status, and set the new URL into the location bar.

\see updateWindowTitle()
*/
void HelpViewerImpl::on_helpBrowser_sourceChanged(QUrl u)
{
	updateWindowTitle();
	
	if (m_classesLRU)
		m_classesLRU->touchItem( u.path() );
	
	locationBar->setText( u.path() );
}

/**
\brief Load the default DCF file

Load the default file (qt.dcf) into the m_dcfFile and setup the
model for displaing it as needed.

\todo hardcoded file? bad bad bad...
*/
void HelpViewerImpl::loadFile()
{
	m_dcfFile = new HelpViewer::dcfFile;
	m_dcfFile->loadFile( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + "/html/qt.dcf" );
	//m_dcfFile->loadFile( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + "/html/assistant.dcf" );
	//m_dcfFile->loadFile( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + "/html/designer.dcf" );
	//m_dcfFile->loadFile( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + "/html/linguist.dcf" );
	//m_dcfFile->loadFile( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + "/html/qmake.dcf" );
	
	//m_dcfModel = new HelpViewer::dcfModel( m_dcfFile, indexListView );
	m_dcfModel = new HelpViewer::dcfModel( m_dcfFile, indexListView );

	// filter for searching classes
	m_filterModel = new QSortFilterProxyModel(this);
	m_filterModel->setFilterCaseSensitivity( Qt::CaseInsensitive );
	m_filterModel->setDynamicSortFilter( true );
	m_filterModel->setSourceModel(m_dcfModel);
	indexListView->setModel( m_filterModel );

	// completer for location bar
	m_locationCompleter = new QCompleter( this );
	m_locationCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	m_locationCompleter->setCompletionMode( QCompleter::PopupCompletion );
	m_locationCompleter->setModel( m_dcfModel );
	locationBar->setCompleter( m_locationCompleter );
}

/**
\brief Filter events sent to the indexEdit

Event filter to catch keyboard events on the indexEdit
Send Key_Down, Key_Up, Key_PageDown, Key_PageUp to the indexListView
This way the user can navigate the list of itmes when the focus
is on the filter input line
Also, pressing escape while focusing the indexEdit will clear it.

*/
bool HelpViewerImpl::eventFilter(QObject *obj, QEvent *event)
{
	if ( (obj == indexEdit) && (event->type() == QEvent::KeyPress) )
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		switch(keyEvent->key()){
			case Qt::Key_Up:
			case Qt::Key_Down:
			case Qt::Key_PageDown:
			case Qt::Key_PageUp:
				QApplication::sendEvent(indexListView, event);
				return true;
			
			case Qt::Key_Escape:
				indexEdit->clear();
				return true;
		}
	}
	
	// standard event processing
	return QObject::eventFilter(obj, event);
}
