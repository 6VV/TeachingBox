#ifndef _TEACHING_BOX_T_INTERPRETER_MANAGER_H_
#define _TEACHING_BOX_T_INTERPRETER_MANAGER_H_

#include "QObject"

class TInterpreterThread;

class TInterpreterManager:public QObject
{
	Q_OBJECT

public:
	TInterpreterManager(QWidget* widget);
	~TInterpreterManager();

	void StartExecuteFromCurrentLine();
	void StepExecuteFromCurrentLine();
	void StopExecute();

private:
	int GetCurrentLineNum() const;
	bool HaveError() const;

signals:
	void SignalStartInterpret(int lineNum);
	void SignalExecuteOneLine(int lineNum);

private:
	QThread* m_thread;
	QWidget* m_widget;

private:
	TInterpreterThread* m_interpreterThread=nullptr;
};

#endif