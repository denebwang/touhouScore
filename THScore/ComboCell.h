#pragma once
#include <QStyledItemDelegate>
class ComboCell : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ComboCell(QObject* parent = nullptr);
	~ComboCell();

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	void setEditorData(QWidget* editor, const QModelIndex& index) const override;

	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
