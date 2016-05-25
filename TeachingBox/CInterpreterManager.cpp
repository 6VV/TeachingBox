#include "stdafx.h"
#include "CInterpreterManager.h"
#include "CParseGrammar.h"
#include "CParseSemantic.h"
#include "CExceptionInterpreter.h"
#include "CWarningInformation.h"
#include "CInterpreter.h"
#include <time.h>
#include "CInterprerterState.h"
#include "CTokenString.h"
#include "CWarningManager.h"

bool CInterpreterManager::IsInterpreting()
{
	return CInterpreterState::GetInstance()->GetAdmit();
}

//CInterpreterManager* CInterpreterManager::m_instance = NULL;

int CInterpreterManager::m_matchState=CInterpreterManager::MATCH_CASE;

CInterpreterManager::CInterpreterManager()
{
	m_rootNode = new CAstTreeNode(new CTokenString(CTokenCategory::tokenRoot, 0, 0,""));
	m_interpreter = new CInterpreter(m_rootNode);
}

CInterpreterManager::~CInterpreterManager()
{
	Clear();
}

CInterpreterManager* CInterpreterManager::GetInstance()
{
	//return m_instance;
	return CSingleTon<CInterpreterManager>::GetInstance();
}

void CInterpreterManager::AddProgramNode(const std::string& strProgramName, const std::string& strProgramText)
{
	CTokenString* tokenString = new CTokenString(CTokenCategory::tokenProgram, 0, 0, strProgramName);
	CAstTreeNode* newProgramNode = new CAstTreeNode(tokenString);

	ParseSemantic(strProgramText, newProgramNode);

	RemoveProgramNode(strProgramName);

	m_rootNode->AddChildNode(newProgramNode);
}

void CInterpreterManager::RemoveProgramNode(const std::string& strProgramName)
{
	CAstTreeNode* programNode = FindProgramNode(strProgramName);

	if (programNode!=NULL)
	{
		CAstTreeNode* priorNode = m_rootNode->m_firstChildNode;

		/*若第一个子节点为目标程序节点*/
		if (priorNode==programNode)
		{
			m_rootNode->m_firstChildNode = m_rootNode->m_firstChildNode->m_nextSiblingNode;
		}
		else
		{
			while (priorNode->m_nextSiblingNode != programNode)
			{
				priorNode = priorNode->m_nextSiblingNode;
			}

			priorNode->m_nextSiblingNode = priorNode->m_nextSiblingNode->m_nextSiblingNode;

			/*若目标节点为结束节点*/
			if (priorNode->m_nextSiblingNode==NULL)
			{
				m_rootNode->m_endChildNode = priorNode;
			}
		}
		delete(programNode);
	}
}

void CInterpreterManager::RemoveAllPrograms()
{
	m_rootNode->ClearChildren();
}

CAstTreeNode* CInterpreterManager::FindProgramNode(const std::string& strProgramName)
{
	CAstTreeNode* currentNode = m_rootNode->m_firstChildNode;

	while (currentNode!=NULL)
	{
		/*若找到*/
		if ((static_cast<CTokenString*>(currentNode->m_token))->m_strName == strProgramName)
		{
			return currentNode;
		}
		currentNode = currentNode->m_nextSiblingNode;
	}

	/*若未找到*/
	return NULL;
}

void CInterpreterManager::ExecuteFromLine(std::string& strProgramName, int lineNumber)
{
	m_interpreter->SetStartNode(strProgramName, lineNumber);
	StartExecute();
	InterpreteNextCommand();
	//m_interpreter->ExecuteFromLine(strProgramName, lineNumber);
}

void CInterpreterManager::ExecuteStep(std::string& strProgramName, int lineNumber)
{
	m_interpreter->SetStartNode(strProgramName, lineNumber);
	StartExecute();
	m_interpreter->ExecuteStep();
	StopExecute();
}

void CInterpreterManager::StartExecute()
{
	CInterpreterState::GetInstance()->SetAdmit(true);
}

void CInterpreterManager::StopExecute()
{
	CInterpreterState::GetInstance()->SetAdmit(false);
}

//void CInterpreterManager::SetRootName(const std::string& strName)
//{
//	static_cast<CTokenString*>(m_rootNode->m_token)->m_strName = strName;
//}

//void CInterpreterManager::Interpreter(std::string& strContext)
//{
//	ParseSemantic(strContext);
//
//	//auto t1 = clock();
//
//	/*解释执行*/
//	m_interpreter = new CInterpreter(*m_parseSemantic);
//	m_interpreter->Execute();
//	//auto t2 = clock();
//	//qDebug() << (t2 - t1)*1000/CLOCKS_PER_SEC;
//}

//void CInterpreterManager::StartInterpreterFromLine(std::string& strContext, int lineNumber)
//{
//	/*如果正在执行，则返回*/
//	if (m_interpreter!=NULL && CInterpreterState::GetInstance()->GetAdmit())
//	{
//		return;
//	}
//
//	ParseSemantic(strContext);
//
//	//auto t1 = clock();
//
//	/*解释执行*/
//	m_interpreter = new CInterpreter(*m_parseSemantic);
//	CInterpreterState::GetInstance()->SetAdmit(true);
//	m_interpreter->ExecuteFromLine(lineNumber);
//}
//
//void CInterpreterManager::StopInterpreter()
//{
//	CInterpreterState::GetInstance()->SetAdmit(false);
//}
//
//void CInterpreterManager::Lexer(std::string& strContext)
//{
//	Clear();
//	
//	/*词法解析*/
//	m_lexer = new CLexer(strContext);
//}
//
//void CInterpreterManager::ParseGrammar(std::string& strContext)
//{
//	/*词法解析*/
//	Lexer(strContext);
//
//	/*语法解析并获取语法树*/
//	m_parseGrammar = new CParseGrammar(*m_lexer);
//
//}
//
void CInterpreterManager::ParseSemantic(std::string& strContext)
{
	CAstTreeNode* node = new CAstTreeNode(new CToken(CTokenCategory::tokenRoot, 0, 0));

	try
	{
		CLexer lexer(strContext);
		CParseGrammar parseGrammar(lexer, node);
		CParseSemantic parseSemantic(parseGrammar);
	}
	catch (CExceptionInterpreter& e)
	{
		delete(node);
		throw(e);
	}

	delete(node);
}

