/**
 * \file dcfmodel.h
 * \brief Definition of the dcf model class
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#ifndef DCFMODEL_H
#define DCFMODEL_H

#include <QAbstractItemModel>

class dcfFile;

class dcfModel : public QAbstractItemModel
{
	Q_OBJECT
public:

	dcfModel(dcfFile *dcf, QObject *parent = 0 );
	~dcfModel();
 
	QModelIndex index( int row, int col, const QModelIndex &parent = QModelIndex() ) const;
	QModelIndex parent( const QModelIndex & ) const;
	int rowCount( const QModelIndex &parent = QModelIndex() ) const;
	int columnCount( const QModelIndex &parent = QModelIndex() ) const;
	QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
	
private:
	dcfFile *m_dcf;
};
#endif
