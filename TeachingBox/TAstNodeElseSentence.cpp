#include "stdafx.h"
#include "TAstNodeElseSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"



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

	return result;
}

TAstNode::ValueReturned TAstNodeElseSentence::Execute() const
{
	throw std::logic_error("The method or operation is not implemented.");
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
