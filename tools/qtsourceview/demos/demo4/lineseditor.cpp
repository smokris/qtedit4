#include <QPainter>
#include <QTextEdit>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextLayout>
#include <QScrollBar>
#include <QPushButton>
#include <QAction>
#include <QKeySequence>
#include <QTimer>
#include <QPalette>
#include <QFile>
#include <QTextStream>
#include <QStyle>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>

#include <QDebug>

#include "privateblockdata.h"
#include "qsvsyntaxhighlighter.h"
#include "lineseditor.h"
#include "samplepanel.h"
#include "transparentwidget.h"

const int floatingWidgetTimeout = 0;

static const char * tabPixmap_img[] = 
{
/* width height ncolors cpp [x_hot y_hot] */
	"8 8 3 2 0 0",
/* colors */
	"  s none       m none  c none",
	"O s iconColor1 m black c black",
	"X s iconColor2 m black c #E0E0E0",
/* pixels */
	"  X     X       ",
	"    X     X     ",
	"      X     X   ",
	"        X     X ",
	"      X     X   ",
	"    X     X     ",
	"  X     X       ",
	"                ",
};

static const char * spacePixmap_img[] = 
{
/* width height ncolors cpp [x_hot y_hot] */
	"8 8 3 2 0 0",
/* colors */
	"  s none       m none  c none",
	"O s iconColor1 m black c black",
	"X s iconColor2 m black c #E0E0E0",
/* pixels */
	"                ",
	"                ",
	"                ",
	"                ",
	"                ",
	"      X         ",
	"      X X       ",
	"                ",
};

LinesEditor::LinesEditor( QWidget *p ) :QTextEdit(p)
{
	tabPixmap		= QPixmap( tabPixmap_img ); 
	spacePixmap		= QPixmap( spacePixmap_img ); 
	currentLineColor	= QColor( "#DCE4F9" );
	bookmarkLineColor	= QColor( "#0000FF" );
	breakpointLineColor	= QColor( "#FF0000" );
	matchBracesColor	= QColor( "#FF0000" );
	searchFoundColor	= QColor( "#DDDDFF" ); //QColor::fromRgb( 220, 220, 255)
	searchNotFoundColor	= QColor( "#FFAAAA" ); //QColor::fromRgb( 255, 102, 102) "#FF6666"
	whiteSpaceColor		= QColor( "#E0E0E0" );
	highlightCurrentLine	= true;
	showWhiteSpaces		= true;
	showMatchingBraces	= true;
	showPrintingMargins	= true;
	printMarginWidth	= 80;
	matchStart		= -1;
	matchEnd		= -1;
	matchingString		= "(){}[]\"\"''``";

	actionFind		= NULL;
	actionFindNext		= NULL;
	actionFindPrev		= NULL;
	actionCapitalize	= NULL;
	actionLowerCase		= NULL;
	actionChangeCase	= NULL;
	actionToggleBookmark	= NULL;
	actionTogglebreakpoint	= NULL;
	
	panel = new SamplePanel( this );
	panel->m_panelColor	= QColor( "#FFFFD0" );
	panel->m_modifiedColor	= QColor( "#00FF00" );
	panel->setVisible( true );

	setFrameStyle( QFrame::NoFrame );
	setLineWrapMode( QTextEdit::NoWrap );
	setAcceptRichText( false );
	QTimer::singleShot( 0, this, SLOT(adjustMarginWidgets()));
	syntaxHighlighter = NULL;

#ifdef WIN32
	QFont f("Courier New", 10);
#else
	QFont f("Monospace", 9);
#endif
	document()->setDefaultFont( f );
	panel->setFont( f );
	setTabSize( 8 );
	
	findWidget = new TransparentWidget( this, 0.80 );
	ui_findWidget.setupUi( findWidget );
	ui_findWidget.searchText->setIcon( QPixmap(":/images/edit-undo.png") );
	findWidget->hide();

	replaceWidget = new TransparentWidget( this, 0.80 );
	ui_replaceWidget.setupUi( replaceWidget );
	ui_replaceWidget.replaceOldText->setIcon( QPixmap(":/images/edit-undo.png") );
	ui_replaceWidget.replaceNewText->setIcon( QPixmap(":/images/edit-undo.png") );
	ui_replaceWidget.optionsFrame->hide();
	replaceWidget->adjustSize();
	replaceWidget->hide();
	
	gotoLineWidget = new TransparentWidget( this, 0.80 );
	ui_gotoLineWidget.setupUi( gotoLineWidget );
	gotoLineWidget->hide();

	fileMessage = new TransparentWidget( this, 0.8 );
	ui_fileMessage.setupUi( fileMessage );
	connect( ui_fileMessage.label, SIGNAL(linkActivated(const QString&)), this, SLOT(on_fileMessage_clicked(QString)));
	fileMessage->hide();
	
	fileSystemWatcher = new QFileSystemWatcher(this);

	//connect( horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjustMarginWidgets()));
	//connect( verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjustMarginWidgets()));
	connect( this, SIGNAL(cursorPositionChanged()), this, SLOT(on_cursorPositionChanged()));
	connect( document(), SIGNAL(contentsChanged()), this, SLOT(on_textDocument_contentsChanged()));
	connect( ui_findWidget.searchText, SIGNAL(textChanged(const QString)), this, SLOT(on_searchText_textChanged(const QString)));
	connect( ui_findWidget.searchText, SIGNAL(editingFinished()), this, SLOT(on_searchText_editingFinished()));
	connect( ui_findWidget.searchText, SIGNAL(returnPressed()), this, SLOT(showFindWidget()));
	connect( ui_findWidget.prevButton, SIGNAL(clicked()), this, SLOT(findPrev()));
	connect( ui_findWidget.nextButton, SIGNAL(clicked()), this, SLOT(findNext()));
	connect( ui_findWidget.closeButton, SIGNAL(clicked()), this, SLOT(showFindWidget()));

	connect( ui_replaceWidget.moreButton, SIGNAL(clicked(bool)), this, SLOT(on_replaceWidget_expand(bool)));
	connect( ui_replaceWidget.replaceOldText, SIGNAL(textChanged(const QString)), this, SLOT(on_replaceOldText_textChanged(const QString)));
	connect( ui_replaceWidget.replaceOldText, SIGNAL(returnPressed()), this, SLOT(on_replaceOldText_returnPressed()));
	connect( ui_replaceWidget.replaceNewText, SIGNAL(returnPressed()), this, SLOT(on_replaceOldText_returnPressed()));
	connect( ui_replaceWidget.replaceButton, SIGNAL(clicked()), this, SLOT(on_replaceOldText_returnPressed()));
	connect( ui_replaceWidget.replaceAllButton, SIGNAL(clicked()), this, SLOT(on_replaceAll_clicked()));
	connect( ui_replaceWidget.closeButton, SIGNAL(clicked()), this, SLOT(showReplaceWidget()));

	connect( ui_gotoLineWidget.lineNumber, SIGNAL(editingFinished()), this, SLOT(on_lineNumber_editingFinished()));
	connect( ui_gotoLineWidget.lineNumber, SIGNAL(valueChanged(int)), this, SLOT(on_lineNumber_valueChanged(int)));
	connect( ui_gotoLineWidget.closeButton, SIGNAL(clicked()), this, SLOT(showGotoLineWidget()));

	connect( ui_fileMessage.closeButton, SIGNAL(clicked()), fileMessage, SLOT(hide()));
	connect( fileSystemWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(on_fileChanged(const QString&)));
}

