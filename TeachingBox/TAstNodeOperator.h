#ifndef _TEACHING_BOX_T_AST_NODE_OPERATOR_H_
#define _TEACHING_BOX_T_AST_NODE_OPERATOR_H_

#include "TAstNode.h"
#include <memory>
#include "TToken.h"


class TAstNodeOperator:public TAstNode
{
public:
	TAstNodeOperator(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeOperator();


	virtual std::shared_ptr<TVariate> Execute() override;
	
	static const int GetPriority(const TToken::TokenType type);
	static const bool IsLeftAssociativity(const TToken::TokenType type);
	static const bool IsExistOperator(const TToken::TokenType type);

private:
	struct OperatorProperty 
	{
		OperatorProperty(){};
		OperatorProperty(const int p, const bool l);
		int priority=0;
		bool isLeft = true;
	};

	static QHash<TToken::TokenType, OperatorProperty> m_operatorProperties;
	
	class Inilization
	{
	public:
		Inilization();
	};
	static Inilization m_inilization;
};

#endif