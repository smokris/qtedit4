/**
 * \file mainwindowimpl.cpp
 * \brief Implementation of the main window
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#include <QTextCursor>

#include "mainwindowimpl.h"
#include "helpviewerimpl.h"

/// constuctor
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
	setupUi(this);
	helpViewer = new HelpViewerImpl(this);
	this->textEdit->setText(
"QString QMainWindow QObject\n\n"
"Missing functionalty: \n"
" * when pressing CTRL+F1, the search line edit (Index tab) is not focused [done]\n"
" * Need an expandable toobar in the browser, a button to show an extra bar, and when "
"   the widget is too small, hide some actions on the main toobar [done, URL bar is displayed on demand]\n"
" * main tab should list the last used pages (with sane defaults for first uses) [done]\n"
" * when the focus is on the lineEdit (Index tab), and you press down, the key press should be passed to the list box  [done]\n"
" * when typing an \"url\" in the Browser, completion is needed [done]\n"
" * when pressing F1, the current word should be search in the help [done]\n"
" * loading the XML file must be done on a separate thread (non blocking GUI) [done]\n\n"

" * code cleanup + documentation [WIP]\n"
" * all the code must be in a namespace [WIP]\n"
" * main tab should be smart enough to give you examples of helps (like VS does)\n"
" * Browser tab should have a search command\n"
" * command for showing the help browser should toggle with context (hidden,showed)\n"
) ;
}

/// called when the action_Quit is triggered
void MainWindowImpl::on_action_Quit_triggered()
{
	this->close();
}

/// handle the help event
void MainWindowImpl::on_action_Help_triggered()
{
	if (!textEdit->hasFocus())
		return;
		
	QTextCursor cursor = textEdit->textCursor();
	cursor.select(QTextCursor::WordUnderCursor);
	QString s = cursor.selectedText();
	helpViewer->displayKeyword( s );
}

/// handle a help search event
void MainWindowImpl::on_action_Seach_Help_triggered()
{
	helpViewer->showContents();
}
