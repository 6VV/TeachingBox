#ifndef _TEACHING_BOX_T_AST_NODE_END_IF_SENTENCE_H_
#define _TEACHING_BOX_T_AST_NODE_END_IF_SENTENCE_H_

#include "TAstNode.h"

class TAstNodeEndIfSentence:public TAstNode
{
public:
	TAstNodeEndIfSentence(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeEndIfSentence();

	static const std::shared_ptr<TAstNode> GetAstNode(TLexer* const lexer);

	virtual ValueReturned Execute() const override;

	virtual void ParseSemantic() const override;

	virtual TAstNode* FindNextValidNode() const override;

};

#endif