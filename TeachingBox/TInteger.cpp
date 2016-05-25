#include "stdafx.h"
#include "TInteger.h"
#include "CParameterManager.h"
#include "CLineEditWithClickedSignal.h"
#include "CRegExpManager.h"
#include "CSymbol.h"
#include "CTreeWidgetItemWithVariate.h"



TInteger::TInteger(const QString& scope, const QString& name, const int value)
	:TVariate(scope,name,CSymbol::TYPE_INTERGER)
{
	m_value = value;
}

TInteger::TInteger(QDataStream& dataStream) :TVariate(dataStream)
{
	dataStream >> m_value;
}

int TInteger::GetValue()
{
	return m_value;
}

void TInteger::SetValue(const int value)
{
	m_value = value;
}

void TInteger::ReadValueStream(QDataStream& dataStream)
{
	dataStream << m_value;
}

void TInteger::UpdateFromValue(TVariate& variate)
{
	m_value = static_cast<TInteger&>(variate).m_value;
}

void TInteger::SlotOnTextChanged(const QString& newValue)
{
	UpdateRamAndDatabaseFrom(TInteger(m_symbol.GetScope(), m_symbol.GetName(), newValue.toInt()));
}


void TInteger::ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget)
{
	CTreeWidgetItemWithVariate* VariateItem = new CTreeWidgetItemWithVariate(parentItem, this);
	QTreeWidgetItem* item = new QTreeWidgetItem(VariateItem, QStringList("Value"));

	CLineEditWithClickedSignal* lineEditValue = new CLineEditWithClickedSignal(
		QString::number(m_value), CRegExpManager::STR_REG_INT);

	treeWidget->setItemWidget(item, 1, lineEditValue);

	connect(lineEditValue, SIGNAL(textChanged(const QString&)), this, SLOT(SlotOnTextChanged(const QString&)));
}

