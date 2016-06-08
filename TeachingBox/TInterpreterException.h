#ifndef _TEACHING_BOX_T_INTERPRETER_EXCEPTION_H_
#define _TEACHING_BOX_T_INTERPRETER_EXCEPTION_H_

#include "QHash"
#include "QString"

class TInterpreterException
{
public:
	enum InterpreterExceptionCode
	{
		UNKNOW_TOKEN,
		WRONG_GRAMMAR,
		NOT_FIND_RIGHT_BRACKET,
		NEGATION_SHOULD_WITH_BOOL,
		NEGATIVE_SHOULD_WITH_INT_OR_DOUBLE,
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