void LinesEditor::setupActions()
{
	actionFind = new QAction( "&Find...", this );
	actionFind->setObjectName("actionFind");
	actionFind->setShortcut( QKeySequence("Ctrl+F") );
	connect( actionFind, SIGNAL(triggered()), this, SLOT(showFindWidget()) );

	actionReplace = new QAction( "&Replace...", this );
	actionReplace->setObjectName("actionReplace");
	actionReplace->setShortcut( QKeySequence("Ctrl+R") );
	connect( actionReplace, SIGNAL(triggered()), this, SLOT(showReplaceWidget()) );

	actionGotoLine = new QAction( "&Goto line...", this );
	actionGotoLine->setObjectName("actionGotoLine");
	actionGotoLine->setShortcut( QKeySequence("Ctrl+G") );
	connect( actionGotoLine, SIGNAL(triggered()), this, SLOT(showGotoLineWidget()) );

	actionFindNext = new QAction( "Find &next", this );
	actionFindNext->setObjectName("actionFindNext");
	actionFindNext->setShortcut( QKeySequence("F3") );
	connect( actionFindNext, SIGNAL(triggered()), this, SLOT(findNext()) );
	
	actionFindPrev = new QAction( "Find &previous", this );
	actionFindPrev->setObjectName("actionFindPrev");
	actionFindPrev->setShortcut( QKeySequence("Shift+F3") );
	connect( actionFindPrev, SIGNAL(triggered()), this, SLOT(findPrev()) );
	
	actionClearSearchHighlight = new QAction( "Clear search &highlight", this );
	actionClearSearchHighlight->setObjectName("actionClearSearchHighlight");
	//actionClearSearchHighlight->setShortcut( QKeySequence("Shift+F3") );
	connect( actionClearSearchHighlight, SIGNAL(triggered()), this, SLOT(clearSearchHighlight()) );
	
	actionCapitalize = new QAction( "Change to &capital letters", this );
	actionCapitalize->setObjectName( "actionCapitalize" );
	actionCapitalize->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_U ) );
	connect( actionCapitalize, SIGNAL(triggered()), this, SLOT(transformBlockToUpper()) );

	actionLowerCase = new QAction( "Change to &lower letters", this );
	actionLowerCase->setObjectName( "actionLowerCase" );
	actionLowerCase->setShortcut( QKeySequence( Qt::CTRL | Qt::SHIFT | Qt::Key_U  ) );
	connect( actionLowerCase, SIGNAL(triggered()), this, SLOT(transformBlockToLower()) );

	actionChangeCase = new QAction( "Change ca&se", this );
	actionChangeCase->setObjectName( "actionChangeCase" );
	connect( actionChangeCase, SIGNAL(triggered()), this, SLOT(transformBlockCase()) );

	actionToggleBookmark = new QAction( "&Toggle line bookmark", this );
	actionToggleBookmark->setObjectName( "actionToggleBookmark" );
	actionToggleBookmark->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_B  ) );
	connect( actionToggleBookmark, SIGNAL(triggered()), this, SLOT(toggleBookmark()) );

	actionPrevBookmark = new QAction( "&Previous bookmark", this );
	actionPrevBookmark->setObjectName( "actionPrevBookmark" );
	actionPrevBookmark->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_PageUp ) );
	connect( actionPrevBookmark, SIGNAL(triggered()), this, SLOT(gotoPrevBookmark()) );

	actionNextBookmark = new QAction( "&Next bookmark", this );
	actionNextBookmark->setObjectName( "actionNextBookmark" );
	actionNextBookmark->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_PageDown ) );
	connect( actionNextBookmark, SIGNAL(triggered()), this, SLOT(gotoNextBookmark()) );

	actionToggleBookmark = new QAction( "Toggle line &bookmark", this );
	actionToggleBookmark->setObjectName( "actionToggleBookmark" );
	actionToggleBookmark->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_B  ) );
	connect( actionToggleBookmark, SIGNAL(triggered()), this, SLOT(toggleBookmark()) );

	actionTogglebreakpoint = new QAction( "Toggle b&reakpoint", this );
	actionTogglebreakpoint->setObjectName( "actionTogglebreakpoint" );
	actionTogglebreakpoint->setShortcut( QKeySequence("F9") );
	connect( actionTogglebreakpoint, SIGNAL(triggered()), this, SLOT(toggleBreakpoint()) );
}

