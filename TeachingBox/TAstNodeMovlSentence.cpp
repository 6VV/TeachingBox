#include "stdafx.h"
#include "TAstNodeMovlSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TTokenWithValue.h"
#include "TVariateManager.h"
#include "TContext.h"

TAstNodeMovlSentence::TAstNodeMovlSentence(const std::shared_ptr<TToken> token /*= nullptr*/) :TAstNode(token)
{

}

TAstNodeMovlSentence::~TAstNodeMovlSentence()
{

}

const std::shared_ptr<TAstNode> TAstNodeMovlSentence::GetAstNode(TLexer* const lexer)
{
	auto token = lexer->GetToken();
	if (token->GetType()!=TToken::MACRO_MOVL)
	{
		throw TInterpreterException(TInterpreterException::NOT_MOVL_SENTENCE, token->GetLineNumber());
	}
	std::shared_ptr<TAstNode> result(new TAstNodeMovlSentence(token));

	result->AddChild(GetParameter(lexer));
	CheckComma(lexer);
	result->AddChild(GetParameter(lexer));
	CheckComma(lexer);
	result->AddChild(GetParameter(lexer));

	return result;
}

const std::shared_ptr<TAstNode> TAstNodeMovlSentence::GetParameter(TLexer* const lexer)
{
	auto token = lexer->GetToken();
	if (token->GetType() != TToken::ID)
	{
		throw TInterpreterException(TInterpreterException::MOVL_SHOULD_WITH_POSITOIN_DYNAMIC_OVERLAP, token->GetLineNumber());
	}
	return std::shared_ptr<TAstNode>(new TAstNode(token));
}

void TAstNodeMovlSentence::CheckComma(TLexer* const lexer)
{
	if (lexer->GetToken()->GetType() != TToken::SEPARATOR_COMMA)
	{
		lexer->UnGetToken();
		throw TInterpreterException(TInterpreterException::LOSE_COMMA, lexer->PeekToken()->GetLineNumber());
	}
}

TAstNode::ValueReturned TAstNodeMovlSentence::Execute() const
{
	throw std::logic_error("The method or operation is not implemented.");
}

void TAstNodeMovlSentence::ParseSemantic() const
{
	CheckParameterType(m_firstChild, CSymbol::TYPE_POSITION);
	CheckParameterType(m_firstChild->GetSibling(), CSymbol::TYPE_DYNAMIC);
	CheckParameterType(m_firstChild->GetSibling()->GetSibling(), CSymbol::TYPE_OVERLAP);
}

void TAstNodeMovlSentence::CheckParameterType(std::shared_ptr<TAstNode> node,int type) const
{
	if (TVariateManager::GetInstance()->GetVariate(TContext::GetCurrentScope(), static_cast<TTokenWithValue<QString>*>(node->GetToken().get())
		->GetValue())->GetType() != type)
	{
		throw TInterpreterException(TInterpreterException::MOVL_SHOULD_WITH_POSITOIN_DYNAMIC_OVERLAP, node->GetToken()->GetLineNumber());
	}
}
