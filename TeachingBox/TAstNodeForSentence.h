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
	static double GetValue(CSymbol::SymbolType type, const QString& scope, const QString& name);

	virtual ValueReturned Execute() const override;

	static const std::shared_ptr<TAstNode> GetAstNode(TLexer* const lexer);

	virtual void ParseSemantic() const override;
	
private:
	static void AddToNode(TLexer* const lexer, std::shared_ptr<TToken> token, std::shared_ptr<TAstNode> result);
	static void AddSentenceNodes(TLexer* const lexer, std::shared_ptr<TAstNode> result);
	static void AddStepNode(TLexer* const lexer, std::shared_ptr<TToken> token, std::shared_ptr<TAstNode> result);

	static void CheckLineBreak(TLexer* const lexer, std::shared_ptr<TToken> token);


};

#endif