#include "stdafx.h"
#include "TAstNodeAssignSentence.h"
#include "TToken.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TAstNodeOperator.h"
#include "TVariateManager.h"
#include "TTokenWithValue.h"
#include "TContext.h"
#include "TInteger.h"
#include "TDouble.h"
#include "TBool.h"
#include "TString.h"



TAstNodeAssignSentence::TAstNodeAssignSentence(const std::shared_ptr<TToken> token /*= nullptr*/)
	:TAstNode(token)
{
}

TAstNodeAssignSentence::~TAstNodeAssignSentence()
{
}

TAstNode::ValueReturned TAstNodeAssignSentence::Execute() const
{

	auto var = TVariateManager::GetInstance()->GetVariateSrollUp(TContext::GetCurrentScope(),
		static_cast<TTokenWithValue<QString>*>(m_firstChild->GetToken().get())->GetValue());

	auto value = m_firstChild->GetSibling()->Execute();

	ValueReturned result(value);

	switch (var->GetType())
	{
	case CSymbol::TYPE_INTERGER:
	{
		TVariateManager::GetInstance()->UpdateInMap(var->GetScope(), var->GetName(), TInteger(var->GetScope(), var->GetName(), value.value));
		result.type = CSymbol::TYPE_INTERGER;
	}break;
	case CSymbol::TYPE_DOUBLE:
	{
		TVariateManager::GetInstance()->UpdateInMap(var->GetScope(), var->GetName(), TDouble(var->GetScope(), var->GetName(), value.value));
		result.type = CSymbol::TYPE_DOUBLE;
	}break;
	case CSymbol::TYPE_BOOL:
	{
		TVariateManager::GetInstance()->UpdateInMap(var->GetScope(), var->GetName(), TBool(var->GetScope(), var->GetName(), value.value));
		result.type = CSymbol::TYPE_BOOL;
	}break;
	default:
		break;
	}

	TContext::SetNextNode(FindNextValidNode());

	return result;
}

const std::shared_ptr<TAstNode> TAstNodeAssignSentence::GetAstNode(TLexer* const lexer)
{
	auto assignNode = GetAssignException(lexer);

	CheckEofEol(lexer);

	return assignNode;
}


const std::shared_ptr<TAstNode> TAstNodeAssignSentence::GetAssignException(TLexer* const lexer)
{
	auto firstToken = lexer->GetToken();

	if (firstToken->GetType() != TToken::ID)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, firstToken->GetLineNumber());
	}

	std::shared_ptr<TAstNode> firstChild(new TAstNodeOperator(firstToken));

	auto assignToken = lexer->GetToken();
	if (assignToken->GetType() != TToken::OPERATOR_ASSIGN)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, assignToken->GetLineNumber());
	}

	std::shared_ptr<TAstNode> assignNode(new TAstNodeAssignSentence(assignToken));

	assignNode->AddChild(firstChild);
	assignNode->AddChild(TAstNodeOperator::GetAstNode(lexer));

	return assignNode;
}

void TAstNodeAssignSentence::ParseSemantic() const
{
	auto var = TVariateManager::GetInstance()->GetVariateSrollUp(TContext::GetCurrentScope(),
		static_cast<TTokenWithValue<QString>*>(m_firstChild->GetToken().get())->GetValue());

	if (!var)
	{
		throw TInterpreterException(TInterpreterException::NOT_FIND_VARIATE, m_token->GetLineNumber(),
			static_cast<TTokenWithValue<QString>*>(m_firstChild->GetToken().get())->GetValue());
	}

	auto type1 = var->GetType();
	auto type2 = TAstNodeOperator::GetSymbolType(m_firstChild->GetSibling());

	if (type1!=type2)
	{
		if ((type1!=CSymbol::TYPE_INTERGER && type1!=CSymbol::TYPE_DOUBLE)
			|| (type2 != CSymbol::TYPE_INTERGER && type2 != CSymbol::TYPE_DOUBLE))
		{
			throw TInterpreterException(TInterpreterException::TYPE_NOT_MATCH, m_token->GetLineNumber());
		}
	}
}
