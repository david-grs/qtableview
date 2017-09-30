#pragma once

#include <QAbstractTableModel>

class TableModel : public QAbstractItemModel
{
public:
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;

	void sort(int column, Qt::SortOrder order) override;

	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex parent(const QModelIndex&) const override { return {}; }
};
