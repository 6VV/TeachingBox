#include "stdafx.h"
#include "CInterpreterTcpAdapter.h"
#include "CTcpManager.h"



CInterpreterTcpAdatper::CInterpreterTcpAdatper(QObject* parent) :QObject(parent)
{
	Init();
}

CInterpreterTcpAdatper::~CInterpreterTcpAdatper()
{

}

void CInterpreterTcpAdatper::SendData(const char* head, const int length)
{
	emit(SignalSendData(head, length));
}

void CInterpreterTcpAdatper::SendData(QByteArray& data)
{
	emit(SignalSendData(data));
}

void CInterpreterTcpAdatper::Init()
{
	connect(this, SIGNAL(SignalSendData(const char*,const int)), CTcpManager::GetInstance(), SLOT(SlotSendData(const char*, const int)));
	connect(this, SIGNAL(SignalSendData(const QByteArray&)), CTcpManager::GetInstance(), SLOT(SlotSendData(const QByteArray&)));
}
