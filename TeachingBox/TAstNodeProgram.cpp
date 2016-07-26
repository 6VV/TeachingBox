#include "stdafx.h"
#include "TAstNodeProgram.h"
#include "TAstNodeFactory.h"
#include "TLexer.h"
#include "TContext.h"



TAstNodeProgram::TAstNodeProgram(const std::shared_ptr<TToken> token /*= nullptr*/)
	:TAstNode(token)
{

}

TAstNodeProgram::~TAstNodeProgram()
{

}

const std::shared_ptr<TAstNode> TAstNodeProgram::GetAstNode(TLexer* const lexer, const std::shared_ptr<TToken> token)
{
	std::shared_ptr<TAstNode> result(new TAstNodeProgram(token));
	std::shared_ptr<TAstNode> node{};
	while (node = TAstNodeFactory::GetOneNode(lexer))
	{
		result->AddChild(node);
		SkipEol(lexer);
	}

	if (lexer->PeekToken()->GetType() != TToken::SEPARATOR_EOF)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, lexer->PeekToken()->GetLineNumber());
	}

	return result;
}

TAstNode::ValueReturned TAstNodeProgram::Execute() const
{
	TContext::SetCurrentProgramNode(this);
	TContext::SetNextNode(m_firstChild.get());

	return TAstNode::Execute();
}

void TAstNodeProgram::ParseSemantic() const
{
	auto child = m_firstChild;

	while (child)
	{
		child->ParseSemantic();
		child = child->GetSibling();
	}
}
