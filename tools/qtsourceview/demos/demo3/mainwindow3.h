#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;
class QsvSyntaxHighlighter;
class QsvColorDefFactory;
class QsvLangDef;

class MainWindow3: public QMainWindow
{
    Q_OBJECT

public:
	MainWindow3(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow3();

	void setupActions();
	void createMenus();
	void createToolbars();

public slots:
	void fileNew();
	void fileOpen();
	
private:
	QsvColorDefFactory	*defaultColors;
	QsvSyntaxHighlighter	*highlight;
	QTextEdit	*textEditor;
	QToolBar	*toolbar;

	QAction		*actionNew;
	QAction		*actionOpen;
	QAction		*actionSave;
	QAction		*actionSaveAs;
	QAction		*actionConfigure;
	QAction		*actionQuit;
};

#endif // MAINWINDOW_H