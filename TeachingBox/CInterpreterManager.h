#ifndef _INTERPRETER_C_INTERPRETER_BUILDER_H_
#define _INTERPRETER_C_INTERPRETER_BUILDER_H_

#include "CToken.h"
#include "CLexer.h"
#include "CAstTreeNode.h"
#include "CExceptionInterpreter.h"
#include "CParseGrammar.h"
#include "CParseSemantic.h"
#include "CInterpreter.h"
#include "CSingleTon.h"

class CInterpreterManager
{
	friend CSingleTon<CInterpreterManager>;

public:
	const static int MATCH_CASE = 1;
	const static int MATCH_NOCASE = 2;

	static int m_matchState;

private:
	CInterpreterManager();
	CInterpreterManager(const CInterpreterManager&);
	CInterpreterManager& operator=(const CInterpreterManager&);
	~CInterpreterManager();

public:
	static CInterpreterManager* GetInstance();

	void AddProgramNode(const std::string& strProgramName,const std::string& strProgramText);	/*添加程序节点*/
	CAstTreeNode* FindProgramNode(const std::string& strProgramName);	/*查找程序节点*/

	void RemoveProgramNode(const std::string& strProgramName);	/*移除程序节点*/
	void RemoveAllPrograms();	/*清除所有程序节点*/

	void ExecuteFromLine(std::string& strProgramName, int lineNumber);
	void ExecuteStep(std::string& strProgramName, int lineNumber);
	void StartExecute();
	void StopExecute();	/*停止执行*/

	void ParseSemantic(std::string& strContext);
	void ParseSemantic(const std::string& strContext, CAstTreeNode* node);

	void InterpreteNextLine();	/*执行下一行*/
	void InterpreteNextCommand();	/*执行到下一命令（需发送指令到控制器）*/

	static void GetWarningInfo(CExceptionInterpreter&e, std::string& warningInfo);
	void DisplayWarningInfo(CExceptionInterpreter& e);
	void Clear();	/*清除成员变量*/

private:
	bool IsInterpreting();

private:
	CAstTreeNode* m_rootNode;	/*程序根节点*/
	CInterpreter* m_interpreter = NULL;	/*解释器*/

private:
	//static CInterpreterManager* m_instance;
	///*释放内存*/
	//class CGarbo
	//{
	//public:
	//	CGarbo();
	//	~CGarbo()
	//	{
	//		if (CInterpreterManager::m_instance)
	//		{
	//			delete CInterpreterManager::m_instance;
	//			CInterpreterManager::m_instance = NULL;
	//		}
	//	}
	//};
	//static CGarbo m_garbo;
};

#endif