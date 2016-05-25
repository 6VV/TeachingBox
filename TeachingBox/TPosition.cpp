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
	}

	for (auto var:lineEdits)
	{
		connect(var, SIGNAL(textChanged(const QString&)), this, SLOT(SlotOnTextChanged()));
	}
	//QTreeWidgetItem *itemAxis1 = new QTreeWidgetItem(VariateItem, QStringList("Axis1"));
	//QTreeWidgetItem *itemAxis2 = new QTreeWidgetItem(VariateItem, QStringList("Axis2"));
	//QTreeWidgetItem *itemAxis3 = new QTreeWidgetItem(VariateItem, QStringList("Axis3"));
	//QTreeWidgetItem *itemAxis4 = new QTreeWidgetItem(VariateItem, QStringList("Axis4"));
	//QTreeWidgetItem *itemAxis5 = new QTreeWidgetItem(VariateItem, QStringList("Axis5"));
	//QTreeWidgetItem *itemAxis6 = new QTreeWidgetItem(VariateItem, QStringList("Axis6"));

	//CLineEditWithClickedSignal* lineEditAxis1 = new CLineEditWithClickedSignal(
	//	QString::number(m_value[0]), CRegExpManager::STR_REG_FLOAT);
	//CLineEditWithClickedSignal* lineEditAxis2 = new CLineEditWithClickedSignal(
	//	QString::number(m_value[1]), CRegExpManager::STR_REG_FLOAT);
	//CLineEditWithClickedSignal* lineEditAxis3 = new CLineEditWithClickedSignal(
	//	QString::number(m_value[2]), CRegExpManager::STR_REG_FLOAT);
	//CLineEditWithClickedSignal* lineEditAxis4 = new CLineEditWithClickedSignal(
	//	QString::number(m_value[3]), CRegExpManager::STR_REG_FLOAT);
	//CLineEditWithClickedSignal* lineEditAxis5 = new CLineEditWithClickedSignal(
	//	QString::number(m_value[4]), CRegExpManager::STR_REG_FLOAT);
	//CLineEditWithClickedSignal* lineEditAxis6 = new CLineEditWithClickedSignal(
	//	QString::number(m_value[5]), CRegExpManager::STR_REG_FLOAT);

	//treeWidget->setItemWidget(itemAxis1, 1, lineEditAxis1);
	//treeWidget->setItemWidget(itemAxis2, 1, lineEditAxis2);
	//treeWidget->setItemWidget(itemAxis3, 1, lineEditAxis3);
	//treeWidget->setItemWidget(itemAxis4, 1, lineEditAxis4);
	//treeWidget->setItemWidget(itemAxis5, 1, lineEditAxis5);
	//treeWidget->setItemWidget(itemAxis6, 1, lineEditAxis6);

	///*信号槽*/

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
