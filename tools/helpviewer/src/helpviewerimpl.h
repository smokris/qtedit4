/**
 * \file helpviewerimpl.cpp
 * \brief Definition of the help viewer object
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#ifndef HELPVIEWERIMPL_H
#define HELPVIEWERIMPL_H

#include <QObject>
#include "ui_helpviewer.h"
#include "lru.h"

//using HelpViewer::LRU;

class QDockWidget;
class QSortFilterProxyModel;
class dcfFile;
class dcfModel;

class HelpViewerImpl : public QObject, public Ui::helpViewerForm
{
Q_OBJECT
public:
	HelpViewerImpl( QWidget * parent = 0 );
private slots:
	void on_mainTab_currentChanged(int index);
	void on_helpBrowser_sourceChanged(QUrl );
	void on_indexEdit_textEdited(QString );

public slots:
	void toggleDock();
	void showContents();
	void showPage( QString page );
	void loadFile();
	
private:
	QWidget		*m_dock_widget;
	QDockWidget	*m_dock;
	dcfFile		*m_dcfFile;
	dcfModel	*m_dcfModel;
	QSortFilterProxyModel *m_filterModel;
	HelpViewer::LRU	*m_classesLRU;
};


#endif

