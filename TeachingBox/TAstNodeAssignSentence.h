#ifndef _TEACHING_BOX_T_AST_NODE_ASSIGN_SENTENCE_H_
#define _TEACHING_BOX_T_AST_NODE_ASSIGN_SENTENCE_H_

#include "TAstNode.h"

class TLexer;

class TAstNodeAssignSentence:public TAstNode
{
public:
	TAstNodeAssignSentence(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeAssignSentence();

	virtual ValueReturned Execute() const override;

	static const std::shared_ptr<TAstNode> GetAstNode(TLexer* const lexer);
	static const std::shared_ptr<TAstNode> GetAssignException(TLexer* const lexer);

	virtual void ParseSemantic() const override;
};

#endif