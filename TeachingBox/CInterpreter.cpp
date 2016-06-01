#include "stdafx.h"
#include "CInterpreter.h"
#include "CTokenCategory.h"
#include "CToken.h"
#include "CTokenString.h"
#include <math.h>
#include "CExceptionInterpreter.h"
#include "CWarningInformation.h"
#include "CTokenConstantNum.h"
#include "CInterprerterState.h"
#include "CInterpreterTcpAdapter.h"
#include "TVariateManager.h"
#include "TInteger.h"
#include "TDouble.h"
#include "TBool.h"
#include "TPosition.h"
#include "TDynamic.h"


//
//CInterpreter::CInterpreter(CParseSemantic& parseSemantic)
//{
//	m_value = CValue::GetInstance();
//	m_rootNode = parseSemantic.GetAstTreeRootNode();
//	//FindFunctionNode();
//}

CInterpreter::CInterpreter(CAstTreeNode* rootNode)
{
	m_value = CValue::GetInstance();
	m_rootNode = rootNode;

	m_interpreterTcpAdapter = new CInterpreterTcpAdatper;
}

CInterpreter::~CInterpreter()
{
	delete(m_interpreterTcpAdapter);
}

void CInterpreter::ExecuteStep()
{
	ExecuteNextLine();
	SendEndData();
}

void CInterpreter::ExecuteFinish()
{
	/*若执行完毕*/
	SendEndData();
	CInterpreterState::GetInstance()->SetAdmit(false);

	m_currentNode = NULL;
}

//void CInterpreter::Execute()
//{
//	/*CAstTreeNode* currentChildNode = rootNode->m_firstChildNode;
//	while (currentChildNode != NULL)
//	{
//		ExecuteSentence(currentChildNode);
//		currentChildNode = currentChildNode->m_nextSiblingNode;
//	}*/
//	/*m_currentNode = m_rootNode->m_firstChildNode;
//	while (m_currentNode->m_token->m_largeCategory!=CTokenCategory::tokenSeparator
//		|| m_currentNode->m_token->m_smallCategory!=CTokenCategory::sepEnd)
//	{
//		ExecuteSentence();
//	}*/
//}
//
//void CInterpreter::Execute(int lineNumber)
//{
//	//while (m_currentNode->m_token->m_largeCategory != CTokenCategory::tokenSeparator
//	//	|| m_currentNode->m_token->m_smallCategory != CTokenCategory::sepEnd)
//	//{
//	//	ExecuteSentence();
//	//}
//		
//}

//void CInterpreter::ExecuteFromLine(std::string& programName, int lineNumber)
//{
//	ExecuteFromLine(GetProgramNode(programName), lineNumber);
//	
//	CInterpreterState::GetInstance()->SetAdmit(false);
//}
//
//
//void CInterpreter::ExecuteFromLine(CAstTreeNode* programNode, int lineNumber)
//{
//	if (FindStartNode(programNode, lineNumber))
//	{
//		/*若不为空，且允许执行*/
//		while (m_currentNode != NULL && CInterpreterState::GetInstance()->GetAdmit())
//		{
//			ExecuteNextLine();
//		}
//	}
//}

void CInterpreter::ExecuteNextLine()
{
	switch (m_currentNode->m_token->m_largeCategory)
	{
		/*若为操作符*/
	case CTokenCategory::tokenOperator:
	{
		switch (m_currentNode->m_token->m_smallCategory)
		{
			/*若为赋值操作符*/
		case CTokenCategory::operAssignment:
		{
			ExecuteAssignExp();
		}break;
		default:
		{
			FindNextValidNode();
		}break;
		}
	}break;
	/*若为命令行*/
	case CTokenCategory::tokenCommand:
	{
		switch (m_currentNode->m_token->m_smallCategory)
		{
			/*若为If语句*/
		case CTokenCategory::commIf:
		{
			ExecuteIfSentence();
		}break;
		case CTokenCategory::commEndif:
		{
			ExecuteEndIfSentence();
		}break;
		case CTokenCategory::commElseIf:
		{
			ExecuteElseIfSentence();
		}break;
		case CTokenCategory::commElse:
		{
			ExecuteElseSentence();
		}break;
		case CTokenCategory::commFor:
		{
			ExecuteForSentence();
		}break;
		case CTokenCategory::commNext:
		{
			ExecuteNextSentence();
		}break;
		case CTokenCategory::commBreak:
		{
			ExecuteBreakSentence();
		}break;
		case CTokenCategory::commSwitch:
		{
			ExecuteSwitchSentence();
		}break;
		case CTokenCategory::commCase:
		{
			ExecuteCaseSentence();
		}break;
		case CTokenCategory::commDefault:
		{
			ExecuteDefaultSentence();
		}break;
		case CTokenCategory::commEndSwitch:
		{
			ExecuteEndSwitchSentence();
		}break;
		case CTokenCategory::commMovl:
		{
			ExecuteMovlSentence();
		}break;
		case CTokenCategory::commMovc:
		{
			ExecuteMovcSentence();
		}break;
		case CTokenCategory::commEof:
		{
			ExecuteEofSentence();
		}break;
		case CTokenCategory::commGosub:
		{
			ExecuteGosubSentence();
		}break;
		default:
		{
			FindNextValidNode();
		}break;
		}
	}break;
	case CTokenCategory::tokenProgram:
	{
		ExecuteProgram();
	}break;
	default:
	{
		FindNextValidNode();
	}break;
	}
}

void CInterpreter::ExecuteNextCommand()
{
	m_isSendData = false;

	/*执行*/
	while (m_currentNode != NULL && CInterpreterState::GetInstance()->GetAdmit() && !m_isSendData)
	{
		ExecuteNextLine();
	}
}

void CInterpreter::SetStartNode(const std::string& programName, int lineNumber)
{
	FindStartNode(GetProgramNode(programName), lineNumber);
}

void CInterpreter::ExecuteProgram()
{
	m_currentNode = m_currentNode->m_firstChildNode;
}

