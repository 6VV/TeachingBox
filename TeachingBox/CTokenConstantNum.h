#ifndef _INTERPRETER_C_TOKEN_FLOAT_H_
#define _INTERPRETER_C_TOKEN_FLOAT_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CTokenConstantNum.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2016/02/18
//  Description:	存储常数词法单元，包括该常数的类型、位置以及具体值
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2016/02/18
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#include "CToken.h"

class CTokenConstantNum:public CToken
{
public:
	CTokenConstantNum(short largeCategory,short smallCategory,short lineNum,int value);
	CTokenConstantNum(short largeCategory, short smallCategory, short lineNum, double value);
	CTokenConstantNum(short largeCategory, short smallCategory, short lineNum, bool value);

	CTokenConstantNum(CToken* token);

	~CTokenConstantNum();
};

#endif