#include "stdafx.h"
#include "TAstNodeEndIfSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TContext.h"



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
	TContext::SetNextNode(FindNextValidNode());

	return ValueReturned();
}

void TAstNodeEndIfSentence::ParseSemantic() const
{
}

TAstNode* TAstNodeEndIfSentence::FindNextValidNode() const
{
	auto desNode = m_parentNode;
	while (desNode->GetToken()->GetType() != TToken::STRUCTURE_IF)
	{
		desNode = desNode->GetParentNode();
	}

	return desNode->FindNextValidNode();
}
