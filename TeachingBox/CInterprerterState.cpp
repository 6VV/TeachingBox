#include "stdafx.h"
#include "CInterprerterState.h"

CInterpreterState* CInterpreterState::m_instance = new CInterpreterState();

CInterpreterState* CInterpreterState::GetInstance()
{
	return m_instance;
}

bool CInterpreterState::GetAdmit()
{
	return m_admitInterpreter;
}

void CInterpreterState::SetAdmit(bool admit)
{
	m_admitInterpreter = admit;
}

CInterpreterState::CInterpreterState()
{

}

CInterpreterState::~CInterpreterState()
{

}

