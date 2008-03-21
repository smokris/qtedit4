#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include "ui_mainwindow.h"

class QString;
class QsvColorDefFactory;
class QsvLangDef;
class QsvSyntaxHighlighter;
struct EditorConfigData;

class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	void initGUI();
	
private slots:
	void on_action_New_triggered();
	void on_action_Open_triggered();
	void on_actionE_xit_triggered();
	void on_action_setup_triggered();
	
private:
	QString			lastDir;
	QsvColorDefFactory	*defColors;
	QsvLangDef		*langDefinition;
};

#endif
