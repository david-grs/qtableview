#pragma once

#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
public:
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
};
