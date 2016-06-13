#include "stdafx.h"
#include "CExceptionInterpreter.h"



CExceptionInterpreter::CExceptionInterpreter(int infoCode)
{
	m_infoCode = infoCode;
}

CExceptionInterpreter::CExceptionInterpreter(const CExceptionInterpreter& e)
{
	m_infoCode = e.GetInfoCode();
	m_lineNumber = e.GetLineNumber();
	m_strText = e.GetWrongText();
}

CExceptionInterpreter::CExceptionInterpreter(int infoCode, int lineNum, const std::string& strText)
{
	m_infoCode = infoCode;
	m_lineNumber = lineNum;
	m_strText = strText;
}

CExceptionInterpreter::CExceptionInterpreter(int infoCode, int lineNum)
{
	m_infoCode = infoCode;
	m_lineNumber = lineNum;
}

CExceptionInterpreter::~CExceptionInterpreter()
{

}

void CExceptionInterpreter::SetLineNumber(int lineNumber)
{
	m_lineNumber = lineNumber;
}

void CExceptionInterpreter::SetWrongText(std::string& strText)
{
	m_strText = strText;
}

const int CExceptionInterpreter::GetInfoCode() const 
{
	return m_infoCode;
}

const int CExceptionInterpreter::GetLineNumber() const
{
	return m_lineNumber;
}

const std::string CExceptionInterpreter::GetWrongText() const
{
	return m_strText;
}
