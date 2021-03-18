#pragma once
#include <QStyledItemDelegate>
#include <QStringList>
class ComboCell : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ComboCell(QStringList optionStrings, QObject* parent = nullptr);
	~ComboCell();

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	void setEditorData(QWidget* editor, const QModelIndex& index) const override;

	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
private:
	QStringList str;
};
