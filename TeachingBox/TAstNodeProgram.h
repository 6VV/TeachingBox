#ifndef _TEACHING_BOX_T_AST_NODE_PROGRAM_H_
#define _TEACHING_BOX_T_AST_NODE_PROGRAM_H_

#include "TAstNode.h"

class TAstNodeProgram:public TAstNode
{
public:
	TAstNodeProgram(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeProgram();

	static const std::shared_ptr<TAstNode> GetAstNode(TLexer* const lexer, const std::shared_ptr<TToken> token);

	virtual ValueReturned Execute() const override;

	virtual void ParseSemantic() const override;

private:
};

#endif