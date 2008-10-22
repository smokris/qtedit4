#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>

class QResizeEvent;
class QTabWidget;
class QWidget;
class QMenu;
class QAction;


class MyMainWindow: public QMainWindow
{
	Q_OBJECT
public:
	MyMainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
	~MyMainWindow();
	
protected:
	void resizeEvent( QResizeEvent * event );
	
public slots:
	void initGUI();
	void initMenu();
	void restoreSettings();
	void saveSettings();
	
	void updateFrame();
	void removeFrame();
	
	void on_maximizeButton_clicked();
	void on_minimizeButton_clicked();
	void on_closeButton_clicked();
	void on_actionToggleGUI_triggered();
	
private:
	QWidget		*leftButtons;
	QWidget		*rightButtons;
	QTabWidget	*tabWidget;
	
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *settingsMenu;
	QMenu *helpMenu;
	
	QAction *actionNew;
	QAction *actionOpen;
	QAction *actionSave;
	QAction *actionSaveAs;
	QAction *actionToggleGUI;
};

#endif // MYMAINWINDOW_H
