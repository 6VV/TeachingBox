#include "stdafx.h"
#include "TInterpreterThread.h"
#include "TInterpreter.h"


void TInterpreterThread::SlotStartExecute(int lineNum)
{
	TInterpreter::ExecuteAll(lineNum);
}

void TInterpreterThread::SlotStepExecute(int lineNum)
{
	TInterpreter::ExecuteOneLine(lineNum);
}

