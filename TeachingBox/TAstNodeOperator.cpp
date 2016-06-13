#include "stdafx.h"
#include "TAstNodeOperator.h"
#include "TToken.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TVariateManager.h"
#include "TContext.h"
#include "TTokenWithValue.h"
#include "TInteger.h"
#include "TDouble.h"
#include "TBool.h"
#include <cmath>


TAstNodeOperator::TAstNodeOperator(const std::shared_ptr<TToken> token /*= nullptr*/)
	: TAstNode(token)
{

}

TAstNodeOperator::~TAstNodeOperator()
{

}

TAstNodeOperator::ValueReturned TAstNodeOperator::Execute() const
{
	return ReturnValue(this);
}

TAstNodeOperator::ValueReturned TAstNodeOperator::ReturnTerminalValue(const TAstNode* const node) const
{
	TAstNodeOperator::ValueReturned result{};

	switch (node->GetToken()->GetType())
	{
	case TToken::LITERAL_INTERGER:
	{
		result.value = static_cast<TTokenWithValue<int>*>(node->GetToken().get())->GetValue();
		result.type = CSymbol::TYPE_INTERGER;
	}break;
	case TToken::LITERAL_DOUBLE:
	{
		result.value = static_cast<TTokenWithValue<double>*>(node->GetToken().get())->GetValue();
		result.type = CSymbol::TYPE_DOUBLE;
	}break;
	case TToken::LITERAL_BOOL:
	{
		result.value = static_cast<TTokenWithValue<bool>*>(node->GetToken().get())->GetValue();
		result.type = CSymbol::TYPE_BOOL;
	}break;
	case TToken::ID:
	{
		auto var = TVariateManager::GetInstance()->GetVariateSrollUp(TContext::GetCurrentScope(),
			static_cast<TTokenWithValue<QString>*>(node->GetToken().get())->GetValue());
		switch (var->GetType())
		{
		case CSymbol::TYPE_INTERGER:
		{
			result.value = static_cast<TInteger*>(var)->GetValue();
			result.type = CSymbol::TYPE_INTERGER;
		}break;
		case CSymbol::TYPE_DOUBLE:
		{
			result.value = static_cast<TDouble*>(var)->GetValue();
			result.type = CSymbol::TYPE_DOUBLE;
		}break;
		case CSymbol::TYPE_BOOL:
		{
			result.value = static_cast<TBool*>(var)->GetValue();
			result.type = CSymbol::TYPE_BOOL;
		}break;
		default:
			break;
		}
	}break;
	default:
		break;
	}

	return result;
}

