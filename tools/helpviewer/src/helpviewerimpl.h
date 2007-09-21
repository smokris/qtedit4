/**
 * \file helpviewerimpl.h
 * \brief Definition of the help viewer object
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#ifndef HELPVIEWERIMPL_H
#define HELPVIEWERIMPL_H

#include <QObject>
#include "ui_helpviewer.h"

class QCompleter;
class QDockWidget;
class QSortFilterProxyModel;
	
// forward declarations
namespace HelpViewer{
	class LRU;
	class dcfModel;
	class dcfFile;
}

class HelpViewerImpl : public QObject, public Ui::helpViewerForm
{
Q_OBJECT
public:
	HelpViewerImpl( QWidget * parent = 0 );

public slots:
	void toggleDock();
	void showContents();
	void showPage( QString page );
	bool displayKeyword( QString keyword );
	void updateWindowTitle();
	void updatePopularLinks();

private slots:
	void on_btnShowPage_clicked(bool b);
	void on_locationBar_returnPressed();
	void on_indexEdit_returnPressed();
	void on_indexEdit_textEdited(QString );
	void on_indexEditTimer_timeout();
	void on_indexListView_activated(QModelIndex);
	void on_helpSuggestions_linkActivated(QString link);
	void on_popularPages_linkActivated(QString link);
	void on_mainTab_currentChanged(int index);
	void on_helpBrowser_sourceChanged(QUrl );
	void loadFile();

protected:
	bool eventFilter(QObject *obj, QEvent *event);

private:
	
	int		m_showMaxTop;
	QWidget		*m_dock_widget;
	QDockWidget	*m_dock;
	QCompleter	*m_locationCompleter;
	QTimer		*m_indexEditTimer;
	QSortFilterProxyModel *m_filterModel;

	HelpViewer::dcfFile	*m_dcfFile;	
	HelpViewer::dcfModel	*m_dcfModel;
	HelpViewer::LRU		*m_classesLRU;
};

#endif
