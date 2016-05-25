#include "stdafx.h"
#include "CTokenConstantNum.h"


CTokenConstantNum::CTokenConstantNum(short largeCategory, short smallCategory, short lineNum, int value) :CToken(largeCategory, smallCategory, lineNum)
{
	m_value.value_int = value;
}

CTokenConstantNum::CTokenConstantNum(short largeCategory, short smallCategory, short lineNum, double value) : CToken(largeCategory, smallCategory, lineNum)
{
	m_value.value_double = value;
}

CTokenConstantNum::CTokenConstantNum(short largeCategory, short smallCategory, short lineNum, bool value) : CToken(largeCategory, smallCategory, lineNum)
{
	m_value.value_bool = value;
}

CTokenConstantNum::CTokenConstantNum(CToken* token)
{
	CTokenConstantNum* tokenNum = static_cast<CTokenConstantNum*>(token);

	this->m_largeCategory = tokenNum->m_largeCategory;
	this->m_smallCategory = tokenNum->m_smallCategory;
	this->m_lineNum = tokenNum->m_lineNum;
	this->m_value = tokenNum->m_value;
}

CTokenConstantNum::~CTokenConstantNum()
{

}
