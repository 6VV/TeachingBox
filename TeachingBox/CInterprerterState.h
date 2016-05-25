#ifndef _INTERPERTER_C_INTERPRETER_STATE_H_
#define _INTERPERTER_C_INTERPRETER_STATE_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CInterprerterState.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2016/03/02
//  Description:	用于记录当前解释执行的方式及执行状态
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2016/03/02
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

class CInterpreterState
{
public:
	static CInterpreterState* GetInstance();
	bool GetAdmit();
	void SetAdmit(bool admit);

private:
	CInterpreterState();
	CInterpreterState(const CInterpreterState&);
	CInterpreterState& operator=(const CInterpreterState&);
	~CInterpreterState();

private:
	static CInterpreterState* m_instance;
	bool m_admitInterpreter=false;

	class Garbo
	{
	public:
		Garbo();
		~Garbo()
		{
			if (CInterpreterState::m_instance)
			{
				delete(CInterpreterState::m_instance);
				CInterpreterState::m_instance = NULL;
			}
		}
	};
	static Garbo m_garbo;
};

#endif