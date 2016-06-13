#ifndef _TEACHING_BOX_T_AST_NODE_IF_SENTENCE_H_
#define _TEACHING_BOX_T_AST_NODE_IF_SENTENCE_H_

#include "TAstNode.h"

class TLexer;

class TAstNodeIfSentence:public TAstNode
{
public:
	TAstNodeIfSentence(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeIfSentence();

	static const std::shared_ptr<TAstNode> GetAstNode(TLexer* const lexer);


	virtual ValueReturned Execute() const override;

	virtual void ParseSemantic() const override;

private:
	static void AddIfChild(TLexer* const lexer, std::shared_ptr<TAstNode> result);
	static void AddContent(TLexer* const lexer, std::shared_ptr<TAstNode> result);


};

#endif