#include "stdafx.h"
#include "CHighLighter.h"
#include "CValue.h"



CHighLighter::CHighLighter(QTextDocument* parent/*=0*/) :QSyntaxHighlighter(parent)
{
	Init();
}

CHighLighter::~CHighLighter()
{

}

void CHighLighter::highlightBlock(const QString &text)
{
	for each (auto var in m_highlightingRules)
	{
		QRegExp expection(var.pattern);
		int index=expection.indexIn(text);

		while (index>=0)
		{
			int length = expection.matchedLength();
			setFormat(index, length, var.format);
			index = expection.indexIn(text, index + length);
		}
	}
}

void CHighLighter::Init()
{
	InitVariableRule();
	InitKeywordRule();
	InitFunctionRule();
	InitCommandRule();
	InitStructureRule();
	InitQuotationRule();
	InitDisableRule();
	InitCommentRule();
}

void CHighLighter::InitKeywordRule()
{
	HighlightRule rule;

	m_keywordFormat.setForeground(Qt::darkBlue);

	QStringList strListKeyword;
	strListKeyword << "TRUE" << "FALSE";
	for each (auto var in CValue::GetInstance()->m_mapLogicalCode)
	{
		strListKeyword.append(QString::fromStdString(var.first));
	}
	QStringList strListRegKeyword;
	for each (auto var in strListKeyword)
	{
		strListRegKeyword << "\\b"+ var + "\\b";
	}

	for each (auto var in strListRegKeyword)
	{
		rule.pattern = QRegExp(var);
		rule.format = m_keywordFormat;
		m_highlightingRules.append(rule);
	}
}

void CHighLighter::InitCommentRule()
{
	HighlightRule rule;

	m_commentFormat.setForeground(Qt::darkGreen);

	rule.pattern = QRegExp("//[^\n]*");
	rule.format = m_commentFormat;

	m_highlightingRules.append(rule);
}

void CHighLighter::InitDisableRule()
{
	HighlightRule rule;

	m_commentFormat.setForeground(Qt::darkRed);

	rule.pattern = QRegExp("#[^\n]*");
	rule.format = m_commentFormat;

	m_highlightingRules.append(rule);
}

void CHighLighter::InitQuotationRule()
{
	HighlightRule rule;

	m_quotationFormat.setForeground(Qt::green);

	rule.pattern = QRegExp("\".*\"");
	rule.format = m_quotationFormat;

	m_highlightingRules.append(rule);
}

void CHighLighter::InitFunctionRule()
{
	HighlightRule rule;

	m_functionFormat.setForeground(Qt::red);

	QStringList strListRegFunction;
	
	for each (auto var in CValue::GetInstance()->m_mapFunctionCode)
	{
		strListRegFunction << "\\b" + QString::fromStdString(var.first) + "\\b";
	}

	for each (auto var in strListRegFunction)
	{
		rule.pattern = QRegExp(var);
		rule.format = m_functionFormat;
		m_highlightingRules.append(rule);
	}
}

void CHighLighter::InitCommandRule()
{
	HighlightRule rule;

	m_functionFormat.setForeground(Qt::blue);

	QStringList strListReg;

	for each (auto var in CValue::GetInstance()->m_mapCommandCode)
	{
		strListReg << "\\b" + QString::fromStdString(var.first) + "\\b";
	}

	for each (auto var in strListReg)
	{
		rule.pattern = QRegExp(var);
		rule.format = m_functionFormat;
		m_highlightingRules.append(rule);
	}
}

void CHighLighter::InitStructureRule()
{
	HighlightRule rule;

	m_functionFormat.setForeground(QColor(0x9f,0x34,0xff));

	QStringList strList;
	strList << "FOR" << "TO" << "STEP"
		<< "NEXT" << "IF" << "ELSEIF" << "ENDIF"
		<< "SWITCH" << "CASE" << "DEFAULT" << "ENDSWITCH"
		<< "BREAK"
		<< "GOTO" << "GOSUB";
	QStringList strListReg;

	for each (auto var in strList)
	{
		strListReg << "\\b" + var + "\\b";
	}

	for each (auto var in strListReg)
	{
		rule.pattern = QRegExp(var);
		rule.format = m_functionFormat;
		m_highlightingRules.append(rule);
	}
}

void CHighLighter::InitVariableRule()
{
	HighlightRule rule;

	QColor color("#6c3365");
	m_variableFormat.setForeground(color);

	rule.pattern = QRegExp("[_a-zA-Z][_a-zA-Z0-9]*");
	rule.format = m_variableFormat;

	m_highlightingRules.append(rule);
}
