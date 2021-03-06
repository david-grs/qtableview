#include "table_model.h"
#include "table_view.h"
#include "table_controller.h"

#include <QApplication>
#include <QTableView>
#include <QTableView>
#include <QSortFilterProxyModel>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	TableModel model;
	TableView table(model);
	TableController controller(table, model);
	TableDelegate delegate(model);

	QObject::connect(&model, &TableModel::sortRequested, &controller, &TableController::sort);

	//QSortFilterProxyModel proxyModel;
	//proxyModel.setSourceModel(&model);
	table.setModel(&model);
	table.setItemDelegateForRow(0, &delegate);

	table.reset();
	table.show();

	return a.exec();
}