QColor LinesEditor::getItemColor( ItemColors role )
{
	switch (role)
	{
		case LinesPanel:	return panel->m_panelColor; 
		case ModifiedColor:	return panel->m_modifiedColor;
		case CurrentLine:	return currentLineColor;
		case MatchBrackets:	return matchBracesColor;
		case NoText:		return searchNoText;
		case TextFound:		return searchFoundColor;
		case TextNoFound:	return searchNotFoundColor;
		case WhiteSpaceColor:	return whiteSpaceColor;
		case BookmarkLineColor:	return bookmarkLineColor;
		case BreakpointLineColor: return breakpointLineColor;
	}
	
	// just to keep gcc happy, will not get executed
	return QColor();
}

void LinesEditor::setItemColor( ItemColors role, QColor c )
{
	switch (role)
	{
		case LinesPanel:
			panel->m_panelColor = c;
			panel->update();
			break;
		case ModifiedColor:
			panel->m_modifiedColor = c;
			panel->update();
			break;
		case CurrentLine:
			currentLineColor = c;
			break;
		case MatchBrackets:
			matchBracesColor = c;
			break;
		case NoText:
			searchNoText = c;
			break;
		case TextFound:
			searchFoundColor = c;
			break;
		case TextNoFound:
			searchNotFoundColor = c;
			break;
		case WhiteSpaceColor:
			whiteSpaceColor = c;
			updateMarkIcons();
			break;
		case BookmarkLineColor:
			bookmarkLineColor = c;
		case BreakpointLineColor: 
			breakpointLineColor = c;
	}
}

void	LinesEditor::setMargin( int width )
{
	printMarginWidth = width;
	showPrintingMargins = (width>0);
}

int	LinesEditor::getMargin()
{
	return printMarginWidth;
}

void	LinesEditor::setTabSize( int size )
{
	const QFontMetrics fm = QFontMetrics( document()->defaultFont() );
	int j = fm.width( " " ) * size;
	setTabStopWidth( j );
}

int	LinesEditor::getTabSize()
{
	const QFontMetrics fm = QFontMetrics( document()->defaultFont() );
	return fm.width( " " ) / tabStopWidth();
}

QsvSyntaxHighlighter* LinesEditor::getSyntaxHighlighter()
{
	return syntaxHighlighter;
}

void LinesEditor::setSyntaxHighlighter( QsvSyntaxHighlighter *newSyntaxHighlighter )
{
	syntaxHighlighter = newSyntaxHighlighter;
	syntaxHighlighter->rehighlight();
}

bool LinesEditor::getDisplayCurrentLine()
{
	return highlightCurrentLine;
}

void LinesEditor::setDisplayCurrentLine( bool b )
{
	highlightCurrentLine = b;
}

bool LinesEditor::getDisplayWhiteSpaces()
{
	return showWhiteSpaces;
}

void LinesEditor::setDisplayWhiteSpaces( bool b )
{
	showWhiteSpaces = b;
}

bool LinesEditor::getDisplayMatchingBrackets()
{
	return showMatchingBraces;
}

void LinesEditor::setDisplayMatchingBrackets( bool b )
{
	showMatchingBraces = b;
}

QString LinesEditor::getMatchingString()
{
	return matchingString;
}

void LinesEditor::setMatchingString( QString s )
{
	matchingString = s;	
}

void	LinesEditor::setBookmark( BookmarkAction action, QTextBlock block  )
{
	PrivateBlockData *data = getPrivateBlockData( block, true );
	if (!data)
		return;

	switch (action)
	{
		case Toggle:
			data->m_isBookmark = ! data->m_isBookmark;
			break;
		case Enable: 
			data->m_isBookmark = true;
			break;
		case Disable:
			data->m_isBookmark = false;
			break;
	}

	updateCurrentLine();
}

void	LinesEditor::setBreakpoint( BookmarkAction action, QTextBlock block )
{
	PrivateBlockData *data = getPrivateBlockData( block, true );
	if (!data)
		return;

	switch (action)
	{
		case Toggle:
			data->m_isBreakpoint = ! data->m_isBreakpoint;
			break;
		case Enable: 
			data->m_isBreakpoint = true;
			break;
		case Disable:
			data->m_isBreakpoint = false;
			break;
	}

	updateCurrentLine();
}

QWidget* LinesEditor::getPanel()
{
	return panel;
}

void	LinesEditor::adjustMarginWidgets()
{
	if (panel->isVisible())
	{
		setViewportMargins( panel->width()-1, 0, 0, 0);
		QRect viewportRect = viewport()->geometry();
		QRect lrect = QRect(viewportRect.topLeft(), viewportRect.bottomLeft());
		lrect.adjust( -panel->width(), 0, 0, 0 );
		panel->setGeometry(lrect);		
	}
	else
	{
		setViewportMargins( 0, 0, 0, 0);
	}
}

