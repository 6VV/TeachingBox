#ifndef _TEACHING_BOX_T_INTERPRETER_EXCEPTION_H_
#define _TEACHING_BOX_T_INTERPRETER_EXCEPTION_H_

#include "QHash"
#include "QString"

class TInterpreterException
{
public:
	enum InterpreterExceptionCode
	{
		FOR_SENTENCE_SHOULD_END_WITH_NEXT,
		FOR_SENTENCE_SHOULD_USE_INTERGER_OR_DOUBLE,
		IF_SENTENCE_SHOULD_END_WITH_ENDIF,
		IF_SENTENCE_SHOULD_WITH_BOOL,
		UNKNOW_TOKEN,
		WRONG_GRAMMAR,
		NOT_FIND_RIGHT_BRACKET,
		NEGATION_SHOULD_WITH_BOOL,
		NEGATIVE_SHOULD_WITH_INT_OR_DOUBLE,
		NOT_FIND_VARIATE,
		ONLY_NUMBER_COULD_TAKE_ARITHMETICAL_OPERATION,
		ONLY_NUMBER_COULD_TAKE_RELATIONAL_OPERATION,
		ONLY_BOOL_COULD_TAKE_LOGICAL_OPERATION,
		ONLY_INTERGER_COULD_TAKE_MOD_OPERATION,
		SENTENCE_NOT_END_WITH_ABNORMAL_END,
		SENTENCE_SHOULD_END_WITH_LINE_BREAK,
		TYPE_NOT_MATCH,

	};

public:
	TInterpreterException(const InterpreterExceptionCode id,const int lineNumber,const QString& text);
	TInterpreterException(const InterpreterExceptionCode id, const int lineNumber);

	const QString GetInfo() const;

private:
	static QHash<int, QString> m_exceptionText;
	InterpreterExceptionCode m_id;
	int m_lineNumber;
	QString m_text;

private:
	class ExceptionInitialization
	{
	public:
		ExceptionInitialization();
	};

	static ExceptionInitialization m_initialzation;
};

#endif