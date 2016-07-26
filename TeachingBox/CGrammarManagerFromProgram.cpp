#include "stdafx.h"
#include "CGrammarManagerFromProgram.h"
#include "CModelCategory.h"
#include "CScreenMain.h"

CGrammarManagerFromProgram::CGrammarManagerFromProgram()
{
	Init();
}

CGrammarManagerFromProgram::~CGrammarManagerFromProgram()
{
	m_domDocument->clear();
	delete(m_domDocument);
}

CGrammarManagerFromProgram* CGrammarManagerFromProgram::GetInstance()
{
	return CSingleTon<CGrammarManagerFromProgram>::GetInstance();
}

void CGrammarManagerFromProgram::UpdateCategoryData()
{
	/*QDomElement rootElement = m_domDocument->documentElement();
	GetCategoryDataFromXml(rootElement, m_modelCategory->GetRootItem());*/
}

void CGrammarManagerFromProgram::UpdateMacroData(QModelIndex& indexCategory)
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

	QStringList strListMacro;
	for (int i = indexList.size() - 1; i >= 0; i--)
	{
		strListMacro << m_modelCategory->index(indexList.at(i).row(), 0, indexList.at(i).parent()).data().toString();
	}

	GetMacroFromProgram(strListMacro);
}

//void CGrammarManagerFromProgram::ModifyText(const QString& text)
//{
//	QStringList strList = GetTokens(text);
//	if (!strList.size())
//	{
//		return;
//	}
//
//	QString strMacro = strList.at(0);
//	QStringList strListParameterName;
//	//QList<QString> strListParameterType;
//
//	QString strMacroUpper = strMacro.toUpper();
//
//	if (strMacroUpper == STR_MACRO_MOVL)
//	{
//		GetMovlList(strList, strListParameterName);
//	}
//	else if (strMacroUpper == STR_MACRO_MOVC)
//	{
//		GetMovcList(strList, strListParameterName);
//	}
//	/*若为For命令*/
//	else if (strMacroUpper == STR_MACRO_FOR)
//	{
//		GetForList(strList, strListParameterName);
//	}
//	/*若为Switch命令*/
//	else if (strMacroUpper == STR_MACRO_SWITCH)
//	{
//		GetSwitchList(strList, strListParameterName);
//	}
//	/*若不是任何命令，返回*/
//	else
//	{
//		return;
//	}
//
//	//CScreenEditMacro::GetInstance()->ChangeContent(strMacro, strListParameterName, strListParameterType);
//	CEditParameter::GetInstance()->ChangeContent(strMacroUpper, strListParameterName);
//	CScreenMain::GetInstance()->ChangeToScreenEditMacro();
//}

void CGrammarManagerFromProgram::SelectMacro(QModelIndex& indexSelected)
{
	QString strMacro = m_modelMacro->index(indexSelected.row(), 0, indexSelected.parent()).data().toString();
	
	/*若为Movl命令*/
	if (strMacro==STR_MACRO_MOVL)
	{
		EditText("MOVL *,*,*");
	}
	/*若为Movc命令*/
	else if (strMacro==STR_MACRO_MOVC)
	{
		EditText("MOVC *,*,*,*");
	}
	/*若为For命令*/
	else if (strMacro==STR_MACRO_FOR_NEXT)
	{
		EditText("FOR *=* TO * STEP *");
	}
	/*若为Switch命令*/
	else if (strMacro==STR_MACRO_SWITCH_DEFAULT_ENDSWITCH)
	{
		EditText("SWITCH *");
	}
	else if (strMacro==STR_MACRO_IF_ENDIF)
	{
		EditText("IF *");
	}
	else if (strMacro==STR_MACRO_CASE)
	{
		EditText("CASE *");
	}
	else if (strMacro==STR_MACRO_DEFAULT)
	{
		EditText("DEFAULT");
	}
	else if (strMacro==STR_MACRO_BREAK)
	{
		EditText("BREAK");
	}
	else if (strMacro==STR_MACRO_ELSEIF)
	{
		EditText("ELSEIF *");
	}
	else if (strMacro==STR_MACRO_GOSUB)
	{
		EditText("GOSUB *");
	}
}

