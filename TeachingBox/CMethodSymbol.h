#ifndef _INTERPRETER_C_METHOD_SYMBOL_H_
#define _INTERPRETER_C_METHOD_SYMBOL_H_

#include "CSymbol.h"
#include "CScope.h"

class CMethodSymbol:public CSymbol,public CScope
{
public:
	CMethodSymbol(const QString& scope, const QString& name, const SymbolType type);
	~CMethodSymbol();
};

#endif