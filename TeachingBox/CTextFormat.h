#ifndef _TEACHING_BOX_C_TEXT_FORMAT_H_
#define _TEACHING_BOX_C_TEXT_FORMAT_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CTextFormat.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2016/03/21
//  Description:	文本格式化
//  Others:			若为运算符（算数运算符、逻辑运算符、比较运算符），则两边加空格；若为逗号，则右边加空格；
					若为括号或变量，则不加空格。
//  History:
//    <author>      刘巍 
//    <time>        2016/03/21
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

class CTextFormat
{
public:
	CTextFormat();
	~CTextFormat();

	void FormatText(const QString& strOldText,QString& strNewText);

	void AddRetract(QString& strText, int retractNum);
	void FormatLineText(QString& strOldText);

protected:
	
private:
	QString strRetract="    ";	/*缩进四个字符*/
};

#endif