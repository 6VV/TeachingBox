#ifndef _TEACHING_BOX_T_AST_NODE_ROOT_H_
#define _TEACHING_BOX_T_AST_NODE_ROOT_H_

#include "TAstNode.h"

class TAstNodeRoot:public TAstNode
{
public:
	TAstNodeRoot(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeRoot();

	virtual ValueReturned Execute() const override;

	virtual void ParseSemantic() const override;

};

#endif