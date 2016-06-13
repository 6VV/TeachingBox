#include "stdafx.h"
#include "TAstNodeElseIfSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TAstNodeOperator.h"


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

	return result;
}

TAstNode::ValueReturned TAstNodeElseIfSentence::Execute() const
{
	throw std::logic_error("The method or operation is not implemented.");
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
