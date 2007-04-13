#include <QIcon>
#include <QAction>
#include <QMessageBox>

#include "richtext_browser.h"

QexRichTextBrowser::QexRichTextBrowser( QString fileName, QWidget *parent )
	: RichText( parent, fileName )
{
	toolbars["main"]->addSeparator();
	toolbars["main"]->addAction( actionBold );
	toolbars["main"]->addAction( actionItalic );
	toolbars["main"]->addAction( actionUnderline );
	toolbars["main"]->addSeparator();
	toolbars["main"]->addActions( alignGroup );
	toolbars["main"]->addSeparator();
	toolbars["main"]->addActions( listGroup );
	toolbars["main"]->addSeparator();
	toolbars["main"]->addAction( actionTextColor );
	toolbars["main"]->addWidget( (QWidget*)fontComboBox );
	toolbars["main"]->addWidget( (QWidget*)comboSize );
	
	actionAlignCenter	->setIcon( QIcon(":/images/textcenter.png") );
	actionBold		->setIcon( QIcon(":/images/textbold.png") );
	actionAlignCenter	->setIcon( QIcon(":/images/textcenter.png") );
	actionItalic		->setIcon( QIcon(":/images/textitalic.png") );
	actionAlignJustify	->setIcon( QIcon(":/images/textjustify.png") );
	actionAlignLeft		->setIcon( QIcon(":/images/textleft.png") );
	actionAlignRight	->setIcon( QIcon(":/images/textright.png") );
	actionUnderline		->setIcon( QIcon(":/images/textunder.png") );
}

bool QexRichTextBrowser::canCloseClient()
{
#if 0
	if (!editor->document()->isModified())
		return true;

        // ask for saving
	int ret = QMessageBox::warning(this, tr("Application"),
				       tr("The document has been modified.\n"
						       "Do you want to save your changes?"),
				       QMessageBox::Yes | QMessageBox::Default,
				       QMessageBox::No,
				       QMessageBox::Cancel | QMessageBox::Escape);

	if (ret == QMessageBox::Yes)
		return fileSave();
	else if (ret == QMessageBox::Cancel)
		return false;
#endif
        // shut up GCC warnings
	return true;
}