void CGrammarManagerFromProgram::EditText(const QString& text)
{
	QStringList strList = GetTokens(text);
	if (!strList.size())
	{
		return;
	}

	QString strMacro = strList.at(0);
	QStringList strListParameterName;

	QString strMacroUpper = strMacro.toUpper();

	if (strMacroUpper == STR_MACRO_MOVL)
	{
		GetMovlList(strList, strListParameterName);
		EditMacroParameter(strMacro, strListParameterName);
	}
	else if (strMacroUpper == STR_MACRO_MOVC)
	{
		GetMovcList(strList, strListParameterName);
		EditMacroParameter(strMacro, strListParameterName);
	}
	else if (strMacroUpper == STR_MACRO_FOR
		|| strMacroUpper == STR_MACRO_SWITCH
		|| strMacro == STR_MACRO_IF
		|| strMacro == STR_MACRO_CASE
		|| strMacro == STR_MACRO_DEFAULT
		|| strMacro == STR_MACRO_BREAK
		|| strMacro == STR_MACRO_ELSEIF
		|| strMacro == STR_MACRO_GOSUB)
	{
		EditNormalParameter(text);
		CScreenMain::GetInstance()->ChangeToScreenProgram();
	}
	/*若不是任何命令，返回*/
	else
	{
		return;
	}
}

void CGrammarManagerFromProgram::GetMacroFromProgram(QStringList& strIndex)
{
	if (strIndex.size()==0)
	{
		return;
	}
	m_modelMacro->ChangeToData(strIndex.at(strIndex.size() - 1));
}

void CGrammarManagerFromProgram::Init()
{
	m_modelCategory = CModelCategory::GetInstance();
	m_modelMacro = CModelMacro::GetInstance();

	QFile file(GRAMMAR_FILE_NAME);
	if (file.open(QIODevice::ReadOnly))
	{
		m_domDocument = new QDomDocument();
		m_domDocument->setContent(&file);
		file.close();
	}

	UpdateCategoryData();
}

void CGrammarManagerFromProgram::GetCategoryDataFromXml(QDomElement& parentElement, CTreeItem* parentItem)
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

void CGrammarManagerFromProgram::GetMacroDataFromXml(QList<QString>& categoryList)
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

void CGrammarManagerFromProgram::InsertMacro(QString strMacro)
{
	CScreenProgram::GetInstance()->InsertTextBeforeLine(strMacro);
	CScreenMain::GetInstance()->ChangeToScreenProgram();
}

void CGrammarManagerFromProgram::GetMovlList(QStringList& strList, QStringList& strListParameter)
{
	if (strList.size() == 4)
	{
		for (int i = 1; i < strList.size(); i++)
		{
			strListParameter.append(strList.at(i));
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			strListParameter.append("*");
		}
	}
}

void CGrammarManagerFromProgram::GetMovcList(QStringList& strList, QStringList& strListParamter)
{
	if (strList.size() == 5)
	{
		for (int i = 1; i < strList.size(); i++)
		{
			strListParamter.append(strList.at(i));
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			strListParamter.append("*");
		}
	}
}

void CGrammarManagerFromProgram::GetForList(QStringList& strList, QStringList& strListParamter)
{
	if (strList.size() == 5)
	{
		for (int i = 1; i < strList.size(); i++)
		{
			strListParamter.append(strList.at(i));
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			strListParamter.append("*");
		}
	}
}

void CGrammarManagerFromProgram::GetSwitchList(QStringList& strList, QStringList& strListParameter)
{
	if (strList.size() == 2)
	{
		strListParameter.append(strList.at(1));
		
	}
	else
	{
		strListParameter.append("*");
	}
}


void CGrammarManagerFromProgram::EditMacroParameter(const QString& macroName, const QStringList& macroParameterList)
{
	CEditParameter::GetInstance()->ChangeContent(macroName, macroParameterList);
	CScreenMain::GetInstance()->ChangeToScreenEditMacro();
}

void CGrammarManagerFromProgram::EditNormalParameter(const QString& text)
{
	CScreenProgram::GetInstance()->EditNormalParameter(text);
}

