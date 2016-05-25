#ifndef _TEACHING_BOX_C_THREAD_INTERPRETER_H_
#define _TEACHING_BOX_C_THREAD_INTERPRETER_H_

#include "CSingleTon.h"
#include "CInterpreterManager.h"
#include "CExceptionInterpreter.h"

class CThreadInterpreterManager:public QObject
{
	//friend CSingleTon<CThreadInterpreter>;

	Q_OBJECT

public:
	CThreadInterpreterManager(CInterpreterManager* interperterManager);
	CThreadInterpreterManager(const CThreadInterpreterManager&);
	CThreadInterpreterManager& operator = (const CThreadInterpreterManager&);
	~CThreadInterpreterManager();

public:
	//static CThreadInterpreter* GetInstance();
	void SetInterpreterManager(CInterpreterManager* interpreterManager);

//signals:
//	void SignalFindWrongInformation(const CExceptionInterpreter&);

protected slots:
	void SlotExecuteFromLine();	/*从某一行开始执行*/
	void SlotExecuteStep();	/*单步执行*/
	void SlotStopExecute();	/*停止执行*/

protected:
	
private:
	CInterpreterManager* m_interpreterManager=NULL;
};

#endif