void CInterpreter::ExecuteEofSentence()
{
	/*若程序执行完毕*/
	if (m_stackFunction.size() == 0)
	{
		ExecuteFinish();
	}
	else
	{
		ProgramStack programStack = m_stackFunction.back();
		m_stackFunction.pop_back();

		m_currentNode = programStack.lastNode;
		m_currentProgramNode = programStack.lastProgramNode;

		/*获取作用域*/
		m_scope = TVariateManager::GetInstance()->GetRootScope().FindScopeScrollDown(QString::fromStdString(m_currentProgramNode->m_token->m_strName));

		FindNextValidNode();
	}

}

//void CInterpreter::ExecuteFunctionMain()
//{
//	m_currentNode = m_currentNode->m_firstChildNode;
//}
//
//void CInterpreter::ExecuteFunction()
//{
//	m_currentNode = m_currentNode->m_firstChildNode->m_nextSiblingNode;
//}

//void CInterpreter::ExecuteEnd()
//{
//	/*若函数执行完毕*/
//	if (m_stackFunction.size()==0)
//	{
//		m_currentNode = NULL;
//	}
//	else
//	{
//		m_currentNode=m_stackFunction.back(); 
//		m_stackFunction.pop_back();
//	}
//}

void CInterpreter::ExecuteGosubSentence()
{
	/*将当前节点入栈*/
	m_stackFunction.push_back(ProgramStack{ m_currentProgramNode, m_currentNode });
	if (m_stackFunction.size()>10000)
	{
		m_currentNode = NULL;
		return;
	}

	/*获取完整函数名*/
	std::string strProgramName = GetWholeProgramName(static_cast<CTokenString*>(m_currentNode->m_firstChildNode->m_token)->m_strName);

	m_currentNode = GetProgramNode(strProgramName);

	
}

void CInterpreter::ExecuteAssignExp()
{
	CAstTreeNode* firstChildNode = m_currentNode->m_firstChildNode;
	CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;

	TYPE_VALUE result = GetExpResult(secondChildNode);

	if (result.type == TYPE_INT)
	{
		qDebug() << QString::number(result.intResult);
	}
	else if (result.type==TYPE_DOUBLE)
	{
		qDebug() << QString::number(result.doubleResult);
	}
	else if (result.type==TYPE_BOOL)
	{
		qDebug() << QString::number(result.intResult);
	}


	/*获取等号左边变量*/
	CTokenString* tokenfirst = (CTokenString*)(firstChildNode->m_token);
	//CScope* scope = m_value->m_scopeSystem.FindScope(m_strScopeName);
	//CSymbol* iterFirst = scope->ResolveSymbol(tokenfirst->m_strName);
	CScope::ScopeSymbol scopeSymbol = m_scope->FindSymbolScopeScrollUp(QString::fromStdString(tokenfirst->m_strName));
	switch (scopeSymbol.symbol->GetType())
	{
	case CSymbol::TYPE_BOOL:
	{
		//m_value->m_mapScopeBool[scopeSymbol.scope->GetScopeName().toStdString()]
		//	[scopeSymbol.symbol->GetName().toStdString()] = result.intResult;
		static_cast<TBool*>(TVariateManager::GetInstance()->GetVariate(scopeSymbol.scope->GetScopeName(),
			scopeSymbol.symbol->GetName()))->SetValue(result.intResult);
	}break;
	case CSymbol::TYPE_INTERGER:
	{
		TInteger* var = static_cast<TInteger*>(TVariateManager::GetInstance()
			->GetVariate(scopeSymbol.scope->GetScopeName(),scopeSymbol.symbol->GetName()));
		if (result.type==TYPE_INT)
		{
			var->SetValue(result.intResult);
		/*	m_value->m_mapScopeInt[scopeSymbol.scope->GetScopeName().toStdString()]
				[scopeSymbol.symbol->GetName().toStdString()] = result.intResult;*/
		}
		else
		{
			var->SetValue(result.doubleResult);
			/*		m_value->m_mapScopeInt[scopeSymbol.scope->GetScopeName().toStdString()]
					[scopeSymbol.symbol->GetName().toStdString()] = (int)result.doubleResult;*/
		}
	}break;
	case CSymbol::TYPE_DOUBLE:
	{
		double doubleResult=0;
		if (result.type == TYPE_INT)
		{
			doubleResult = result.intResult;
		}
		else
		{
			doubleResult = result.doubleResult;
		}
		static_cast<TDouble*>(TVariateManager::GetInstance()
			->GetVariate(scopeSymbol.scope->GetScopeName(), scopeSymbol.symbol->GetName()))
			->SetValue(doubleResult);
	}break;
	default:
		break;
	}

	/*寻找下一个可执行节点*/
	FindNextValidNode();
}

void CInterpreter::ExecuteIfSentence()
{
	//CAstTreeNode* currentNode = rootNode->m_firstChildNode;

	m_currentNode = m_currentNode->m_firstChildNode;
	/*若If条件为真*/
	if (GetBoolResult(m_currentNode))
	{
		m_currentNode = m_currentNode->m_nextSiblingNode;
		//CLexer::DisplayToken(m_currentNode->m_token);
		if (m_currentNode->m_firstChildNode!=NULL)
		{
			m_currentNode = m_currentNode->m_firstChildNode;
			//CLexer::DisplayToken(m_currentNode->m_token);
		}
		else
		{
			FindNextValidNode();
		}
		return;
		//while (currentNode != NULL)
		//{
		//	if (currentNode->m_token->m_largeCategory == CTokenCategory::tokenCommand)
		//	{
		//		switch (currentNode->m_token->m_smallCategory)
		//		{
		//			/*若为else、elseif、endif语句，则结束*/
		//		case CTokenCategory::commElse:case CTokenCategory::commElseIf:case CTokenCategory::commEndif:
		//		{
		//			return;
		//		}break;
		//		default:
		//			break;
		//		}
		//	}
		//	/*若为其它语句*/
		//	ExecuteSentence(currentNode);
		//	currentNode = currentNode->m_nextSiblingNode;
		//}
	}
	/*若If条件为假*/
	else
	{
		m_currentNode = m_currentNode->m_nextSiblingNode->m_nextSiblingNode;
		while (m_currentNode != NULL)
		{
			if (m_currentNode->m_token->m_largeCategory == CTokenCategory::tokenCommand)
			{
				switch (m_currentNode->m_token->m_smallCategory)
				{
					/*若为endif语句*/
				case CTokenCategory::commEndif:
				{
					return;
				}break;
				/*若为else语句*/
				case CTokenCategory::commElse:
				{
					/*CAstTreeNode* elseCurrentChild = currentNode->m_firstChildNode;
					while (elseCurrentChild != NULL)
					{
						ExecuteSentence(elseCurrentChild);
						elseCurrentChild = elseCurrentChild->m_nextSiblingNode;
					}*/
					return;
				}break;
				/*若为elseif语句*/
				case CTokenCategory::commElseIf:
				{
					///*若该elseif条件为真*/
					//if (GetBoolResult(m_currentNode->m_firstChildNode))
					//{
					//	/*ExecuteIfSentence(currentNode);*/
					//	return;
					//}
					return;
				}break;
				}
			}
			m_currentNode = m_currentNode->m_nextSiblingNode;
		}
	}
}

