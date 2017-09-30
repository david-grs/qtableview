#pragma once

#include <QAbstractTableModel>

#include <vector>

class TableModel : public QAbstractItemModel
{
public:
	TableModel();

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;

	void sort(int column, Qt::SortOrder order) override;
	bool setData(const QModelIndex& index, const QVariant& value, int role);

	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex parent(const QModelIndex&) const override { return {}; }

private:
	std::vector<QString> mFilters;
};
