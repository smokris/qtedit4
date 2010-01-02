#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QStandardItemModel>

class AbstractProject;

class ProjectModel : public QStandardItemModel
{
public:
	ProjectModel( QObject* parent, AbstractProject *project );
	void resync();
	
private:
	AbstractProject *m_project;
	QHash<QString,QStandardItem*> m_categorieItems;
};

#endif // PROJECTMODEL_H