void CInterpreter::ExecuteElseIfSentence()
{
	/*若该elseif条件为真*/
	if (GetBoolResult(m_currentNode->m_firstChildNode))
	{
		m_currentNode = m_currentNode->m_firstChildNode;
		FindNextValidNode();
		/*ExecuteIfSentence(currentNode);*/
		return;
	}
	else
	{
		FindNextValidNode();
	}

	/*m_currentNode = m_currentNode->m_firstChildNode;
	FindNextValidNode();*/
}

void CInterpreter::ExecuteElseSentence()
{
	if (m_currentNode->m_firstChildNode == NULL)
	{
		FindNextValidNode();
	}
	else
	{
		m_currentNode = m_currentNode->m_firstChildNode;
	}
}

void CInterpreter::ExecuteEndIfSentence()
{
	/*若父节点不是if*/
	while (m_currentNode->m_token->m_largeCategory!=CTokenCategory::tokenCommand
		|| m_currentNode->m_token->m_smallCategory!=CTokenCategory::commIf)
	{
		m_currentNode = m_currentNode->m_parentNode;
	}
	/*寻找下一个可执行节点*/
	FindNextValidNode();
}

void CInterpreter::ExecuteForSentence()
{
	CAstTreeNode* firstChildNode = m_currentNode->m_firstChildNode;
	CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;
	CAstTreeNode* thirdChildNode = secondChildNode->m_nextSiblingNode;

	TYPE_VALUE firstValue = GetNumExpResult(firstChildNode->m_firstChildNode->m_nextSiblingNode);
	TYPE_VALUE secondValue = GetNumExpResult(secondChildNode);

	CTokenString* firstToken = (CTokenString*)(firstChildNode->m_firstChildNode->m_token);
	std::string varName = firstToken->m_strName;

	///*若第二个参数大于第一个参数，则为递增*/
	//bool isIncrease = false;
	//if (secondValue.intResult >= firstValue.intResult)
	//{
	//	isIncrease = true;
	//}


	/*第一个参数为int类型*/
	CScope::ScopeSymbol scopeSymbol = m_scope->FindSymbolScopeScrollUp(QString::fromStdString(varName));
	TInteger* firstVariate = static_cast<TInteger*>(TVariateManager::GetInstance()->GetVariate(
		scopeSymbol.scope->GetScopeName(), QString::fromStdString(varName)));
	//int& iterVar = m_value->m_mapScopeInt[scopeSymbol.scope->GetScopeName().toStdString()].at(varName);
	firstVariate->SetValue(firstValue.intResult);
	//iterVar= firstValue.intResult;

	/*若是递增*/
	//if (isIncrease)
	//{
	if (firstVariate->GetValue() <= secondValue.intResult)
		{
			m_currentNode = thirdChildNode->m_nextSiblingNode;
			return;
			//while (currentNode != NULL)
			//{
			//	ExecuteSentence();
			//	currentNode = currentNode->m_nextSiblingNode;

			//	/*若遇到break命令*/
			//	if (m_seeBreak)
			//	{
			//		m_seeBreak = false;
			//		return;
			//	}
			//}
			//iterVar->second = iterVar->second + thirdValue.intResult;
		}
		else
		{
			FindNextValidNode();
		}
	//}
	///*若是递减*/
	//else
	//{
	//	while (iterVar->second >= GetNumExpResult(secondChildNode).intResult)
	//	{
	//		CAstTreeNode* currentNode = thirdChildNode->m_nextSiblingNode;
	//		while (currentNode != NULL)
	//		{
	//			ExecuteSentence();
	//			currentNode = currentNode->m_nextSiblingNode;

	//			/*若遇到break命令*/
	//			if (m_seeBreak)
	//			{
	//				m_seeBreak = false;
	//				return;
	//			}
	//		}
	//		iterVar->second = iterVar->second + thirdValue.intResult;
	//	}
	//}
}

void CInterpreter::ExecuteNextSentence()
{
	m_currentNode = m_currentNode->m_parentNode;

	CAstTreeNode* firstChildNode = m_currentNode->m_firstChildNode;
	CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;
	CAstTreeNode* thirdChildNode = secondChildNode->m_nextSiblingNode;

	TYPE_VALUE secondValue = GetNumExpResult(secondChildNode);
	TYPE_VALUE thirdValue = GetNumExpResult(thirdChildNode);

	CTokenString* firstToken = (CTokenString*)(firstChildNode->m_firstChildNode->m_token);
	std::string varName = firstToken->m_strName;

	///*若第二个参数大于第一个参数，则为递增*/
	//bool isIncrease = false;
	//if (secondValue.intResult >= firstValue.intResult)
	//{
	//	isIncrease = true;
	//}


	/*第一个参数为int类型*/
	CScope::ScopeSymbol scopeSymbol = m_scope->FindSymbolScopeScrollUp(QString::fromStdString(varName));
	TInteger* firstVariate= static_cast<TInteger*>(TVariateManager::GetInstance()
		->GetVariate(scopeSymbol.scope->GetScopeName(), QString::fromStdString(varName)));
	//int& iterVar = m_value->m_mapScopeInt[scopeSymbol.scope->GetScopeName().toStdString()].at(varName);

	/*若满足循环条件*/
	if (firstVariate ->GetValue()+ thirdValue.intResult <= secondValue.intResult)
	{
		firstVariate->SetValue(firstVariate->GetValue() + thirdValue.intResult);
		//iterVar += thirdValue.intResult;
		m_currentNode = thirdChildNode->m_nextSiblingNode;
	}
	else
	{
		FindNextValidNode();
	}
}

