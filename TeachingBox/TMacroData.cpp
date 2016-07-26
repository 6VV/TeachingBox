#include "stdafx.h"
#include "TMacroData.h"
#include "DataStruct.h"
#include "CRemote.h"



void TMacroData::Send(const MacroData& data)
{
	QByteArray byteArray(data.data,1024);
	CRemote::SendData(byteArray);
}

void TMacroData::AddLock(MacroData& data, int& offset)
{
	*(data.data + offset) = 0;
	++offset;
}

void TMacroData::AddCommandAttribute(MacroData& data, int& offset, const int commandId, const int length, const int lineNum, const long long programAddress)
{
	tTeachCmdAttribute attribute;
	attribute.m_length = COMMAND_HEAD_LENGTH + length;
	attribute.m_ID = commandId;
	attribute.m_type = CmdAttributeType::CMD_ATTRIBUTE_NORMAL_COMMAND;
	attribute.m_LineNumber = lineNum;
	attribute.m_programAddress = programAddress;

	*(tTeachCmdAttribute*)(data.data + offset) = attribute;
	offset += sizeof(tTeachCmdAttribute);
}