int LinesEditor::loadFile( QString s )
{
	QFile file(s);
	QFileInfo fileInfo(file);
	
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return -1;
	
	QTextStream in(&file);
	setPlainText( in.readAll() );
	
	// clear older watches, and add a new one
	QStringList sl = fileSystemWatcher->directories();
	if (!sl.isEmpty())
		fileSystemWatcher->removePaths( sl );
	
	fileName = fileInfo.absoluteFilePath();
	fileSystemWatcher->addPath( fileName );
	
	return 0;
}

void LinesEditor::updateMarkIcons()
{
	int x, y;
	QImage img;
	
	img = tabPixmap.toImage();
	for( x=0; x< tabPixmap.width(); x++ )
		for( y=0; y< tabPixmap.height(); y++ )
		{
			uint rgb = qRgb(  whiteSpaceColor.red(), whiteSpaceColor.green(), whiteSpaceColor.blue() );
			if (img.pixel(x,y) != 0)
				img.setPixel( x, y, rgb );
		}
	tabPixmap = QPixmap::fromImage( img );

	img = spacePixmap.toImage();
	for( x=0; x< tabPixmap.width(); x++ )
		for( y=0; y< tabPixmap.height(); y++ )
		{
			uint rgb = qRgb(  whiteSpaceColor.red(), whiteSpaceColor.green(), whiteSpaceColor.blue() );
			if (img.pixel(x,y) != 0)
				img.setPixel( x, y, rgb );
		}
	spacePixmap = QPixmap::fromImage( img );
}

void	LinesEditor::showFindWidget()
{
	if (replaceWidget->isVisible())
		replaceWidget->hide();
	
	if (gotoLineWidget->isVisible())
		gotoLineWidget->hide();
	
	if (findWidget->isVisible())
	{
		QTimer::singleShot( floatingWidgetTimeout, findWidget, SLOT(hide()) );
		this->setFocus();
		return;
	}

	searchCursor = textCursor();
	widgetToBottom( findWidget );
	ui_findWidget.searchText->clear();
	ui_findWidget.searchText->setFocus();
}

void	LinesEditor::showReplaceWidget()
{
	if (findWidget->isVisible())
		findWidget->hide();
	
	if (gotoLineWidget->isVisible())
		gotoLineWidget->hide();
	
	if (replaceWidget->isVisible())
	{
		QTimer::singleShot( floatingWidgetTimeout, replaceWidget, SLOT(hide()) );
		this->setFocus();
		return;
	}

	searchCursor = textCursor();
	ui_replaceWidget.replaceOldText->clear();
	ui_replaceWidget.replaceOldText->setFocus();
	widgetToBottom( replaceWidget );
}

void	LinesEditor::showGotoLineWidget()
{
	if (findWidget->isVisible())
		findWidget->hide();
	
	if (replaceWidget->isVisible())
		replaceWidget->hide();

	if (gotoLineWidget->isVisible())
	{
		QTimer::singleShot( floatingWidgetTimeout, gotoLineWidget, SLOT(hide()) );
		this->setFocus();
		return;
	}
	
	QTextCursor c = textCursor();
	ui_gotoLineWidget.lineNumber->setValue( c.blockNumber()+1 );
	
	c.movePosition(QTextCursor::End);
	ui_gotoLineWidget.lineNumber->setMaximum( c.blockNumber()+1 );
	ui_gotoLineWidget.linesCountLabel->setText( tr("%1 lines available").arg(c.blockNumber()+1) );
	ui_gotoLineWidget.lineNumber->setMinimum( 1 );
	ui_gotoLineWidget.lineNumber->setFocus();
	QLineEdit *lineEdit = ui_gotoLineWidget.lineNumber->findChild<QLineEdit*>();
	if (lineEdit)
		lineEdit->selectAll();
	widgetToBottom( gotoLineWidget );
}

void	LinesEditor::clearSearchHighlight()
{
	highlightString.clear();
	viewport()->update();
}

void	LinesEditor::findNext()
{
	issue_search( ui_findWidget.searchText->text(), textCursor(), getSearchFlags() );
}

void LinesEditor::findPrev()
{
	issue_search( ui_findWidget.searchText->text(), textCursor(), getSearchFlags() | QTextDocument::FindBackward );
}

void	LinesEditor::toggleBreakpoint()
{
	setBreakpoint( Toggle, textCursor().block() );
}

void	LinesEditor::toggleBookmark()
{
	setBookmark( Toggle, textCursor().block() );
}

void	LinesEditor::gotoPrevBookmark()
{
	QTextCursor cursor = textCursor();
	QTextBlock block = cursor.block();
	PrivateBlockData *data = getPrivateBlockData( block, false );
	
	while (block.isValid())
	{
		block = block.previous();
		data = getPrivateBlockData( block, false );

		if (data)
		{
			if (data->m_isBookmark)
			{
				cursor.setPosition( block.position() );
				setTextCursor(cursor);
				return;
			}
		}
	}
}

void	LinesEditor::gotoNextBookmark()
{
	QTextCursor cursor = textCursor();
	QTextBlock block = cursor.block();
	PrivateBlockData *data = getPrivateBlockData( block, false );
	
	while (block.isValid())
	{
		block = block.next();
		data = getPrivateBlockData( block, false );
		if (data)
		{
			if (data->m_isBookmark)
			{
				cursor.setPosition( block.position() );
				setTextCursor(cursor);
				return;
			}
		}
	}
}