void CInterpreterManager::ParseSemantic(const std::string& strContext, CAstTreeNode* node)
{
	try
	{
		CLexer lexer(strContext);
		CParseGrammar parseGrammar(lexer, node);
		CParseSemantic parseSemantic(parseGrammar);
	}
	catch (CExceptionInterpreter& e)
	{
		delete(node);
		throw(e);
	}
}

void CInterpreterManager::InterpreteNextLine()
{
	if (IsInterpreting())
	{
		m_interpreter->ExecuteNextLine();
	}
}

void CInterpreterManager::InterpreteNextCommand()
{
	if (IsInterpreting())
	{
		m_interpreter->ExecuteNextCommand();
	}
}

void CInterpreterManager::GetWarningInfo(CExceptionInterpreter&e, std::string& warningInfo)
{
	if (e.GetWrongText().size() > 0)
	{
		warningInfo.append(e.GetWrongText());
		warningInfo.append(":");
	}
	switch (e.GetInfoCode())
	{
	case CWarningInformation::UNDEFINED_VARIANT:
	{
		warningInfo.append("undefined variant");
	}break;
	case CWarningInformation::UNKNOWN_TOKEN:
	{
		warningInfo.append("unknown token");
	}break;
	case CWarningInformation::UNMATCH_TYPE:
	{
		warningInfo.append("unmatch type");
	}break;
	case CWarningInformation::WRONG_GRAMMAR:
	{
		warningInfo.append("wrong grammar");
	}break;
	case CWarningInformation::NOT_PROGRAM:
	{
		warningInfo.append("not program");
	}break;
	case CWarningInformation::NOT_MEET_END:
	{
		warningInfo.append("not meet end");
	}break;
	case CWarningInformation::NOT_FIND_MAIN_PROGRAM:
	{
		warningInfo.append("not find main program");
	}break;
	case CWarningInformation::NOT_FIND_EOF:
	{
		warningInfo.append("not find eof");
	}break;
	case CWarningInformation::NOT_FIND_STRART_PROGRAM:
	{
		warningInfo.append("not find start program");
	}break;
	case CWarningInformation::TOO_MANY_MAIN_PROGRAM:
	{
		warningInfo.append("too many main program");
	}break;
	case CWarningInformation::EXIST_SAME_PROGRAM:
	{
		warningInfo.append("exist same program");
	}break;
	case CWarningInformation::UNKNOWN_LABEL:
	{
		warningInfo.append("unknown label");
	}break;
	case CWarningInformation::IF_WITH_NOT_BOOL:
	{
		warningInfo.append("if with not bool");
	}break;
	case CWarningInformation::ELSEIF_WITH_NOT_BOOL:
	{
		warningInfo.append("elseif with not bool");
	}break;
	case CWarningInformation::BREAK_NOT_IN_LOOP_OR_SWITCH:
	{
		warningInfo.append("break not int loop or switch");
	}break;
	case CWarningInformation::SWITCH_WITH_NOT_INT:
	{
		warningInfo.append("switch with not int");
	}break;
	case CWarningInformation::CASE_WITH_NOT_INT:
	{
		warningInfo.append("case with not int");
	}break;
	case CWarningInformation::PARAMETER_NOT_MATCH:
	{
		warningInfo.append("parameter not match");
	}break;
	case CWarningInformation::DIVISOR_IS_ZERO:
	{
		warningInfo.append("divisor is zero");
	}break;
	case CWarningInformation::NOT_EXIST_THIS_PROGRAM:
	{
		warningInfo.append("not exist this program");
	}break;
	case  CWarningInformation::EMPTY:
	{
		warningInfo.append("empty text");
	}break;
	default:
		break;
	}
}


void CInterpreterManager::DisplayWarningInfo(CExceptionInterpreter& e)
{
	qDebug() << QString("Line:") + QString::number(e.GetLineNumber());
	if (e.GetWrongText().size()>0)
	{
		qDebug() << QString::fromStdString(e.GetWrongText());
	}
	switch (e.GetInfoCode())
	{
	case CWarningInformation::UNDEFINED_VARIANT:
	{
		qDebug() << "undefined variant";
	}break;
	case CWarningInformation::UNKNOWN_TOKEN:
	{
		qDebug() << "unknown token";
	}break;
	case CWarningInformation::UNMATCH_TYPE:
	{
		qDebug() << "unmatch type";
	}break;
	case CWarningInformation::WRONG_GRAMMAR:
	{
		qDebug() << "wrong grammar";
	}break;
	case CWarningInformation::NOT_PROGRAM:
	{
		qDebug() << "not function";
	}break;
	case CWarningInformation::NOT_MEET_END:
	{
		qDebug() << "not meet end";
	}break;
	default:
		break;
	}
}

void CInterpreterManager::Clear()
{
	delete(m_rootNode);
	delete(m_interpreter);
}

