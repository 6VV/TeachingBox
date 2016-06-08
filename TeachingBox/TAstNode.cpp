#include "stdafx.h"
#include "TAstNode.h"


TAstNode::TAstNode(const std::shared_ptr<TToken> token /*= nullptr*/)
	:m_token(token)
{

}

TAstNode::~TAstNode()
{

}

void TAstNode::AddChild(std::shared_ptr<TAstNode> child)
{
	child->m_parentNode = this;
	child->m_siblingChild = nullptr;

	if (!m_firstChild)
	{
		m_firstChild = child;
		m_endChild = child;
	}
	else
	{
		m_endChild->m_siblingChild = child;
		m_endChild = child;
	}
}

std::shared_ptr<TToken> TAstNode::GetToken() const
{
	return m_token;
}

std::shared_ptr<TAstNode> TAstNode::GetFirstChild() const
{
	return m_firstChild;
}

std::shared_ptr<TAstNode> TAstNode::GetSibling() const
{
	return m_siblingChild;
}

std::shared_ptr<TVariate> TAstNode::Execute()
{
	return nullptr;
}
