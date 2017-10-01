#include "table_view.h"

#include <QHeaderView>
#include <QTimer>
#include <QSettings>

#include <iostream>

TableView::TableView()
{
	setSortingEnabled(true);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setEditTriggers(QAbstractItemView::AllEditTriggers);
	sortByColumn(0, Qt::AscendingOrder);

	QHeaderView& headerView = *horizontalHeader();
	headerView.setSectionsMovable(true);
	headerView.show();

	verticalHeader()->hide();

	connect(&headerView, &QHeaderView::sectionMoved, this, [this](int logicalIndex, int oldVisualIndex, int newVisualIndex)
	{
		std::cout << logicalIndex << " " << oldVisualIndex << " " << newVisualIndex << std::endl;
		SaveSettings();
	});

	connect(&headerView, &QHeaderView::sectionResized, this, [this](int logicalIndex, int oldSize, int newSize)
	{
		std::cout << logicalIndex << " " << oldSize << " " << newSize << std::endl;
		SaveSettings();
	});

	//QTimer::singleShot(0, this, SLOT(LoadSettings()));
}

void TableView::SaveSettings()
{
	QByteArray state = horizontalHeader()->saveState();

	QFile file("settings.data");
	file.open(QIODevice::WriteOnly);
	file.write(state);
	file.close();

	QSettings MySetting(QSettings::IniFormat, QSettings::UserScope, "Test");
		MySetting.setValue("column_width", state);

	std::cout << "bla =  " << MySetting.fileName().toStdString() << std::endl;
}

void TableView::LoadSettings()
{
	QFile file("settings.data");
	file.open(QIODevice::ReadOnly);
	QByteArray state = file.readAll();
	file.close();

	if (horizontalHeader()->restoreState(state))
		std::cout << "success";
	else
		std::cout << "fail";

	std::cout << std::endl;

	for (int i = 0; i < horizontalHeader()->count(); ++i)
		std::cout << "section " << i << ": " << horizontalHeader()->isSectionHidden(i) << std::endl;

}
