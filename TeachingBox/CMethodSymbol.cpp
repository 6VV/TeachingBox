#include "stdafx.h"
#include "CMethodSymbol.h"



CMethodSymbol::CMethodSymbol(const QString& scope, const QString& name, const SymbolType type)
	:CSymbol(scope, name, CATEGORY_METHOD, type), CScope(name)
{
}

CMethodSymbol::~CMethodSymbol()
{

}
