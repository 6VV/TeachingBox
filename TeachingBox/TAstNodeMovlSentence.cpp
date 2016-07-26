#include "stdafx.h"
#include "TAstNodeMovlSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TTokenWithValue.h"
#include "TVariateManager.h"
#include "TContext.h"
#include "TPosition.h"
#include "TDynamic.h"
#include "TOverlap.h"
#include "TMacroData.h"

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
	CheckLineBreak(lexer);

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
	SendMovlData();
	TContext::SetNextNode(GetSibling().get());
	return ValueReturned();
}

void TAstNodeMovlSentence::ParseSemantic() const
{
	CheckParameterType(m_firstChild, CSymbol::TYPE_POSITION);
	CheckParameterType(m_firstChild->GetSibling(), CSymbol::TYPE_DYNAMIC);
	CheckParameterType(m_firstChild->GetSibling()->GetSibling(), CSymbol::TYPE_OVERLAP);
}

void TAstNodeMovlSentence::CheckParameterType(std::shared_ptr<TAstNode> node,int type) const
{
	auto variate = TVariateManager::GetInstance()->GetVariateSrollUp(TContext::GetCurrentScope(),
		static_cast<TTokenWithValue<QString>*>(node->GetToken().get())->GetValue());

	if (variate==nullptr)
	{
		throw TInterpreterException(TInterpreterException::UNKNOW_TOKEN, node->GetToken()->GetLineNumber()
			, static_cast<TTokenWithValue<QString>*>(node->GetToken().get())->GetValue());
	}

	if (variate->GetType() != type)
	{
		throw TInterpreterException(TInterpreterException::MOVL_SHOULD_WITH_POSITOIN_DYNAMIC_OVERLAP, node->GetToken()->GetLineNumber());
	}
}

void TAstNodeMovlSentence::SendMovlData() const
{
	TMacroData::Send(GetMovlParameter(), COMMAND_ID::MOVL, m_token->GetLineNumber(),(long long)(TContext::GetCurrentProgramNode()));
}

tMovLParam TAstNodeMovlSentence::GetMovlParameter() const
{
	tMovLParam movlParam;

	std::shared_ptr<TAstNode> firstChild = this->GetFirstChild();
	tAxesAllPositions position = GetPosition(static_cast<TTokenWithValue<QString>*>(firstChild->GetToken().get())->GetValue());


	std::shared_ptr<TAstNode> secondChild = firstChild->GetSibling();
	tDynamicConstraint dynamic = GetDynamic(static_cast<TTokenWithValue<QString>*>(secondChild->GetToken().get())->GetValue());

	std::shared_ptr<TAstNode> thirdChild = secondChild->GetSibling();
	tOverlapConstraint overlap = GetOverlap(static_cast<TTokenWithValue<QString>*>(thirdChild->GetToken().get())->GetValue());

	movlParam.m_Destination = position;
	movlParam.m_Dynamic = dynamic;
	movlParam.m_Overlap = overlap;

	return movlParam;
}

tAxesAllPositions TAstNodeMovlSentence::GetPosition(const QString& name) const
{
	return static_cast<TPosition*>(TVariateManager::GetInstance()
		->GetVariateSrollUp(TContext::GetCurrentScope(), name))->GetValue();
}

tDynamicConstraint TAstNodeMovlSentence::GetDynamic(const QString& name) const
{
	return static_cast<TDynamic*>(TVariateManager::GetInstance()
		->GetVariateSrollUp(TContext::GetCurrentScope(), name))->GetValue();
}

tOverlapConstraint TAstNodeMovlSentence::GetOverlap(const QString& name) const
{
	return static_cast<TOverlap*>(TVariateManager::GetInstance()
		->GetVariateSrollUp(TContext::GetCurrentScope(), name))->GetValue();
}
