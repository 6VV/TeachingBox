#include "stdafx.h"
#include "TVariate.h"
#include "TVariateManager.h"


TVariate::TVariate(QDataStream& dataStream) : m_symbol(dataStream)
{
}

TVariate::TVariate(const QString& scope, const QString& name, CSymbol::SymbolType type)
	: m_symbol(scope, name, CSymbol::CATEGORY_VARIABLE, type)
{

}

QString TVariate::GetScope() const
{
	return m_symbol.GetScope();
}

QString TVariate::GetName() const 
{
	return m_symbol.GetName();
}

CSymbol::SymbolType TVariate::GetType() const
{
	return m_symbol.GetType();
}

void TVariate::SetName(const QString& name)
{
	m_symbol.SetName(name);
}

void TVariate::ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget,
	const CSymbol::SymbolType type)
{
	if (m_symbol.GetType() == type)
	{
		this->ReadTreeWidgetItem(parentItem, treeWidget);
	}
}

void TVariate::ReadDataStream(QDataStream& dataStream) const
{
	m_symbol.ReadDataStream(dataStream);

	ReadValueStream(dataStream);
}

void TVariate::ReadCollection(SET& collection, const CSymbol::SymbolType type)
{
	if (m_symbol.GetType()==type)
	{
		collection.insert(this);
	}
}

void TVariate::UpdateFromVariate(const TVariate& variate)
{
	m_symbol = variate.m_symbol;
	UpdateFromValue(variate);
}

void TVariate::UpdateRamAndDatabaseFrom(TVariate& variate) const
{
	TVariateManager::GetInstance()->Update(m_symbol.GetScope(), m_symbol.GetName(), variate);
}

