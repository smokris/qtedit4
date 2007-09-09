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
	connect(dcf, SIGNAL(newContentAvaialable()), this, SLOT(updateDCFData()) );
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

QModelIndex dcfModel::parent( const QModelIndex & ) const
{
	return QModelIndex();
}

int dcfModel::rowCount( const QModelIndex &parent ) const
{
	int i;
	if (m_dcf == NULL)
		i = 0;
	else
		i = m_dcf->sections.count();
	
	//qDebug("using %d items", i );
	return i;
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

	dcfSection s = m_dcf->sections[index.row()];
	
	if (roles == Qt::DisplayRole)
		return s.title;
	else if (roles == Qt::StatusTipRole)
		return s.reference;
	else if (roles == Qt::EditRole)
		return s.reference;
	else
		return QVariant();
}

void dcfModel::updateDCFData()
{
	reset();
}
