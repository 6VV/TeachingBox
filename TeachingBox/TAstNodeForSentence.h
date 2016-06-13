#ifndef _TEACHING_BOX_T_AST_NODE_FOR_SENTENCE_H_
#define _TEACHING_BOX_T_AST_NODE_FOR_SENTENCE_H_

#include "TAstNode.h"
#include "CSymbol.h"

class TLexer;

class TAstNodeForSentence:public TAstNode
{
public:
	TAstNodeForSentence(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeForSentence();

	static void UpdateValue(CSymbol::SymbolType type, const QString& scope, const QString& name, double value);

	static const std::shared_ptr<TAstNode> GetAstNode(TLexer* const lexer);
	static double GetValue(CSymbol::SymbolType type, const QString& scope, const QString& name);

	virtual ValueReturned Execute() const override;
	virtual void ParseSemantic() const override;
	
private:
	static void AddToNode(TLexer* const lexer, std::shared_ptr<TToken> token, std::shared_ptr<TAstNode> result);
	static void AddStepNode(TLexer* const lexer, std::shared_ptr<TToken> token, std::shared_ptr<TAstNode> result);

};

#endif