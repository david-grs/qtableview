#include "table_model.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace rj = rapidjson;

TableModel::TableModel()
{
	connect(&mWebSocket, &QWebSocket::connected, this, &TableModel::OnWebSocketConnected);
	connect(&mWebSocket, &QWebSocket::disconnected, this, &TableModel::OnWebSocketDisconnected);
	mWebSocket.open(QUrl("ws://127.0.0.1:8080/"));
}

void TableModel::OnWebSocketConnected()
{
	qDebug() << "websocket, connected";

	connect(&mWebSocket, &QWebSocket::textMessageReceived, this, &TableModel::OnWebSocketMessage);

#if 0
	rj::Document res;
	rj::Document::AllocatorType& allocator = res.GetAllocator();

	res.SetObject();
	res.AddMember("message", "feed_subscribe", res.GetAllocator())
	mWebSocket.sendTextMessage(QStringLiteral("{\"\"}"));
#endif
}

void TableModel::OnWebSocketDisconnected()
{
	qDebug() << "websocket, disconnected";
}

void TableModel::sort(int column, Qt::SortOrder order)
{
	const std::string& colName = mColumns.at(column);
	const bool asc = order == Qt::AscendingOrder;

	rj::Document res;
	auto& allocator = res.GetAllocator();

	res.SetObject();
	res.AddMember("field", rj::Value(colName.c_str(), allocator), allocator);
	res.AddMember("order", rj::Value(asc ? "asc" : "desc", allocator), allocator);

	QString json;

	{
		rj::StringBuffer buffer;
		rj::Writer<rj::StringBuffer> writer(buffer);
		res.Accept(writer);
		json.append(buffer.GetString());
	}

	mWebSocket.sendTextMessage(json);
}

void TableModel::OnWebSocketMessage(QString json)
{
	mDocument.Parse(json.toStdString().c_str());

	const std::string message = mDocument["message"].GetString();
	if (message == "feed_definition")
	{
		auto it = mDocument.FindMember("fields");

		if (it == mDocument.MemberEnd() || !it->value.IsArray())
		{
			qWarning() << "malformed feed_definition";
			assert(false);
			return;
		}

		const rj::Value& fields = it->value;
		const int columnCount = fields.Capacity();

		insertColumns(0, columnCount);

		int column = 0;
		for (rj::Value::ConstValueIterator it = fields.Begin(); it != fields.End(); ++it)
		{
			const char* colName = it->GetString();

			qDebug() << "set col index" << column << "to" << colName;
			setHeaderData(column, Qt::Horizontal, QObject::tr(colName), Qt::DisplayRole);

			++column;
		}
	}
	else if (message == "feed")
	{
		assert(!mColumns.empty());

		insertRow(1);

		int column = 0;
		for (const std::string& field : mColumns)
		{
			QVariant data;

			auto it = mDocument.FindMember(field.c_str());
			if (it != mDocument.MemberEnd())
			{
				const rj::Value& value = it->value;

				if (value.IsString())
				{
					data = QString(value.GetString());
				}
				else if (value.IsBool())
				{
					data = value.GetBool();
				}
				else if (value.IsDouble())
				{
					data = value.GetDouble();
				}
				else if (value.IsInt())
				{
					data = value.GetInt();
				}
				else
				{
					qWarning() << "unspported type for field" << field.c_str();
				}
			}
			else
			{
				qWarning() << "field" << field.c_str() << "not found";
			}

			setData(index(1, column), data, Qt::DisplayRole);
			++column;
		}

		qDebug() << "insert row, rows=" << mRows.size();
	}
}

bool TableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
	qDebug() << "inserting" << count << "column(s)";

	beginInsertColumns(parent, column, column + count - 1);
	mColumns.resize(mColumns.size() + count);
	mFilters.resize(mColumns.size() + count);
	endInsertColumns();
	return true;
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
	qDebug() << "inserting" << count << "row(s)";

	beginInsertRows(parent, row, row + count - 1);
	mRows.resize(mRows.size() + count, Row(mColumns.size()));
	endInsertRows();
	return true;
}

int TableModel::rowCount(const QModelIndex&) const
{
	return mRows.size() + 1;
}

int TableModel::columnCount(const QModelIndex&) const
{
	return mColumns.size();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return QString::fromStdString(mColumns.at(section));

	return {};
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
		else if (index.row() > 0)
		{
			// cf setData(): row "1" = last row, as we append every row in the underlying vector but we prepend them to the view
			const Row& row = mRows[mRows.size() - index.row()];

			if (row.size() > index.column())
				return row[index.column()];
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
		qDebug() << "set filter " << index.column() << " to " << value.toString();

		emit dataChanged(index, index);
		return true;
	}
	else if (index.row() > 0 && role == Qt::DisplayRole)
	{
		// row "1" = last row, as we append every row in the underlying vector but we prepend them to the view
		const int row = mRows.size() - index.row();
		mRows.at(row).at(index.column()) = value.toString();
		qDebug() << "set data " << index.row() << "," << index.column() << " to " << value.toString();

		emit dataChanged(index, index);
		return true;
	}

	qWarning() << "ignoring set data " << index.row() << " " << index.column();
	return false;
}

bool TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role)
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		mColumns.at(section) = value.toString().toStdString();

		emit headerDataChanged(orientation, section, section);
		return true;
	}

	return false;
}

