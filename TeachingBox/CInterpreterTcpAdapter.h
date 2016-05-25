#ifndef _TEACHING_BOX_C_INTERPRETER_TCP_ADAPTER_H_
#define _TEACHING_BOX_C_INTERPRETER_TCP_ADAPTER_H_

#include "QObject"
#include "QByteArray"

class CInterpreterTcpAdatper:public QObject
{
	Q_OBJECT
public:
	CInterpreterTcpAdatper(QObject* parent=0);
	~CInterpreterTcpAdatper();

	void SendData(const char* head, const int length);
	void SendData(QByteArray& data);

signals:
	void SignalSendData(const char* head, const int length);
	void SignalSendData(const QByteArray& data);
	
private:
	void Init();


};

#endif