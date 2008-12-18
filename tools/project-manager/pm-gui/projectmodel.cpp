#include "projectmodel.h"
#include "../abstractproject.h"

#include <QDebug>

ProjectModel::ProjectModel( QObject* parent, AbstractProject *project )
            :QAbstractItemModel(parent)
{
    m_project = project;
}


// reimplemented shit
QModelIndex ProjectModel::index( int row, int col, const QModelIndex &parent  ) const
{
// QModelIndex QAbstractItemModel::createIndex ( int row, int column, void * ptr = 0 ) const   [protected]
	int id = parent.internalId();
	qDebug() << __FILE__ << __LINE__ << id;
	return createIndex( row, col, id+1 );
}

QModelIndex ProjectModel::parent( const QModelIndex & child ) const
{
	int id = child.internalId();
	qDebug() << __FILE__ << __LINE__ << id;
	return QModelIndex();
        Q_UNUSED(child);
}

int ProjectModel::rowCount( const QModelIndex &parent ) const
{
	if (m_project == NULL)
		return 0;

	int count = 0;
	if (parent.column() == -1)
		count = m_project->getCategoryList().count();
	else
		count = m_project->getFiles(m_project->getCategoryList().at(parent.row())).count();
	if (count<2)
		count = 0;

	return count;
}

int ProjectModel::columnCount( const QModelIndex &parent ) const
{
	return 2;
}

QVariant ProjectModel::data( const QModelIndex &index, int role ) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (!index.isValid())
		return QVariant();

	int column = index.column();
	int row    = index.row();

	QString cat;
	switch (column)
	{
		case 0:
//			qDebug() << __FILE__ << __LINE__ << QString("at item %1, parent %2").arg(row).arg(index.parent().row());
			if (m_project->getCategoryList().count() <= row)
				return QVariant();
			cat = m_project->getCategoryList().at( row );

	//		QString cat = m_project->getCategoryList().at( index.parent().row() );
	//		return m_project->getFiles(cat).at(row);

			return cat;
		case 1:
			int itemCount = rowCount( index );
			if (itemCount == 0)
			{
				if (m_project->getCategoryList().count() <= row)
					return QVariant();

				QString cat = m_project->getCategoryList().at( row );
				if (m_project->getFiles(cat).count()!=1)
					return QVariant();

				return m_project->getFiles(cat).at(0);
			}
			else
				return QVariant();
	}

	// make gcc happy
	return QVariant();
}
