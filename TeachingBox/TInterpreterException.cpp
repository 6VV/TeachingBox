#include "stdafx.h"
#include "TInterpreterException.h"

QHash<int, QString> TInterpreterException::m_exceptionText = QHash<int, QString>{};

TInterpreterException::ExceptionInitialization::ExceptionInitialization()
{
	m_exceptionText.insert(UNKNOW_TOKEN, "Unknown Token");
}

TInterpreterException::TInterpreterException(const InterpreterException id, const int lineNumber,const QString& text)
	:m_id(id), m_lineNumber(lineNumber), m_text(text)
{
}

const QString TInterpreterException::GetInfo() const
{
	return m_exceptionText[m_id]
		+ "\nText:" + m_text
		+"\nLine Number:"+QString::number(m_lineNumber);
}

