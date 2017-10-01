#include "table_view.h"

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QTimer>
#include <QSettings>

#include <cassert>

TableView::TableView(QAbstractItemModel& model) :
	mModel(model)
{
	setSortingEnabled(true);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setEditTriggers(QAbstractItemView::AllEditTriggers);
	sortByColumn(0, Qt::AscendingOrder);

	{
		QHeaderView& header = *verticalHeader();
		header.setSectionResizeMode(QHeaderView::Fixed);
		header.setDefaultSectionSize(24);
	}

	QHeaderView& header = *horizontalHeader();
	header.setSectionsMovable(true);
	header.show();

	verticalHeader()->hide();

	connect(&header, &QHeaderView::sectionMoved, this, [this](int column, int /*oldVisualIndex*/, int /*newVisualIndex*/)
	{
		QHeaderView& header = *horizontalHeader();
		//assert(mModel.columnCount() == header.count());

		//for (int column = 0; column < mModel.columnCount(); ++column)
		//{
			const QString& columnName = mModel.headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();
			mColumnPosition[columnName] = header.visualIndex(column);
		//}
	});

	connect(&header, &QHeaderView::sectionResized, this, [this](int logicalIndex, int /*oldSize*/, int newSize)
	{
		const QString columnName = mModel.headerData(logicalIndex, Qt::Horizontal, Qt::DisplayRole).toString();
		mColumnSize[columnName] = newSize;
	});
}

void TableView::setColumnVisibility(int column, bool show)
{
	const QString columnName = mModel.headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();
	mColumnVisibility[columnName] = show;

	if (show)
		showColumn(column);
	else
		hideColumn(column);
}

QMap<QString, QVariantMap> TableView::saveSettings()
{
	QMap<QString, QVariantMap> m;
	m["column_position"] = mColumnPosition;
	m["column_size"] = mColumnSize;
	m["column_visibility"] = mColumnVisibility;
	return m;
}

void TableView::loadSettings(QMap<QString, QVariantMap> settings)
{
	loadPositionSettings(settings["column_position"]);
	loadSizeSettings(settings["column_size"]);
	loadVisibilitySettings(settings["column_visibility"]);
}

void TableView::loadPositionSettings(QVariantMap m)
{
	QHeaderView& header = *horizontalHeader();

	for (int column = 0; column < mModel.columnCount(); ++column)
	{
		const QString& field = mModel.headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();

		auto it = m.find(field);
		if (it == m.cend() || !it->canConvert<int>())
			continue;

		const int from = header.visualIndex(column);
		const int to = it->toInt();

		header.moveSection(from, to);
	}
}

void TableView::loadSizeSettings(QVariantMap m)
{
	for (int column = 0; column < mModel.columnCount(); ++column)
	{
		const QString& field = mModel.headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();

		auto it = m.find(field);
		if (it == m.cend() || !it->canConvert<int>())
			continue;

		setColumnWidth(column, it->toInt());
	}
}

void TableView::loadVisibilitySettings(QVariantMap m)
{
	for (int column = 0; column < mModel.columnCount(); ++column)
	{
		const QString& field = mModel.headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();

		auto it = m.find(field);
		if (it == m.cend() || !it->canConvert<bool>())
			continue;

		setColumnHidden(column, it->toBool());
	}
}