void	LinesEditor::transformBlockToUpper()
{
	QTextCursor cursor = getCurrentTextCursor();
	QString s_before = cursor.selectedText();
	QString s_after  = s_before.toUpper();
	
	if (s_before != s_after)
	{
		cursor.beginEditBlock();
		cursor.deleteChar();
		cursor.insertText( s_after );
		cursor.endEditBlock();
		setTextCursor( cursor );
	}
}

void	LinesEditor::transformBlockToLower()
{
	QTextCursor cursor = getCurrentTextCursor();
	QString s_before = cursor.selectedText();
	QString s_after  = s_before.toLower();
	
	if (s_before != s_after)
	{
		cursor.beginEditBlock();
		cursor.deleteChar();
		cursor.insertText( s_after );
		cursor.endEditBlock();
		setTextCursor( cursor );
	}
}

void	LinesEditor::transformBlockCase()
{
	QTextCursor cursor = getCurrentTextCursor();
	QString s_before = cursor.selectedText();
	QString s_after = s_before;
	uint s_len = s_before.length();
	
	for( uint i=0; i< s_len; i++ )
		if (s_after[i].isLower())
			s_after[i] = s_after[i].toUpper();
		else if (s_after[i].isUpper())
			s_after[i] = s_after[i].toLower();
		
		
	if (s_before != s_after)
	{
		cursor.beginEditBlock();
		cursor.deleteChar();
		cursor.insertText( s_after );
		cursor.endEditBlock();
		setTextCursor( cursor );
	}
}

void	LinesEditor::updateCurrentLine()
{
	if (highlightCurrentLine)
		viewport()->update();
		
	panel->update();
}

void LinesEditor::on_searchText_textChanged( const QString & text )
{
	if (text.isEmpty())
	{
		QPalette p = palette();
		p.setColor( QPalette::Base, QColor("#FFFFFF") ); // white
		ui_findWidget.searchText->setPalette( p );
		return;
	}
	
	issue_search( text, searchCursor, getSearchFlags() ); 
}

void	LinesEditor::on_searchText_editingFinished()
{
	//showFindWidget();
	highlightString = ui_findWidget.searchText->text();
	viewport()->update();
}

void	LinesEditor::on_replaceWidget_expand( bool checked )
{
	ui_replaceWidget.optionsFrame->setVisible( checked );
	ui_replaceWidget.frame->adjustSize();
	replaceWidget->adjustSize();
	widgetToBottom( replaceWidget );
}

void	LinesEditor::on_replaceOldText_textChanged( const QString & text )
{
	QPalette palette = this->palette();
	if (text.isEmpty())
	{
		palette.setColor( QPalette::Base, QColor("#FFFFFF") ); // white
		ui_replaceWidget.replaceOldText->setPalette( palette );
		return;
	}

	QTextCursor c = textCursor();
	//c.movePosition(QTextCursor::Start);
	c = document()->find( text, c, getReplaceFlags() );
	
	if (!c.isNull())
		palette.setColor( QPalette::Base, searchFoundColor );
	else
		palette.setColor( QPalette::Base, searchNotFoundColor );
	ui_replaceWidget.replaceOldText->setPalette( palette );
}

void	LinesEditor::on_replaceOldText_returnPressed()
{
	if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) ||
	    QApplication::keyboardModifiers().testFlag(Qt::AltModifier) ||
	    QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) )
	{
		on_replaceAll_clicked();
		showReplaceWidget();
		return;
	}
	
	QTextCursor c = textCursor();
	c = document()->find( ui_replaceWidget.replaceOldText->text(), c, getReplaceFlags() );
	if (c.isNull())
		return;
	
	c.beginEditBlock();
	c.deleteChar();
	c.insertText( ui_replaceWidget.replaceNewText->text() );
	c.endEditBlock();
	setTextCursor( c );
}

void	LinesEditor::on_replaceAll_clicked()
{
	// WHY NOT HIDING THE WIDGET?
	// it seems that if you hide the widget, when the replace all action 
	// is triggered by pressing control+enter on the replace widget
	// eventually an "enter event" is sent to the text eidtor.
	// the work around is to update the transparency of the widget, to let the user
	// see the text bellow the widget
	
	//showReplaceWidget();
	
	int replaceCount = 0;
	replaceWidget->setWidgetTransparency( 0.2 );
	QTextCursor c = textCursor();
	c = document()->find( ui_replaceWidget.replaceOldText->text(), c, getReplaceFlags() );
	
	while (!c.isNull())
	{
		setTextCursor( c );
		QMessageBox::StandardButton button = QMessageBox::question( this, tr("Replace all"), tr("Replace this text?"),
		    QMessageBox::Yes | QMessageBox::Ignore | QMessageBox::Cancel );
		
		if (button == QMessageBox::Cancel)
		{
			break;
			
		}
		else if (button == QMessageBox::Yes)
		{
			c.beginEditBlock();
			c.deleteChar();
			c.insertText( ui_replaceWidget.replaceNewText->text() );
			c.endEditBlock();
			setTextCursor( c );
			replaceCount++;
		}
		
		c = document()->find( ui_replaceWidget.replaceOldText->text(), c, getReplaceFlags() );
	}
	replaceWidget->setWidgetTransparency( 0.8 );
	
	QMessageBox::information( 0, tr("Replace all"), tr("%1 replacement(s) made").arg(replaceCount) );
}

