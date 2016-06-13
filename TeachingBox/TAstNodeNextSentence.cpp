#include "stdafx.h"
#include "TAstNodeNextSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TContext.h"
#include "TTokenWithValue.h"
#include "TVariateManager.h"
#include "TAstNodeForSentence.h"



TAstNodeNextSentence::TAstNodeNextSentence(const std::shared_ptr<TToken> token /*= nullptr*/) :TAstNode(token)
{

}

TAstNodeNextSentence::~TAstNodeNextSentence()
{

}

TAstNode::ValueReturned TAstNodeNextSentence::Execute() const
{
	auto thirdChild = m_parentNode->GetFirstChild()->GetSibling()->GetSibling();
	auto thirdValue = thirdChild->Execute();

	auto scope = TContext::GetCurrentScope();
	auto name = static_cast<TTokenWithValue<QString>*>(m_parentNode->GetFirstChild()->GetFirstChild()->GetToken().get())->GetValue();
	auto type = TVariateManager::GetInstance()->GetVariateSrollUp(scope, name)->GetType();

	auto newValue = TAstNodeForSentence::GetValue(type, scope, name) + thirdValue.value;

	TAstNodeForSentence::UpdateValue(type, scope, name, newValue);

	auto sencondValue = m_parentNode->GetFirstChild()->GetSibling()->Execute();

	if (newValue<sencondValue.value)
	{
		TContext::SetNextNode(thirdChild->GetSibling().get());
	}
	else
	{
		TContext::SetNextNode(FindNextValidNode());
	}


	return ValueReturned{};
}

const std::shared_ptr<TAstNode> TAstNodeNextSentence::GetAstNode(TLexer* const lexer)
{
	auto token = lexer->GetToken();
	if (token->GetType()!=TToken::STRUCTURE_NEXT)
	{
		throw TInterpreterException(TInterpreterException::FOR_SENTENCE_SHOULD_END_WITH_NEXT, token->GetLineNumber());
	}

	std::shared_ptr<TAstNode> result(new TAstNodeNextSentence(token));

	if (!IsEofOrEol(lexer->GetToken()->GetType()))
	{
		throw TInterpreterException(TInterpreterException::SENTENCE_NOT_END_WITH_ABNORMAL_END, token->GetLineNumber());
	}

	return result;
}

void TAstNodeNextSentence::ParseSemantic() const
{
}
