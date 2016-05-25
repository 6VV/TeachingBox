#ifndef _TEACHING_BOX_C_TCP_SPECIAL_COMMNAD_H_
#define _TEACHING_BOX_C_TCP_SPECIAL_COMMNAD_H_

#include "DataStruct.h"
#include "QByteArray"

class CTcpSpecialCommand
{
public:
	static QByteArray CommandStopExecute();
	static QByteArray CommandGetPosition();
	static QByteArray CommandPlusAxis1();
	static QByteArray CommandPlusAxis2();
	static QByteArray CommandPlusAxis3();
	static QByteArray CommandPlusAxis4();
	static QByteArray CommandPlusAxis5();
	static QByteArray CommandPlusAxis6();
	static QByteArray CommandMinusAxis1();
	static QByteArray CommandMinusAxis2();
	static QByteArray CommandMinusAxis3();
	static QByteArray CommandMinusAxis4();
	static QByteArray CommandMinusAxis5();
	static QByteArray CommandMinusAxis6();
	static QByteArray CommandStopMove();

private:
	static void GetData(QByteArray& data, COMMAND_ID id);
	static void GetLock(QByteArray& data);
	static void GetAttribute(QByteArray& data, COMMAND_ID id);
};

#endif