void	LinesEditor::on_lineNumber_editingFinished()
{
	// toggle the widget visibility only if visible
	if (gotoLineWidget->isVisible())
		showGotoLineWidget();
}

void	LinesEditor::on_lineNumber_valueChanged(int i)
{
	int requestedBlockNum = i - 1;
	QTextCursor c = textCursor();
	for( c.movePosition(QTextCursor::Start); requestedBlockNum && (!c.isNull()); --requestedBlockNum )
		c.movePosition( QTextCursor::NextBlock );
	
	if (c.isNull())
		return;
	
	setTextCursor( c );
}

void	LinesEditor::on_cursorPositionChanged()
{
	QTextCursor cursor = textCursor();
	int pos = cursor.position();
	if (pos == -1)
	{
		matchStart = matchEnd = -1;
		currentChar = matchChar = 0;
		if (highlightCurrentLine)
			updateCurrentLine();
		return;
	}
		
	QTextBlock  block = cursor.block();
		int i = cursor.position();
	currentChar = block.text()[i - block.position() ];
	matchStart = i;

	int j = matchingString.indexOf( currentChar );

	if (j == -1)
	{
		matchStart = matchEnd = -1;
		currentChar = matchChar = 0;
		if (highlightCurrentLine)
			updateCurrentLine();
		return;
	}

	if ( matchingString[j] != matchingString[j+1] )
		if (j %2 == 0)
			findMatching( matchingString[j], matchChar = matchingString[j+1], true, block );
		else
			findMatching( matchingString[j], matchChar = matchingString[j-1], false, block );
	else
		findMatching( matchChar = matchingString[j], block );
		
	updateCurrentLine();
}

void	LinesEditor::on_textDocument_contentsChanged()
{
	PrivateBlockData* data = getPrivateBlockData( textCursor().block(), true );
	if (!data)
		return;
	
	if (data->m_isModified)
		return;
	
	data->m_isModified = true;
	panel->update();
}

void	LinesEditor::on_fileChanged( const QString &fName )
{
	if (fName != fileName)
		return;
		
	QFileInfo f (fileName);
	
	if (f.exists())
		ui_fileMessage.label->setText( tr("File has been modified outside the editor. <a href=':reload' title='Clicking this links will revert all changes to this editor'>Click here to reload.</a>") );
	else
		ui_fileMessage.label->setText( tr("File has been deleted outside the editor.") );
	
	widgetToTop( fileMessage );
}

void	LinesEditor::on_fileMessage_clicked( QString s )
{
	if (s == ":reload")
	{
		loadFile( fileName );
		ui_fileMessage.label->setText( "" );
		fileMessage->hide();
	}
}

void LinesEditor::keyPressEvent( QKeyEvent *event )
{
	switch (event->key())
	{
		case Qt::Key_Escape:
			if (findWidget->isVisible())
				showFindWidget();
			else if (replaceWidget->isVisible())
				showReplaceWidget();
			else if (gotoLineWidget->isVisible())
				showGotoLineWidget();
			else
			{	// clear selection
				QTextCursor c = textCursor();
				if (c.hasSelection())
				{
					c.clearSelection();
					setTextCursor(c);
				}
			}
			break;
			
		case Qt::Key_Enter:
		case Qt::Key_Return:
			if (findWidget->isVisible() || replaceWidget->isVisible() || gotoLineWidget->isVisible())
			{
				event->ignore();
				return;
			}
			break;
			
		case Qt::Key_Tab:
			if (replaceWidget->isVisible())
			{	// since the replace widget has many subwidgets
				// tab should cycle between them
				event->ignore();
				return;
			}
			//if (tabIndents)
			{
				if (handleIndentEvent( !(event->modifiers() & Qt::ShiftModifier) ))
					// do not call original hanlder, if this was handled by that function
					return; 
			}
		default:
			if (handleKeyPressEvent(event))
				return;
	} // end case
	
	QTextEdit::keyPressEvent( event );
}

void LinesEditor::resizeEvent ( QResizeEvent *event )
{
	QTextEdit::resizeEvent( event );
	adjustMarginWidgets();

	if (findWidget->isVisible())
	{
		findWidget->hide();
		showFindWidget();
	}
	if (replaceWidget->isVisible())
	{
		replaceWidget->hide();
		showReplaceWidget();
	}

	if (gotoLineWidget->isVisible())
	{
		gotoLineWidget->hide();
		showGotoLineWidget();
	}

	if (fileMessage->isVisible())
	{
		fileMessage->hide();
		widgetToTop( fileMessage );
	}
}

void	LinesEditor::timerEvent( QTimerEvent *event )
{
	// TODO
	Q_UNUSED( event );
}

void LinesEditor::paintEvent(QPaintEvent *e)
{
	// if no special painting, no need to create the QPainter
	if (highlightCurrentLine || showWhiteSpaces || showMatchingBraces)
	{
		QPainter p( viewport() );
		
		printBackgrounds(p);
		QTextEdit::paintEvent(e);
		
		if (showMatchingBraces)
			printMatchingBraces( p );
	}
	else
		QTextEdit::paintEvent(e);
}

