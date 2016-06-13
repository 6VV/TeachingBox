#ifndef _TEACHING_BOX_T_AST_NODE_OPERATOR_H_
#define _TEACHING_BOX_T_AST_NODE_OPERATOR_H_

#include "TAstNode.h"
#include <memory>
#include "TToken.h"

class TLexer;

class TAstNodeOperator:public TAstNode
{
public:
	TAstNodeOperator(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeOperator();

	virtual ValueReturned Execute() const override;


	static const std::shared_ptr<TAstNode> GetAstNode(TLexer* const lexer);
	static const CSymbol::SymbolType GetSymbolType(const std::shared_ptr<TAstNode>& node);

private:
	static const std::shared_ptr<TAstNode> GetNode(const std::shared_ptr<TToken>& token);
	static const int GetPriority(const TToken::TokenType type);
	static const bool IsLeftAssociativity(const TToken::TokenType type);
	static const bool IsExistOperator(const TToken::TokenType type);
	static const std::shared_ptr<TAstNode> GetValue(TLexer* const lexer);
	static const std::shared_ptr<TAstNode> GetShiftOperator(const std::shared_ptr<TAstNode>& leftValue,
		const std::shared_ptr<TAstNode>& leftOper, TLexer* const lexer);

	static const std::shared_ptr<TAstNode> PeekOperator(TLexer* const lexer);

	static const bool IsRightExpr(const std::shared_ptr<TAstNode>& leftOper, const std::shared_ptr<TAstNode>& rightOper);

private:
	static const CSymbol::SymbolType GetNonTerminalSymbol(const std::shared_ptr<TAstNode>& node);
	static const CSymbol::SymbolType GetTerminalSymbolType(const std::shared_ptr<TAstNode>& node);

	ValueReturned ReturnValue(const TAstNode* const node) const;
	ValueReturned ReturnTerminalValue(const TAstNode* const node) const;
	ValueReturned ReturnNonTerminalValue(const TAstNode* const node) const;

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