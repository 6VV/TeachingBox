#include "stdafx.h"
#include "CTokenString.h"



CTokenString::CTokenString(short largeCategory, short smallCategory, short lineNum,const std::string& strName) :CToken(largeCategory, smallCategory,lineNum)
{
	m_strName = strName;
}

CTokenString::CTokenString(CToken* token)
{
	CTokenString* tokenString=static_cast<CTokenString*>(token);

	this->m_largeCategory = tokenString->m_largeCategory;
	this->m_smallCategory = tokenString->m_smallCategory;
	this->m_lineNum = tokenString->m_lineNum;
	this->m_strName = tokenString->m_strName;
	this->m_type = tokenString->m_type;
}

CTokenString::~CTokenString()
{

}
