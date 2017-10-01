#include "table_model.h"

TableModel::TableModel()
{
}

void TableModel::sort(int column, Qt::SortOrder order)
{
	emit sortRequested(column, order);
}

bool TableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
	qDebug() << "inserting" << count << "column(s)";

	beginInsertColumns(parent, column, column + count - 1);
	mColumns.resize(mColumns.size() + count);
	mFilters.resize(mColumns.size() + count);
	endInsertColumns();
	return true;
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
	qDebug() << "inserting" << count << "row(s)";

	beginInsertRows(parent, row, row + count - 1);
	mRows.resize(mRows.size() + count, Row(mColumns.size()));
	endInsertRows();
	return true;
}

int TableModel::rowCount(const QModelIndex&) const
{
	return mRows.size() + 1;
}

int TableModel::columnCount(const QModelIndex&) const
{
	return mColumns.size();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return QString::fromStdString(mColumns.at(section));

	return {};
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
		else if (index.row() > 0)
		{
			// cf setData(): row "1" = last row, as we append every row in the underlying vector but we prepend them to the view
			const Row& row = mRows[mRows.size() - index.row()];

			if (row.size() > index.column())
				return row[index.column()];
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
		qDebug() << "set filter " << index.column() << " to " << value.toString();

		emit dataChanged(index, index);
		return true;
	}
	else if (index.row() > 0 && role == Qt::DisplayRole)
	{
		// row "1" = last row, as we append every row in the underlying vector but we prepend them to the view
		const int row = mRows.size() - index.row();
		mRows.at(row).at(index.column()) = value.toString();
		qDebug() << "set data " << index.row() << "," << index.column() << " to " << value.toString();

		emit dataChanged(index, index);
		return true;
	}

	qWarning() << "ignoring set data " << index.row() << " " << index.column();
	return false;
}

bool TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role)
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		mColumns.at(section) = value.toString().toStdString();

		emit headerDataChanged(orientation, section, section);
		return true;
	}

	return false;
}

