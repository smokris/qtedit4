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

#include <QDebug>

#include "helpviewerimpl.h"
#include "dcffile.h"
#include "dcfmodel.h"

/// default constructor
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
	
	m_dock->setWidget( m_dock_widget );
	m_dock->resize( parent->width() / 6, parent->height() );
	m_dock->setWindowTitle( tr("Help browser") );
	helpBrowser->setSearchPaths( QStringList( QLibraryInfo::location ( QLibraryInfo::DocumentationPath ) + "/html/" ) );
	helpBrowser->setSource( QUrl("index.html") );
	helpBrowser->zoomOut( 1 );
		
	locationBar->hide();
	QTimer::singleShot( 0, this, SLOT(loadFile()));
}

/// toggle the help dock window:
///  * if called when the help dock is displayed it will hide the help dock window
///  * if the dock window is hidden, it will re-parent the dock (if needed) and display the dock window
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
	}
}

/// display the help contents, simply loads "index.html"
void HelpViewerImpl::showContents()
{
	showPage("index.html");
}

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

void HelpViewerImpl::on_helpBrowser_sourceChanged(QUrl u)
{
	//qDebug( "Showing page %s", qPrintable(u.path() ) );
	if (m_classesLRU)
		m_classesLRU->touchItem( u.path() );
}

void HelpViewerImpl::on_mainTab_currentChanged(int index)
{
	// handle only when the index tab is showed
	if (index == 1)
		return;
		
	QString s = tr("Popular pages (based on previous searches)") + "<ul>";
	HelpViewer::LRU_List l = m_classesLRU->getTop(3);
	HelpViewer::LRU_List::iterator i;

	for (i = l.begin(); i != l.end(); ++i)
	{
		//qDebug( "\t%s - %d hits", qPrintable((*i).name), (*i).value );
		s += QString("<li><a href='%1'>%2</a></li>").arg((*i).name).arg((*i).name);
	}
	s+= "</ul><br>";
	popularPages->setText( s );
	//qDebug("list end");
}
