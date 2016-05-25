#include "stdafx.h"
#include "CSymbol.h"


CSymbol::CSymbol(const QString& scope, const QString& name, const SymbolCategory category, const SymbolType type)
{
	m_scope = scope;
	m_name = name;
	m_category = category;
	m_type = type;
}

CSymbol::CSymbol(QDataStream& dataStream)
{
	dataStream >> m_scope;
	dataStream >> m_name;
	dataStream >> m_type;
}

CSymbol& CSymbol::operator=(const CSymbol& symbol)
{
	m_scope = symbol.m_scope;
	m_name = symbol.m_name;
	m_type = symbol.m_type;
	m_category = symbol.m_category;

	return *this;
}

CSymbol::SymbolType CSymbol::GetType() const
{
	return static_cast<SymbolType>(m_type);
}

CSymbol::SymbolCategory CSymbol::GetCategory() const
{
	return m_category;
}

QString CSymbol::GetName() const
{
	return m_name;
}

QString CSymbol::GetScope() const
{
	return m_scope;
}

void CSymbol::ReadDataStream(QDataStream& dataStream)
{
	dataStream << m_scope;
	dataStream << m_name;
	dataStream << m_type;
}

QHash<CSymbol::SymbolType, QString> CSymbol::Map_Type = QHash<CSymbol::SymbolType, QString>{};

CSymbol::CInitMap CSymbol::m_init = CSymbol::CInitMap{Map_Type};
