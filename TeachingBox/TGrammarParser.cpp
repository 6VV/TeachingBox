#include "stdafx.h"
#include "TGrammarParser.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TAstNode.h"
#include "TAstNodeOperator.h"



TGrammarParser::TGrammarParser(TLexer& lexer) :m_lexer(&lexer), m_rootNode(new TAstNode(nullptr))
{
	Parse();
}

std::shared_ptr<TAstNode> TGrammarParser::GetRootNode() const
{
	return m_rootNode;
}

void TGrammarParser::Parse()
{
	auto token = m_lexer->GetToken();
	m_lexer->UnGetToken();
	switch (token->GetType())
	{
	case TToken::ID:
	{
		ParseAssignSentence();
	}break;
	default:
	{
		ThrowException(TInterpreterException::WRONG_GRAMMAR, token->GetLineNumber());
	}break;
	}
}

void TGrammarParser::ParseAssignSentence()
{
	std::shared_ptr<TAstNode> firstChild(new TAstNode(m_lexer->GetToken()));

	auto assignToken = m_lexer->GetToken();
	if (assignToken->GetType() != TToken::OPERATOR_ASSIGN)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, assignToken->GetLineNumber());
	}

	std::shared_ptr<TAstNode> assignNode(new TAstNodeOperator(assignToken));
	m_rootNode->AddChild(assignNode);
	assignNode->AddChild(firstChild);
	assignNode->AddChild(GetExpressionNode());
}

std::shared_ptr<TAstNode> TGrammarParser::PeekOperator()
{
	auto token = m_lexer->GetToken();
	m_lexer->UnGetToken();

	if (TAstNodeOperator::IsExistOperator(token->GetType()))
	{
		return GetNode(token);
	}
	return nullptr;
}

void TGrammarParser::ThrowException(const TInterpreterException::InterpreterExceptionCode exceptionCode, const int lineNubmer)
{
	throw TInterpreterException(exceptionCode, lineNubmer);
}

std::shared_ptr<TAstNode> TGrammarParser::GetNode(const std::shared_ptr<TToken>& token)
{
	return std::shared_ptr<TAstNode>(new TAstNode(token));
}

std::shared_ptr<TAstNode> TGrammarParser::GetExpressionNode()
{
	auto leftValue = GetValue();
	
	while (auto leftOper = PeekOperator())
	{
		leftValue = GetShiftOperator(leftValue, leftOper);
	}

	return leftValue;
}

std::shared_ptr<TAstNode> TGrammarParser::GetValue()
{
	auto value = m_lexer->GetToken();
	switch (value->GetType())
	{
	case TToken::LITERAL_INTERGER:case TToken::LITERAL_DOUBLE:case TToken::LITERAL_BOOL:case TToken::ID:
	{
		return GetNode(value);
	}break;
	case TToken::OPERATOR_MINUS:
	{
		auto nextValue = m_lexer->GetToken();
		switch (nextValue->GetType())
		{
		case TToken::LITERAL_INTERGER:case TToken::LITERAL_DOUBLE:case TToken::ID:
		{
			auto result = GetNode(value);
			result->AddChild(GetNode(nextValue));
			return result;
		}break;
		default:
		{
			ThrowException(TInterpreterException::NEGATIVE_SHOULD_WITH_INT_OR_DOUBLE, value->GetLineNumber());
		}break;
		}
	}break;
	case TToken::OPERATOR_NEGATION:
	{
		auto nextValue = m_lexer->GetToken();
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
			ThrowException(TInterpreterException::NEGATION_SHOULD_WITH_BOOL, value->GetLineNumber());
		}break;
		}
	}break;
	case TToken::OPERATOR_LEFT_BRACKET:
	{
		auto nextValue = GetExpressionNode();
		if (m_lexer->GetToken()->GetType()!=TToken::OPERATOR_RIGHT_BRACKET)
		{
			ThrowException(TInterpreterException::NOT_FIND_RIGHT_BRACKET, value->GetLineNumber());
		}

		return nextValue;
	}break;
	default:
	{
		ThrowException(TInterpreterException::WRONG_GRAMMAR, value->GetLineNumber());
	}
		break;
	}
	return nullptr;
}

std::shared_ptr<TAstNode> TGrammarParser::GetShiftOperator(const std::shared_ptr<TAstNode>& leftValue, const std::shared_ptr<TAstNode>& leftOper)
{
	m_lexer->GetToken();
	auto rightValue = GetValue();
	std::shared_ptr<TAstNode> rightOper;

	while ((rightOper = PeekOperator()) != nullptr && IsRightExpr(leftOper, rightOper))
	{
		rightValue = GetShiftOperator(rightValue, rightOper);
	}

	leftOper->AddChild(leftValue);
	leftOper->AddChild(rightValue);
	return leftOper;
	
}

const bool TGrammarParser::IsRightExpr(const std::shared_ptr<TAstNode>& leftOper, const std::shared_ptr<TAstNode>& rightOper) const
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
