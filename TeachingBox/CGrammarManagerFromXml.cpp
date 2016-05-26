#include "stdafx.h"
#include "CGrammarManagerFromXml.h"

CGrammarManagerFromXml::CGrammarManagerFromXml()
{
	Init();
}

CGrammarManagerFromXml::~CGrammarManagerFromXml()
{
	m_domDocument->clear();
	delete(m_domDocument);
}

CGrammarManagerFactory* CGrammarManagerFromXml::GetInstance()
{
	return CSingleTon<CGrammarManagerFactory>::GetInstance();
}

void CGrammarManagerFromXml::UpdateCategoryData()
{
	/*QDomElement rootElement = m_domDocument->documentElement();
	GetCategoryDataFromXml(rootElement, m_modelCategory->GetRootItem());*/
}

void CGrammarManagerFromXml::UpdateMacroData(QModelIndex& indexCategory)
{
	if (!indexCategory.isValid())
	{
		return;
	}

	/*若还有子节点，则返回*/
	if (indexCategory.child(0, 0).isValid())
	{
		return;
	}

	/*获取父节点及当前节点列表*/
	QList<QModelIndex> indexList;
	QModelIndex currentIndex = indexCategory;
	do
	{
		indexList << currentIndex;
		currentIndex = currentIndex.parent();
	} while (currentIndex.isValid());

	QList<QString> strListMacro;
	for (int i = indexList.size() - 1; i >= 0; i--)
	{
		strListMacro << m_modelCategory->index(indexList.at(i).row(), 0, indexList.at(i).parent()).data().toString();
	}

	GetMacroDataFromXml(strListMacro);
}

void CGrammarManagerFromXml::SelectMacro(QModelIndex& indexSelected)
{
	SelectMacroFromXml(indexSelected);
}

void CGrammarManagerFromXml::EditText()
{

}

QDomElement CGrammarManagerFromXml::SearchMacroFromXml(QModelIndex& indexSelected)
{
	/*选取的宏名称*/
	QString strMacro = m_modelMacro->index(indexSelected.row(), 0, indexSelected.parent()).data().toString();

	/*遍历查找对应的宏命令*/
	QDomElement elementMacro;
	QDomNodeList nodeListMacro = m_domDocument->elementsByTagName(XML_TAG_MACRO);
	for (int i = 0; i < nodeListMacro.size(); i++)
	{
		if (nodeListMacro.at(i).toElement().attribute(XML_ATTRIBUTE_NAME) == strMacro)
		{
			elementMacro = nodeListMacro.at(i).toElement();
			break;
		}
	}
	return elementMacro;
}

void CGrammarManagerFromXml::SelectMacroFromXml(QModelIndex& indexSelected)
{
	if (!indexSelected.isValid())
	{
		return;
	}

	QDomElement elementMacro = SearchMacroFromXml(indexSelected);

	/*输出宏命令*/
	QString strListGrammar;	/*当前宏命令语法文本*/
	QList<QString> listParameter;	/*当前宏命令参数文本*/

	QDomNodeList childNodes = elementMacro.childNodes();
	for (int i = 0; i < childNodes.size(); i++)
	{
		if (childNodes.at(i).isElement())
		{
			QDomElement childElement = childNodes.at(i).toElement();
			/*获取节点类型*/
			QString strType = childElement.attribute(XML_ATTRIBUTE_TYPE);

			/*若类型为copy，则直接复制源文本*/
			if (strType == XML_ATTRIBUTE_TYPE_COPY)
			{
				strListGrammar.append(childElement.text());
				continue;
			}
			/*若类型为escape，则进行转义*/
			else if (strType == "escape")
			{
				QString strChildText = childElement.text();
				if (strChildText == "\\n")
				{
					strListGrammar.append("\n");
				}
				continue;
			}
			else
			{
				strListGrammar.append(childElement.text());
			}
			listParameter.append(strType);
		}
	}

	///*判断是否调用新窗口编辑宏命令*/
	//if (elementMacro.attribute(XML_ATTRIBUTE_EDIT) == XML_ATTRIBUTE_EDIT_TABLE)
	//{
	//	CScreenEditMacro::GetInstance()->ChangeContent(strListGrammar, listParameter);
	//	CScreenMain::GetInstance()->ChangeToScreenEditMacro();
	//}
	//else
	//{
	//	CCodeEditor::GetInstance()->appendPlainText(strListGrammar);
	//	CScreenMain::GetInstance()->ChangeToScreenProgram();
	//}
}

void CGrammarManagerFromXml::Init()
{
	/*m_modelCategory = CModelCategoryFromXml::GetInstance();
	m_modelMacro = CModelMacroFromXml::GetInstance();

	QFile file(GRAMMAR_FILE_NAME);
	if (file.open(QIODevice::ReadOnly))
	{
		m_domDocument = new QDomDocument();
		m_domDocument->setContent(&file);
		file.close();
	}

	UpdateCategoryData();*/
}

void CGrammarManagerFromXml::GetCategoryDataFromXml(QDomElement& parentElement, CTreeItem* parentItem)
{
	QDomNodeList childrenNode = parentElement.childNodes();
	for (int i = 0; i < childrenNode.size(); i++)
	{
		if (childrenNode.at(i).isElement())
		{
			QDomElement childElement = childrenNode.at(i).toElement();
			if (childElement.tagName() == XML_TAG_CATEGORY)
			{
				/*创建分类模型子节点*/
				QList<QVariant> childData;
				childData << childElement.attribute(XML_ATTRIBUTE_NAME);
				CTreeItem* childItem = new CTreeItem(childData, parentItem);

				/*将子节点添加到模型中*/
				parentItem->appendChild(childItem);

				GetCategoryDataFromXml(childElement, childItem);
			}
		}

	}
}

void CGrammarManagerFromXml::GetMacroDataFromXml(QList<QString>& categoryList)
{
	QDomElement currentElement = m_domDocument->documentElement();

	/*遍历分类列表*/
	for (int i = 0; i < categoryList.size(); i++)
	{
		/*所查找的分类*/
		QString strCategory = categoryList.at(i);
		/*当前分类下所有子节点*/
		QDomNodeList childNodes = currentElement.childNodes();
		/*遍历子节点*/
		for (int j = 0; j < childNodes.size(); j++)
		{
			if (childNodes.at(j).isElement())
			{
				QDomElement childElement = childNodes.at(j).toElement();
				/*若找到该分类，则遍历该分类下的子节点*/
				if (childElement.attribute(XML_ATTRIBUTE_NAME) == strCategory)
				{
					currentElement.clear();
					currentElement = childElement;
					break;
				}
			}
		}
	}

	/*宏命令列表*/
	QList<QString> strListMacro;
	QDomNodeList childNodes = currentElement.childNodes();
	for (int i = 0; i < childNodes.size(); i++)
	{
		if (childNodes.at(i).isElement())
		{
			strListMacro << childNodes.at(i).toElement().attribute(XML_ATTRIBUTE_NAME);

		}
	}

	//m_modelMacro->UpdateData(strListMacro);

}