void CInterpreter::ExecuteSwitchSentence()
{
	/*获取第一个子节点*/
	CAstTreeNode* firstChild = m_currentNode->m_firstChildNode;

	/*开关值*/
	int keyNum;

	/*获取开关值*/
	if (firstChild->m_token->m_largeCategory == CTokenCategory::tokenVariant)
	{
		CScope::ScopeSymbol scopeSymbol = m_scope->FindSymbolScopeScrollUp(
			QString::fromStdString(firstChild->m_token->m_strName));
		keyNum = static_cast<TInteger*>(TVariateManager::GetInstance()
			->GetVariate(scopeSymbol.scope->GetScopeName(),scopeSymbol.symbol->GetName()))->GetValue();

		//keyNum = m_value->m_mapScopeInt[scopeSymbol.scope->GetScopeName().toStdString()]
		//	[scopeSymbol.symbol->GetName().toStdString()];
	}
	else
	{
		keyNum = ((CTokenConstantNum*)(firstChild->m_token))->m_value.value_int;
	}

	CAstTreeNode* currentNode = firstChild->m_nextSiblingNode;

	/*是否遇到匹配值*/
	bool isMatch = false;

	while (currentNode != NULL)
	{
		switch (currentNode->m_token->m_smallCategory)
		{
		case CTokenCategory::commCase:
		{
			/*if (isMatch)
			{
				m_currentNode = currentNode;
			}
			else
			{*/
				int matchNum;
				/*获取匹配值*/
				if (currentNode->m_firstChildNode->m_token->m_largeCategory == CTokenCategory::tokenVariant)
				{
					matchNum = static_cast<TInteger*>(TVariateManager::GetInstance()
						->GetVariate(
						m_scope->FindSymbolScopeScrollUp(
						QString::fromStdString(currentNode->m_firstChildNode->m_token->m_strName)).scope->GetScopeName(), 
						QString::fromStdString(currentNode->m_firstChildNode->m_token->m_strName)))->GetValue();
					/*matchNum = m_value->m_mapScopeInt[m_scope->FindSymbolScopeScrollUp(
						QString::fromStdString(currentNode->m_firstChildNode->m_token->m_strName)).scope
						->GetScopeName().toStdString()]
						[currentNode->m_firstChildNode->m_token->m_strName];*/
				}
				else
				{
					matchNum = currentNode->m_firstChildNode->m_token->m_value.value_int;
				}

				/*若匹配*/
				if (keyNum == matchNum)
				{
					//isMatch = true;
					m_currentNode = currentNode;
					return;
				}
			//}
		}break;
		case CTokenCategory::commDefault:
		{
			/*ExecuteDefaultSentence(currentNode);*/
			m_currentNode = currentNode;
			return;
		}break;
		/*若为endswitch命令*/
		case CTokenCategory::commEndSwitch:
		{
			m_currentNode = currentNode;
			return;
		}break;
		}

		///*若遇到break命令*/
		//if (m_seeBreak)
		//{
		//	m_seeBreak = false;
		//	return;
		//}

		currentNode = currentNode->m_nextSiblingNode;
	}
}

void CInterpreter::ExecuteBreakSentence()
{
	while (m_currentNode->m_token->m_largeCategory!=CTokenCategory::tokenCommand
		|| (m_currentNode->m_token->m_smallCategory != CTokenCategory::commSwitch
		&& m_currentNode->m_token->m_smallCategory != CTokenCategory::commFor))
	{
		m_currentNode = m_currentNode->m_parentNode;
	}

	FindNextValidNode();
}

void CInterpreter::ExecuteDefaultSentence()
{
	if (m_currentNode->m_firstChildNode!=NULL)
	{
		m_currentNode = m_currentNode->m_firstChildNode;
	}
	else
	{
		FindNextValidNode();
	}
	//CAstTreeNode* currentNode = rootNode->m_firstChildNode;

	//while (currentNode != NULL)
	//{
	//	ExecuteSentence();
	//	currentNode = currentNode->m_nextSiblingNode;

	//	/*若遇到break命令*/
	//	if (m_seeBreak)
	//	{
	//		return;
	//	}
	//}
}

void CInterpreter::ExecuteEndSwitchSentence()
{
	while (m_currentNode->m_token->m_largeCategory != CTokenCategory::tokenCommand
		|| m_currentNode->m_token->m_smallCategory != CTokenCategory::commSwitch)
	{
		m_currentNode = m_currentNode->m_parentNode;
	}
	FindNextValidNode();
}

void CInterpreter::ExecuteCaseSentence()
{
	CAstTreeNode* currentNode = m_currentNode->m_firstChildNode->m_nextSiblingNode;

	if(currentNode != NULL)
	{
		m_currentNode = currentNode;
	}
	else
	{
		FindNextValidNode();
	}
}

void CInterpreter::ExecuteMovlSentence()
{
	SendMovlData();
	FindNextValidNode();
}

void CInterpreter::ExecuteMovcSentence()
{
	SendMovcData();
	FindNextValidNode();
}

