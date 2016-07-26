#include "stdafx.h"
#include "TInterpreter.h"
#include "TLexer.h"
#include "TContext.h"
#include "TAstNodeFactory.h"
#include "TAstNode.h"
#include "CTcpManager.h"
#include "CTcpSpecialCommand.h"
#include "CCodeEditor.h"

void TInterpreter::ExecuteAll(int lineNum)
{
	TContext::SetExecuteState(true);
	InitContext(lineNum);

	TAstNode* nextNode = nullptr;
	while ((nextNode = TContext::GetNextNode()) != nullptr && TContext::GetExecuteState())
	{
		nextNode->Execute();
	}

	StopExecute();
}

void TInterpreter::ExecuteOneLine(int lineNum)
{
	InitContext(lineNum);
	TAstNode* nextNode = TContext::GetNextNode();

	if (nextNode!=nullptr)
	{
		nextNode->Execute();
	}
	CCodeEditor::GetInstance()->HighlightPCLine(TContext::GetNextNode()->GetToken()->GetLineNumber());
}

void TInterpreter::StopExecute()
{
	TContext::SetExecuteState(false);
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandStopExecute());
}

void TInterpreter::Parse(const QString& text)
{
	auto rootNode = TAstNodeFactory::CreateAst();
	rootNode->ParseSemantic();

	TContext::SetRootNode(rootNode);
	TContext::SetNextNode(rootNode.get());
}

TAstNode* TInterpreter::FindStartNode(std::shared_ptr<TAstNode> rootNode,int lineNumber)
{
	std::shared_ptr<TAstNode> currentNode = rootNode->GetFirstChild();
	if (currentNode == nullptr)
	{
		return nullptr;
	}
	else if (currentNode->GetToken()->GetLineNumber() == lineNumber)
	{
		return currentNode.get();
	}

	/*查询最后一个不大于起始行的子节点*/
	while (currentNode->GetSibling() != nullptr)
	{
		if (currentNode->GetSibling()->GetToken()->GetLineNumber() < lineNumber)
		{
			currentNode = currentNode->GetSibling();
		}
		else if (currentNode->GetSibling()->GetToken()->GetLineNumber() == lineNumber)
		{
			return currentNode->GetSibling().get();
		}
		else
		{
			break;
		}
	}

	return FindStartNode(currentNode, lineNumber);
}

void TInterpreter::InitContext(int lineNum)
{
	TContext::SetNextNode(FindStartNode(TContext::GetRootNode(), lineNum));

	TAstNode* node = TContext::GetNextNode();
	while (node->GetParentNode()->GetParentNode())
	{
		node = node->GetParentNode();
	}

	TContext::SetCurrentProgramNode(node);
}


