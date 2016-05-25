#include "stdafx.h"
#include "CTcpSpecialCommand.h"



QByteArray CTcpSpecialCommand::CommandStopExecute()
{
	QByteArray data;
	GetData(data, COMMAND_ID::STOP_EXECUTE);
	return data;
}

QByteArray CTcpSpecialCommand::CommandGetPosition()
{
	QByteArray data;
	GetData(data, COMMAND_ID::ROBOT_POSITION);
	return data;
}

QByteArray CTcpSpecialCommand::CommandPlusAxis1()
{
	QByteArray data;
	GetData(data,COMMAND_ID::MOVE_AXIS_PLUS_1);
	return data;
}

QByteArray CTcpSpecialCommand::CommandPlusAxis2()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_PLUS_2);
	return data;
}

QByteArray CTcpSpecialCommand::CommandPlusAxis3()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_PLUS_3);
	return data;
}

QByteArray CTcpSpecialCommand::CommandPlusAxis4()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_PLUS_4);
	return data;
}

QByteArray CTcpSpecialCommand::CommandPlusAxis5()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_PLUS_5);
	return data;
}

QByteArray CTcpSpecialCommand::CommandPlusAxis6()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_PLUS_6);
	return data;
}

QByteArray CTcpSpecialCommand::CommandMinusAxis1()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_MINUS_1);
	return data;
}

QByteArray CTcpSpecialCommand::CommandMinusAxis2()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_MINUS_2);
	return data;
}

QByteArray CTcpSpecialCommand::CommandMinusAxis3()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_MINUS_3);
	return data;
}

QByteArray CTcpSpecialCommand::CommandMinusAxis4()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_MINUS_4);
	return data;
}

QByteArray CTcpSpecialCommand::CommandMinusAxis5()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_MINUS_5);
	return data;
}

QByteArray CTcpSpecialCommand::CommandMinusAxis6()
{
	QByteArray data;
	GetData(data, COMMAND_ID::MOVE_AXIS_MINUS_6);
	return data;
}

QByteArray CTcpSpecialCommand::CommandStopMove()
{
	QByteArray data;
	GetData(data, COMMAND_ID::STOP_MOVE);
	return data;
}

void CTcpSpecialCommand::GetData(QByteArray& data, COMMAND_ID id)
{
	GetLock(data);
	GetAttribute(data, id);
}

void CTcpSpecialCommand::GetLock(QByteArray& data)
{
	data.append('\0');
}

void CTcpSpecialCommand::GetAttribute(QByteArray& data, COMMAND_ID id)
{
	tTeachCmdAttribute attribute;
	attribute.m_type = CmdAttributeType::CMD_ATTRIBUTE_SPECIAL_COMMAND;
	attribute.m_length = COMMAND_HEAD_LENGTH;
	attribute.m_ID = id;
	data.append((char*)&attribute, sizeof(tTeachCmdAttribute));
}

