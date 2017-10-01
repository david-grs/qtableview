#include "table_model.h"

#include <iostream>

TableModel::TableModel()
{
	connect(&mWebSocket, &QWebSocket::connected, this, &TableModel::OnWebSocketConnected);
	connect(&mWebSocket, &QWebSocket::disconnected, this, &TableModel::OnWebSocketDisconnected);
	mWebSocket.open(QUrl("ws://127.0.0.1:8080/"));

	mFilters.resize(columnCount());
}

void TableModel::OnWebSocketConnected()
{
	qDebug() << "websocket, connected";

	connect(&mWebSocket, &QWebSocket::textMessageReceived, this, &TableModel::OnWebSocketMessage);
	mWebSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void TableModel::OnWebSocketDisconnected()
{
	qDebug() << "websocket, disconnected";
}

void TableModel::OnWebSocketMessage(QString message)
{
	qDebug() << "websocket, received message " << message;
}

int TableModel::rowCount(const QModelIndex&) const
{
	return 3;
}

int TableModel::columnCount(const QModelIndex&) const
{
	return 4;
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::EditRole && index.row() == 0)
	{
		return mFilters[index.column()];
	}

	if (role == Qt::DisplayRole)
	{
		if (index.row() == 0)
		{
			return mFilters[index.column()];
		}
		else
		{
			return 4 - index.row() + index.column();
		}
	}

	return {};
}

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEnabled | (index.row() == 0 ? Qt::ItemIsEditable : Qt::ItemIsSelectable);
}

QModelIndex TableModel::index(int row, int column, const QModelIndex&) const
{
	return QAbstractItemModel::createIndex(row, column, Q_NULLPTR);
}

bool TableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.row() == 0 && (role == Qt::UserRole || role == Qt::EditRole))
	{
		mFilters[index.column()] = value.toString();
		std::cout <<mFilters[index.column()].toStdString() << std::endl;
	}

	return true;
}

void TableModel::sort(int column, Qt::SortOrder order)
{
	std::cout << "sort " << column << std::endl;

}

