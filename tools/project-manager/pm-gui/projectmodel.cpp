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
	resync_p(false);
	resync_p(true);
	// WTF this needs to be here?
	setHeaderData( 0, Qt::Horizontal, tr("Category"), Qt::DisplayRole );
	setHeaderData( 1, Qt::Horizontal, tr("Value")   , Qt::DisplayRole );
}

void ProjectModel::resync_p(bool onlyVars)
{
	int i = 0;
	foreach (QString s, m_project->getCategoryList()) {
		if (m_project->isSectionList(s) != onlyVars){
			continue;
		}
		
		QStandardItem *category = new QStandardItem(s);
		QStringList   items     = m_project->getFiles(s);

		m_categorieItems[s] = category;
		category->setColumnCount(2);
		category->setEditable(false);
		appendRow(category);

		if (m_project->isSectionList(s)) {
			foreach( QString itemName, items) {
				QStandardItem *item = new QStandardItem(itemName);
				category->appendRow(item);
			}
		}
		else {
			QString itemName;
			itemName = items.join(",");
			category->setToolTip(itemName);
			QStandardItem *item = new QStandardItem(itemName);
			setItem(i,1,item);
		}
		i++;
	}
}

