#include "stdafx.h"
#include "TInterpreterManager.h"
#include "TInterpreterThread.h"
#include "TInterpreterException.h"
#include "TInterpreter.h"
#include "CCodeEditor.h"
#include "CWarningManager.h"

TInterpreterManager::TInterpreterManager(QWidget* widget) :m_widget(widget)
{
	m_interpreterThread = new TInterpreterThread();
	m_thread = new QThread();

	m_interpreterThread->moveToThread(m_thread);

	connect(this, SIGNAL(SignalStartInterpret(int)), m_interpreterThread, SLOT(SlotStartExecute(int)));
	connect(this, SIGNAL(SignalExecuteOneLine(int)), m_interpreterThread, SLOT(SlotStepExecute(int)));

	m_thread->start();
}

TInterpreterManager::~TInterpreterManager()
{
	delete m_interpreterThread;
	delete m_thread;
}

void TInterpreterManager::StartExecuteFromCurrentLine()
{
	if (!HaveError())
	{
		emit SignalStartInterpret(GetCurrentLineNum());
	}
}

void TInterpreterManager::StepExecuteFromCurrentLine()
{
	if (!HaveError())
	{
		emit SignalExecuteOneLine(GetCurrentLineNum());
	}
}

void TInterpreterManager::StopExecute()
{
	TInterpreter::StopExecute();
}

int TInterpreterManager::GetCurrentLineNum() const
{
	return CCodeEditor::GetInstance()->GetPCLineNumber();
}

bool TInterpreterManager::HaveError() const
{
	try
	{
		TInterpreter::Parse(CCodeEditor::GetInstance()->toPlainText());
		return false;
	}
	catch (TInterpreterException& e)
	{
		CWarningManager::Warning(m_widget, e.GetInfo());
		return true;
	}
}

