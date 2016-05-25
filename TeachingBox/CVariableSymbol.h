#ifndef _INTERPERTER_C_VARIABLE_SYMBOL_H_
#define _INTERPERTER_C_VARIABLE_SYMBOL_H_

#include "CSymbol.h"

class CVariableSymbol:public CSymbol
{
public:
	CVariableSymbol(const QString& scope, const QString& name, const SymbolType type);
	~CVariableSymbol();
	
};

#endif