TAstNodeOperator::ValueReturned TAstNodeOperator::ReturnNonTerminalValue(const TAstNode* const node) const
{
	TAstNodeOperator::ValueReturned result{};

	switch (node->GetToken()->GetType())
	{
	case TToken::OPERATOR_NEGATIVE:
	{
		result = ReturnValue(node->GetFirstChild().get());
		result.value *= -1;
	}break;
	case TToken::OPERATOR_NEGATION:
	{
		result = ReturnValue(node->GetFirstChild().get());
		result.value = !result.value;
	}
	case TToken::OPERATOR_PLUS:case TToken::OPERATOR_MINUS:case TToken::OPERATOR_MULTIPLY:case TToken::OPERATOR_DIVIDE:
	case TToken::OPERATOR_POWER:case TToken::OPERATOR_PERCENT:
	{
		auto result1 = ReturnValue(node->GetFirstChild().get());
		auto result2 = ReturnValue(node->GetFirstChild()->GetSibling().get());

		if (result1.type==CSymbol::TYPE_INTERGER && result2.type==CSymbol::TYPE_INTERGER)
		{
			result.type = CSymbol::TYPE_INTERGER;
			switch (node->GetToken()->GetType())
			{
			case TToken::OPERATOR_PLUS:result.value = static_cast<int>(result1.value) + static_cast<int>(result2.value); break;
			case TToken::OPERATOR_MINUS:result.value = static_cast<int>(result1.value) - static_cast<int>(result2.value); break;
			case TToken::OPERATOR_MULTIPLY:result.value = static_cast<int>(result1.value) * static_cast<int>(result2.value); break;
			case TToken::OPERATOR_DIVIDE:result.value = static_cast<int>(result1.value) / static_cast<int>(result2.value); break;
			case TToken::OPERATOR_POWER:result.value = pow(static_cast<int>(result1.value), static_cast<int>(result2.value)); break;
			case TToken::OPERATOR_PERCENT:result.value = static_cast<int>(result1.value) % static_cast<int>(result2.value); break;
			}
		}
		else
		{
			result.type = CSymbol::TYPE_DOUBLE;
			switch (node->GetToken()->GetType())
			{
			case TToken::OPERATOR_PLUS:result.value = result1.value + result2.value; break;
			case TToken::OPERATOR_MINUS:result.value = result1.value - result2.value; break;
			case TToken::OPERATOR_MULTIPLY:result.value = result1.value * result2.value; break;
			case TToken::OPERATOR_DIVIDE:result.value = result1.value / result2.value; break;
			case TToken::OPERATOR_POWER:result.value = pow(result1.value, result2.value); break;
			case TToken::OPERATOR_PERCENT:result.value = static_cast<int>(result1.value) % static_cast<int>(result2.value); break;
			}
		}
		
	}break;
	case TToken::OPERATOR_GREATE_THAN:case TToken::OPERATOR_GREATE_EQUAL:case TToken::OPERATOR_LESS_THAN:case TToken::OPERATOR_LESS_EQUAL:
	case TToken::OPERATOR_EQUAL:case TToken::OPERATOR_NOT_EQUAL:case TToken::OPERATOR_AND:case TToken::OPERATOR_OR:
	{
		result.type = CSymbol::TYPE_BOOL;

		auto result1 = ReturnValue(node->GetFirstChild().get());
		auto result2 = ReturnValue(node->GetFirstChild()->GetSibling().get());

		switch (node->GetToken()->GetType())
		{
		case TToken::OPERATOR_GREATE_THAN:result.value = result1.value > result2.value;break;
		case TToken::OPERATOR_GREATE_EQUAL:result.value = result1.value >= result2.value; break;
		case TToken::OPERATOR_LESS_THAN:result.value = result1.value < result2.value; break;
		case TToken::OPERATOR_LESS_EQUAL:result.value = result1.value <= result2.value; break;
		case TToken::OPERATOR_EQUAL:result.value = result1.value == result2.value; break;
		case TToken::OPERATOR_NOT_EQUAL:result.value = result1.value != result2.value; break;
		case TToken::OPERATOR_AND:result.value = result1.value && result2.value; break;
		case TToken::OPERATOR_OR:result.value = result1.value || result2.value; break;
		}
	}break;
	default:
		break;
	}

	return result;
}

const std::shared_ptr<TAstNode> TAstNodeOperator::GetAstNode(TLexer* const lexer)
{
	auto leftValue = GetValue(lexer);

	while (auto leftOper = PeekOperator(lexer))
	{
		leftValue = GetShiftOperator(leftValue, leftOper, lexer);
	}

	return leftValue;
}

const std::shared_ptr<TAstNode> TAstNodeOperator::GetNode(const std::shared_ptr<TToken>& token)
{
	return std::shared_ptr<TAstNode>(new TAstNodeOperator(token));
}

const int TAstNodeOperator::GetPriority(const TToken::TokenType type)
{
	return m_operatorProperties[type].priority;
}

const bool TAstNodeOperator::IsLeftAssociativity(const TToken::TokenType type)
{
	return m_operatorProperties[type].isLeft;
}

const bool TAstNodeOperator::IsExistOperator(const TToken::TokenType type)
{
	if (m_operatorProperties.find(type) == m_operatorProperties.end())
	{
		return false;
	}
	return true;
}

