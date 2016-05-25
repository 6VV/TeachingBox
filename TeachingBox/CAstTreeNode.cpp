#include "stdafx.h"
#include "CAstTreeNode.h"


CAstTreeNode::CAstTreeNode(CToken* token)
{
	m_token = token;
}

CAstTreeNode::CAstTreeNode()
{

}

/*清除该节点及所有子节点*/
CAstTreeNode::~CAstTreeNode()
{
	delete(m_token);
	ClearChildren();
}

void CAstTreeNode::AddChildNode(CAstTreeNode* newChildNode)
{
	newChildNode->m_parentNode = this;

	if (m_firstChildNode==NULL)
	{
		m_firstChildNode = newChildNode;
		m_endChildNode = newChildNode;
	}
	else
	{
		m_endChildNode->m_nextSiblingNode = newChildNode;
		m_endChildNode = newChildNode;
	}

	m_endChildNode->m_nextSiblingNode = NULL;
}

void CAstTreeNode::ClearChildren()
{
	CAstTreeNode* currentNode = m_firstChildNode;
	CAstTreeNode* preNode;
	while (currentNode != NULL)
	{
		preNode = currentNode;
		currentNode = currentNode->m_nextSiblingNode;
		delete(preNode);
	}
	m_firstChildNode = NULL;
}