CInterpreter::TYPE_VALUE CInterpreter::GetExpResult(CAstTreeNode* rootNode)
{
	TYPE_VALUE result;
	/*计算等号右边表达式*/
	switch (rootNode->m_token->m_largeCategory)
	{
		/*若为变量*/
	case CTokenCategory::tokenVariant:
	{
		CTokenString* tokenString = (CTokenString*)(rootNode->m_token);
		int varType = m_scope->FindSymbolScrollUp(QString::fromStdString(tokenString->m_strName))->GetType();
		switch (varType)
		{
			/*若为布尔变量*/
		case CSymbol::TYPE_BOOL:
		{
			result.intResult = GetBoolResult(rootNode);
			result.type = TYPE_BOOL;
		}break;
		/*若为其它变量*/
		default:
		{
			return GetNumExpResult(rootNode);
		}break;
		}
	}break;
	/*若为常数*/
	case CTokenCategory::tokenConstantNumber:
	{
		switch (rootNode->m_token->m_smallCategory)
		{
		case CTokenCategory::constantBool:
		{
			result.intResult = GetBoolResult(rootNode);
			result.type = TYPE_BOOL;
		}break;
		default:
		{
			return GetNumExpResult(rootNode);
		}break;
		}

	}break;
	/*若为函数*/
	case CTokenCategory::tokenFunction:
	{
		return GetNumExpResult(rootNode);
	}break;
	/*若为操作符*/
	case CTokenCategory::tokenOperator:
	{
		switch (rootNode->m_token->m_smallCategory)
		{
			/*若为逻辑运算符*/
		case CTokenCategory::operAnd:case CTokenCategory::operOr:
			/*若为关系运算符*/
		case CTokenCategory::operGreatEqual:case CTokenCategory::operGreatThan:case CTokenCategory::operLessEqual:
		case CTokenCategory::operLessThan:case CTokenCategory::operEqual:case CTokenCategory::operNotEqual:
		{
			result.intResult = GetBoolResult(rootNode);
			result.type = TYPE_BOOL;
		}break;
		/*若为其它运算符*/
		default:
		{
			return GetNumExpResult(rootNode);
		}
		break;
		}
	}break;
	default:
		break;
	}

	return result;
}

bool CInterpreter::GetBoolResult(CAstTreeNode* rootNode)
{
	switch (rootNode->m_token->m_largeCategory)
	{
		/*若为变量*/
	case CTokenCategory::tokenVariant:
	{
		CScope::ScopeSymbol scopeSymbol = m_scope->FindSymbolScopeScrollUp(
			QString::fromStdString(rootNode->m_token->m_strName));
		return static_cast<TBool*>(TVariateManager::GetInstance()->GetVariate(
			scopeSymbol.scope->GetScopeName(), scopeSymbol.symbol->GetName()))->GetValue();
		//return m_value->m_mapScopeBool[.toStdString()]
		//	[scopeSymbol.symbol->GetName().toStdString()];
	}break;
	/*若为常数*/
	case CTokenCategory::tokenConstantNumber:
	{
		CTokenConstantNum* tokenNum = (CTokenConstantNum*)(rootNode->m_token);
		return tokenNum->m_value.value_bool;
	}break;
	/*若为运算符*/
	default:
	{
		switch (rootNode->m_token->m_smallCategory)
		{
		case CTokenCategory::operAnd:case CTokenCategory::operOr:
		{
			return GetLogicalExpResult(rootNode);
		}break;
		default:
		{
			return GetRelationalExpResult(rootNode);
		}break;
		}
	}break;
	}

	return false;
}

bool CInterpreter::GetLogicalExpResult(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChildNode = rootNode->m_firstChildNode;
	CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;

	switch (rootNode->m_token->m_smallCategory)
	{
		/*若为And操作符*/
	case CTokenCategory::operAnd:
	{
		return GetBoolResult(firstChildNode) && GetBoolResult(secondChildNode);
	}break;
	/*若为Or操作符*/
	case CTokenCategory::operOr:
	{
		return GetBoolResult(firstChildNode) || GetBoolResult(secondChildNode);
	}break;
	default:
	{
		return false;
	}break;
	}
}

bool CInterpreter::GetRelationalExpResult(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChildNode = rootNode->m_firstChildNode;
	CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;

	TYPE_VALUE value1 = GetNumExpResult(firstChildNode);
	TYPE_VALUE value2 = GetNumExpResult(secondChildNode);

	switch (rootNode->m_token->m_smallCategory)
	{
		/*若为>=*/
	case CTokenCategory::operGreatEqual:
	{
		if (value1.type == TYPE_INT)
		{
			if (value2.type == TYPE_INT)
			{
				return value1.intResult >= value2.intResult;
			}
			else
			{
				return value1.intResult >= value2.doubleResult;
			}
		}
		else
		{
			if (value2.type == TYPE_INT)
			{
				return value1.doubleResult >= value2.intResult;
			}
			else
			{
				return value1.doubleResult >= value2.doubleResult;
			}
		}
	}break;
	/*若为>*/
	case CTokenCategory::operGreatThan:
	{
		if (value1.type == TYPE_INT)
		{
			if (value2.type == TYPE_INT)
			{
				return value1.intResult > value2.intResult;
			}
			else
			{
				return value1.intResult > value2.doubleResult;
			}
		}
		else
		{
			if (value2.type == TYPE_INT)
			{
				return value1.doubleResult > value2.intResult;
			}
			else
			{
				return value1.doubleResult > value2.doubleResult;
			}
		}
	}break;
	/*若为<=*/
	case CTokenCategory::operLessEqual:
	{
		if (value1.type == TYPE_INT)
		{
			if (value2.type == TYPE_INT)
			{
				return value1.intResult <= value2.intResult;
			}
			else
			{
				return value1.intResult <= value2.doubleResult;
			}
		}
		else
		{
			if (value2.type == TYPE_INT)
			{
				return value1.doubleResult <= value2.intResult;
			}
			else
			{
				return value1.doubleResult <= value2.doubleResult;
			}
		}
	}break;
	/*若为<*/
	case CTokenCategory::operLessThan:
	{
		if (value1.type == TYPE_INT)
		{
			if (value2.type == TYPE_INT)
			{
				return value1.intResult < value2.intResult;
			}
			else
			{
				return value1.intResult < value2.doubleResult;
			}
		}
		else
		{
			if (value2.type == TYPE_INT)
			{
				return value1.doubleResult < value2.intResult;
			}
			else
			{
				return value1.doubleResult < value2.doubleResult;
			}
		}
	}break;
	/*若为==*/
	case CTokenCategory::operEqual:
	{
		if (value1.type == TYPE_INT)
		{
			if (value2.type == TYPE_INT)
			{
				return value1.intResult == value2.intResult;
			}
			else
			{
				return value1.intResult == value2.doubleResult;
			}
		}
		else
		{
			if (value2.type == TYPE_INT)
			{
				return value1.doubleResult == value2.intResult;
			}
			else
			{
				return value1.doubleResult == value2.doubleResult;
			}
		}
	}break;
	/*若为!=*/
	case CTokenCategory::operNotEqual:
	{
		if (value1.type == TYPE_INT)
		{
			if (value2.type == TYPE_INT)
			{
				return value1.intResult != value2.intResult;
			}
			else
			{
				return value1.intResult != value2.doubleResult;
			}
		}
		else
		{
			if (value2.type == TYPE_INT)
			{
				return value1.doubleResult != value2.intResult;
			}
			else
			{
				return value1.doubleResult != value2.doubleResult;
			}
		}
	}break;
	default:
	{
		return false;
	}break;
	}
}

