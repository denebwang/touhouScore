#include "ComboCell.h"
#include <QComboBox>
ComboCell::ComboCell(QObject* parent) :QStyledItemDelegate(parent)
{
}

ComboCell::~ComboCell()
{
}

QWidget* ComboCell::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QComboBox* cb = new QComboBox(parent);
	QStringList str;
	str << "All" << "Mid+Boss" << "Mid+Boss+Bonus";
	cb->addItems(str);
	return cb;
}

void ComboCell::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QComboBox* cb = qobject_cast<QComboBox*>(editor);
	Q_ASSERT(cb);
	// get the index of the text in the combobox that matches the current value of the item
	const QString currentText = index.data(Qt::EditRole).toString();
	const int cbIndex = cb->findText(currentText);
	// if it is valid, adjust the combobox
	if (cbIndex >= 0)
		cb->setCurrentIndex(cbIndex);
}

void ComboCell::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QComboBox* cb = qobject_cast<QComboBox*>(editor);
	Q_ASSERT(cb);
	model->setData(index, cb->currentText(), Qt::EditRole);
}

void ComboCell::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}
