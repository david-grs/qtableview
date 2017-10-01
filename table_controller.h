#pragma once

#include <QObject>
#include <QtWebSockets/QWebSocket>

#include <rapidjson/document.h>

class QTableView;
class QAbstractItemModel;

class TableController : public QObject
{
	Q_OBJECT

public:
	TableController(QTableView&, QAbstractItemModel&);

private slots:
	void OnWebSocketConnected();
	void OnWebSocketDisconnected();
	void OnWebSocketMessage(QString message);

private:
	QUrl mUrl;
	QWebSocket mWebSocket;

	rapidjson::Document mDocument; // no state, re-use only

	QTableView& mView;
	QAbstractItemModel& mModel;

};