CInterpreter::TYPE_VALUE CInterpreter::GetNumExpResult(CAstTreeNode* rootNode)
{
	TYPE_VALUE result;

	switch (rootNode->m_token->m_largeCategory)
	{
		/*若为函数*/
	case CTokenCategory::tokenFunction:
	{
		result.type = TYPE_DOUBLE;
		result.doubleResult = GetFunctionExpResult(rootNode);
	}break;
	/*若为变量*/
	case CTokenCategory::tokenVariant:
	{
		std::string strVarName = rootNode->m_token->m_strName;
		CScope::ScopeSymbol scopeSymbol = m_scope->FindSymbolScopeScrollUp(QString::fromStdString(strVarName));
		int varType = scopeSymbol.symbol->GetType();

		switch (varType)
		{
		case CSymbol::TYPE_INTERGER:
		{
			result.type = TYPE_INT;
			result.intResult = static_cast<TInteger*>(TVariateManager::GetInstance()
				->GetVariate(scopeSymbol.scope->GetScopeName(),
				QString::fromStdString(strVarName)))->GetValue();

			//result.intResult = m_value->m_mapScopeInt[scopeSymbol.scope->GetScopeName().toStdString()][strVarName];
		}break;
		case CSymbol::TYPE_DOUBLE:
		{
			result.type = TYPE_DOUBLE;
			
			result.intResult = static_cast<TDouble*>(TVariateManager::GetInstance()
				->GetVariate(scopeSymbol.scope->GetScopeName(), QString::fromStdString(strVarName)))
				->GetValue();
		}break;
		default:
			break;
		}
	}break;
	/*若为常数*/
	case CTokenCategory::tokenConstantNumber:
	{
		CTokenConstantNum* tokenNum = (CTokenConstantNum*)(rootNode->m_token);
		switch (rootNode->m_token->m_smallCategory)
		{
			/*若为浮点数*/
		case CTokenCategory::constantFloat:
		{
			result.type = TYPE_DOUBLE;
			result.doubleResult = tokenNum->m_value.value_double;
		}break;
		/*若为整数*/
		case CTokenCategory::constantInt:
		{
			result.type = TYPE_INT;
			result.intResult = tokenNum->m_value.value_int;
		}break;
		default:
			break;
		}
	}break;
	/*若为操作符*/
	case CTokenCategory::tokenOperator:
	{
		result = GetOperatorExpResult(rootNode);
	}break;
	default:
		break;
	}

	return result;
}

double CInterpreter::GetFunctionExpResult(CAstTreeNode* rootNode)
{
	TYPE_VALUE result = GetNumExpResult(rootNode->m_firstChildNode);

	switch (rootNode->m_token->m_smallCategory)
	{
		/*若为sin函数*/
	case CTokenCategory::funSin:
	{
		if (result.type == TYPE_INT)
		{
			return std::sin(result.intResult);
		}
		else if (result.type == TYPE_DOUBLE)
		{
			return std::sin(result.doubleResult);
		}

	}break;
	/*若为cos函数*/
	case CTokenCategory::funCos:
	{
		if (result.type == TYPE_INT)
		{
			return std::cos(result.intResult);
		}
		else if (result.type == TYPE_DOUBLE)
		{
			return std::cos(result.doubleResult);
		}
	}break;
	/*若为tan函数*/
	case CTokenCategory::funTan:
	{
		if (result.type == TYPE_INT)
		{
			return std::tan(result.intResult);
		}
		else if (result.type == TYPE_DOUBLE)
		{
			return std::tan(result.doubleResult);
		}
	}break;
	/*若为sqrt函数*/
	case CTokenCategory::funSqrt:
	{
		if (result.type == TYPE_INT)
		{
			return std::sqrt(result.intResult);
		}
		else if (result.type == TYPE_DOUBLE)
		{
			return std::sqrt(result.doubleResult);
		}
	}break;
	}
	return 0;
}

