#include "stdafx.h"
#include "TAstNodeElseSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TAstNodeEndIfSentence.h"
#include "TContext.h"



TAstNodeElseSentence::TAstNodeElseSentence(const std::shared_ptr<TToken> token /*= nullptr*/)
	:TAstNode(token)
{

}

TAstNodeElseSentence::~TAstNodeElseSentence()
{

}

const std::shared_ptr<TAstNode> TAstNodeElseSentence::GetAstNode(TLexer* const lexer)
{
	auto token = lexer->GetToken();

	if (token->GetType()!=TToken::STURCTURE_ELSE)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, token->GetLineNumber());
	}
	
	CheckLineBreak(lexer);

	std::shared_ptr<TAstNode> result(new TAstNodeElseSentence(token));

	AddSentenceNodes(lexer, result);

	result->AddChild(std::shared_ptr<TAstNode>(new TAstNodeEndIfSentence(
		std::shared_ptr<TToken>(new TToken(TToken::STURCTURE_END_IF, result->GetEndChild()->GetToken()->GetLineNumber())))));

	return result;
}

TAstNode::ValueReturned TAstNodeElseSentence::Execute() const
{
	TContext::SetNextNode(m_firstChild.get());

	return ValueReturned();
}

void TAstNodeElseSentence::ParseSemantic() const
{
	auto child = m_firstChild;

	while (child)
	{
		child->ParseSemantic();
		child = child->GetSibling();
	}
}
