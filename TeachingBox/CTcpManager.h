#ifndef _TEACHING_BOX_C_TCP_MANAGER_H_
#define _TEACHING_BOX_C_TCP_MANAGER_H_

#include <QTcpSocket>
#include "QHostAddress"
#include "QObject"
#include "CSingleTon.h"
#include "CCommandRemoteParser.h"

class CTcpManager:public QObject
{
	Q_OBJECT;

	friend CSingleTon<CTcpManager>;

public:
	static CTcpManager* GetInstance();

private:
	CTcpManager();
	CTcpManager(const CTcpManager&);
	CTcpManager& operator=(const CTcpManager&);
	~CTcpManager();

public:
	void SendData(const char *data,const int length);
	void SendData(const QByteArray& data);
	void ConnectAddress(const QHostAddress& address);

public slots:
	void SlotSendData(const char* text, const int length);
	void SlotSendData(const QByteArray& data);

protected slots:
	void SlotOnReceiveData();
	void SlotOnConnect();
	void SlotOnDisconnect();

private:
	void Init();
	void InitRemoteAddress();

private:
	static const int PORT = 1234;
	QTcpSocket *m_tcpSocket = NULL;
};

#endif