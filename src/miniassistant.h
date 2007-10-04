/**
 * \file miniassistant.h
 * \brief Definition of the MiniAssistant class
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#ifndef HELPVIEWERIMPL_H
#define HELPVIEWERIMPL_H

#include <QObject>
#include "ui_miniassistant.h"

class QCompleter;
class QDockWidget;
class QSortFilterProxyModel;
class QTextEdit;
class QTextDocument;
	
// forward declarations
namespace MiniAssistantInt 
{
	class LRU;
	class dcfModel;
	class dcfFile;
}

class MiniAssistant : public QObject
{
Q_OBJECT
public:
	MiniAssistant( QWidget * parent = 0 );

public slots:
	void toggleDock();
	void showContents();
	void showMiniBrowser();
	void loadFile( QString dcfFileName );
	void showPage( QString page );
	bool displayKeyword( QString keyword );
	void updateWindowTitle();
	void updatePopularLinks();
	void updateSuggestedLinks( QTextEdit* );
	
	void find_keywords(QString text, QStringList &l );

private slots:
	void on_btnShowPage_clicked(bool b);
	void on_locationBar_returnPressed();
	void on_referencesListFilter_returnPressed();
	void on_referencesListFilter_textEdited(QString);
	void on_indexEditTimer_timeout();
	void on_referencesListView_activated(QModelIndex);
	void on_suggestedPanel_linkActivated(QString link);
	void on_popularPanel_linkActivated(QString link);
	void on_mainTab_currentChanged(int index);
	void on_helpBrowser_sourceChanged(QUrl );
	void on_dcfFile_loaded();

protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void timerEvent(QTimerEvent *event);
	
private:
	int		m_showMaxTop;
	int		m_indexEditTimer;
	int		m_indexEdit_Timeout;
	QWidget		*m_dock_widget;
	QDockWidget	*m_dock;
	QCompleter	*m_locationCompleter;
	QSortFilterProxyModel *m_filterModel;

	Ui::helpViewerForm		ui;
	MiniAssistantInt::dcfFile	*m_dcfFile;	
	MiniAssistantInt::dcfModel	*m_dcfModel;
	MiniAssistantInt::LRU		*m_classesLRU;
};

#endif