CInterpreter::TYPE_VALUE CInterpreter::GetOperatorExpResult(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChildNode = rootNode->m_firstChildNode;
	CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;

	TYPE_VALUE firstResult = GetNumExpResult(firstChildNode);
	TYPE_VALUE secondResult = GetNumExpResult(secondChildNode);

	TYPE_VALUE result;

	switch (rootNode->m_token->m_smallCategory)
	{
		/*若为加*/
	case CTokenCategory::operPlus:
	{
		if (firstResult.type == TYPE_INT)
		{
			if (secondResult.type = TYPE_INT)
			{
				result.type = TYPE_INT;
				result.intResult = firstResult.intResult + secondResult.intResult;
			}
			else if (secondResult.type == TYPE_DOUBLE)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.intResult + secondResult.doubleResult;
			}
		}
		else if (firstResult.type == TYPE_DOUBLE)
		{
			if (secondResult.type = TYPE_INT)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.doubleResult + secondResult.doubleResult;
			}
			else if (secondResult.type == TYPE_DOUBLE)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.doubleResult + secondResult.doubleResult;
			}
		}
	}break;
	/*若为减*/
	case CTokenCategory::operMinus:
	{
		if (firstResult.type == TYPE_INT)
		{
			if (secondResult.type = TYPE_INT)
			{
				result.type = TYPE_INT;
				result.intResult = firstResult.intResult - secondResult.intResult;
			}
			else if (secondResult.type == TYPE_DOUBLE)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.intResult - secondResult.doubleResult;
			}
		}
		else if (firstResult.type == TYPE_DOUBLE)
		{
			if (secondResult.type = TYPE_INT)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.doubleResult - secondResult.doubleResult;
			}
			else if (secondResult.type == TYPE_DOUBLE)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.doubleResult - secondResult.doubleResult;
			}
		}
	}break;
	/*若为乘*/
	case CTokenCategory::operMultiply:
	{
		if (firstResult.type == TYPE_INT)
		{
			if (secondResult.type = TYPE_INT)
			{
				result.type = TYPE_INT;
				result.intResult = firstResult.intResult * secondResult.intResult;
			}
			else if (secondResult.type == TYPE_DOUBLE)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.intResult * secondResult.doubleResult;
			}
		}
		else if (firstResult.type == TYPE_DOUBLE)
		{
			if (secondResult.type = TYPE_INT)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.doubleResult * secondResult.doubleResult;
			}
			else if (secondResult.type == TYPE_DOUBLE)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.doubleResult * secondResult.doubleResult;
			}
		}
	}break;
	/*若为除*/
	case CTokenCategory::operDivide:
	{
		/*若除数为0*/
		if ((secondResult.type == TYPE_INT && secondResult.intResult == 0)
			|| (secondResult.type == TYPE_DOUBLE && secondResult.doubleResult == 0))
		{
			throw(CExceptionInterpreter(CWarningInformation::DIVISOR_IS_ZERO, rootNode->m_token->m_lineNum));
		}

		if (firstResult.type == TYPE_INT)
		{
			if (secondResult.type = TYPE_INT)
			{
				result.type = TYPE_INT;
				result.intResult = firstResult.intResult / secondResult.intResult;
			}
			else if (secondResult.type == TYPE_DOUBLE)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.intResult / secondResult.doubleResult;
			}
		}
		else if (firstResult.type == TYPE_DOUBLE)
		{
			if (secondResult.type = TYPE_INT)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.doubleResult / secondResult.doubleResult;
			}
			else if (secondResult.type == TYPE_DOUBLE)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = firstResult.doubleResult / secondResult.doubleResult;
			}
		}
	}break;
	/*若为乘方*/
	case CTokenCategory::operPower:
	{
		if (firstResult.type == TYPE_INT)
		{
			if (secondResult.type = TYPE_INT)
			{
				result.type = TYPE_INT;
				result.intResult = std::pow(firstResult.intResult, secondResult.intResult);
			}
			else if (secondResult.type == TYPE_DOUBLE)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = std::pow(firstResult.intResult, secondResult.doubleResult);
			}
		}
		else if (firstResult.type == TYPE_DOUBLE)
		{
			if (secondResult.type = TYPE_INT)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = std::pow(firstResult.doubleResult, secondResult.doubleResult);
			}
			else if (secondResult.type == TYPE_DOUBLE)
			{
				result.type = TYPE_DOUBLE;
				result.doubleResult = std::pow(firstResult.doubleResult, secondResult.doubleResult);
			}
		}
	}break;
	}

	return result;
}


std::string CInterpreter::GetWholeProgramName(const std::string& programSelfName)
{
	QString strName = QString::fromStdString(m_scope->GetScopeName().toStdString());
	QStringList strList = strName.split('.');
	return strList.at(0).toStdString() + "." + programSelfName;
}

void CInterpreter::FindNextValidNode()
{
	//CLexer::DisplayToken(m_currentNode->m_token);
	while (m_currentNode->m_nextSiblingNode==NULL)
	{
		m_currentNode = m_currentNode->m_parentNode;
	}
	m_currentNode = m_currentNode->m_nextSiblingNode;
}

bool CInterpreter::FindStartNode(CAstTreeNode* rootNode, int lineNumber)
{
	CAstTreeNode* currentNode = rootNode->m_firstChildNode;
	if (currentNode==NULL)
	{
		return false;
	}
	else if (currentNode->m_token->m_lineNum==lineNumber)
	{
		m_currentNode = currentNode;
		return true;
	}

	//CLexer::DisplayToken(currentNode->m_token);

	/*查询最后一个不大于起始行的子节点*/
	while (currentNode->m_nextSiblingNode!=NULL)
	{
		if (currentNode->m_nextSiblingNode->m_token->m_lineNum<lineNumber)
		{
			currentNode = currentNode->m_nextSiblingNode;
		}
		else if (currentNode->m_nextSiblingNode->m_token->m_lineNum==lineNumber)
		{
			m_currentNode = currentNode->m_nextSiblingNode;
			return true;
		}
		else
		{
			break;
		}
	}

	return FindStartNode(currentNode,lineNumber);
}


CAstTreeNode* CInterpreter::GetProgramNode(const std::string& program)
{
	/*获取作用域*/
	m_scope = TVariateManager::GetInstance()->GetRootScope().FindScopeScrollDown(QString::fromStdString(program));

	CAstTreeNode* currentNode = m_rootNode->m_firstChildNode;

	CAstTreeNode* programNode = NULL;
	while (currentNode != NULL)
	{
		if (static_cast<CTokenString*>(currentNode->m_token)->m_strName == m_scope->GetScopeName().toStdString())
		{
			programNode = currentNode;
			break;
		}
		currentNode = currentNode->m_nextSiblingNode;
	}

	if (programNode == NULL)
	{
		throw(CExceptionInterpreter(CWarningInformation::NOT_FIND_STRART_PROGRAM));
	}

	m_currentProgramNode = programNode;

	return programNode;
}

