#ifndef _INTERPRETER_C_TOKEN_H_
#define _INTERPRETER_C_TOKEN_H_

#include "CTokenCategory.h"

class CToken
{
public:
	CToken();
	CToken(short largeCategory, short smallCategory,short m_lineNum);
	CToken(CToken*);
	~CToken();

	short m_largeCategory;
	short m_smallCategory;
	short m_lineNum;

	std::string m_strName;
	int m_type;

	union
	{
		int value_int;
		double value_double;
		bool value_bool;
	}m_value;
};

#endif