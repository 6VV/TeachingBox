#ifndef _INTERPRETER_C_WARNING_INFORMATION_H_
#define _INTERPRETER_C_WARNING_INFORMATION_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CWarningInformation.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2016/02/18
//  Description:	存储警告信息
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2016/02/18
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#include <unordered_map>

class CWarningInformation
{

private:
	CWarningInformation();
	~CWarningInformation();

public:

	enum WARNING_CODE
	{
		UNKNOWN_TOKEN,		/*未知的词法单元*/
		WRONG_GRAMMAR,		/*语法错误*/
		UNDEFINED_VARIANT,	/*未定义的变量*/
		UNKNOWN_LABEL,		/*未定义的标签*/
		UNMATCH_TYPE,		/*类型不匹配*/
		IF_WITH_NOT_BOOL,	/*if之后不是布尔值*/
		ELSEIF_WITH_NOT_BOOL,	/*elseif之后不是布尔值*/
		BREAK_NOT_IN_LOOP_OR_SWITCH,	/*break不在循环或开关中*/
		SWITCH_WITH_NOT_INT,	/*switch后跟的不是整数*/
		CASE_WITH_NOT_INT,	/*case后跟的不是整数*/
		PARAMETER_NOT_MATCH,	/*参数不匹配*/
		DIVISOR_IS_ZERO,	/*除数为0*/
		NOT_PROGRAM,		/*不是函数*/
		NOT_MEET_END,			/*函数未遇到end*/
		TOO_MANY_MAIN_PROGRAM,	/*过多的主函数*/
		EXIST_SAME_PROGRAM,		/*已存在改函数*/
		NOT_FIND_MAIN_PROGRAM,	/*未找到主函数*/
		NOT_EXIST_THIS_PROGRAM,	/*不存在该函数*/
		NOT_FIND_EOF,		/*未找到EOF*/
		NOT_FIND_STRART_PROGRAM,	/*未找到启动函数*/
		EMPTY,	/*空文本*/
	};

};



#endif