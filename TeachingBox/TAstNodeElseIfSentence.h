#ifndef _TEACHING_BOX_T_AST_NODE_ELSE_IF_SENTENCE_H_
#define _TEACHING_BOX_T_AST_NODE_ELSE_IF_SENTENCE_H_

#include "TAstNode.h"

class TAstNodeElseIfSentence:public TAstNode
{
public:
	TAstNodeElseIfSentence(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeElseIfSentence();

	static const std::shared_ptr<TAstNode> GetAstNode(TLexer* const lexer);

	virtual ValueReturned Execute() const override;

	virtual void ParseSemantic() const override;


};

#endif