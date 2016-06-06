#include "stdafx.h"
#include "TLexer.h"
#include "TTokenWithValue.h"
#include "TInterpreterException.h"


TLexer::TLexer(const QString& text) :m_reader(text)
{
	Parse();
}

TLexer::~TLexer()
{
}

const std::shared_ptr<TToken> TLexer::GetToken() const
{
	if (m_index>=m_tokens.size())
	{
		return nullptr;
	}

	return m_tokens.at(m_index++);
}

const bool TLexer::PushDigit(const char c)
{
	if (!IsDigit(c))
	{
		return false;
	}

	QString text(c);
	GetInterger(text);

	char currentChar = m_reader.GetCurrentChar();
	if (currentChar == '.')
	{
		text.append(currentChar);

		if (!IsDigit(m_reader.PeekChar(1)))
		{
			ThrowException(text);
		}
		else
		{
			GetInterger(text);
			m_tokens.push_back(std::shared_ptr<TToken>(new TTokenWithValue<double>(TToken::LITERAL_DOUBLE, m_reader.GetLineNumber(), text.toDouble())));
		}
	}
	else
	{
		m_tokens.push_back(std::shared_ptr<TToken>(new TTokenWithValue<int>(TToken::LITERAL_INTERGER, m_reader.GetLineNumber(), text.toInt())));
	}

	return true;
}

const bool TLexer::PushId(const char c)
{
	if (!IsLetter(c) && c!='_' )
	{
		return false;
	}

	QString text(c);
	GetId(text);

	if (PushKeyWord(text))
	{
		return true;
	}

	m_tokens.push_back(std::shared_ptr<TTokenWithValue<QString>>(new TTokenWithValue<QString>(TToken::ID, m_reader.GetLineNumber(), text)));
	return true;
}

const bool TLexer::PushOtherToken(const char c)
{
	switch (c)
	{
	case '\n':{PushToken(TToken::SEPARATOR_EOL);}break;
	case '+':{PushToken(TToken::OPERATOR_PLUS);}break;
	case '-':{PushToken(TToken::OPERATOR_MINUS);}break;
	case '*':{PushToken(TToken::OPERATOR_MULTIPLY);}break;
	case '/':{PushToken(TToken::OPERATOR_DIVIDE);}break;
	case '^':{PushToken(TToken::OPERATOR_POWER);}break;
	case '%':{PushToken(TToken::OPERATOR_PERCENT);}break;
	case '&':{PushToken(TToken::OPERATOR_AND);}break;
	case '|':{PushToken(TToken::OPERATOR_OR);}break;
	case '(':{PushToken(TToken::OPERATOR_LEFT_BRACKET);}break;
	case ')':{PushToken(TToken::OPERATOR_RIGHT_BRACKET);}break;
	case '=':{
		if (m_reader.PeekChar(1)=='='){
			PushToken(TToken::OPERATOR_EQUAL);
			m_reader.GetNextChar();
		}else{
			PushToken(TToken::OPERATOR_ASSIGN);
		}
	}break;
	case '!':{
		switch (m_reader.GetNextChar()){
		case '=':{PushToken(TToken::OPERATOR_NOT_EQUAL); break; };
		case '>':{PushToken(TToken::OPERATOR_LESS_EQUAL); break; };
		case '<':{PushToken(TToken::OPERATOR_GREATE_EQUAL); break; };
		default:{PushToken(TToken::OPERATOR_NOT); m_reader.UnGetChar(); break; };
		}
	}break;
	case '>':{
		if (m_reader.PeekChar(1)=='='){
			PushToken(TToken::OPERATOR_GREATE_EQUAL);
			m_reader.GetNextChar();
		}else{
			PushToken(TToken::OPERATOR_GREATE_THAN);
		}
	}break;
	case '<':{
		if (m_reader.PeekChar(1) == '='){
			PushToken(TToken::OPERATOR_LESS_EQUAL);
			m_reader.GetNextChar();
		}
		else{
			PushToken(TToken::OPERATOR_LESS_THAN);
		}
	}break;
	default:{return false;}
	}

	m_reader.GetNextChar();
	return true;
}

void TLexer::ThrowException(const QString& text)
{
	throw TInterpreterException(TInterpreterException::UNKNOW_TOKEN, m_reader.GetLineNumber(), text);
}

void TLexer::GetInterger(QString &text)
{
	char currentChar;

	while (IsDigit(currentChar = m_reader.GetNextChar()))
	{
		text.append(currentChar);
	}
}

void TLexer::GetId(QString& text)
{
	char currentChar;
	while (IsIdText(currentChar=m_reader.GetNextChar()))
	{
		text.append(currentChar);
	}
}

const bool TLexer::IsDigit(const char c) const
{
	return c >= '0'&& c <= '9';
}

const bool TLexer::IsLetter(const char c) const
{
	return c >= 'a'&& c <= 'z' || c >= 'A'&& c <= 'Z';
}

const bool TLexer::IsIdText(const char c) const
{
	return IsLetter(c) || c == '_' || IsDigit(c);
}

const bool TLexer::PushKeyWord(const QString& text)
{
	auto iter = TToken::m_keyMap.find(text);
	if (iter==TToken::m_keyMap.end())
	{
		return false;
	}

	PushToken(iter.value());
	return true;
}

void TLexer::Parse()
{
	char c;
	while (c=m_reader.GetCurrentChar())
	{
		m_reader.SkipBlank();
		c = m_reader.GetCurrentChar();

		if (PushDigit(c))
		{
			continue;
		}
		else if (PushId(c))
		{
			continue;
		}
		else if (PushOtherToken(c))
		{
			continue;
		}
		else
		{
			ThrowException(QString(c));
		}
	}

	m_tokens.push_back(std::shared_ptr<TToken>(nullptr));
}

void TLexer::PushToken(const TToken::TokenType type)
{
	m_tokens.push_back(std::shared_ptr<TToken>(new TToken(type, m_reader.GetLineNumber())));
}

TLexer::Reader::Reader(const QString& text) :m_text(&text)
{
	const_cast<QString*>(m_text)->append(QChar('\0'));
}

const char TLexer::Reader::GetNextChar()
{
	if (GetCurrentChar()=='\n')
	{
		++m_lineNumber;
	}

	return m_text->at(++m_index).toLatin1();
}

const char TLexer::Reader::GetCurrentChar() const
{
	return m_text->at(m_index).toLatin1();
}

const int TLexer::Reader::GetLineNumber() const
{
	return m_lineNumber;
}

const bool TLexer::Reader::IsEof() const
{
	return m_index >= m_text->size();
}

const char TLexer::Reader::PeekChar(const int index) const
{
	return m_text->at(m_index + index).toLatin1();
}

void TLexer::Reader::SkipBlank()
{
	while (IsBlank(m_text->at(m_index).toLatin1()))
	{
		++m_index;
	}
}

void TLexer::Reader::UnGetChar()
{
	if (m_index==0)
	{
		return;
	}

	if (m_text->at(--m_index)=='\n')
	{
		m_lineNumber;
	}
}

const bool TLexer::Reader::IsBlank(const char c) const
{
	return c > 0 && c <= ' ' && c != '\n';
}
