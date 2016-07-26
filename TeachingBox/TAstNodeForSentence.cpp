#include "stdafx.h"
#include "TAstNodeForSentence.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TAstNodeFactory.h"
#include "TAstNodeAssignSentence.h"
#include "TAstNodeOperator.h"
#include "TAstNodeNextSentence.h"
#include "TVariateManager.h"
#include "TContext.h"
#include "TTokenWithValue.h"
#include "TInteger.h"
#include "TDouble.h"



TAstNodeForSentence::TAstNodeForSentence(const std::shared_ptr<TToken> token /*= nullptr*/)
	:TAstNode(token)
{

}

TAstNodeForSentence::~TAstNodeForSentence()
{

}

TAstNode::ValueReturned TAstNodeForSentence::Execute() const
{
	auto secondChild = m_firstChild->GetSibling();
	auto thirdChild = secondChild->GetSibling();

	auto firstValue = m_firstChild->Execute();
	auto sencondValue = secondChild->Execute();
	//auto thirdValue = thirdChild->Execute();

	auto scope = TContext::GetCurrentScope();
	auto name = static_cast<TTokenWithValue<QString>*>(m_firstChild->GetFirstChild()->GetToken().get())->GetValue();
	auto type = TVariateManager::GetInstance()->GetVariateSrollUp(scope, name)->GetType();

	UpdateValue(type, scope, name, firstValue.value);
	
	if (GetValue(type, scope, name)<sencondValue.value)
	{
		TContext::SetNextNode(thirdChild->GetSibling().get());
	}
	else
	{
		TContext::SetNextNode(FindNextValidNode());
	}

	/*auto child = thirdChild->GetSibling();
	while (GetValue(type,scope,name)<sencondValue.value)
	{
		if (child->GetToken()->GetType()==TToken::STRUCTURE_NEXT)
		{
			UpdateValue(type, scope, name, GetValue(type, scope, name) + thirdValue.value);
			child = thirdChild->GetSibling();
			continue;
		}
		child->Execute();
		child = child->GetSibling();
	}*/

	return ValueReturned{0,CSymbol::TYPE_VOID};
}

void TAstNodeForSentence::UpdateValue(CSymbol::SymbolType type, const QString& scope, const QString& name, double value)
{
	if (type == CSymbol::TYPE_INTERGER)
	{
		TVariateManager::GetInstance()->UpdateInMap(scope, name, TInteger(scope, name, value));
	}
	else
	{
		TVariateManager::GetInstance()->UpdateInMap(scope, name, TDouble(scope, name, value));
	}
}

double TAstNodeForSentence::GetValue(CSymbol::SymbolType type, const QString& scope, const QString& name)
{
	if (type == CSymbol::TYPE_INTERGER)
	{
		return static_cast<TInteger*>(TVariateManager::GetInstance()->GetVariateSrollUp(scope, name))->GetValue();
	}
	else
	{
		return static_cast<TDouble*>(TVariateManager::GetInstance()->GetVariateSrollUp(scope, name))->GetValue();
	}
}

const std::shared_ptr<TAstNode> TAstNodeForSentence::GetAstNode(TLexer* const lexer)
{
	auto token = lexer->GetToken();
	if (token->GetType()!=TToken::STRUCTURE_FOR)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, token->GetLineNumber());
	}

	std::shared_ptr<TAstNode> result(new TAstNodeForSentence(token));

	result->AddChild(TAstNodeAssignSentence::GetAssignException(lexer));

	AddToNode(lexer, token, result);
	AddStepNode(lexer, token, result);

	CheckLineBreak(lexer);

	AddSentenceNodes(lexer, result);

	result->AddChild(TAstNodeNextSentence::GetAstNode(lexer));

	return result;
}

void TAstNodeForSentence::AddStepNode(TLexer* const lexer, std::shared_ptr<TToken> token, std::shared_ptr<TAstNode> result)
{
	if (lexer->GetToken()->GetType() != TToken::STRUCTURE_STEP)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, token->GetLineNumber());
	}

	result->AddChild(TAstNodeOperator::GetAstNode(lexer));
}

void TAstNodeForSentence::AddToNode(TLexer* const lexer, std::shared_ptr<TToken> token, std::shared_ptr<TAstNode> result)
{
	if (lexer->GetToken()->GetType() != TToken::STRUCTURE_TO)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, token->GetLineNumber());
	}

	result->AddChild(TAstNodeOperator::GetAstNode(lexer));
}

void TAstNodeForSentence::ParseSemantic() const
{
	auto type1 = TAstNodeOperator::GetSymbolType(m_firstChild->GetFirstChild()->GetSibling());
	auto type2 = TAstNodeOperator::GetSymbolType(m_firstChild->GetSibling());
	auto type3 = TAstNodeOperator::GetSymbolType(m_firstChild->GetSibling()->GetSibling());

	if ((type1!=CSymbol::TYPE_INTERGER && type1!=CSymbol::TYPE_DOUBLE)
		|| (type2 != CSymbol::TYPE_INTERGER && type2 != CSymbol::TYPE_DOUBLE)
		|| (type3 != CSymbol::TYPE_INTERGER && type3 != CSymbol::TYPE_DOUBLE))
	{
		throw TInterpreterException(TInterpreterException::FOR_SENTENCE_SHOULD_USE_INTERGER_OR_DOUBLE, m_firstChild->GetToken()->GetLineNumber());
	}

	auto child = m_firstChild->GetSibling()->GetSibling()->GetSibling();
	while (child)
	{
		child->ParseSemantic();
		child = child->GetSibling();
	}
}
