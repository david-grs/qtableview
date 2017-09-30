#include "table_model.h"

#include <iostream>

TableModel::TableModel()
{
	mFilters.resize(columnCount());
}

int TableModel::rowCount(const QModelIndex&) const
{
	return 3;
}

int TableModel::columnCount(const QModelIndex&) const
{
	return 2;
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::EditRole && index.row() == 0)
	{
		return mFilters[index.column()];
	}

	if (role == Qt::DisplayRole)
	{
		if (index.row() == 0)
		{
			return mFilters[index.column()];
		}
		else
		{
			return 4 - index.row() + index.column();
		}
	}

	return {};
}

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEnabled | (index.row() == 0 ? Qt::ItemIsEditable : Qt::ItemIsSelectable);
}

QModelIndex TableModel::index(int row, int column, const QModelIndex&) const
{
	return QAbstractItemModel::createIndex(row, column, Q_NULLPTR);
}

bool TableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.row() == 0 && (role == Qt::UserRole || role == Qt::EditRole))
	{
		mFilters[index.column()] = value.toString();
		std::cout <<mFilters[index.column()].toStdString() << std::endl;
	}

	return true;
}

void TableModel::sort(int column, Qt::SortOrder order)
{

}

