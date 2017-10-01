#pragma once

#include <QAbstractTableModel>
#include <QtWebSockets/QWebSocket>

#include <QString>
#include <QVariant>

#include <vector>

class TableModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	TableModel();

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int, Qt::Orientation, int role) const override;

	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;

	void sort(int column, Qt::SortOrder order) override;

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex&) const override { return {}; }

	bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

signals:
	void sorted(int column, Qt::SortOrder order);

private:
	std::vector<QString> mFilters;
	std::vector<std::string> mColumns;

	using Row = std::vector<QVariant>;
	std::vector<Row> mRows;
};
