#pragma once

#include <QTableView>
#include <QKeyEvent>

class TableView : public QTableView
{
public:
	bool event(QEvent* e) override
	{
		/*
		QModelIndex idx = this->currentIndex();
		if (!mFocus
			&& this->state() == QAbstractItemView::EditingState
			&& idx.row() == 0
			&& e->type() == QEvent::KeyRelease)
		{
			const QString pressedChar = static_cast<QKeyEvent*>(e)->text();

			QVariant currentValue = this->model()->data(idx);
			assert(currentValue.isValid() && currentValue.canConvert<QString>());

			this->model()->setData(idx, currentValue.toString() + pressedChar, Qt::UserRole);
		}
		*/

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


#include <QStyledItemDelegate>
#include <QLineEdit>

class TableDelegate : public QStyledItemDelegate
{
public:
	explicit TableDelegate(QAbstractItemModel& model) :
		mModel(model)
	{}

	void setEditorData(QWidget* editor, const QModelIndex& index) const override
	{
		QStyledItemDelegate::setEditorData(editor, index);

		QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
		if (lineEdit)
		{
			connect(lineEdit, &QLineEdit::textChanged, this, [this, index](const QString& text)
			{
				mModel.setData(index, text, Qt::UserRole);
			});
		}
	}

private:
	QAbstractItemModel& mModel;
};
