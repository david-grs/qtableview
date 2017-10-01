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

public slots:
	void sort(int, Qt::SortOrder);

private slots:
	void webSocketConnected();
	void webSocketDisconnected();
	void webSocketMessage(QString message);

private:
	QUrl mUrl;
	QWebSocket mWebSocket;

	rapidjson::Document mDocument; // no state, re-use only

	QTableView& mView;
	QAbstractItemModel& mModel;

};
