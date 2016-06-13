#include "stdafx.h"
#include "TDouble.h"
#include "CTreeWidgetItemWithVariate.h"
#include "CRegExpManager.h"
#include "CLineEditWithRegExpAndKeyboard.h"

TDouble::TDouble(const QString& scope, const QString& name, const double value)
	:TVariate(scope, name, CSymbol::TYPE_DOUBLE)
{
	m_value = value;
}

TDouble::TDouble(QDataStream& dataStream) : TVariate(dataStream)
{
	dataStream >> m_value;
}

double TDouble::GetValue()
{
	return m_value;
}

void TDouble::SetValue(const double value)
{
	m_value = value;
}

void TDouble::ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget)
{
	CTreeWidgetItemWithVariate* VariateItem = new CTreeWidgetItemWithVariate(parentItem, this);
	QTreeWidgetItem* item = new QTreeWidgetItem(VariateItem, QStringList("Value"));

	CLineEditWithRegExpAndKeyboard* lineEditValue = new CLineEditWithRegExpAndKeyboard(
		QString::number(m_value), CRegExpManager::STR_REG_FLOAT);

	treeWidget->setItemWidget(item, 1, lineEditValue);

	connect(lineEditValue, SIGNAL(textChanged(const QString&)), this, SLOT(SlotOnTextChanged(const QString&)));

}

void TDouble::ReadValueStream(QDataStream& dataStream)const
{
	dataStream << m_value;
}

void TDouble::UpdateFromValue(const TVariate& variate)
{
	m_value = static_cast<const TDouble&>(variate).m_value;
}

void TDouble::SlotOnTextChanged(const QString& newValue)
{
	UpdateRamAndDatabaseFrom(TDouble(m_symbol.GetScope(), m_symbol.GetName(), newValue.toDouble()));
}

