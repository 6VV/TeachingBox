#include "stdafx.h"
#include "TInterpreterException.h"

QHash<int, QString> TInterpreterException::m_exceptionText = QHash<int, QString>{};

TInterpreterException::ExceptionInitialization::ExceptionInitialization()
{
	m_exceptionText.insert(UNKNOW_TOKEN, "unknown token");
	m_exceptionText.insert(WRONG_GRAMMAR, "wrong grammar");
	m_exceptionText.insert(NOT_FIND_RIGHT_BRACKET, "not find right bracket");
	m_exceptionText.insert(NEGATION_SHOULD_WITH_BOOL, "\"!\" should with bool");
	m_exceptionText.insert(NEGATIVE_SHOULD_WITH_INT_OR_DOUBLE, "\"-\" should with integer or double");

}

TInterpreterException::TInterpreterException(const InterpreterExceptionCode id, const int lineNumber,const QString& text)
	:m_id(id), m_lineNumber(lineNumber), m_text(text)
{
}

TInterpreterException::TInterpreterException(const InterpreterExceptionCode id, const int lineNumber)
	: m_id(id), m_lineNumber(lineNumber)
{
}

const QString TInterpreterException::GetInfo() const
{
	QString text = m_exceptionText[m_id] + "\nLine Number:" + QString::number(m_lineNumber);

	if (m_text.size()>0)
	{
		text.append("\nText:" + m_text);
	}
	return text;
}