void	LinesEditor::printBackgrounds( QPainter &p )
{
	const int contentsY = verticalScrollBar()->value();
	const qreal pageBottom = contentsY + viewport()->height();
	const QFontMetrics fm = QFontMetrics( document()->defaultFont() );
	
	for ( QTextBlock block = document()->begin(); block.isValid(); block = block.next() )
	{
		QTextLayout* layout = block.layout();
		const QRectF boundingRect = layout->boundingRect();
		QPointF position = layout->position();
		
		if ( position.y() +boundingRect.height() < contentsY )
			continue;
		if ( position.y() > pageBottom )
			break;
			
		if (highlightCurrentLine)
			printCurrentLines( p, block );
		if (showWhiteSpaces)
			printWhiteSpaces( p, block, fm );
		if (!highlightString.isEmpty())
			printHighlightString( p, block, fm );
	}

	if (showPrintingMargins)
		printMargins( p );
}

void	LinesEditor::printWhiteSpaces( QPainter &p, const QTextBlock &block, const QFontMetrics &fm )
{
	const QString txt = block.text();
	const int len = txt.length();
	
	for ( int i=0; i<len; i++ )
	{
		QPixmap *p1 = 0;
		
		if (txt[i] == ' ' )
			p1 = &spacePixmap;
		else if (txt[i] == '\t' )
			p1 = &tabPixmap;
		else 
			continue;
		
		// pixmaps are of size 8x8 pixels
		QTextCursor cursor = textCursor();
		cursor.setPosition( block.position() + i, QTextCursor::MoveAnchor);
		
		QRect r = cursorRect( cursor );
		int x = r.x() + 4;
		int y = r.y() + fm.height() / 2 - 5;
		p.drawPixmap( x, y, *p1 );
	}
}

void	LinesEditor::printCurrentLines( QPainter &p, const QTextBlock &block )
{
	PrivateBlockData *data = dynamic_cast<PrivateBlockData*>( block.userData() );
	QTextCursor cursor = textCursor();
	cursor.setPosition( block.position(), QTextCursor::MoveAnchor);
	QRect r = cursorRect( cursor );
	r.setX( 0 );
	r.setWidth( viewport()->width() );

	p.save();
	p.setOpacity( 0.8 );
	if (r.top() == cursorRect().top() )
		p.fillRect( r, currentLineColor );
	p.setOpacity( 0.2 );
	if (data)
	{
		if (data->m_isBookmark)
			p.fillRect( r, bookmarkLineColor );
			
		if (data->m_isBreakpoint)
			p.fillRect( r, breakpointLineColor );
			
		// TODO: print all other states in a generic way
	}
	p.restore();
}

void	LinesEditor::printMatchingBraces( QPainter &p )
{
	if (matchStart == -1)
		return;
		
	QFont f = document()->defaultFont();
	QTextCursor cursor = textCursor();

	f.setBold( true );
	p.setFont( f );
	p.setPen( matchBracesColor );
	cursor.setPosition(matchStart+1, QTextCursor::MoveAnchor);
	QRect r = cursorRect( cursor );
	p.drawText(r.x()-1, r.y(), r.width(), r.height(), Qt::AlignLeft | Qt::AlignVCenter, currentChar );

	if (matchEnd == -1)
		return;
		
	cursor.setPosition(matchEnd+1, QTextCursor::MoveAnchor);
	r = cursorRect( cursor );
		p.drawText(r.x()-1, r.y(), r.width(), r.height(), Qt::AlignLeft | Qt::AlignVCenter, matchChar );
}

void	LinesEditor::printHighlightString( QPainter &p, const QTextBlock &block, const QFontMetrics &fm )
{
	int highlightStringLen = fm.width( highlightString );
	const QString t = block.text();
	QTextCursor cursor = textCursor();
	Qt::CaseSensitivity caseSensitive = getSearchFlags().testFlag(QTextDocument::FindCaseSensitively)?
		Qt::CaseSensitive : Qt::CaseInsensitive;
	QRect r;
	
	int k=0;
	int t_len = t.length();
	do
	{
		k = t.indexOf( highlightString, k, caseSensitive );
		if (k == -1)
			break;

		cursor.setPosition( block.position()+k+1, QTextCursor::MoveAnchor);
		r = cursorRect( cursor );
		p.setOpacity( 0.7 );
		p.fillRect(r.x()-1, r.y(), highlightStringLen, r.height(), Qt::yellow );
		p.setOpacity( 1 );
		k = k + highlightString.length();
	} while(k< t_len);
}

void	LinesEditor::printMargins( QPainter &p )
{
	int lineLocation;
	
	p.setFont( document()->defaultFont() );
	p.setPen( whiteSpaceColor );
	lineLocation = p.fontMetrics().width( " " ) * printMarginWidth + 0;
	p.drawLine( lineLocation, 0, lineLocation, height() );
}

void LinesEditor::widgetToBottom( QWidget *w )
{
	QRect r1 = viewport()->geometry();
	QRect r2 = w->geometry();

	int i = r2.height();
	r2.setX( r1.left() + 5 );
	r2.setY( r1.height() - i - 5 );
	r2.setWidth( r1.width() - 10 );
	r2.setHeight( i );
	
	w->setGeometry(r2);
	w->show();
}

void LinesEditor::widgetToTop( QWidget *w )
{
	QRect r1 = viewport()->geometry();
	QRect r2 = w->geometry();

	int i = r2.height();
	r2.setX( r1.left() + 5 );
	r2.setY( 5 );
	r2.setWidth( r1.width() - 10 );
	r2.setHeight( i );
	
	w->setGeometry(r2);
	w->show();	
}

