#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>

class QTabWidget;
class QWidget;

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
	void restoreSettings();
	void saveSettings();
	void removeFrame();
	
	void on_maximizeButton_clicked();
	void on_minimizeButton_clicked();
	void on_closeButton_clicked();
	
private:
	QWidget		*leftButton;
	QWidget		*rightButtons;
	QTabWidget	*tabWidget;
};

#endif // MYMAINWINDOW_H
