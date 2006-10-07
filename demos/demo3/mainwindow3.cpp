#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QSyntaxHighlighter>
#include <QFile>

#include "qsvcolordef.h"
#include "qsvcolordeffactory.h"
#include "qsvlangdef.h"
#include "qsvlangdeffactory.h"
#include "qsvsyntaxhighlighter.h"
#include "mainwindow3.h"

const int MessageTimeOut = 10000;

MainWindow3::MainWindow3( QMainWindow *parent )
:QMainWindow( parent )
{
	setupUi( this );	
//	QString dataPath  = QApplication::applicationDirPath() + "/../../";
	QString dataPath  = QApplication::applicationDirPath();
	
	// setup the mime types definitions
	QsvLangDefFactory::getInstanse()->addMimeTypes( dataPath +"/src/mime.types" );
	QsvLangDefFactory::getInstanse()->loadDirectory( dataPath + "/data/langs/" );

	// load a default color set
	defColors = new QsvColorDefFactory( dataPath + "/data/colors/kate.xml" );

	// load a default language definition
	langDefinition = QsvLangDefFactory::getInstanse()->getHighlight("1.cpp");

	// assign to it the new syntax highlighter, with the default colors and language
	highlight = new QsvSyntaxHighlighter( textEdit, defColors, langDefinition );
	
        statusBar()->showMessage(tr("Welcome, the default syntax is C++"), MessageTimeOut );
}

void MainWindow3::on_action_New_triggered()
{
	textEdit->clear();
}

void MainWindow3::on_action_Open_triggered()
{        
	QString fileName = QFileDialog::getOpenFileName( this, "Open file", "", "*" );
	
	if (fileName.isEmpty() )
		return;
	
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, tr("Demo 3"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}

	// detect the best hightlight for this file type
	// clear the document, so it will not get painted as well
	textEdit->clear();
	langDefinition = QsvLangDefFactory::getInstanse()->getHighlight( fileName );
	highlight->setHighlight( langDefinition );
	
	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEdit->setPlainText(in.readAll());
	QApplication::restoreOverrideCursor();
	
	if (langDefinition)
		statusBar()->showMessage(tr("File loaded, with syntax highlighter for %1").arg(langDefinition->getName()), MessageTimeOut );
	else
		statusBar()->showMessage(tr("File loaded, no syntax highlighter"), MessageTimeOut );
}

void MainWindow3::on_action_About_triggered()
{
	QMessageBox::information(this, "Demo 1",
		"Third demo of the qtsourceview library.\n"
		"Diego Iastrubni <elcuco@kde.org> 2006, lincensed under the terms of the LGPL.");
}

void MainWindow3::on_actionAbout_Qt_triggered()
{
	QMessageBox::aboutQt( this, "diego" );
}

void MainWindow3::on_actionE_xit_triggered()
{
	this->close();
}
