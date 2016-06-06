#include "stdafx.h"
#include "TToken.h"

QHash<QString, TToken::TokenType> TToken::m_keyMap = QHash<QString, TToken::TokenType>{};

QHash<QString, TToken::TokenType> TToken::m_structureMap = QHash<QString, TToken::TokenType>{};

QHash<QString, TToken::TokenType> TToken::m_macroMap = QHash<QString, TToken::TokenType>{};

QHash<QString, TToken::TokenType> TToken::m_reserveValueMap = QHash<QString, TToken::TokenType>{};

TToken::TToken(const TokenType type, const int lineNumber)
	:m_type(type), m_lineNumber(lineNumber)
{
}

TToken::~TToken()
{

}

const int TToken::GetLineNumber() const
{
	return m_lineNumber;
}

const TToken::TokenType TToken::GetType() const
{
	return m_type;
}


TToken::Initlization TToken::m_initlization = TToken::Initlization{};

TToken::Initlization::Initlization()
{
	InitStructureMap();
	InitMacroMap();
	InitReserveValueMap();

	InitKeyMap();
}

void TToken::Initlization::InitKeyMap()
{
	m_keyMap.unite(m_structureMap);
	m_keyMap.unite(m_macroMap);
	m_keyMap.unite(m_reserveValueMap);
}

void TToken::Initlization::InitReserveValueMap()
{
	m_reserveValueMap.insert("TRUE", VALUE_TRUE);
	m_reserveValueMap.insert("FALSE", VALUE_FALSE);
}

void TToken::Initlization::InitMacroMap()
{
	m_macroMap.insert("MOVL", MACRO_MOVL);
	m_macroMap.insert("MOVC", MACRO_MOVC);
}

void TToken::Initlization::InitStructureMap()
{
	m_structureMap.insert("FOR", STRUCTURE_FOR);
	m_structureMap.insert("NEXT", STRUCTURE_NEXT);
	m_structureMap.insert("BREAK", STRUCTURE_BREAK);
	m_structureMap.insert("CONTINUE", STRUCTURE_CONTINUE);
	m_structureMap.insert("IF", STRUCTURE_IF);
	m_structureMap.insert("ELSEIF", STRUCTURE_ELSE_IF);
	m_structureMap.insert("ELSE", STURCTURE_ELSE);
	m_structureMap.insert("ENDIF", STURCTURE_END_IF);
	m_structureMap.insert("SWITCH", STURCTURE_SWITCH);
	m_structureMap.insert("CASE", STURCTURE_CASE);
	m_structureMap.insert("DEFAULT", STURCTURE_DEFAULT);
	m_structureMap.insert("ENDSWITCH", STURCTURE_END_SWITCH);
}
