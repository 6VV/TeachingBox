#ifndef _TEACHING_BOX_T_CONTEXT_H_
#define _TEACHING_BOX_T_CONTEXT_H_

#include "CSingleTon.h"

class TAstNode;

class TContext
{
public:
	static const QString& GetCurrentScope();
	static const std::shared_ptr<TAstNode> GetRootNode();
	static TAstNode* GetNextNode();
	static bool GetExecuteState();
	static const TAstNode* GetCurrentProgramNode();

	static void SetCurrentScope(const QString& scope);
	static void SetRootNode(const std::shared_ptr<TAstNode> rootNode);
	static void SetNextNode(TAstNode* const nextNode);
	static void SetExecuteState(bool state);
	static void SetCurrentProgramNode(const TAstNode* const currentProgramNode);

private:
	static QString m_currentScope;
	static std::shared_ptr<TAstNode> m_rootNode;
	static TAstNode*  m_nextNode;
	static bool m_executeState;
	static const TAstNode* m_currentProgramNode;
};

#endif