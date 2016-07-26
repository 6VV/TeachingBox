#include "stdafx.h"
#include "TAstNodeIfSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TAstNodeOperator.h"
#include "TAstNodeFactory.h"
#include "TAstNodeEndIfSentence.h"
#include "TAstNodeElseSentence.h"
#include "TAstNodeElseIfSentence.h"
#include "TContext.h"

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

	AddThenChild(lexer, result);

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

void TAstNodeIfSentence::AddThenChild(TLexer* const lexer, std::shared_ptr<TAstNode> result)
{
	std::shared_ptr<TAstNode> thenNode(new TAstNode(std::shared_ptr<TToken>(new TToken(TToken::STRUCTURE_THEN, result->GetToken()->GetLineNumber()))));
	result->AddChild(thenNode);

	std::shared_ptr<TAstNode> childNode{};
	while (childNode = TAstNodeFactory::GetOneNode(lexer))
	{
		thenNode->AddChild(childNode);
	}

	thenNode->AddChild(std::shared_ptr<TAstNode>(new TAstNodeEndIfSentence(
		std::shared_ptr<TToken>(new TToken(TToken::STURCTURE_END_IF, thenNode->GetEndChild()->GetToken()->GetLineNumber())))));
}

TAstNode::ValueReturned TAstNodeIfSentence::Execute() const
{
	if (m_firstChild->Execute().value)
	{
		TContext::SetNextNode(m_firstChild->GetSibling()->GetFirstChild().get());
	}
	else
	{
		TContext::SetNextNode(m_firstChild->GetSibling()->GetSibling().get());
	}

	return ValueReturned();
};

void TAstNodeIfSentence::ParseSemantic() const
{
	if (TAstNodeOperator::GetSymbolType(m_firstChild)!=CSymbol::TYPE_BOOL)
	{
		throw TInterpreterException(TInterpreterException::IF_SENTENCE_SHOULD_WITH_BOOL, m_token->GetLineNumber());
	}

	ParseChildren(m_firstChild->GetSibling()->GetFirstChild());

	ParseChildren(m_firstChild->GetSibling()->GetSibling());
}

void TAstNodeIfSentence::ParseChildren(std::shared_ptr<TAstNode> child) const
{
	while (child)
	{
		child->ParseSemantic();
		child = child->GetSibling();
	}
}
