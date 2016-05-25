#include "stdafx.h"
#include "CCommandRemoteParser.h"
#include "QString"
#include "DataStruct.h"
#include "CInterpreterManager.h"
#include "CScreenProject.h"



CCommandRemoteParser* CCommandRemoteParser::GetInstance()
{
	return CSingleTon<CCommandRemoteParser>::GetInstance();
}


CCommandRemoteParser::CCommandRemoteParser(QObject* parent/*=0*/) :QObject(parent)
{

}

CCommandRemoteParser::~CCommandRemoteParser()
{

}

void CCommandRemoteParser::ParseCommands(QByteArray& commands)
{
	QList<QByteArray> commandList;
	SplitCommand(commandList, commands);

	for each (auto cmd in commandList)
	{
		ParseOneCommand(cmd);
	}
	
}

void CCommandRemoteParser::ParseOneCommand(QByteArray& command)
{
	tTeachCmdAttribute* attribute = GetAttribute(command.data());

	switch (attribute->m_ID)
	{
	case COMMAND_ID::NORMAL_COMMAND:
	{
		SendNextCommand();
	}break;
	case COMMAND_ID::NORMAL_COMMAND_FEEDBACK:
	{
		RefreshLineNumber(*attribute);
	}break;
	case COMMAND_ID::ROBOT_POSITION:
	{
		OnResivePosition(command);
	}break;
	default:
		break;
	}
}

tTeachCmdAttribute* CCommandRemoteParser::GetAttribute(char* command)
{
	return (tTeachCmdAttribute*)(command+LOCK_LENGTH);
}


void CCommandRemoteParser::SplitCommand(QList<QByteArray>& commandList, QByteArray& commands)
{
	int length=0;
	while (commands.size() > 0)
	{
		length = GetAttribute(commands.data())->m_length;
		if (length<=0 || length>commands.size())
		{
			break;
		}
		QByteArray newCommand = commands.left(length);
		commandList.append(newCommand);
		commands.remove(0, length);
	}
}

void CCommandRemoteParser::SendNextCommand()
{
	CInterpreterManager::GetInstance()->InterpreteNextCommand();
}

void CCommandRemoteParser::RefreshLineNumber(const tTeachCmdAttribute& attribute)
{
	CAstTreeNode* programNode = (CAstTreeNode*)attribute.m_programAddress;
	CScreenProject::GetInstance()->RefreshProgramLineNumber(QString::fromStdString(programNode->m_token->m_strName), attribute.m_LineNumber);
}

void CCommandRemoteParser::OnResivePosition(QByteArray& command)
{
	tAxesAllPositions position;
	position = *(tAxesAllPositions*)(command.data() + COMMAND_HEAD_LENGTH);
	emit(SignalOnResivePosition(position));
}




