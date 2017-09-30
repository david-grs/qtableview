#pragma once

#include <QAbstractTableModel>

class TableModel : public QAbstractItemModel
{
public:
	int rowCount(const QModelIndex &parent = QModelIndex()) const ;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex&) const { return {}; }


};
