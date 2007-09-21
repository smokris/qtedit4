/**
 * \file dcfmodel.cpp
 * \brief Implementation of the DCF model class
 * \author Diego Iastrubni (elcuco@kde.org)
 * License LGPL
 */

#include <QModelIndex>

#include "dcfmodel.h"
#include "dcffile.h"

namespace MiniAssistantInt {

/**
\class dcfModel
\brief A model for displaying the list of references in a DCF file.

This class will display the list references available in a dcfFile a list of strings
which contain the title, the tooltips will be the refernce. The full documentation of 
the roles is available in dcfModel::data().

For more information about the MVC (model view controller) read the chapter 
called "Model/View Programming" in Assistant, or you can also read it online:
http://doc.trolltech.com/4.3/model-view-programming.html

\see data()
*/


/**
\var dcfModel::m_dcf
\brief A reference to the DCF file

This pointer represents the DCF file loaded, and is not deleted by this class.
It's passed only in the constructor only, and it's used as read only reference.
You should de-allocate the memory of this DCF file, only after this class has 
been deleted.

\todo maybe use smart pointer?
*/

/**
\brief Construct the model 
\param dcf a loaded DCF file
\param parent the parent of this model

Constructs the model by referencing the DCF file. Will also connect the newContentAvaialable() signal from
the DCF to the updateDCFData() slot, to be noticed of changes in the content of the DCF file
*/
dcfModel::dcfModel(dcfFile *dcf, QObject *parent)
	:QAbstractItemModel(parent)
{
	m_dcf = dcf;
	connect(dcf, SIGNAL(newContentAvaialable()), this, SLOT(updateDCFData()) );
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
		i = m_dcf->references.count();
	
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

/**
\brief Return the data corresponding to the specific role

This function wil return the following data:
 
 - Qt::DisplayRole: dcfReference::title
 - Qt::StatusTipRole: dcfReference::reference
 - Qt::ToolTipRole : dcfReference::reference
 - Qt::EditRole:  dcfReference::reference
*/
QVariant dcfModel::data( const QModelIndex &index, int roles ) const
{	
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_dcf->references.count())
		return QVariant();

	dcfReference s = m_dcf->references[index.row()];
	
	if (roles == Qt::DisplayRole)
		return s.title;
	else if (roles == Qt::StatusTipRole)
		return s.reference;
	else if (roles == Qt::ToolTipRole)
		return s.reference;
	else if (roles == Qt::EditRole)
		return s.reference;
	else
		return QVariant();
}

/**
\brief Called when the dcfFile has new references

This function will invalidate the data of the model, and re-display
the list of references available in the dcfFile
*/
void dcfModel::updateDCFData()
{
	reset();
}

} // end of namespace
