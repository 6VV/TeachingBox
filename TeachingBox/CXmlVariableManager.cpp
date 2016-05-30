#include "stdafx.h"
#include "CXmlVariableManager.h"
#include "QFile"
#include "CValue.h"
#include "Init.h"

CXmlVariableManager::CXmlVariableManager()
{
}

CXmlVariableManager::~CXmlVariableManager()
{

}

void CXmlVariableManager::SetProjectName(QString strProjectName)
{
	m_projectName = strProjectName;
	m_filePath = PROJECT_FILE_PATH + "/" + strProjectName+"/"+FILE_NAME;
}

void CXmlVariableManager::SetProgramName(QString strProgramName)
{
	m_programName = strProgramName;
}

void CXmlVariableManager::WriteProgramVariable()
{
	qDebug() << m_filePath;
	QFile file(m_filePath);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		QXmlStreamWriter writer(&file);
		writer.setAutoFormatting(true);
		writer.setCodec("UTF-8");

		/*写入文档*/
		writer.writeStartDocument();
		{
			writer.writeStartElement("Program");
			writer.writeAttribute("name", m_programName);

			WriteAllVariable(writer);

			writer.writeEndElement();
		}
		writer.writeEndDocument();

		file.close();
	}
}

void CXmlVariableManager::WriteAllVariable(QXmlStreamWriter& writer)
{
	writer.writeStartElement("Variable");

	WritePositionVariable(writer);
	WriteVelocityVariable(writer);
	WriteZoneVariable(writer);

	writer.writeEndElement();
}

void CXmlVariableManager::WritePositionVariable(QXmlStreamWriter& writer)
{
	/*writer.writeStartElement("Positions");

	{
	CInterpreterAdapter* interpreter = CInterpreterAdapter::GetInstance();

	CValue::TYPE_MAP_POSITION map;
	CDatabaseManager::GetInstance()->SelectLocalPositionValue(m_projectName, m_programName, map);

	for each (auto position in map)
	{
	writer.writeStartElement(QString::fromStdString(position.first));

	writer.writeTextElement("A1", QString::number(position.second.at(0)));
	writer.writeTextElement("A2", QString::number(position.second.at(1)));
	writer.writeTextElement("A3", QString::number(position.second.at(2)));
	writer.writeTextElement("A4", QString::number(position.second.at(3)));
	writer.writeTextElement("A5", QString::number(position.second.at(4)));
	writer.writeTextElement("A6", QString::number(position.second.at(5)));

	writer.writeEndElement();
	}
	}*/

	writer.writeEndElement();
}

void CXmlVariableManager::WriteVelocityVariable(QXmlStreamWriter& writer)
{
	//writer.writeStartElement("Velocity");

	//{
	//	CInterpreterAdapter* interpreter = CInterpreterAdapter::GetInstance();
	//	CValue::TYPE_MAP_DYNAMIC map;
	//	//CDatabaseManager::GetInstance()->SelectLocalDynamicValue(m_projectName, m_programName, map);
	//	
	//	for each (auto velocity in map)
	//	{
	//		writer.writeStartElement(QString::fromStdString(velocity.first));

	//		writer.writeTextElement("Velocity", QString::number(velocity.second.m_Velocity));
	//		writer.writeTextElement("Accelerate", QString::number(velocity.second.m_Acceleration));
	//		writer.writeTextElement("Decelerate", QString::number(velocity.second.m_Deceleration));
	//		writer.writeTextElement("PostureVelocity", QString::number(velocity.second.m_PostureVelocity));
	//		writer.writeTextElement("PostureAccelerate", QString::number(velocity.second.m_PostureAcceleration));
	//		writer.writeTextElement("PostureDecelerate", QString::number(velocity.second.m_PostureDeceleration));


	//		writer.writeEndElement();
	//	}
	//}

	//writer.writeEndElement();
}

void CXmlVariableManager::WriteZoneVariable(QXmlStreamWriter& writer)
{
	writer.writeStartElement("Zone");

	{
		CValue::TYPE_MAP_OVERLAP map;
		//CDatabaseManager::GetInstance()->SelectOverlapValue(m_projectName, m_programName, map);

		for each (auto zone in map)
		{
			writer.writeStartElement(QString::fromStdString(zone.first));

			writer.writeTextElement("MODE", QString::number(zone.second.m_TransitionMode));
			writer.writeTextElement("VALUE", QString::number(zone.second.m_TransitionParameter));

			writer.writeEndElement();
		}
	}

	writer.writeEndElement();
}

