#include <QApplication>
#include <QAction>
#include <QToolBar>
#include <QTimer>
#include <QMainWindow>

#include "qsvtextedit.h"
#include "qsvsyntaxhighlighterbase.h"
#include "qsvtextoperationswidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QMainWindow w;
	
	QsvSyntaxHighlighterBase *s             = new QsvSyntaxHighlighterBase;
	QsvTextEdit              *e             = new QsvTextEdit(&w, s);
	QsvTextOperationsWidget *textOpetations = new QsvTextOperationsWidget(e);

	QToolBar *b = w.addToolBar( "" );
	b->addAction( w.tr("Find"), textOpetations, SLOT(showSearch()))
	 ->setShortcut(QKeySequence("Ctrl+F"));
	b->addAction( w.tr("Replace"), textOpetations, SLOT(showReplace()))
	 ->setShortcut(QKeySequence("Ctrl+R"));
	b->setMovable(false);

	e->setPlainText(
"#include <stdio.h>\n\n"
"int main()       \n"
"{       \n"
"	{ printf(\"Hello world!\\n\"); }           \n"
"		}\n" );
	e->removeModifications();
	
	// tests for defaults
	e->setShowLineNumbers(true);
	e->setShowMargins(true);
	e->setTabSize(8);
	e->setTabIndents(true);
	e->setInsertSpacesInsteadOfTabs(true);
	e->setShowWhiteSpace(true);
	
	// in real applications, this would be part of a menu, or toolbar
	w.addAction(e->actionChangeCase);
	w.addAction(e->actionCapitalize);
	w.addAction(e->actionLowerCase);
	
	w.setCentralWidget(e);
	w.showMaximized();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
