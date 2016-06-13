#include "stdafx.h"
#include "TString.h"
#include "CTreeWidgetItemWithVariate.h"



TString::TString(const QString& scope, const QString& name, const QString& value)
	:TVariate(scope,name,CSymbol::TYPE_STRING)
{
	m_string = value;
}

TString::TString(QDataStream& dataStream) : TVariate(dataStream)
{
	dataStream >> m_string;
}

const QString& TString::GetValue() const
{
	return m_string;
}

void TString::SetValue(const QString& value)
{
	m_string = value;
}

void TString::ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget)
{
	CTreeWidgetItemWithVariate* VariateItem = new CTreeWidgetItemWithVariate(parentItem, this);
	QTreeWidgetItem* item = new QTreeWidgetItem(VariateItem, QStringList("Value"));

	QLineEdit* lineEditValue = new QLineEdit(m_string);

	treeWidget->setItemWidget(item, 1, lineEditValue);

	connect(lineEditValue, SIGNAL(textChanged(const QString&)), this, SLOT(SlotOnTextChanged(const QString&)));
}

void TString::ReadValueStream(QDataStream& dataStream)const
{
	dataStream << m_string;
}

void TString::UpdateFromValue(const TVariate& variate)
{
	m_string = static_cast<const TString&>(variate).m_string;
}

void TString::SlotOnTextChanged(const QString& text)
{
	m_string = text;
	UpdateRamAndDatabaseFrom(*this);
}
