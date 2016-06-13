#include "stdafx.h"
#include "TContext.h"
#include "TAstNode.h"


const QString& TContext::GetCurrentScope()
{
	return m_currentScope;
}

const std::shared_ptr<TAstNode> TContext::GetRootNode()
{
	return m_rootNode;
}

TAstNode* TContext::GetNextNode()
{
	return m_nextNode;
}

void TContext::SetCurrentScope(const QString& scope)
{
	m_currentScope = scope;
}

void TContext::SetRootNode(const std::shared_ptr<TAstNode> rootNode)
{
	m_rootNode = rootNode;
}


void TContext::SetNextNode(TAstNode* const nextNode)
{
	m_nextNode = nextNode;
}

TAstNode* TContext::m_nextNode{};

std::shared_ptr<TAstNode> TContext::m_rootNode{};

QString TContext::m_currentScope{};
