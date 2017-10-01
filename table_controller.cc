#include "table_controller.h"

#include <QTableView>
#include <QAbstractItemModel>
#include <QTimer>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace rj = rapidjson;

TableController::TableController(QTableView& view, QAbstractItemModel& model) :
	mView(view),
	mModel(model)
{
	connect(&mWebSocket, &QWebSocket::connected, this, &TableController::OnWebSocketConnected);
	connect(&mWebSocket, &QWebSocket::disconnected, this, &TableController::OnWebSocketDisconnected);
	mWebSocket.open(QUrl("ws://127.0.0.1:8080/"));
}

void TableController::OnWebSocketConnected()
{
	qDebug() << "websocket, connected";

	connect(&mWebSocket, &QWebSocket::textMessageReceived, this, &TableController::OnWebSocketMessage);

	// TODO send subscribe feed
}

void TableController::OnWebSocketDisconnected()
{
	qDebug() << "websocket, disconnected";

	mModel.removeRows(0, mModel.rowCount());

	QTimer::singleShot(1000, this, SLOT(OnWebSocketConnected()));
}

void TableController::OnWebSocketMessage(QString json)
{
	mDocument.Parse(json.toStdString().c_str());

	const std::string message = mDocument["message"].GetString();
	if (message == "feed_definition" && mModel.columnCount() == 0)
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

		mModel.insertColumns(0, columnCount);

		int column = 0;
		for (rj::Value::ConstValueIterator it = fields.Begin(); it != fields.End(); ++it)
		{
			const char* colName = it->GetString();

			qDebug() << "set col index" << column << "to" << colName;
			mModel.setHeaderData(column, Qt::Horizontal, QObject::tr(colName), Qt::DisplayRole);

			++column;
		}
	}
	else if (message == "feed")
	{
		assert(mModel.columnCount() != 0);

		mModel.insertRow(1);

		for (int column = 0; column < mModel.columnCount(); ++column)
		{
			QVariant data;

			const QString& field = mModel.headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();
			auto it = mDocument.FindMember(field.toStdString().c_str());
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
					qWarning() << "unspported type for field" << field;
				}
			}
			else
			{
				qWarning() << "field" << field << "not found";
			}

			mModel.setData(mModel.index(1, column), data, Qt::DisplayRole);
		}
	}
}