const std::shared_ptr<TAstNode> TAstNodeOperator::GetValue(TLexer* const lexer)
{
	auto value = lexer->GetToken();
	switch (value->GetType())
	{
	case TToken::LITERAL_INTERGER:case TToken::LITERAL_DOUBLE:case TToken::LITERAL_BOOL:case TToken::ID:
	{
		return GetNode(value);
	}break;
	case TToken::OPERATOR_MINUS:
	{
		auto nextValue = lexer->GetToken();
		switch (nextValue->GetType())
		{
		case TToken::LITERAL_INTERGER:case TToken::LITERAL_DOUBLE:case TToken::ID:
		{
			value->SetType(TToken::OPERATOR_NEGATIVE);
			auto result = GetNode(value);
			result->AddChild(GetNode(nextValue));
			return result;
		}break;
		default:
		{
			throw TInterpreterException(TInterpreterException::NEGATIVE_SHOULD_WITH_INT_OR_DOUBLE, value->GetLineNumber());
		}break;
		}
	}break;
	case TToken::OPERATOR_NEGATION:
	{
		auto nextValue = lexer->GetToken();
		switch (nextValue->GetType())
		{
		case TToken::LITERAL_BOOL:case TToken::ID:
		{
			auto result = GetNode(value);
			result->AddChild(GetNode(nextValue));
			return result;
		}break;
		default:
		{
			throw TInterpreterException(TInterpreterException::NEGATION_SHOULD_WITH_BOOL, value->GetLineNumber());
		}break;
		}
	}break;
	case TToken::OPERATOR_LEFT_BRACKET:
	{
		auto nextValue = GetAstNode(lexer);
		if (lexer->GetToken()->GetType() != TToken::OPERATOR_RIGHT_BRACKET)
		{
			throw TInterpreterException(TInterpreterException::NOT_FIND_RIGHT_BRACKET, value->GetLineNumber());
		}

		return nextValue;
	}break;
	default:
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, value->GetLineNumber());
	}
	break;
	}
}

const std::shared_ptr<TAstNode> TAstNodeOperator::GetShiftOperator(const std::shared_ptr<TAstNode>& leftValue,
	const std::shared_ptr<TAstNode>& leftOper, TLexer* const lexer)
{
	lexer->GetToken();
	auto rightValue = GetValue(lexer);
	std::shared_ptr<TAstNode> rightOper;

	while ((rightOper = PeekOperator(lexer)) != nullptr && IsRightExpr(leftOper, rightOper))
	{
		rightValue = GetShiftOperator(rightValue, rightOper,lexer);
	}

	leftOper->AddChild(leftValue);
	leftOper->AddChild(rightValue);
	return leftOper;
}

const std::shared_ptr<TAstNode> TAstNodeOperator::PeekOperator(TLexer* const lexer)
{
	auto token = lexer->GetToken();
	lexer->UnGetToken();

	if (TAstNodeOperator::IsExistOperator(token->GetType()))
	{
		return GetNode(token);
	}
	return nullptr;
}

const bool TAstNodeOperator::IsRightExpr(const std::shared_ptr<TAstNode>& leftOper, const std::shared_ptr<TAstNode>& rightOper)
{
	if (TAstNodeOperator::IsLeftAssociativity(rightOper->GetToken()->GetType()))
	{
		return TAstNodeOperator::GetPriority(leftOper->GetToken()->GetType()) > TAstNodeOperator::GetPriority(rightOper->GetToken()->GetType());
	}
	else
	{
		return TAstNodeOperator::GetPriority(leftOper->GetToken()->GetType()) >= TAstNodeOperator::GetPriority(rightOper->GetToken()->GetType());
	}
}

const CSymbol::SymbolType TAstNodeOperator::GetSymbolType(const std::shared_ptr<TAstNode>& node)
{
	if (node->GetFirstChild()== nullptr)
	{
		return GetTerminalSymbolType(node);
	}

	return GetNonTerminalSymbol(node);

}

