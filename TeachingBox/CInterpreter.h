#ifndef _INTERPRETER_C_INTERTERTER_H_
#define _INTERPRETER_C_INTERTERTER_H_

#include "CAstTreeNode.h"
#include "CValue.h"
#include "CParseSemantic.h"
#include "CTcpManager.h"
#include "CInterpreterTcpAdapter.h"

class CInterpreter
{
private:
	const int TYPE_INT = 1;
	const int TYPE_DOUBLE = 2;
	const int TYPE_BOOL = 3;

	struct TYPE_VALUE
	{
		int type=-1;
		int intResult;
		double doubleResult;
	};

	struct ProgramStack 
	{
		CAstTreeNode* lastProgramNode;	/*上个函数节点*/
		CAstTreeNode* lastNode;	/*上个函数的下一个节点*/
	};

public:
	//CInterpreter(CParseSemantic& parseSemantic);
	CInterpreter(CAstTreeNode* rootNode);
	~CInterpreter();

	//void Execute();				/*执行*/
	//void Execute(int lineNumber);	/*执行某一行*/
	void ExecuteStep();	/*单步执行*/
	void ExecuteNextLine();			/*执行语句*/
	void ExecuteNextCommand();		/*执行到下一条需要发送指令的语句*/

	void SetStartNode(const std::string& programName, int lineNumber);	/*设置起始执行位置*/
	//void SetStart(bool isStart);	/*是否执行*/
	//bool IsInterpretering();		/*是否正在执行*/

private:
	void ExecuteFinish();	/*检查执行状态*/

	//void ExecuteFromLine(std::string& programName, int lineNumber);	/*从某一行开始执行*/
	//void ExecuteFromLine(CAstTreeNode* programNode, int lineNumber);

	void ExecuteProgram();				/*执行程序*/
	void ExecuteEofSentence();			/*执行Eof语句*/
	void ExecuteGosubSentence();		/*执行gosub语句*/

	void ExecuteAssignExp();		/*执行赋值表达式*/

	void ExecuteIfSentence();		/*执行If语句*/
	void ExecuteElseIfSentence();	/*执行ElseIf语句*/
	void ExecuteElseSentence();		/*执行Else语句*/
	void ExecuteEndIfSentence();	/*执行EndIf语句*/

	void ExecuteForSentence();		/*执行For语句*/
	void ExecuteNextSentence();		/*执行Next语句*/
	void ExecuteBreakSentence();	/*执行Break语句*/

	void ExecuteSwitchSentence();/*执行Switch语句*/
	void ExecuteCaseSentence();	/*执行Case语句*/
	void ExecuteDefaultSentence();/*执行Default语句*/
	void ExecuteEndSwitchSentence();	/*执行EndSwitch语句*/

	void ExecuteMovlSentence();	/*执行movl语句*/
	void ExecuteMovcSentence();	/*执行movc语句*/

	TYPE_VALUE GetExpResult(CAstTreeNode* rootNode);	/*计算表达式的值*/
	bool GetBoolResult(CAstTreeNode* rootNode);	/*计算布尔表达式值*/
	bool GetLogicalExpResult(CAstTreeNode* rootNode);	/*计算逻辑表达式*/
	bool GetRelationalExpResult(CAstTreeNode* rootNode);	/*计算关系表达式*/

	TYPE_VALUE GetNumExpResult(CAstTreeNode* rootNode);	/*计算数值表达式*/
	double GetFunctionExpResult(CAstTreeNode* rootNode);	/*计算函数表达式*/
	TYPE_VALUE GetOperatorExpResult(CAstTreeNode* rootNode);	/*计算操作符表达式*/

	std::string GetWholeProgramName(const std::string& programSelfName);	/*获取程序名*/
	CAstTreeNode* GetProgramNode(const std::string& program);	/*获取程序节点*/

	void FindNextValidNode();	/*寻找下一个可执行的节点*/
	bool FindStartNode(CAstTreeNode* rootNode, int lineNumber);	/*查找开始执行的节点*/

	//void FindFunctionNode();	/*记录主函数及子函数位置*/

private:
	void AddLock(char *head,int &offset);		/*添加锁*/
	//void AddLength(char* head, int &offset, int length);	/*添加长度*/
	void AddCommandAttribute(char* head, int& offset, int commandCode, int commandLength);	/*添加属性*/

	template<typename TYPE>
	void AddCommand(char* head, int& offset, const TYPE& command);	/*添加命令*/

	template<typename TYPE>
	void AddDataSended(char* head, const TYPE& command, const int commandId);	/*添加所用发送的字节*/

	//void AddDataSendedWithoutCommand(char* head, const int commandId);	/*添加所用发送的字节（不含命令段）*/

	CValue::TYPE_POSITION GetPosition(const std::string& name);
	CValue::TYPE_DYNAMIC &GetDynamic(const std::string& name);
	CValue::TYPE_OVERLAP &GetOverlap(const std::string& name);

	tMovLParam GetMovlParameter(CAstTreeNode* parentNode);
	tMovCParam GetMovcParameter(CAstTreeNode* parentNode);

	void SendData();	/*发送命令*/
	void SendEndData();		/*发送结束命令*/
	void SendMovlData();	/*发送Movl指令*/
	void SendMovcData();	/*发送Movc命令*/

private:
	CValue* m_value;
	CAstTreeNode* m_rootNode;	/*根节点*/
	CAstTreeNode* m_currentNode;	/*当前执行语句*/
	CAstTreeNode* m_currentProgramNode=NULL;	/*当前程序节点*/
	CScope* m_scope;	/*作用域名*/

	bool m_isSendData;	/*是否发送了指令*/

	std::list<ProgramStack> m_stackFunction;	/*函数调用堆栈，存储上一个调用节点的位置*/

	CInterpreterTcpAdatper* m_interpreterTcpAdapter = NULL;
	char m_commandSend[COMMAND_MAX_LENGTH];
};

template<typename TYPE>
void CInterpreter::AddCommand(char* head, int& offset, const TYPE& command)
{
	*(TYPE*)(head + offset) = command;
	offset += sizeof(TYPE);
}

template<typename TYPE>
void CInterpreter::AddDataSended(char* head, const TYPE& command, const int commandId)
{
	int offset = 0;
	AddLock(head, offset);
	//AddLength(head, offset, sizeof(TYPE));
	AddCommandAttribute(head, offset, commandId, sizeof(TYPE));
	AddCommand(head, offset, command);
}


#endif