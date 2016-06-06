#ifndef _TEACHING_BOX_T_INTERPRETER_EXCEPTION_H_
#define _TEACHING_BOX_T_INTERPRETER_EXCEPTION_H_

#include "QHash"
#include "QString"

class TInterpreterException
{
public:
	enum InterpreterException
	{
		UNKNOW_TOKEN,
	};

public:
	TInterpreterException(const InterpreterException id,const int lineNumber,const QString& text);

	const QString GetInfo() const;

private:
	static QHash<int, QString> m_exceptionText;
	InterpreterException m_id;
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