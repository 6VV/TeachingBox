#include "stdafx.h"
#include "TInterpreterException.h"

QHash<int, QString> TInterpreterException::m_exceptionText = QHash<int, QString>{};

TInterpreterException::ExceptionInitialization TInterpreterException::m_initialzation{};

TInterpreterException::ExceptionInitialization::ExceptionInitialization()
{
	m_exceptionText.insert(UNKNOW_TOKEN, "unknown token");
	m_exceptionText.insert(WRONG_GRAMMAR, "wrong grammar");
	m_exceptionText.insert(NOT_FIND_RIGHT_BRACKET, "not find right bracket");
	m_exceptionText.insert(NEGATION_SHOULD_WITH_BOOL, "\"!\" should with bool");
	m_exceptionText.insert(NEGATIVE_SHOULD_WITH_INT_OR_DOUBLE, "\"-\" should with integer or double");
	m_exceptionText.insert(NOT_FIND_VARIATE, "not find variate");
	m_exceptionText.insert(ONLY_NUMBER_COULD_TAKE_ARITHMETICAL_OPERATION, "only number could take part in arithmetical opration");
	m_exceptionText.insert(ONLY_NUMBER_COULD_TAKE_RELATIONAL_OPERATION, "only number could take part in relational opration");
	m_exceptionText.insert(ONLY_BOOL_COULD_TAKE_LOGICAL_OPERATION, "only bool could take part in logical opration");
	m_exceptionText.insert(ONLY_INTERGER_COULD_TAKE_MOD_OPERATION, "only interger could take part in mod opration");
	m_exceptionText.insert(SENTENCE_NOT_END_WITH_ABNORMAL_END, "sentence not end with abnormal end");
	m_exceptionText.insert(SENTENCE_SHOULD_END_WITH_LINE_BREAK, "sentence should end with line break");
	m_exceptionText.insert(TYPE_NOT_MATCH, "type not match");
	m_exceptionText.insert(FOR_SENTENCE_SHOULD_END_WITH_NEXT, "for sentence should end with next");
	m_exceptionText.insert(FOR_SENTENCE_SHOULD_USE_INTERGER_OR_DOUBLE, "for sentence should use interger or double");
	m_exceptionText.insert(IF_SENTENCE_SHOULD_END_WITH_ENDIF, "if sentence should end with endif");
	m_exceptionText.insert(IF_SENTENCE_SHOULD_WITH_BOOL, "if sentence should  with bool");
	m_exceptionText.insert(NOT_MOVL_SENTENCE, "not movl sentence");
	m_exceptionText.insert(MOVL_SHOULD_WITH_POSITOIN_DYNAMIC_OVERLAP, "movl should should be with position,dynamic,overlap");
	m_exceptionText.insert(LOSE_COMMA, "lose comma");
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
	QString text = "Information: "+m_exceptionText[m_id] + "\nLine Number: " + QString::number(m_lineNumber);

	if (m_text.size()>0)
	{
		text.append("\nText: " + m_text);
	}
	return text;
}

