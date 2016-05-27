#include "stdafx.h"
#include "TPosition.h"
#include "CTreeWidgetItemWithVariate.h"
#include "CRegExpManager.h"
#include "CLineEditWithTree.h"



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
	CTreeWidgetItemWithVariate* variateItem = new CTreeWidgetItemWithVariate(parentItem, this);

	QVector<QTreeWidgetItem*> treeItems;
	for (int i = 0; i < AXIS_SIZE;++i)
	{
		QString name = "Axis"+QString::number(i + 1);

		treeItems.push_back(new QTreeWidgetItem(variateItem, QStringList{ name }));
	}

	QVector<CLineEditWithTree*> lineEdits;
	for (int i = 0; i < treeItems.size();++i)
	{
		CLineEditWithTree* lineEdit = new CLineEditWithTree(variateItem,treeWidget,
			QString::number(m_value[i]), CRegExpManager::STR_REG_FLOAT);

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
	CLineEditWithTree* currentWidget = static_cast<CLineEditWithTree*>(sender());
	QTreeWidgetItem* parentItem = currentWidget->GetParentItem();
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