bool	LinesEditor::handleKeyPressEvent( QKeyEvent *event )
{
	QTextCursor cursor = textCursor();
	int i,j;
	QString s;
	
	if (event->key() == Qt::Key_Delete)
	{
		if (cursor.hasSelection())
			return false;
		
		i = cursor.position() - cursor.block().position();
		QChar c1 = cursor.block().text()[ i ];
		j = matchingString.indexOf( c1 );
		if (j == -1)
			return false;
		
		if (j%2 == 0)
		{
			qDebug("Deleting forward");
			QChar c2 = cursor.block().text()[ i+1 ];
			if (c2 != matchingString[j+1])
				return false;
			cursor.deletePreviousChar();
			cursor.deleteChar();
		}
		else
		{
			qDebug("Deleting backward");
			QChar c2 = cursor.block().text()[ i-1 ];
			if (c2 != matchingString[j-1])
				return false;
			cursor.deletePreviousChar();
			cursor.deleteChar();
		}
		
		goto FUNCTION_END;
	}
	
	// handle only normal key presses
	s = event->text();
	if (s.isEmpty())
		return false;
	
	// don't handle if not in the matching list
	j = matchingString.indexOf( s[0] );
	if ((j == -1) || (j%2 == 1))
		return false;
	
	i = cursor.position();
	
	if (!cursor.hasSelection())
	{
		cursor.insertText( QString(matchingString[j]) );
		cursor.insertText( QString(matchingString[j+1]) );
	}
	else
	{
		QString s = cursor.selectedText();
		cursor.beginEditBlock();
		cursor.deleteChar();
		s = matchingString[j] + s + matchingString[j+1];
		cursor.insertText(s);
		cursor.endEditBlock();
	}
	cursor.setPosition( i + 1 );
	setTextCursor(cursor);
	
FUNCTION_END:
	event->accept();
	return true;
}

bool	LinesEditor::handleIndentEvent( bool forward )
{
	QTextCursor cursor = textCursor();
	if (!cursor.hasSelection())
	{	// TODO
		qDebug("no selection, not handling");
		return false;
	}
	
	return true;
}

bool LinesEditor::issue_search( const QString &text, QTextCursor newCursor, QFlags<QTextDocument::FindFlag> findOptions  )
{
	QTextCursor c = document()->find( text, newCursor, findOptions );
	bool found = ! c.isNull();
	
	if (!found)
	{
		//lets try again, from the start
		c.movePosition(findOptions.testFlag(QTextDocument::FindBackward)? QTextCursor::End : QTextCursor::Start);
		c = document()->find( ui_findWidget.searchText->text(), c, findOptions );
		found = ! c.isNull();
	}
	
	if (found)
	{
		QPalette ok = this->palette();
		ok.setColor( QPalette::Base, searchFoundColor );
		ui_findWidget.searchText->setPalette( ok );
		setTextCursor( c );
	}
	else
	{
		QPalette bad = this->palette();
		bad.setColor( QPalette::Base, searchNotFoundColor );
		ui_findWidget.searchText->setPalette( bad );	
		setTextCursor( searchCursor );
	}

	return found;
}

void LinesEditor::findMatching( QChar c1, QChar c2, bool forward, QTextBlock &block )
{
	int i = matchStart;
	int n = 1;
	QChar c;
	QString blockString = block.text();
	
	do
	{
		if (forward)
		{
			i ++;
			if ((i - block.position()) == block.length())
			{
				block = block.next();
				blockString = block.text();
			}
		}
		else
		{
			i --;
			if ((i - block.position()) == -1)
			{
				block = block.previous();
				blockString = block.text();
			}
		}
		if (block.isValid())
			c = blockString[i - block.position()];
		else
			break;

		if (c == c1)
			n++;
		else if (c == c2)
			n--;
	} while (n!=0);
	
	if (n == 0)
		matchEnd = i;
	else
		matchEnd = -1;
}

void	LinesEditor::findMatching( QChar c, QTextBlock &block )
{
	int n = 0;
	QString blockString = block.text();
	int blockLen = block.length();
	
	// try forward
	while (n < blockLen) 
	{
		if (n != matchStart - block.position())
			if (blockString[n] == c)
			{
				matchEnd = block.position() + n;
				return;
			}
		n++;
	}
}

PrivateBlockData* LinesEditor::getPrivateBlockData( QTextBlock block, bool createIfNotExisting )
{
	QTextBlockUserData *d1 = block.userData();
	PrivateBlockData *data = dynamic_cast<PrivateBlockData*>( d1 );
	
	// a user data has been defined, and it's not our structure
	if (d1 && !data)
		return NULL;
	
	if (!data && createIfNotExisting)
	{
		data = new PrivateBlockData;
		block.setUserData( data );
	}
	return data;
}

QTextCursor	LinesEditor::getCurrentTextCursor()
{
	QTextCursor cursor = textCursor();
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	return cursor;
}


QFlags<QTextDocument::FindFlag> LinesEditor::getSearchFlags()
{
	QFlags<QTextDocument::FindFlag> f;
	
	if (ui_findWidget.caseSensitive->isChecked())
		f = f | QTextDocument::FindCaseSensitively;
	
	if (ui_findWidget.wholeWords->isChecked())
		f = f | QTextDocument::FindWholeWords;
		
	return f;
}

QFlags<QTextDocument::FindFlag> LinesEditor::getReplaceFlags()
{
	QFlags<QTextDocument::FindFlag> f;
	
	if (ui_replaceWidget.caseSensitive->isChecked())
		f = f | QTextDocument::FindCaseSensitively;
	
	if (ui_replaceWidget.wholeWords->isChecked())
		f = f | QTextDocument::FindWholeWords;

	if (ui_replaceWidget.findBackwards->isChecked())
		f = f | QTextDocument::FindBackward;
	
	return f;
}
