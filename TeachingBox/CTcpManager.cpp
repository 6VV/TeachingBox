#include "stdafx.h"
#include "CTcpManager.h"
#include "UserManager.h"



CTcpManager* CTcpManager::GetInstance()
{
	return CSingleTon<CTcpManager>::GetInstance();
}

CTcpManager::CTcpManager()
{
	Init();
}

CTcpManager::~CTcpManager()
{
	if (m_tcpSocket->state()==QAbstractSocket::ConnectedState)
	{
		m_tcpSocket->disconnectFromHost();
	}

	delete(m_tcpSocket);

	m_tcpSocket = NULL;
}

void CTcpManager::SendData(const char *text,const int length)
{
	if (m_tcpSocket->state() == QAbstractSocket::ConnectedState)
	{
		m_tcpSocket->write(text, length);
	}
}

void CTcpManager::SendData(const QByteArray& data)
{
	if (m_tcpSocket->state() == QAbstractSocket::ConnectedState)
	{
		m_tcpSocket->write(data);
	}
}

void CTcpManager::SlotSendData(const QByteArray& data)
{
	SendData(data);
}

void CTcpManager::ConnectAddress(const QHostAddress& address)
{
	m_tcpSocket->connectToHost(address, PORT);
}

void CTcpManager::SlotSendData(const char* text, const int length)
{
	SendData(text, length);
}

void CTcpManager::SlotOnReceiveData()
{
	while (m_tcpSocket->bytesAvailable() > 0)
	{
		QByteArray data;
		data.resize(m_tcpSocket->bytesAvailable());
		m_tcpSocket->read(data.data(), data.size());
		CCommandRemoteParser::GetInstance()->ParseCommands(data);
	}
}

void CTcpManager::SlotOnConnect()
{
}

void CTcpManager::SlotOnDisconnect()
{
}

void CTcpManager::Init()
{
	m_tcpSocket = new QTcpSocket;

	InitRemoteAddress();

	connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(SlotOnConnect()));
	connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(SlotOnDisconnect()));
	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(SlotOnReceiveData()));
}

void CTcpManager::InitRemoteAddress()
{
	QHostAddress address;
	address.setAddress(CUserManager::GetInstance()->GetIpAddress());
	ConnectAddress(address);
}
