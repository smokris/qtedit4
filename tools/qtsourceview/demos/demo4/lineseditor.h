#ifndef __LINESEDITOR_H__
#define __LINESEDITOR_H__

#include <QTextEdit>
#include "ui_findwidget.h"

class SamplePanel;
class TransparentWidget;

class LinesEditor: public QTextEdit
{
	Q_OBJECT
public:
	LinesEditor( QWidget *p=NULL );
	void setupActions();
	QColor getLinesPanelColor();
	QColor getCurrentLineColor();
	virtual void findMatching( QChar c1, QChar c2, bool forward, QTextBlock &block); 

public slots:
	void on_searchText_textChanged( const QString & text );
	void setLinesPanelColor( QColor c );
	void setCurrentLineColor( QColor c );
	void showFindWidget();
	int loadFile( QString );
	
protected slots:
	void cursorPositionChanged();
	void updateCurrentLine();
	void adjustMarginWidgets();
	
protected:
	void keyReleaseEvent ( QKeyEvent * event );
	void resizeEvent ( QResizeEvent *event );
	void paintEvent(QPaintEvent *e);
	void printWhiteSpaces( QPainter &p );
	void printCurrentLine( QPainter &p );
	void printMatchingBraces( QPainter &p );
	void widgetToBottom( QWidget *w );
	void widgetToTop( QWidget *w );

	QPixmap tabPixmap;
	QPixmap spacePixmap;
	QColor	currentLineColor;
	QColor	linesPanelColor;
	QColor	matchBracesColor;
	QColor	searchFoundColor;
	QColor	searchNotFoundColor;
	bool	highlightCurrentLine;
	bool	showWhiteSpaces;
	bool	showMatchingBraces;
	int	matchStart;
	int	matchEnd;
	QChar	currentChar;
	QChar	matchChar;
	QString matchingString;
	QString	fileName;
	SamplePanel	*panel;
	
	QAction	*actionFind;
	
	TransparentWidget *findWidget;
	Ui::FindWidget	ui_findWidget;
};

#endif // __LINESEDITOR_H__
