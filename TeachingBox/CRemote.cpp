#include "stdafx.h"
#include "CRemote.h"
#include "CInterpreterTcpAdapter.h"


void CRemote::SendData(QByteArray& data)
{
	static CInterpreterTcpAdatper tcpAdatper;
	tcpAdatper.SendData(data);
}
