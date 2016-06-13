#ifndef _TEACHING_BOX_T_AST_NODE_ELSE_H_
#define _TEACHING_BOX_T_AST_NODE_ELSE_H_

#include "TAstNode.h"

class TAstNodeElseSentence:public TAstNode
{
public:
	TAstNodeElseSentence(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeElseSentence();

	static const std::shared_ptr<TAstNode> GetAstNode(TLexer* const lexer);

	virtual ValueReturned Execute() const override;

	virtual void ParseSemantic() const override;

};

#endif