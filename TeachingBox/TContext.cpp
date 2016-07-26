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

bool TContext::GetExecuteState()
{
	return m_executeState;
}

const TAstNode* TContext::GetCurrentProgramNode()
{
	return m_currentProgramNode;
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

void TContext::SetExecuteState(bool state)
{
	m_executeState = state;
}

void TContext::SetCurrentProgramNode(const TAstNode* const currentProgramNode)
{
	m_currentProgramNode = currentProgramNode;
}

bool TContext::m_executeState = false;

const TAstNode* TContext::m_currentProgramNode{ nullptr };

TAstNode* TContext::m_nextNode{};

std::shared_ptr<TAstNode> TContext::m_rootNode{};

QString TContext::m_currentScope{};
