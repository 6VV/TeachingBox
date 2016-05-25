#ifndef _INTERPRETER_C_TREE_NODE_H_
#define _INTERPRETER_C_TREE_NODE_H_

#include "CToken.h"

class CAstTreeNode
{
public:
	CAstTreeNode(CToken* token);
	CAstTreeNode();
	~CAstTreeNode();

	void AddChildNode(CAstTreeNode* newChildNode);
	void ClearChildren();


	CToken* m_token=NULL;

	CAstTreeNode *m_firstChildNode = NULL;
	CAstTreeNode *m_endChildNode = NULL;
	CAstTreeNode *m_nextSiblingNode = NULL;
	CAstTreeNode *m_parentNode = NULL;
};

#endif