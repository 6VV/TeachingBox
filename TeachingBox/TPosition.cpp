#include "stdafx.h"
#include "TPosition.h"
#include "CTreeWidgetItemWithVariate.h"
#include "CLineEditWithClickedSignal.h"
#include "CRegExpManager.h"



TPosition::TPosition(const QString& scope, const QString& name, const TYPE_POSITION& value)
	:TVariate(scope,name,CSymbol::TYPE_POSITION)
{
	m_value = value;
}

TPosition::TPosition(QDataStream& dataStream) : TVariate(dataStream)
{
	double value;

	for (int i = 0; i < AXIS_SIZE;++i)
	{
		dataStream >> value;
		m_value.push_back(value);
	}
}

const TPosition::TYPE_POSITION& TPosition::GetValue() const
{
	return m_value;
}

void TPosition::SetValue(const TYPE_POSITION& value)
{
	m_value = value;
}

void TPosition::ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget)
{
	CTreeWidgetItemWithVariate* VariateItem = new CTreeWidgetItemWithVariate(parentItem, this);

	QVector<QTreeWidgetItem*> treeItems;
	for (int i = 0; i < AXIS_SIZE;++i)
	{
		QString name = "Axis"+QString::number(i + 1);

		treeItems.push_back(new QTreeWidgetItem(VariateItem, QStringList{ name }));
	}

	QVector<CLineEditWithClickedSignal*> lineEdits;
	for (int i = 0; i < treeItems.size();++i)
	{
		CLineEditWithClickedSignal* lineEdit = new CLineEditWithClickedSignal(
			QString::number(m_value[i]), CRegExpManager::STR_REG_FLOAT);
		lineEdit->SetTreeItem(VariateItem,treeWidget);

		lineEdits.push_back(lineEdit);
		treeWidget->setItemWidget(treeItems.at(i), 1, lineEdit);

		connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(SlotOnTextChanged()));
	}

}

void TPosition::ReadValueStream(QDataStream& dataStream)
{
	for (auto var:m_value)
	{
		dataStream << var;
	}
}

void TPosition::UpdateFromValue(TVariate& variate)
{
	m_value = static_cast<TPosition&>(variate).m_value;
}

void TPosition::SlotOnTextChanged()
{
	CLineEditWithClickedSignal* currentWidget = static_cast<CLineEditWithClickedSignal*>(sender());
	QTreeWidgetItem* parentItem = currentWidget->GetTreeWidgetItem();
	QTreeWidget* treeWidget = currentWidget->GetTreeWidget();

	TYPE_POSITION position;
	for (int i = 0; i < parentItem->childCount();++i)
	{
		QLineEdit* lineEdit=static_cast<QLineEdit*>(treeWidget->itemWidget(parentItem->child(i), 1));
		position.push_back(lineEdit->text().toDouble());
	}

	m_value = position;
	UpdateRamAndDatabaseFrom(*this);
}
