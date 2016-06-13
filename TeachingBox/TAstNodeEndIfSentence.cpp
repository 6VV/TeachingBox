#include "stdafx.h"
#include "TAstNodeEndIfSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"



TAstNodeEndIfSentence::TAstNodeEndIfSentence(const std::shared_ptr<TToken> token /*= nullptr*/)
	:TAstNode(token)
{

}

TAstNodeEndIfSentence::~TAstNodeEndIfSentence()
{

}

const std::shared_ptr<TAstNode> TAstNodeEndIfSentence::GetAstNode(TLexer* const lexer)
{
	auto token = lexer->GetToken();
	if (token->GetType()!=TToken::STURCTURE_END_IF)
	{
		throw TInterpreterException(TInterpreterException::IF_SENTENCE_SHOULD_END_WITH_ENDIF, token->GetLineNumber());
	}

	CheckEofEol(lexer);

	return std::shared_ptr<TAstNode>(new TAstNodeEndIfSentence(token));
}

TAstNode::ValueReturned TAstNodeEndIfSentence::Execute() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

void TAstNodeEndIfSentence::ParseSemantic() const
{
}
