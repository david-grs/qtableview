#include "table_view.h"

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QTimer>
#include <QSettings>

#include <iostream>

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

	connect(&header, &QHeaderView::sectionMoved, this, [this](int logicalIndex, int /*oldVisualIndex*/, int newVisualIndex)
	{
		const QString columnName = mModel.headerData(logicalIndex, Qt::Horizontal, Qt::DisplayRole).toString();
		mColumnPosition[columnName] = newVisualIndex;
	});

	connect(&header, &QHeaderView::sectionResized, this, [this](int logicalIndex, int /*oldSize*/, int newSize)
	{
		const QString columnName = mModel.headerData(logicalIndex, Qt::Horizontal, Qt::DisplayRole).toString();
		mColumnSize[columnName] = newSize;
	});
}

QMap<QString, QVariantMap> TableView::saveSettings()
{
	QMap<QString, QVariantMap> m;
	m["column_position"] = mColumnPosition;
	m["column_size"] = mColumnSize;
	m["column_visibility"] = mColumnVisibility;
	return m;
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

