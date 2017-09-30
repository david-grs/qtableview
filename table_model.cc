#include "table_model.h"

int TableModel::rowCount(const QModelIndex &parent) const
{
	return 2;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
	{
		return 4 - index.row() + index.column();
	}
	}
	return QVariant();
}

QModelIndex TableModel::index(int row, int column, const QModelIndex&) const
{
	return QAbstractItemModel::createIndex(row, column, Q_NULLPTR);
}
