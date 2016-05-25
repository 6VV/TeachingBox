#ifndef _INTERPRETER_C_TOKEN_STRING_H_
#define _INTERPRETER_C_TOKEN_STRING_H_

#include "CToken.h"

class CTokenString:public CToken
{
public:
	CTokenString(short largeCategory, short smallCategory, short lineNum,const std::string& strName);
	CTokenString(CToken* token);
	~CTokenString();

};

#endif