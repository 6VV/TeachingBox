#include "stdafx.h"
#include "CToken.h"

CToken::CToken(short largeCategory, short smallCategory, short lineNum)
{
	this->m_largeCategory = largeCategory;
	this->m_smallCategory = smallCategory;
	this->m_lineNum = lineNum;
}

CToken::CToken(CToken* token)
{
	this->m_largeCategory = token->m_largeCategory;
	this->m_smallCategory = token->m_smallCategory;
	this->m_lineNum = token->m_lineNum;
	this->m_strName = token->m_strName;
	this->m_type = token->m_type;
	this->m_value = token->m_value;
}

CToken::CToken()
{

}

CToken::~CToken()
{

}
