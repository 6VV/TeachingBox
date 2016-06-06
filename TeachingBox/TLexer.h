#ifndef _TEACHING_BOX_C_T_LEXER_H_
#define _TEACHING_BOX_C_T_LEXER_H_

#include <memory>
#include "QString"
#include <vector>
#include "TToken.h"


class TLexer
{
public:
	TLexer(const QString& text);
	~TLexer();

	const std::shared_ptr<TToken> GetToken() const;

private:
	void GetInterger(QString &text);
	void GetId(QString& text);

	const bool IsDigit(const char c) const;
	const bool IsLetter(const char c) const;
	const bool IsIdText(const char c) const;

	void Parse();
	void PushToken(const TToken::TokenType type);
	const bool PushDigit(const char c);
	const bool PushId(const char c);
	const bool PushKeyWord(const QString& text);
	const bool PushOtherToken(const char);

	void ThrowException(const QString& text);

private:
	std::vector < std::shared_ptr<TToken>> m_tokens;
	mutable int m_index=0;

private:
	class Reader
	{
	public:
		Reader(const QString& text);

		const char GetNextChar();
		const char GetCurrentChar() const;
		const int GetLineNumber() const;

		const bool IsEof() const;

		const char PeekChar(const int index) const;

		void SkipBlank();

		void UnGetChar();
	private:
		const bool IsBlank(const char c) const;

	private:
		const QString* m_text = nullptr;
		int m_index = 0;
		int m_lineNumber = 1;
	};

	Reader m_reader;
};

#endif