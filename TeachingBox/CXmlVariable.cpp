#include "stdafx.h"
#include "CXmlVariable.h"
#include "QFile"
#include "QDomDocument"
#include "CScreenWarning.h"

CXmlVariable::CXmlVariable(const char* xmlName)
{
	m_fileName = xmlName;
	Init();
}

CXmlVariable::CXmlVariable(const QString& xmlName)
{
	m_fileName = xmlName;
	Init();
}

CXmlVariable::~CXmlVariable()
{
	m_doc->clear();
	delete(m_doc);
}

void CXmlVariable::Read()
{
	QDomElement root = m_doc->documentElement();
	QDomNode node = root.firstChild();
	
	while (!node.isNull())
	{
		if (node.isElement())
		{
			QDomElement element = node.toElement();
			if (element.tagName()=="positions")
			{
				QDomNodeList positionList = element.childNodes();
				for (int i = 0; i < positionList.size(); i++)
				{
					QDomNode positionNode = positionList.at(i);
					if (positionNode.isElement())
					{
						QDomNodeList positionChildList = positionNode.childNodes();
						for (int j = 0; j < positionChildList.size();++j)
						{
							qDebug() << "success";
							QString tagName = positionChildList.at(j).toElement().text();
							qDebug() << tagName << " ";
						}
						
					}
				}
			}
			
		}
		node = node.nextSibling();
	}
}

void CXmlVariable::Init()
{
	/*创建文档*/
	m_doc = new QDomDocument;

	QFile file;
	if (!file.exists(m_fileName))
	{
		Create();
	}
	else
	{
		ReadDocument();
	}
}

void CXmlVariable::ReadDocument()
{
	QFile file(m_fileName);
	QString strError = "Fail to get data";

	/*文件打开失败*/
	if (!file.open(QIODevice::ReadOnly | QFile::Text))
	{
		CScreenWarning::GetInstance()->GetLabel()->setText(strError);
	}

	/*文件读取失败*/
	if (!m_doc->setContent(&file, false))
	{
		CScreenWarning::GetInstance()->GetLabel()->setText(strError);
	}
	file.close();
}

void CXmlVariable::Create()
{
	/*创建文件*/
	QFile file(m_fileName);
	file.open(QIODevice::ReadWrite);

	/*文档树形*/
	QDomProcessingInstruction instruction;
	instruction = m_doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
	m_doc->appendChild(instruction);

	/*根节点*/
	QDomElement root = m_doc->createElement("variable");
	m_doc->appendChild(root);
	//QDomText text = m_doc->createTextNode("");
	//root.appendChild(text);

	/*点节点*/
	QDomElement points = m_doc->createElement("positions");
	root.appendChild(points);

	/*将文档存储于文件中*/
	QTextStream out(&file);
	m_doc->save(out, 4);

	file.close();
}

/*添加点节点*/
void CXmlVariable::AddPositionNode(QString nodeName, double* pointValue)
{
	/*获取根节点*/
	QDomElement root = m_doc->documentElement();
	/*if (root.isNull())
	{
	root = m_doc->createElement("variable");
	m_doc->appendChild(root);
	}*/

	QDomElement elementPositions=root.elementsByTagName("positions").at(0).toElement();

	/*if (nodeList.isEmpty())
	{
		elementPositions = m_doc->createElement("positions");
		root.appendChild(elementPositions);
	}
	else
	{
		elementPositions = nodeList.at(0).toElement();
	}*/

	QDomElement elementChildPoint = m_doc->createElement(nodeName);
	elementPositions.appendChild(elementChildPoint);

	QDomElement elementA1 = m_doc->createElement("A1");
	QDomElement elementA2 = m_doc->createElement("A2");
	QDomElement elementA3 = m_doc->createElement("A3");
	QDomElement elementA4 = m_doc->createElement("A4");
	QDomElement elementA5 = m_doc->createElement("A5");
	QDomElement elementA6 = m_doc->createElement("A6");

	QDomText text;
	text = m_doc->createTextNode(QString::number(pointValue[0]));
	elementA1.appendChild(text);
	text = m_doc->createTextNode(QString::number(pointValue[1]));
	elementA2.appendChild(text);
	text = m_doc->createTextNode(QString::number(pointValue[2]));
	elementA3.appendChild(text);
	text = m_doc->createTextNode(QString::number(pointValue[3]));
	elementA4.appendChild(text);
	text = m_doc->createTextNode(QString::number(pointValue[4]));
	elementA5.appendChild(text);
	text = m_doc->createTextNode(QString::number(pointValue[5]));
	elementA6.appendChild(text);
	text.clear();

	elementChildPoint.appendChild(elementA1);
	elementChildPoint.appendChild(elementA2);
	elementChildPoint.appendChild(elementA3);
	elementChildPoint.appendChild(elementA4);
	elementChildPoint.appendChild(elementA5);
	elementChildPoint.appendChild(elementA6);

	QFile file(m_fileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		CScreenWarning::GetInstance()->GetLabel()->setText("Fail to save data");
	}

	QTextStream out(&file);
	m_doc->save(out, 4);
	file.close();
}
