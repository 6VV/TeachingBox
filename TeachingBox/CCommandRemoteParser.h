#ifndef _TEACHING_BOX_C_COMMAND_PARSER_H_
#define _TEACHING_BOX_C_COMMAND_PARSER_H_

#include "QByteArray"
#include "DataStruct.h"
#include "QList"
#include "QObject"
#include "CSingleTon.h"

class CCommandRemoteParser:public QObject
{
	friend CSingleTon<CCommandRemoteParser>;

	Q_OBJECT

public:
	static CCommandRemoteParser* GetInstance();

private:
	CCommandRemoteParser(QObject* parent=0);
	CCommandRemoteParser(const CCommandRemoteParser&);
	CCommandRemoteParser& operator=(const CCommandRemoteParser&);
	~CCommandRemoteParser();

public:
	void ParseCommands(QByteArray& commands);

signals:
	void SignalOnResivePosition(tAxesAllPositions&);

private:
	void ParseOneCommand(QByteArray& command);
	tTeachCmdAttribute* GetAttribute(char* command);
	void SplitCommand(QList<QByteArray>& commandList, QByteArray& commands);

	void SendNextCommand();		/*发送下一条指令*/
	void RefreshLineNumber(const tTeachCmdAttribute& attribute);	/*命令反馈*/
	void OnResivePosition(QByteArray& command);	/*获取位置*/
};

#endif