const CSymbol::SymbolType TAstNodeOperator::GetNonTerminalSymbol(const std::shared_ptr<TAstNode>& node)
{
	switch (node->GetToken()->GetType())
	{
	case TToken::OPERATOR_NEGATIVE:
	{
		auto type = GetSymbolType(node->GetFirstChild());
		switch (type)
		{
		case CSymbol::TYPE_INTERGER:case CSymbol::TYPE_DOUBLE:return type;
		default:throw TInterpreterException(TInterpreterException::NEGATIVE_SHOULD_WITH_INT_OR_DOUBLE, node->GetToken()->GetLineNumber());
		}
	}break;
	case TToken::OPERATOR_NEGATION:
	{
		if (GetSymbolType(node->GetFirstChild()) == CSymbol::TYPE_BOOL)
		{
			return CSymbol::TYPE_BOOL;
		}
		throw TInterpreterException(TInterpreterException::NEGATION_SHOULD_WITH_BOOL, node->GetToken()->GetLineNumber());
	}break;
	case TToken::OPERATOR_PLUS:case TToken::OPERATOR_MINUS:case TToken::OPERATOR_MULTIPLY:case TToken::OPERATOR_DIVIDE:
	case TToken::OPERATOR_POWER:
	{
		auto firstType = GetSymbolType(node->GetFirstChild());
		auto secondType = GetSymbolType(node->GetFirstChild()->GetSibling());

		if (firstType == CSymbol::TYPE_INTERGER && secondType == CSymbol::TYPE_INTERGER)
		{
			return CSymbol::TYPE_INTERGER;
		}
		else if ((firstType == CSymbol::TYPE_INTERGER || firstType == CSymbol::TYPE_DOUBLE)
			&& (secondType == CSymbol::TYPE_INTERGER|| secondType == CSymbol::TYPE_DOUBLE))
		{
			return CSymbol::TYPE_DOUBLE;
		}
		else
		{
			throw TInterpreterException(TInterpreterException::ONLY_NUMBER_COULD_TAKE_ARITHMETICAL_OPERATION, node->GetToken()->GetLineNumber());
		}
	}break;
	case TToken::OPERATOR_PERCENT:
	{
		auto firstType = GetSymbolType(node->GetFirstChild());
		auto secondType = GetSymbolType(node->GetFirstChild()->GetSibling());

		if (firstType == CSymbol::TYPE_INTERGER && secondType == CSymbol::TYPE_INTERGER)
		{
			return CSymbol::TYPE_INTERGER;
		}

		throw TInterpreterException(TInterpreterException::ONLY_INTERGER_COULD_TAKE_MOD_OPERATION, node->GetToken()->GetLineNumber());
	}break;
	case TToken::OPERATOR_GREATE_THAN:case TToken::OPERATOR_GREATE_EQUAL:case TToken::OPERATOR_LESS_THAN:case TToken::OPERATOR_LESS_EQUAL:
	case TToken::OPERATOR_EQUAL: case TToken::OPERATOR_NOT_EQUAL:
	{
		auto firstType = GetSymbolType(node->GetFirstChild());
		auto secondType = GetSymbolType(node->GetFirstChild()->GetSibling());
		if ((firstType == CSymbol::TYPE_INTERGER || firstType == CSymbol::TYPE_DOUBLE)
			&& (secondType == CSymbol::TYPE_INTERGER || secondType == CSymbol::TYPE_DOUBLE))
		{
			return CSymbol::TYPE_BOOL;
		}
		throw TInterpreterException(TInterpreterException::ONLY_NUMBER_COULD_TAKE_RELATIONAL_OPERATION, node->GetToken()->GetLineNumber());
	}break;
	case TToken::OPERATOR_AND:case TToken::OPERATOR_OR:
	{
		auto firstType = GetSymbolType(node->GetFirstChild());
		auto secondType = GetSymbolType(node->GetFirstChild()->GetSibling());
		if (firstType == CSymbol::TYPE_BOOL && secondType == CSymbol::TYPE_BOOL)
		{
			return CSymbol::TYPE_BOOL;
		}
		throw TInterpreterException(TInterpreterException::ONLY_BOOL_COULD_TAKE_LOGICAL_OPERATION, node->GetToken()->GetLineNumber());
	}break;
	default:
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, node->GetToken()->GetLineNumber());
		break;
	}
}

