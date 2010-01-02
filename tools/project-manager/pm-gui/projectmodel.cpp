#include "projectmodel.h"
#include "../abstractproject.h"

#include <QDebug>

ProjectModel::ProjectModel( QObject* parent, AbstractProject *project )
	    :QStandardItemModel(parent)
{
	m_project = project;
	setColumnCount(2);
	resync();
}

void ProjectModel::resync()
{
	this->clear();
	m_categorieItems.clear();
	int i = 0;

	foreach (QString s, m_project->getCategoryList()) {
		QStandardItem *category = new QStandardItem(s);
		QStringList   items     = m_project->getFiles(s);

		m_categorieItems[s] = category;
		category->setColumnCount(2);
		appendRow(category);

		if (items.count() < 2){
			QString value;
			value = items.join(",");
			category->setToolTip(value);
			QStandardItem *i2 = new QStandardItem(value);
			setItem(i,1,i2);
//			qDebug( "single value [%s] = %s", qPrintable(s),qPrintable(value));
		}
		else {
			foreach( QString f, items) {
				QStandardItem *i2 = new QStandardItem(f);
				category->appendRow(i2);
			}
		}
		i++;
	}

	// WTF this needs to be here?
	setHeaderData( 0, Qt::Horizontal, tr("Category"), Qt::DisplayRole );
	setHeaderData( 1, Qt::Horizontal, tr("Value")   , Qt::DisplayRole );
}


