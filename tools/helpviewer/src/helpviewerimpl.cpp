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

#include <QDebug>

#include "helpviewerimpl.h"
#include "dcffile.h"
#include "dcfmodel.h"

/// Default constructor
/// Will create the main GUI, and issue the connection of the slots
/// (slots are not autoconnected, since HelpViewerImpl is not a QWidget)
///
/// The constructor will also add some defaults to the LRU
///	* qabstractbutton.h" );
///	* qmenubar.h
///	* qtextedit.h
///	* qstring.h
///	* qwidget.h
///
HelpViewerImpl::HelpViewerImpl( QWidget * parent ) 
	: QObject(parent)
{
	m_classesLRU = new HelpViewer::LRU;

	m_dock = new QDockWidget;
	m_dock_widget = new QWidget(m_dock);
	
	setupUi(m_dock_widget);
	connect( indexEdit, SIGNAL(textEdited(QString)), this, SLOT(on_indexEdit_textEdited(QString)));
	connect( helpBrowser, SIGNAL(sourceChanged(QUrl)), this, SLOT(on_helpBrowser_sourceChanged(QUrl)));
	connect( mainTab, SIGNAL(currentChanged(int)), this, SLOT(on_mainTab_currentChanged(int)));
	connect( helpSuggestions, SIGNAL(linkActivated(QString)), this, SLOT(on_helpSuggestions_linkActivated(QString)));
	connect( popularPages, SIGNAL(linkActivated(QString)), this, SLOT(on_popularPages_linkActivated(QString)));
	connect( indexListView, SIGNAL(activated(QModelIndex )), this, SLOT(on_indexListView_activated(QModelIndex)));
	connect( indexEdit, SIGNAL(returnPressed()), this, SLOT(on_indexEdit_returnPressed()) );
	
	m_dock->setWidget( m_dock_widget );
	m_dock->resize( parent->width() / 6, parent->height() );
	updateWindowTitle();
	
	helpBrowser->setSearchPaths( QStringList( QLibraryInfo::location ( QLibraryInfo::DocumentationPath ) + "/html/" ) );
	helpBrowser->setSource( QUrl("index.html") );
	helpBrowser->zoomOut( 1 );
	
	indexEdit->installEventFilter( this );
	
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
	m_showMaxTop = 4;
	updatePopularLinks();
}

/// Event filter to catch keyboard events on the indexEdit
/// Send Key_Down, Key_Up, Key_PageDown, Key_PageUp to the indexListView
/// This way the user can navigate the list of itmes when the focus
/// is on the filter input line
/// Also, pressing escape while focusing the indexEdit will clear it.
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

/// toggle the help dock window:
///  * if called when the help dock is displayed it will hide the help dock window
///  * if the dock window is hidden, it will re-parent the dock (if needed) and display the dock window
/// This function will also focus the first tabe, and the search bar
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

/// display the help contents, simply loads "index.html"
void HelpViewerImpl::showContents()
{
	showPage("index.html");
}

/// load the default file (qt.dcf) into the m_dcfFile and setup the
/// model for displaing it as needed
void HelpViewerImpl::loadFile()
{
	m_dcfFile = new dcfFile;
	m_dcfFile->loadFile( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + "/html/qt.dcf" );
	//m_dcfFile->loadFile( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + "/html/assistant.dcf" );
	//m_dcfFile->loadFile( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + "/html/designer.dcf" );
	//m_dcfFile->loadFile( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + "/html/linguist.dcf" );
	//m_dcfFile->loadFile( QLibraryInfo::location(QLibraryInfo::DocumentationPath) + "/html/qmake.dcf" );
	
	m_dcfModel = new dcfModel( m_dcfFile, indexListView );
	m_filterModel = new QSortFilterProxyModel(this);
	m_filterModel->setSourceModel(m_dcfModel);
	m_filterModel->setFilterCaseSensitivity( Qt::CaseInsensitive );
	indexListView->setModel( m_filterModel );
}

/// opens a page on the help browser. this functino does not try to detect if
/// the file is available, and it does not add a "html" extension.
void HelpViewerImpl::showPage( QString page )
{
	toggleDock();
	if (m_dock->isVisible())
		helpBrowser->setSource( QUrl(page) );
}

/// update the filter proxy when the user presses the keyboard and starts searching something
void HelpViewerImpl::on_indexEdit_textEdited(QString s )
{
	m_filterModel->setFilterRegExp( s );
}

/// event to be called when the user displays a new page on the browser
/// it will update the title of the dock window, and update the lru with
/// the new status
void HelpViewerImpl::on_helpBrowser_sourceChanged(QUrl u)
{
	updateWindowTitle();
	
	if (m_classesLRU)
		m_classesLRU->touchItem( u.path() );
}

/// Update the popular pages link.
/// This function is called when the tab widget is changed.
/// When the tab index is "1", the second tab page, the event is ignored.
/// When the tab index is "0", the first tab page, the top 5 items
/// will be updated on the popular links widget.
///
/// The function will also set the focus to the indexEdit.
void HelpViewerImpl::on_mainTab_currentChanged(int index)
{
	updateWindowTitle();
	
	// handle only when the index tab is showed
	if (index == 1)
	{
		helpBrowser->setFocus();
		return;
	}
		
	indexEdit->setFocus();
	indexEdit->activateWindow();
	updatePopularLinks();
}

void HelpViewerImpl::on_helpSuggestions_linkActivated(QString link)
{
	mainTab->setCurrentIndex( 1 );
	helpBrowser->setSource( QUrl(link) );
}

void HelpViewerImpl::on_popularPages_linkActivated(QString link)
{
	mainTab->setCurrentIndex( 1 );
	helpBrowser->setSource( QUrl(link) );
}

void HelpViewerImpl::on_indexListView_activated(QModelIndex index)
{
	mainTab->setCurrentIndex( 1 );
	helpBrowser->setSource( QUrl(index.data(Qt::StatusTipRole).toString() ) );
}

void HelpViewerImpl::on_indexEdit_returnPressed()
{
	QModelIndex index = indexListView->currentIndex();
	
	if (!index.isValid())
		return;

	mainTab->setCurrentIndex( 1 );
	helpBrowser->setSource( QUrl( index.data(Qt::StatusTipRole).toString() ) );
}

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

void HelpViewerImpl::updatePopularLinks()
{
	QString s = tr("Popular pages (based on previous searches)") + "<ul>";
	HelpViewer::LRU_List l = m_classesLRU->getTop(m_showMaxTop);
	HelpViewer::LRU_List::iterator i;

	for (i = l.begin(); i != l.end(); ++i)
	{
		//s += QString("<li><a href='%1'>%2</a></li>").arg((*i).name).arg((*i).name);
		s +=QString( "<li><a href='%1'>%2 (%3 hits)</a></li>").arg((*i).name).arg((*i).name).arg((*i).value );
	}
	s+= "</ul><br>";
	popularPages->setText( s );
}
