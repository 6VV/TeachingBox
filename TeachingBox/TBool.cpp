#include "stdafx.h"
#include "TBool.h"
#include "CTreeWidgetItemWithVariate.h"

const QString TBool::STR_VALUE_TRUE = "TRUE";
const QString TBool::STR_VALUE_FALSE = "FALSE";

TBool::TBool(const QString& scope, const QString& name, const bool value)
	:TVariate(scope, name, CSymbol::TYPE_BOOL), m_value(value)
{

}

TBool::TBool(QDataStream& dataStream) : TVariate(dataStream)
{
	dataStream >> m_value;
}

const bool TBool::GetValue() const
{
	return m_value;
}

void TBool::SetValue(const bool value)
{
	m_value = value;
}

void TBool::ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget)
{
	CTreeWidgetItemWithVariate* VariateItem = new CTreeWidgetItemWithVariate(parentItem, this);
	QTreeWidgetItem* item = new QTreeWidgetItem(VariateItem, QStringList("Value"));

	QComboBox* comboValue = new QComboBox;
	comboValue->addItem(STR_VALUE_FALSE);
	comboValue->addItem(STR_VALUE_TRUE);

	if (m_value==true)
	{
		comboValue->setCurrentText(STR_VALUE_TRUE);
	}
	else
	{
		comboValue->setCurrentText(STR_VALUE_FALSE);
	}

	treeWidget->setItemWidget(item, 1, comboValue);

	connect(comboValue, SIGNAL(currentIndexChanged(const QString&)), 
		this, SLOT(SlotOnIndexChanged(const QString&)));
}

void TBool::ReadValueStream(QDataStream& dataStream)const
{
	dataStream << m_value;
}

void TBool::UpdateFromValue(const TVariate& variate)
{
	m_value = static_cast<const TBool&>(variate).m_value;
}

void TBool::SlotOnIndexChanged(const QString& text)
{
	bool value = false;
	if (text==STR_VALUE_TRUE)
	{
		value = true;
	}
	UpdateRamAndDatabaseFrom(TBool(m_symbol.GetScope(), m_symbol.GetName(), value));
}