void CInterpreter::AddLock(char *head, int &offset)
{
	*(head + offset) = 0;
	++offset;
}
//
//void CInterpreter::AddLength(char* head, int &offset, int length)
//{
//	*(int*)(head + offset) = sizeof(tTeachCmdAttribute) + length;
//	offset += sizeof(int);
//}

void CInterpreter::AddCommandAttribute(char* head, int& offset, int commandCode, int commandLength)
{
	tTeachCmdAttribute attribute;
	attribute.m_length = COMMAND_HEAD_LENGTH + commandLength;
	attribute.m_ID = commandCode;
	attribute.m_type = CmdAttributeType::CMD_ATTRIBUTE_NORMAL_COMMAND;
	attribute.m_LineNumber = m_currentNode->m_token->m_lineNum;
	attribute.m_programAddress = (long long)m_currentProgramNode;

	*(tTeachCmdAttribute*)(head + offset) = attribute;
	offset += sizeof(tTeachCmdAttribute);
}

//void CInterpreter::AddDataSendedWithoutCommand(char* head, const int commandId)
//{
//	int offset = 0;
//	//AddLock(head, offset);
//	//AddLength(head, offset, 0);
//	AddCommandAttribute(head, offset, commandId, 0);
//}

CValue::TYPE_POSITION CInterpreter::GetPosition(const std::string& name)
{
	return static_cast<TPosition*>(TVariateManager::GetInstance()
		->GetVariateSrollUp(m_scope->GetScopeName(), QString::fromStdString(name)))->GetValue();
	//const TPosition::TYPE_POSITION& tPosition=static_cast<TPosition*>(TVariateManager::GetInstance()
	//	->GetVariateSrollUp(m_scope->GetScopeName(), QString::fromStdString(name)))->GetValue();
	
	//CScope::ScopeSymbol scopeSymbol = m_scope->FindSymbolScopeScrollUp(QString::fromStdString(name));
	//return m_value->m_mapScopePosition[scopeSymbol.scope->GetScopeName().toStdString()][name];
}

CValue::TYPE_DYNAMIC CInterpreter::GetDynamic(const std::string& name)
{
	CScope::ScopeSymbol scopeSymbol = m_scope->FindSymbolScopeScrollUp(QString::fromStdString(name));
	return static_cast<TDynamic*>(TVariateManager::GetInstance()
		->GetVariate(scopeSymbol.scope->GetScopeName(),QString::fromStdString(name)))
		->GetValue();
	//return m_value->m_mapScopeDynamic[scopeSymbol.scope->GetScopeName().toStdString()][name];
}

CValue::TYPE_OVERLAP &CInterpreter::GetOverlap(const std::string& name)
{
	CScope::ScopeSymbol scopeSymbol = m_scope->FindSymbolScopeScrollUp(QString::fromStdString(name));
	return m_value->m_mapScopeOverlap[scopeSymbol.scope->GetScopeName().toStdString()][name];
}

tMovLParam CInterpreter::GetMovlParameter(CAstTreeNode* parentNode)
{
	tMovLParam movlParam;

	CAstTreeNode* firstChild = parentNode->m_firstChildNode;
	CValue::TYPE_POSITION position = GetPosition(firstChild->m_token->m_strName);


	CAstTreeNode* secondChild = firstChild->m_nextSiblingNode;
	CValue::TYPE_DYNAMIC dynamic = GetDynamic(secondChild->m_token->m_strName);

	CAstTreeNode* thirdChild = secondChild->m_nextSiblingNode;
	CValue::TYPE_OVERLAP overlap = GetOverlap(thirdChild->m_token->m_strName);

	movlParam.m_Destination = position;
	movlParam.m_Dynamic = dynamic;
	movlParam.m_Overlap = overlap;

	return movlParam;
}

tMovCParam CInterpreter::GetMovcParameter(CAstTreeNode* parentNode)
{
	tMovCParam movcParam;

	CAstTreeNode* firstChild = parentNode->m_firstChildNode;
	CValue::TYPE_POSITION position1 = GetPosition(firstChild->m_token->m_strName);

	CAstTreeNode* secondChild = firstChild->m_nextSiblingNode;
	CValue::TYPE_POSITION position2 = GetPosition(secondChild->m_token->m_strName);

	CAstTreeNode* thirdChild = secondChild->m_nextSiblingNode;
	CValue::TYPE_DYNAMIC dynamic = GetDynamic(thirdChild->m_token->m_strName);

	CAstTreeNode* forthChild = thirdChild->m_nextSiblingNode;
	CValue::TYPE_OVERLAP overlap = GetOverlap(forthChild->m_token->m_strName);

	movcParam.m_AuxPoint = position1;
	movcParam.m_EndPoint = position2;
	movcParam.m_Dynamic = dynamic;
	movcParam.m_Overlap = overlap;

	return movcParam;
}

void CInterpreter::SendData()
{
	QByteArray data(m_commandSend, ((tTeachCmdAttribute*)(m_commandSend + LOCK_LENGTH))->m_length);
	//m_interpreterTcpAdapter->SendData(m_commandSend, ((tTeachCmdAttribute*)(m_commandSend+LOCK_LENGTH))->m_length);
	m_interpreterTcpAdapter->SendData(data);
	m_isSendData = true;
}

void CInterpreter::SendEndData()
{
	int offect = 0;
	AddLock(m_commandSend, offect);
	AddCommandAttribute(m_commandSend, offect, COMMAND_ID::END, 0);
	//AddDataSendedWithoutCommand(m_commandSend, COMMAND_ID::END);

	SendData();
}

void CInterpreter::SendMovlData()
{
	AddDataSended(m_commandSend, GetMovlParameter(m_currentNode), COMMAND_ID::MOVL);

	SendData();
}

void CInterpreter::SendMovcData()
{
	AddDataSended(m_commandSend, GetMovcParameter(m_currentNode), COMMAND_ID::MOVC);

	SendData();
}

