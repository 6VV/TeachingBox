#include "stdafx.h"
#include "TAstNode.h"
#include "TToken.h"


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
	child->m_siblingNode = nullptr;

	if (!m_firstChild)
	{
		m_firstChild = child;
		m_endChild = child;
	}
	else
	{
		m_endChild->m_siblingNode = child;
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
	return m_siblingNode;
}

TAstNode::ValueReturned TAstNode::Execute() const
{
	return ValueReturned{};
}

bool TAstNode::IsEofOrEol(const int type)
{
	if (type==TToken::SEPARATOR_EOF || type==TToken::SEPARATOR_EOL)
	{
		return true;
	}

	return false;
}

TAstNode* TAstNode::FindNextValidNode() const
{
	if (m_siblingNode != nullptr)
	{
		return m_siblingNode.get();
	}

	if (m_parentNode != nullptr)
	{
		return m_parentNode->FindNextValidNode();
	}

	return nullptr;
}
