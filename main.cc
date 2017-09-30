#include "table_model.h"

#include <QApplication>
#include <QTableView>
#include <QTableView>
#include <QSortFilterProxyModel>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTableView table;
	TableModel model;

	QSortFilterProxyModel proxyModel;
	proxyModel.setSourceModel(&model);
	table.setModel(&proxyModel);

	table.setModel(&model);
	table.setSortingEnabled(true);
	table.sortByColumn(0, Qt::AscendingOrder);
	table.reset();
	table.show();

	return a.exec();
}
