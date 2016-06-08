#include "stdafx.h"
#include "TAstNodeOperator.h"
#include "TToken.h"



TAstNodeOperator::TAstNodeOperator(const std::shared_ptr<TToken> token /*= nullptr*/)
	: TAstNode(token)
{

}

TAstNodeOperator::~TAstNodeOperator()
{

}

std::shared_ptr<TVariate> TAstNodeOperator::Execute()
{
	throw std::logic_error("The method or operation is not implemented.");
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
