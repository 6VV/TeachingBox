#ifndef _TEACHING_BOX_T_INTERPRETER_THREAD_H_
#define _TEACHING_BOX_T_INTERPRETER_THREAD_H_

#include "QObject"

class QWidget;

class TInterpreterThread:public QObject
{
	Q_OBJECT

public slots :
	void SlotStartExecute(int lineNum);
	void SlotStepExecute(int lineNum);

};

#endif