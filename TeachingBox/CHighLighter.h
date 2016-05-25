#ifndef _TEACHING_BOX_C_HIGH_LIGHTER_H_
#define _TEACHING_BOX_C_HIGH_LIGHTER_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CHighLighter.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2016/03/21
//  Description:	高亮显示代码关键词
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2016/03/21
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#include "QSyntaxHighlighter"
#include "QTextDocument"
#include "QTextFormat"

class CHighLighter:public QSyntaxHighlighter
{
	Q_OBJECT

public:
	CHighLighter(QTextDocument* parent=0);
	~CHighLighter();

protected:
	void highlightBlock(const QString &text) Q_DECL_OVERRIDE;
	
private:
	void Init();

	void InitKeywordRule();
	void InitCommentRule();
	void InitDisableRule();
	void InitQuotationRule();
	void InitFunctionRule();
	void InitCommandRule();
	void InitStructureRule();
	void InitVariableRule();

private:
	struct HighlightRule 
	{
		QRegExp pattern;
		QTextCharFormat format;
	};

	QVector<HighlightRule> m_highlightingRules;

	QTextCharFormat m_variableFormat;	/*变量（深紫色）*/
	QTextCharFormat m_keywordFormat;	/*关键词（TRUE，AND等），（深蓝色）*/
	QTextCharFormat m_functionFormat;	/*函数（红色）*/
	QTextCharFormat m_commandFormat;	/*命令（蓝色）*/
	QTextCharFormat m_structureFormat;	/*结构（浅紫色）*/
	QTextCharFormat m_quotationFormat;	/*引号（绿色）*/
	QTextCharFormat m_disableFormat;	/*禁用（深红色）*/
	QTextCharFormat m_commentFormat;	/*注释（深绿色）*/
};


#endif