#pragma once

#include <QTableView>
#include <QKeyEvent>

#include <cassert>

class TableView : public QTableView
{
public:
	bool event(QEvent* e) override
	{
		QModelIndex idx = this->currentIndex();

		if (!mFocus && idx.row() == 0 && e->type() == QEvent::KeyRelease)
		{
			const QString pressedChar = static_cast<QKeyEvent*>(e)->text();

			QVariant currentValue = this->model()->data(idx);
			assert(currentValue.isValid() && currentValue.canConvert<QString>());

			this->model()->setData(idx, currentValue.toString() + pressedChar, Qt::UserRole);
		}

		return QTableView::event(e);
	}

	void focusInEvent(QFocusEvent* event) override
	{
		mFocus = true;
	}

	void focusOutEvent(QFocusEvent* event) override
	{
		mFocus = false;
	}

	void keyPressEvent(QKeyEvent* event) override
	{
		QTableView::keyPressEvent(event);
	}

private:
	bool mFocus{false};
};
