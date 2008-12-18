#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>

class AbstractProject;

class ProjectModel : public QAbstractItemModel
{
public:
	ProjectModel( QObject* parent, AbstractProject *project );
	
	// reimplemented shit
	QModelIndex index( int row, int col, const QModelIndex &parent = QModelIndex() ) const;
	QModelIndex parent( const QModelIndex & child ) const;
	int rowCount( const QModelIndex &parent = QModelIndex() ) const;
	int columnCount( const QModelIndex &parent = QModelIndex() ) const;
	QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
	
private:
	AbstractProject *m_project;
};

#endif // PROJECTMODEL_H
