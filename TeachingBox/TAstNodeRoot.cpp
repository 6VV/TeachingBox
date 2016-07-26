#include "stdafx.h"
#include "TAstNodeRoot.h"
#include "CSymbol.h"
#include "TContext.h"



TAstNodeRoot::TAstNodeRoot(const std::shared_ptr<TToken> token /*= nullptr*/)
	:TAstNode(token)
{

}

TAstNodeRoot::~TAstNodeRoot()
{

}

TAstNode::ValueReturned TAstNodeRoot::Execute() const
{
	TContext::SetNextNode(m_firstChild.get());

	return ValueReturned{ 0, CSymbol::TYPE_VOID };
}

void TAstNodeRoot::ParseSemantic() const
{
	auto child = m_firstChild;

	while (child)
	{
		child->ParseSemantic();
		child = child->GetSibling();
	}
}
