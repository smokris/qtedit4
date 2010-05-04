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
	resync_p(false);
	resync_p(true);
	// WTF this needs to be here?
	setHeaderData( 0, Qt::Horizontal, tr("Category"), Qt::DisplayRole );
	setHeaderData( 1, Qt::Horizontal, tr("Value")   , Qt::DisplayRole );
}

void ProjectModel::resync_p(bool onlyVars)
{
	foreach (QString categoryName, m_project->getCategoryList()) {
		if (m_project->isSectionList(categoryName) != onlyVars){
			continue;
		}
		
		QStandardItem *category = new QStandardItem(categoryName);
		QStringList   items     = m_project->getFiles(categoryName);
		QString       itemName;
		QStandardItem *item;
		
		itemName = items.join(",");
		if (m_project->isSectionList(categoryName))
			itemName = "(" + itemName + ")";
		
		category->setColumnCount(2);
		category->setEditable(false);
		category->setToolTip(itemName);
		appendRow(category);
		
		item = new QStandardItem(itemName);
		item->setToolTip(itemName);
		setItem(category->row(),1,item);
		
		if (m_project->isSectionList(categoryName)) {
			item->setEditable(false);
			QList<QStandardItem*> data, fillers;
			foreach(itemName, items) {
				QStandardItem *file   = new QStandardItem(itemName);
				QStandardItem *filler = new QStandardItem("");
				filler->setEditable(false);
				file->setToolTip(itemName);
				
				fillers << filler;
				data    << file;
			}
			category->insertColumn(0, fillers);
			category->insertColumn(1, data);
		}
	}
}

