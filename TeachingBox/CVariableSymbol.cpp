#include "stdafx.h"
#include "CVariableSymbol.h"





CVariableSymbol::CVariableSymbol(const QString& scope, const QString& name, const SymbolType type)
	:CSymbol(scope, name, CSymbol::CATEGORY_VARIABLE, type)
{
}

CVariableSymbol::~CVariableSymbol()
{

}
