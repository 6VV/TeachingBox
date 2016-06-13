#include "stdafx.h"
#include "TAstNodeIfSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TAstNodeOperator.h"
#include "TGrammarParser.h"
#include "TAstNodeEndIfSentence.h"
#include "TAstNodeElseSentence.h"
#include "TAstNodeElseIfSentence.h"

TAstNodeIfSentence::TAstNodeIfSentence(const std::shared_ptr<TToken> token /*= nullptr*/)
	:TAstNode(token)
{

}

TAstNodeIfSentence::~TAstNodeIfSentence()
{

}

const std::shared_ptr<TAstNode> TAstNodeIfSentence::GetAstNode(TLexer* const lexer)
{
	auto token = lexer->GetToken();
	if (token->GetType() != TToken::STRUCTURE_IF)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, token->GetLineNumber());
	}

	std::shared_ptr<TAstNode> result(new TAstNodeIfSentence(token));

	result->AddChild(TAstNodeOperator::GetAstNode(lexer));
	CheckLineBreak(lexer);

	AddIfChild(lexer, result);

	AddContent(lexer, result);

	result->AddChild(TAstNodeEndIfSentence::GetAstNode(lexer));

	return result;
}

void TAstNodeIfSentence::AddContent(TLexer* const lexer, std::shared_ptr<TAstNode> result)
{
	int type = 0;
	while ((type = lexer->PeekToken()->GetType()) != TToken::STURCTURE_END_IF)
	{
		switch (type)
		{
		case TToken::STRUCTURE_ELSE_IF:
		{
			result->AddChild(TAstNodeElseIfSentence::GetAstNode(lexer));
		}break;
		case TToken::STURCTURE_ELSE:
		{
			result->AddChild(TAstNodeElseSentence::GetAstNode(lexer));
			break;
		}break;
		default:
		{
			throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, lexer->PeekToken()->GetLineNumber());
		}break;
		}
	}
}

void TAstNodeIfSentence::AddIfChild(TLexer* const lexer, std::shared_ptr<TAstNode> result)
{
	std::shared_ptr<TAstNode> childNode{};
	while (childNode = TGrammarParser::GetOneNode(lexer))
	{
		result->AddChild(childNode);
	}
}

TAstNode::ValueReturned TAstNodeIfSentence::Execute() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

void TAstNodeIfSentence::ParseSemantic() const
{
	if (TAstNodeOperator::GetSymbolType(m_firstChild)!=CSymbol::TYPE_BOOL)
	{
		throw TInterpreterException(TInterpreterException::IF_SENTENCE_SHOULD_WITH_BOOL, m_token->GetLineNumber());
	}

	auto child = m_firstChild->GetSibling();

	while (child)
	{
		child->ParseSemantic();
		child = child->GetSibling();
	}
}
