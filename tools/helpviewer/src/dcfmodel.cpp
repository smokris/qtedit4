/**
 * \file dcfmodel.cpp
 * \brief Implementation of the dcf model class
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#include "dcfmodel.h"
#include "dcffile.h"

#include <QModelIndex>

dcfModel::dcfModel(dcfFile *dcf, QObject *parent)
	:QAbstractItemModel(parent)
{
	m_dcf = dcf;
}

dcfModel::~dcfModel()
{	
	//m_dcf = dcf;
}

QModelIndex dcfModel::index( int row, int col, const QModelIndex &parent ) const
{
	return createIndex( row, col, 0 );
	
	// shut up gcc warnings
	parent.data();
}

QModelIndex dcfModel::parent( const QModelIndex &child ) const
{
	return QModelIndex();

	// shut up gcc warnings
	child.data();
}

int dcfModel::rowCount( const QModelIndex &parent ) const
{
	if (m_dcf == NULL)
		return 0;
	else
		return m_dcf->sections.count();
	
	// shut up gcc warnings
	parent.row();
}

int dcfModel::columnCount( const QModelIndex &parent ) const
{
	return 1;

	// shut up gcc warnings
	parent.data();
}

QVariant dcfModel::data( const QModelIndex &index, int roles ) const
{	
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_dcf->sections.count())
		return QVariant();

	if (roles == Qt::DisplayRole)
		return m_dcf->sections[index.row()].title;
	else
		return QVariant();
}
