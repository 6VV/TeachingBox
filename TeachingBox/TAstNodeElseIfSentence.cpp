#include "stdafx.h"
#include "TAstNodeElseIfSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TAstNodeOperator.h"
#include "TAstNodeEndIfSentence.h"
#include "TContext.h"


TAstNodeElseIfSentence::TAstNodeElseIfSentence(const std::shared_ptr<TToken> token /*= nullptr*/)
	:TAstNode(token)
{

}

TAstNodeElseIfSentence::~TAstNodeElseIfSentence()
{

}

const std::shared_ptr<TAstNode> TAstNodeElseIfSentence::GetAstNode(TLexer* const lexer)
{
	auto token = lexer->GetToken();

	if (token->GetType()!=TToken::STRUCTURE_ELSE_IF)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, token->GetLineNumber());
	}

	std::shared_ptr<TAstNode> result(new TAstNodeElseIfSentence(token));
	result->AddChild(TAstNodeOperator::GetAstNode(lexer));
	CheckLineBreak(lexer);

	AddSentenceNodes(lexer, result);

	result->AddChild(std::shared_ptr<TAstNode>(new TAstNodeEndIfSentence(
		std::shared_ptr<TToken>(new TToken(TToken::STURCTURE_END_IF, result->GetEndChild()->GetToken()->GetLineNumber())))));

	return result;
}

TAstNode::ValueReturned TAstNodeElseIfSentence::Execute() const
{
	if (m_firstChild->Execute().value)
	{
		TContext::SetNextNode(m_firstChild->GetSibling().get());
	}
	else
	{
		TContext::SetNextNode(m_siblingNode.get());
	}

	return ValueReturned();
}

void TAstNodeElseIfSentence::ParseSemantic() const
{
	if (TAstNodeOperator::GetSymbolType(m_firstChild) != CSymbol::TYPE_BOOL)
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