const CSymbol::SymbolType TAstNodeOperator::GetTerminalSymbolType(const std::shared_ptr<TAstNode>& node)
{
	switch (node->GetToken()->GetType())
	{
	case TToken::LITERAL_INTERGER:return CSymbol::TYPE_INTERGER;
	case TToken::LITERAL_DOUBLE:return CSymbol::TYPE_DOUBLE;
	case TToken::LITERAL_BOOL:return CSymbol::TYPE_BOOL;
	case TToken::LITERAL_STRING:return CSymbol::TYPE_STRING;
	case TToken::ID:
	{
		auto var = TVariateManager::GetInstance()->GetVariateSrollUp(TContext::GetCurrentScope(),
			static_cast<TTokenWithValue<QString>*>(node->GetToken().get())->GetValue());

		if (var)
		{
			return var->GetType();
		}
		else
		{
			throw TInterpreterException(TInterpreterException::NOT_FIND_VARIATE, node->GetToken()->GetLineNumber(),
				static_cast<TTokenWithValue<QString>*>(node->GetToken().get())->GetValue());
		}
	}break;
	default:
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, node->GetToken()->GetLineNumber());
	}
	}
}

TAstNodeOperator::ValueReturned TAstNodeOperator::ReturnValue(const TAstNode* const node) const
{
	if (!node->GetFirstChild())
	{
		return ReturnTerminalValue(node);
	}

	return ReturnNonTerminalValue(node);
}

QHash<TToken::TokenType, TAstNodeOperator::OperatorProperty> TAstNodeOperator::m_operatorProperties{};

TAstNodeOperator::Inilization TAstNodeOperator::m_inilization{};

TAstNodeOperator::Inilization::Inilization()
{
	m_operatorProperties.insert(TToken::OPERATOR_MULTIPLY, TAstNodeOperator::OperatorProperty{ 3, true });
	m_operatorProperties.insert(TToken::OPERATOR_DIVIDE, TAstNodeOperator::OperatorProperty{ 3, true });
	m_operatorProperties.insert(TToken::OPERATOR_PERCENT, TAstNodeOperator::OperatorProperty{ 3, true });
	m_operatorProperties.insert(TToken::OPERATOR_POWER, TAstNodeOperator::OperatorProperty{ 3, true });

	m_operatorProperties.insert(TToken::OPERATOR_PLUS, TAstNodeOperator::OperatorProperty{ 4, true });
	m_operatorProperties.insert(TToken::OPERATOR_MINUS, TAstNodeOperator::OperatorProperty{ 4, true });

	m_operatorProperties.insert(TToken::OPERATOR_GREATE_THAN, TAstNodeOperator::OperatorProperty{ 6, true });
	m_operatorProperties.insert(TToken::OPERATOR_GREATE_EQUAL, TAstNodeOperator::OperatorProperty{ 6, true });
	m_operatorProperties.insert(TToken::OPERATOR_LESS_THAN, TAstNodeOperator::OperatorProperty{ 6, true });
	m_operatorProperties.insert(TToken::OPERATOR_LESS_EQUAL, TAstNodeOperator::OperatorProperty{ 6, true });

	m_operatorProperties.insert(TToken::OPERATOR_EQUAL, TAstNodeOperator::OperatorProperty{ 7, true });
	m_operatorProperties.insert(TToken::OPERATOR_NOT_EQUAL, TAstNodeOperator::OperatorProperty{ 7, true });

	m_operatorProperties.insert(TToken::OPERATOR_AND, TAstNodeOperator::OperatorProperty{ 11, true });
	m_operatorProperties.insert(TToken::OPERATOR_OR, TAstNodeOperator::OperatorProperty{ 12, true });

}

TAstNodeOperator::OperatorProperty::OperatorProperty(const int p,const bool l):priority(p), isLeft(l){}
