/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CExceptionInterpreter.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2016/02/18
//  Description:	异常类，用于解释器中给出程序错误信息
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2016/02/18
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _INTERPRETER_C_EXCEPTION_INTERPRETER_H_
#define _INTERPRETER_C_EXCEPTION_INTERPRETER_H_

#include <string>

class CExceptionInterpreter
{
public:
	CExceptionInterpreter(int infoCode);
	CExceptionInterpreter(CExceptionInterpreter& e);
	CExceptionInterpreter(int infoCode, int lineNum, std::string& strText);
	CExceptionInterpreter(int infoCode, int lineNum);
	~CExceptionInterpreter();

	void SetLineNumber(int lineNumber);
	void SetWrongText(std::string& strText);

	const int GetInfoCode();
	const int GetLineNumber();
	const std::string GetWrongText();

protected:
	
private:
	int m_infoCode;
	int m_lineNumber;
	std::string m_strText;
};

#endif