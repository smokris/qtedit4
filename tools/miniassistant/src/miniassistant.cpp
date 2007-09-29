/**
 * \file miniassistant.cpp
 * \brief Implementation of the MiniAssistant class
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#include <QWidget>
#include <QDockWidget>
#include <QApplication>
#include <QMainWindow>
#include <QStringList>
#include <QDir>
#include <QUrl>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QKeyEvent>
#include <QCompleter>

#include <QTextDocument>
#include <QTextEdit>
#include <QTextBlock>
#include <QRegExp>

#include "miniassistant.h"
#include "lru.h"
#include "dcffile.h"
#include "dcfmodel.h"

/**
\namespace MiniAssistantInt
\brief Internal namespace

Internal namepace which ontains all the needed classes
needed by the help viewer. They are all considered to be
\b internal and you should not use them directly. 
*/

/**
\class MiniAssistant
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
mAsistant = new MiniAssistant(this);
mAsistant->showContents();

// In this example we take the word under a QTextEdit and feed it to the browser
QTextCursor cursor = textEdit->textCursor();
cursor.select(QTextCursor::WordUnderCursor);
mAsistant->displayKeyword( cursor.selectedText() );
\endcode

The filtering in the index is done via a timer. Each time a user presses
a key, a timer is triggered, and when that timer times out, only then
the text is filtered and displayed on screen. This way if the user types
"too fast" on the keyboard, the list will be generated 2-3 times per second
instead of beeing re-generated on every keyboard click. This also makes the 
GUI more responsive as it gets frozen only maximum times per second.

\see on_indexEditTimer_timeout()
\see loadFile()

*/

/**
\var MiniAssistant::m_showMaxTop
\brief The number or most viewed pages to be displayed

When updating the popular links this parameter defines
how much items to display (defaults is 5 items).

If you modify this parameter please call updatePopularLinks() manually.

\todo this is private, maybe make it public...? or add a function to modify this value?
\see updatePopularLinks()
*/

/**
\var MiniAssistant::m_dock_widget
\brief The widget seen on the dock widget

This is what you see inside the dock widget. This is a place holder for the GUI, which is 
designed in the designer. This widget will be inserted into the dock widget.

\see m_dock
*/

/**
\var MiniAssistant::m_dock
\brief The dock widget used for containing the main help viewer

The MiniAssistant is not a widget, it's a class which controlls the main
widget to be displayed o nscreen. This widget is currently a QDockWidget, 
which is represented by this variable.

The m_dock will contain the m_dock_widget.

\see m_dock_widget
*/

/**
\var MiniAssistant::m_locationCompleter
\brief The compltetion for the URL like browsing.

The help browser contains a small input line, for writing manually 
the classes you want to browse. This is the completer for that 
widget.

It uses the model for completition.

\see m_dcfModel
*/

/**
\var MiniAssistant::m_indexEditTimer
\brief Use to help filtering text

When the user presses the keyboard on the reference list filter
this timer will be triggered. The full documentation 
can be found in on_referencesListFilter_textEdited.

\see timerEvent()
\see on_referencesListFilter_textEdited()
*/

/**
\var MiniAssistant::m_filterModel
\brief Filter the results on the referencesListView

When the user wants to filter the list of classes, this class
will be used to filter the display in the referencesListView.

\see m_dcfModel
*/

/**
\var MiniAssistant::m_dcfFile
\brief The help topics are loaded from this file

This variable contains all the help topics the user can choose to view.

\todo move to a list of dcf files instead of a single dcf file
\see m_dcfModel
\see MiniAssistantInt::dcfFile
*/

/**
\var MiniAssistant::m_dcfModel
\brief Display the list of help topics available

This class will display on the list view the available topics.

\todo move to a model which knows how to deal with a list of DCF files

\see MiniAssistantInt::dcfModel
\see MiniAssistantInt::dcfFile
*/

/**
\var MiniAssistant::m_classesLRU
\brief The least recently used help pages

This contains the list of recently viewed pages.

\see updatePopularLinks()
\see MiniAssistantInt::LRU
*/

