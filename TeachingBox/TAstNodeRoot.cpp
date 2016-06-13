#include "stdafx.h"
#include "TAstNodeRoot.h"
#include "CSymbol.h"
#include "TContext.h"



TAstNodeRoot::TAstNodeRoot(const std::shared_ptr<TToken> token /*= nullptr*/)
{

}

TAstNodeRoot::~TAstNodeRoot()
{

}

TAstNode::ValueReturned TAstNodeRoot::Execute() const
{
	/*auto child = m_firstChild;

	while (child)
	{
		child->Execute();
		child = child->GetSibling();
	}*/
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
