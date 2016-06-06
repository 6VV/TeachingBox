#ifndef _TEACHING_BOX_T_TOKEN_H_
#define _TEACHING_BOX_T_TOKEN_H_

#include "CSymbol.h"

class TToken
{
public:
	enum TokenType
	{
		ID = 1,

		LITERAL_INTERGER = 100,
		LITERAL_DOUBLE,
		LITERAL_BOOL,
		LITERAL_STRING,

		OPERATOR_PLUS = 200,
		OPERATOR_MINUS,
		OPERATOR_MULTIPLY,
		OPERATOR_DIVIDE,
		OPERATOR_POWER,
		OPERATOR_PERCENT,

		OPERATOR_AND,
		OPERATOR_OR,

		OPERATOR_EQUAL,
		OPERATOR_NOT_EQUAL,
		OPERATOR_GREATE_THAN,
		OPERATOR_GREATE_EQUAL,
		OPERATOR_LESS_THAN,
		OPERATOR_LESS_EQUAL,

		OPERATOR_ASSIGN,
		OPERATOR_NOT,

		OPERATOR_LEFT_BRACKET,
		OPERATOR_RIGHT_BRACKET,

		SEPARATOR_EOL = 300,
		SERARATOR_EOF,

		VALUE_TRUE = 400,
		VALUE_FALSE,

		STRUCTURE_FOR=500,
		STRUCTURE_NEXT,
		STRUCTURE_BREAK,
		STRUCTURE_CONTINUE,
		STRUCTURE_IF,
		STRUCTURE_ELSE_IF,
		STURCTURE_ELSE,
		STURCTURE_END_IF,
		STURCTURE_SWITCH,
		STURCTURE_CASE,
		STURCTURE_DEFAULT,
		STURCTURE_END_SWITCH,

		MACRO_MOVL=600,
		MACRO_MOVC,
	};

	static QHash<QString, TokenType> m_keyMap;
	static QHash<QString, TokenType> m_structureMap;
	static QHash<QString, TokenType> m_macroMap;
	static QHash<QString, TokenType> m_reserveValueMap;

public:
	TToken(const TokenType type, const int lineNumber);
	virtual ~TToken();

	const int GetLineNumber() const;
	const TokenType GetType() const;

private:
	TokenType m_type;
	int m_lineNumber;

private:
	class Initlization
	{
	public:
		Initlization();

	private:
		void InitKeyMap();
		void InitReserveValueMap();
		void InitMacroMap();
		void InitStructureMap();
	};

	static Initlization m_initlization;
};

#endif