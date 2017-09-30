#include "table_model.h"
#include "table_view.h"

#include <QApplication>
#include <QTableView>
#include <QTableView>
#include <QSortFilterProxyModel>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	TableView table;
	TableModel model;
	TableDelegate delegate(model);

	//QSortFilterProxyModel proxyModel;
	//proxyModel.setSourceModel(&model);
	table.setModel(&model);
	table.setItemDelegateForRow(0, &delegate);

	table.setSortingEnabled(true);
	table.setSelectionBehavior(QAbstractItemView::SelectRows);
	table.setSelectionMode(QAbstractItemView::ExtendedSelection);
	table.setEditTriggers(QAbstractItemView::AllEditTriggers);
	table.sortByColumn(0, Qt::AscendingOrder);
	table.reset();
	table.show();

	return a.exec();
}
