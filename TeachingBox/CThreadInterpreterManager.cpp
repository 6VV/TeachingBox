#include "stdafx.h"
#include "CThreadInterpreterManager.h"
#include "CCodeEditor.h"
#include "CInterprerterState.h"
#include "CScreenProject.h"


CThreadInterpreterManager::CThreadInterpreterManager(CInterpreterManager* interpreterManager)
{
	m_interpreterManager = interpreterManager;
}

CThreadInterpreterManager::~CThreadInterpreterManager()
{
}

void CThreadInterpreterManager::SetInterpreterManager(CInterpreterManager* interpreterManager)
{
	m_interpreterManager = interpreterManager;
}

void CThreadInterpreterManager::SlotExecuteFromLine()
{
	/*若已经在执行，则返回*/
	if (CInterpreterState::GetInstance()->GetAdmit())
	{
		return;
	}

	/*从设置行开始执行*/
	m_interpreterManager->ExecuteFromLine(CScreenProject::GetInstance()->GetStartFileName().toStdString(),
		CCodeEditor::GetInstance()->GetPCLineNumber());
}

void CThreadInterpreterManager::SlotExecuteStep()
{
	/*若已经在执行，则返回*/
	if (CInterpreterState::GetInstance()->GetAdmit())
	{
		return;
	}

	/*从设置行开始执行*/
	m_interpreterManager->ExecuteStep(CScreenProject::GetInstance()->GetStartFileName().toStdString(),
		CCodeEditor::GetInstance()->GetPCLineNumber());
}

void CThreadInterpreterManager::SlotStopExecute()
{
	m_interpreterManager->StopExecute();
}