/**
\brief Default constructor
\param parent the main window to attach the dock widget to

Will create the main GUI, and issue the connection of the slots
(slots are not autoconnected, since MiniAssistant is not a QWidget).

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
MiniAssistant::MiniAssistant( QWidget * parent ) 
	: QObject(parent)
{
	m_classesLRU = new MiniAssistantInt::LRU;
	m_dock = new QDockWidget;
	m_dock_widget = new QWidget(m_dock);
	
	ui.setupUi(m_dock_widget);
	connect( ui.referencesListFilter, SIGNAL(textEdited(QString)), this, SLOT(on_referencesListFilter_textEdited(QString)));
	connect( ui.helpBrowser, SIGNAL(sourceChanged(QUrl)), this, SLOT(on_helpBrowser_sourceChanged(QUrl)));
	connect( ui.mainTab, SIGNAL(currentChanged(int)), this, SLOT(on_mainTab_currentChanged(int)));
	connect( ui.suggestedPanel, SIGNAL(linkActivated(QString)), this, SLOT(on_suggestedPanel_linkActivated(QString)));
	connect( ui.popularPanel, SIGNAL(linkActivated(QString)), this, SLOT(on_popularPanel_linkActivated(QString)));
	connect( ui.referencesListView, SIGNAL(activated(QModelIndex )), this, SLOT(on_referencesListView_activated(QModelIndex)));
	connect( ui.referencesListFilter, SIGNAL(returnPressed()), this, SLOT(on_referencesListFilter_returnPressed()));
	connect( ui.locationBar, SIGNAL(returnPressed()), this, SLOT(on_locationBar_returnPressed()));
	connect( ui.btnShowPage, SIGNAL(clicked(bool)), this, SLOT(on_btnShowPage_clicked(bool)));
	
	m_dock->setWidget( m_dock_widget );
	if (parent)
		m_dock->resize( parent->width() / 6, parent->height() );
	updateWindowTitle();
	
	ui.helpBrowser->zoomOut( 1 );
	
	ui.referencesListFilter->installEventFilter( this );
	ui.referencesListView->setAlternatingRowColors( true );
	ui.locationBar->hide();

	m_dcfModel = NULL;
	m_dcfFile = NULL;

	// filter for searching classes
	m_filterModel = new QSortFilterProxyModel(this);
	m_filterModel->setFilterCaseSensitivity( Qt::CaseInsensitive );
	m_filterModel->setDynamicSortFilter( true );
	
	// completer for location bar
	m_locationCompleter = new QCompleter( this );
	m_locationCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	m_locationCompleter->setCompletionMode( QCompleter::PopupCompletion );
	
	m_showMaxTop = 5;
	m_indexEditTimer = -1;
	m_indexEdit_Timeout = 450; //msec
}

/**
\brief Toggle the help dock window

Call this function if you want to display of hide the dock window:
 - If called when the help dock is displayed it will hide the help dock window
 - If the dock window is hidden, it will re-parent the dock (if needed) and display the dock window

\see on_mainTab_currentChanged
*/
void MiniAssistant::toggleDock()
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
	}
}

/**
\brief Display the help contents

This function will toggle the contents viewer, the Index tab:
 - if the dock is not visible it will display the dock and focus the \b Index tab
 - if the dock is not visible, then 
    - if the \b Index tab is displayed, dock will be hidden
    - if other tab is focused, the focus will shift to the \b Index tab
    
\todo refactor this code     
*/
void MiniAssistant::showContents()
{
	QString s = m_dcfFile->getReference();
	
	// this is called an educated guess
	if (s[s.length()-1] == QDir::separator())
		s += "index.html";

	if (m_dock->isVisible())
	{
		if (ui.mainTab->currentIndex() != 0)
		{
			ui.helpBrowser->setSource( QUrl(s) );
			ui.mainTab->setCurrentIndex(0);
		}
		else	// hide it
			toggleDock();
	}
	else
	{	// show it
		ui.helpBrowser->setSource( QUrl::fromLocalFile(s) );
		ui.mainTab->setCurrentIndex(0);
		on_mainTab_currentChanged( 0 );
		this->toggleDock();
	}
}

