#include <QApplication>
#include <QAction>
#include <QToolBar>
#include <QTimer>
#include <QMainWindow>
#include <QFileDialog>

#include "qsvtextedit.h"
#include "qsvsyntaxhighlighterbase.h"
#include "qsvtextoperationswidget.h"

class MainWindow : QMainWindow
{
	Q_OBJECT
public:
	MainWindow( const QString &file )
	{
		QsvSyntaxHighlighterBase *s             = new QsvSyntaxHighlighterBase;
		QsvTextEdit              *e             = new QsvTextEdit(this, s);
		QsvTextOperationsWidget *textOpetations = new QsvTextOperationsWidget(e);
	
		QToolBar *b = addToolBar( "" );
		b->addAction( tr("Open"), this, SLOT(loadFile()))
		 ->setShortcut(QKeySequence("Ctrl+O"));
		b->addAction( tr("Find"), textOpetations, SLOT(showSearch()))
		 ->setShortcut(QKeySequence("Ctrl+F"));
		b->addAction( tr("Replace"), textOpetations, SLOT(showReplace()))
		 ->setShortcut(QKeySequence("Ctrl+R"));
		b->addAction( tr("Fing next"), textOpetations, SLOT(searchNext()))
		 ->setShortcut(QKeySequence("F3"));
		b->setMovable(false);
		b->addAction( tr("Fing prev"), textOpetations, SLOT(searchPrev()))
				->setShortcut(QKeySequence("Shift+F3"));
		b->setMovable(false);
		
		setCentralWidget(e);
		showMaximized();
		
		if (!file.isEmpty())
			loadFile(file);
		else
			setWindowTitle("QtSourceView demo5");

#if 0
		// tests for defaults
		e->setShowLineNumbers(true);
		e->setShowMargins(true);
		e->setTabSize(8);
		e->setTabIndents(true);
		e->setInsertSpacesInsteadOfTabs(true);
		e->setShowWhiteSpace(true);
#endif
	}
	
public slots:
	void loadFile( QString filename ="" )
	{
		if (filename.isEmpty()) {
			filename = QFileDialog::getOpenFileName(this,tr("Load file"));
			if (filename.isEmpty())
				return;
		}
		QsvTextEdit *e = findChild<QsvTextEdit*>();
		if (!e)
			return;
		
//		TODO
//		if (e->isModified){
//			e->save();
//		}
		
		e->displayBannerMessage( tr("Loaded file %1").arg(filename));
		e->loadFile(filename);
		e->removeModifications();
		setWindowTitle(filename);
	}

private:
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w( a.arguments().count()>=2 ? a.arguments().at(1) : QString() );
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}

#include "main5.moc"