/**
\brief Display the mini browser

This function will toggle the \b Mini \b Browser viewer:
 - if the dock is not visible it will display the dock and focus the \b Mini \b Browser tab
 - if the dock is not visible, then 
    - if the \b Mini \b Browser tab is displayed, the dock will be hidden
    - if other tab is focused, the focus will shift to the \b Mini \b Browser tab
    
\todo refactor this code 
*/
void MiniAssistant::showMiniBrowser()
{
	if (m_dock->isVisible())
	{
		if (ui.mainTab->currentIndex() != 1)
		{
			ui.mainTab->setCurrentIndex(1);
		}
		else	// hide it
			toggleDock();
	}
	else
	{	// show it
		ui.mainTab->setCurrentIndex(1);
		on_mainTab_currentChanged(1);
		this->toggleDock();
	}
}

/**
\brief Opens a page on the help browser.

This function does not try to detect if the file is available, 
and it does not add a "html" extension. It will just pass the URL
to the help browser.

This function will also ensure that the dock is visible.

\see toggleDock
*/
void MiniAssistant::showPage( QString page )
{
	if (!m_dock->isVisible())
		toggleDock();
	
	ui.helpBrowser->setSource( QUrl(page) );
}

/**
\brief Load a DCF file
\param dcfFileName the dcfFile to be loaded to the Mini Assistant

Load a new DCF to the Mini Assistant. It will reload all
the pages, and reset the LRU. When the file has been loaded
a signal will be emitted and a slot will be called
to update the search path of the help browser.

\see on_dcfFile_loaded()
*/
void MiniAssistant::loadFile( QString dcfFileName )
{
	if (m_dcfModel)
		delete m_dcfModel;
	if (m_dcfFile)
		delete m_dcfFile;
		
	m_dcfFile = new MiniAssistantInt::dcfFile;
	m_dcfModel = new MiniAssistantInt::dcfModel( m_dcfFile, ui.referencesListView );
	
	m_classesLRU->reset();
	m_dcfFile->loadFile( dcfFileName );
	m_filterModel->setSourceModel(m_dcfModel);
	m_locationCompleter->setModel( m_dcfModel );
	ui.referencesListView->setModel( m_filterModel );
	ui.locationBar->setCompleter( m_locationCompleter );

	connect( m_dcfFile, SIGNAL(newContentAvaialable()), this, SLOT(on_dcfFile_loaded()) );
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
bool MiniAssistant::displayKeyword( QString keyword )
{
	keyword = keyword.toLower() + ".html";
	if (m_dcfFile->containsPage(keyword))
	{
		ui.helpBrowser->setSource( QUrl(keyword) );

		if (!m_dock->isVisible())
			toggleDock();
		ui.mainTab->setCurrentIndex( 1 );
		ui.helpBrowser->setFocus();
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
void MiniAssistant::updateWindowTitle()
{
	QString s = tr("Assistant");
	switch (ui.mainTab->currentIndex())
	{
		case 0:
			m_dock->setWindowTitle( s );
			break;
		case 1:
			m_dock->setWindowTitle( s + " - " + ui.helpBrowser->documentTitle() );
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
void MiniAssistant::updatePopularLinks()
{
	QString s = tr("Popular pages (based on previous searches):") + "<ul>";
	MiniAssistantInt::LRU_List l = m_classesLRU->getTop(m_showMaxTop);
	MiniAssistantInt::LRU_List::iterator i;

	for (i = l.begin(); i != l.end(); ++i)
	{
		// TODO modify the popular links text for release
#if 0		
		s += QString("<li><a href='%1'>%2</a></li>").arg((*i).name).arg((*i).name);
#else
		s += QString( "<li><a href='%1'>%2 (%3 hits)</a></li>").arg((*i).name).arg((*i).name).arg((*i).value );
#endif
	}
	s+= "</ul><br>";
	ui.popularPanel->setText( s );
}

/**
\brief Update the suggested links for a text editor

Make an educated guess about the help pages that the user 
wants to see, based on the text on his text editor.

*/
void MiniAssistant::updateSuggestedLinks( QTextEdit *edit )
{
	QStringList l;
	QTextBlock b_up, b_down; 
	QString s;
	int n = 5;
	int lines_depth = 7;

	b_down = b_up = edit->textCursor().block();
	find_keywords( b_down.text(), l );
	for( n=0; n<lines_depth; n++ )
	{
		if (b_down.isValid())
		{
			b_down = b_down.previous();
			find_keywords( b_down.text(), l );
		}
		
		if (b_up.isValid())
		{
			b_up = b_up.next();
			find_keywords( b_up.text(), l  );
		}
	}

	// now we have a list of keyworkds
	n = 0;
	while( n< m_showMaxTop)
	{
		if (n >= l.size())
			break;
		s += QString( "<li><a href='%1'>%2</a></li>").arg(l[n].toLower() + ".html").arg( l[n] );
		n++;
	}
	
	if (s.isEmpty())
		s = tr("Suggested pages:") + "<ul> <li>" + tr("No suggestions") + "</li></ul><br>";
	else
		s = tr("Suggested pages:") + "<ul>" + s + "</ul><br>";
	ui.suggestedPanel->setText( s );
}

void MiniAssistant::find_keywords(QString text, QStringList &list )
{
	QStringList l;
	
	//qDebug("looking for keywords - %s", qPrintable(text) );
	l = text.split( QRegExp("[\\s()\"\\._']+") );
	//l = text.split( " " );
	foreach( QString s, l )
	{
		if (m_dcfFile->containsPage( s.toLower() +".html" ))
			if (!list.contains(s,Qt::CaseInsensitive))
				list << s;
	}
}

/**
\brief Called when the btnShowPage button is clicked.

The button is location on top of the help browser, and will display or hide
a line text edit which will give the user a way to input URLs to browse.
*/
void MiniAssistant::on_btnShowPage_clicked(bool b)
{
	ui.locationBar->setVisible( b );
	if (b)
	{
		ui.locationBar->setFocus();
		ui.locationBar->selectAll();
	}
}

/**
\brief Called when "enter" is pressed on the location bar.

This function will try and load the page written in the location bar
and set the focus to the help browser.
*/
void MiniAssistant::on_locationBar_returnPressed()
{
	ui.helpBrowser->setSource( ui.locationBar->text() );
	ui.helpBrowser->setFocus();
}

/**
\brief Called when "enter" is pressed on the index edit

This function will open the page listed on the list box of the
Index tab in the help browser, and then set the current page
the browser tab and enforce the focus on the help browser.
*/
void MiniAssistant::on_referencesListFilter_returnPressed()
{
	QModelIndex index = ui.referencesListView->currentIndex();
	
	if (!index.isValid())
		return;
	
	ui.mainTab->setCurrentIndex( 1 );
	ui.helpBrowser->setSource( QUrl( index.data(Qt::StatusTipRole).toString() ) );
	ui.helpBrowser->setFocus();
}

/**
\brief  Called when the user modifies the text of the index edit

Update the filter proxy when the user presses the keyboard and starts searching something. 
It will issue a timer in 450msec to do the actualy change. This way you can safely type
as fast as you want and the filtering will be done 2-3 times per second. Most programmers
can type 10 chars in less then a second, and this technique will update the filter only one
while typing and once when the class name has been written.

\todo should the timer be stopped if runnning? this way the filter is done only when the user finishes typing

\see timerEvent()
*/
void MiniAssistant::on_referencesListFilter_textEdited(QString)
{
	if (m_indexEditTimer != -1)
	{
		killTimer( m_indexEditTimer );
		m_indexEditTimer = -1;
		
		// TODO: maybe we should just return...?
		//return;
	}

	m_indexEditTimer = startTimer( m_indexEdit_Timeout );
}

/**
\brief Called when the keypress timer is dome

When the user modifies the references list filter a timer is issued, and only when the timer
ends this callback is generated. This function will update the filter regex for the
proxy model, and will update the list of calsses seen on screen.

\see on_referencesListFilter_textEdited
*/
void MiniAssistant::on_indexEditTimer_timeout()
{
	m_filterModel->setFilterRegExp( ui.referencesListFilter->text() );
}

/**
\brief Called when the user double clicks an item in the list view

When the user clicks an item on the list view (the list of classes in the "Index" tab)
this callback is triggered. It will change the tab widget to the browser and display the
page which is described on that item.

\see dcfModel
\see dcfFile
*/
void MiniAssistant::on_referencesListView_activated(QModelIndex index )
{
	ui.mainTab->setCurrentIndex( 1 );
	ui.helpBrowser->setSource( QUrl(index.data(Qt::StatusTipRole).toString() ) );
}

/**
\brief Called when the user pressed a link in the help suggestions panel

The help suggestions panel contains links to different pages in the documentation.
When the user clicks one of those, this function is triggered and the corresponding
page will be opened. The browser tab will be displayed and the focus will be set
to the help browser.
*/
void MiniAssistant::on_suggestedPanel_linkActivated(QString link)
{
	ui.mainTab->setCurrentIndex( 1 );
	ui.helpBrowser->setSource( QUrl(link) );
	ui.helpBrowser->setFocus();
}

/**
\brief Called when the user pressed a link in the popular pages pane;

The help suggestions panel contains links to different pages in the documentation.
When the user clicks one of those, this function is triggered and the corresponding
page will be opened. The browser tab will be displayed and the focus will be set
to the help browser.
*/
void MiniAssistant::on_popularPanel_linkActivated(QString link)
{
	ui.mainTab->setCurrentIndex( 1 );
	ui.helpBrowser->setSource( QUrl(link) );
	ui.helpBrowser->setFocus();
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
void MiniAssistant::on_mainTab_currentChanged(int index)
{
	updateWindowTitle();
	
	// handle only when the index tab is showed
	if (index == 1)
	{
		if (ui.locationBar->isVisible() )
		{
			ui.locationBar->setFocus();
			ui.locationBar->selectAll();
		}
		else
			ui.helpBrowser->setFocus();
		return;
	}
		
	ui.referencesListView->setFocus();
	updatePopularLinks();
}

/**
\brief Called when a new page is displayed on the help browser

This function will update the title of the dock window, update the LRU with
the new status, and set the new URL into the location bar.

\see updateWindowTitle()
*/
void MiniAssistant::on_helpBrowser_sourceChanged(QUrl u)
{
	updateWindowTitle();
	
	if (m_classesLRU)
		m_classesLRU->touchItem( u.path() );
	
	ui.locationBar->setText( u.path() );
}

/**
\brief Filter events sent to the references list filter

Event filter to catch keyboard events on the referencesListFilter
Send Key_Down, Key_Up, Key_PageDown, Key_PageUp to the referencesListView
This way the user can navigate the list of itmes when the focus
is on the filter input line
Also, pressing escape while focusing the references list filter will clear it.

*/
bool MiniAssistant::eventFilter(QObject *obj, QEvent *event)
{
	if ( (obj == ui.referencesListFilter) && (event->type() == QEvent::KeyPress) )
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		switch(keyEvent->key()){
			case Qt::Key_Up:
			case Qt::Key_Down:
			case Qt::Key_PageDown:
			case Qt::Key_PageUp:
				QApplication::sendEvent(ui.referencesListView, event);
				return true;
			
			case Qt::Key_Escape:
				ui.referencesListFilter->clear();
				return true;
		}
	}
	
	// standard event processing
	return QObject::eventFilter(obj, event);
}

/**
\brief Overloaded event timer

This fimer is fired up when the user filters for class names.

\see on_referencesListFilter_textEdited
*/
void MiniAssistant::timerEvent(QTimerEvent *)
{
	killTimer( m_indexEditTimer );
	m_filterModel->setFilterRegExp( ui.referencesListFilter->text() );
	m_indexEditTimer = -1;
}

/**
\brief Called when the DCF file has been loaded

This function updates the search paths of the help browser. It's connected
by loadFile().

\see loadFile()
*/
void MiniAssistant::on_dcfFile_loaded()
{
	ui.helpBrowser->setSearchPaths( QStringList( m_dcfFile->getDirectory() ) );
	ui.helpBrowser->setSource( QUrl::fromLocalFile( m_dcfFile->getReference() ) );
	
// TODO
#if 0
	// default "recently used" items
	for( int i=0; i<2; i++ )
	{
		m_classesLRU->touchItem( "qabstractbutton.html" );
		m_classesLRU->touchItem( "qmenubar.html" );
		m_classesLRU->touchItem( "qtextedit.html" );
		m_classesLRU->touchItem( "qstring.html" );
		m_classesLRU->touchItem( "qwidget.html" );
	}
	updatePopularLinks();
#endif
}
