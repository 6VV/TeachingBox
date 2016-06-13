#include "stdafx.h"
#include "CGrammarManagerFactory.h"
#include "qdom.h"
#include "CGrammarManagerFromProgram.h"


const QString CGrammarManagerFactory::STR_PARAMETER_TYPE_POSITON = "position";
const QString CGrammarManagerFactory::STR_PARAMETER_TYPE_VELOCITY = "velocity";
const QString CGrammarManagerFactory::STR_PARAMETER_TYPE_ZONE = "zone";
const QString CGrammarManagerFactory::STR_PARAMETER_TYPE_FLOAT = "double";
const QString CGrammarManagerFactory::STR_PARAMETER_TYPE_LABEL = "label";
const QString CGrammarManagerFactory::STR_PARAMETER_TYPE_INT = "int";
const QString CGrammarManagerFactory::STR_PARAMETER_TYPE_BOOL = "bool";
const QString CGrammarManagerFactory::STR_PARAMETER_TYPE_STRING = "string";
const QString CGrammarManagerFactory::STR_PARAMETER_TYPE_NUMBER = "number";
const QString CGrammarManagerFactory::STR_PARAMETER_TYPE_VAR_NUMBER = "varNumber";

const QString CGrammarManagerFactory::STR_MACRO_MOVL = "MOVL";
const QString CGrammarManagerFactory::STR_MACRO_MOVC = "MOVC";
const QString CGrammarManagerFactory::STR_MACRO_MOVJ = "MOVJ";
const QString CGrammarManagerFactory::STR_MACRO_MOVX = "MOVX";
const QString CGrammarManagerFactory::STR_MACRO_MOVJX = "MOVJX";
const QString CGrammarManagerFactory::STR_MACRO_MOVY = "MOVY";
const QString CGrammarManagerFactory::STR_MACRO_MOVZ = "MOVZ";
const QString CGrammarManagerFactory::STR_MACRO_ROTX = "ROTX";
const QString CGrammarManagerFactory::STR_MACRO_ROTY = "ROTY";
const QString CGrammarManagerFactory::STR_MACRO_ROTZ = "ROTZ";

const QString CGrammarManagerFactory::STR_MACRO_SIN = "SIN";
const QString CGrammarManagerFactory::STR_MACRO_COS = "COS";
const QString CGrammarManagerFactory::STR_MACRO_TAN = "TAN";
const QString CGrammarManagerFactory::STR_MACRO_SQRT = "SQRT";
const QString CGrammarManagerFactory::STR_MACRO_LABEL = "LABEL";
const QString CGrammarManagerFactory::STR_MACRO_GOTO = "GOTO";
const QString CGrammarManagerFactory::STR_MACRO_GOSUB = "GOSUB";
const QString CGrammarManagerFactory::STR_MACRO_FOR = "FOR";
const QString CGrammarManagerFactory::STR_MACRO_FOR_NEXT = "FOR...NEXT";
const QString CGrammarManagerFactory::STR_MACRO_IF="IF";
const QString CGrammarManagerFactory::STR_MACRO_IF_ENDIF = "IF...ENDIF";
const QString CGrammarManagerFactory::STR_MACRO_ELSEIF = "ELSEIF";
const QString CGrammarManagerFactory::STR_MACRO_SWITCH = "SWITCH";
const QString CGrammarManagerFactory::STR_MACRO_SWITCH_DEFAULT_ENDSWITCH = "SWITCH...ENDSWITCH";
const QString CGrammarManagerFactory::STR_MACRO_CASE = "CASE";
const QString CGrammarManagerFactory::STR_MACRO_DEFAULT="DEFAULT";
const QString CGrammarManagerFactory::STR_MACRO_BREAK = "BREAK";
const QString CGrammarManagerFactory::STR_MACRO_ABORT = "ABORT";
const QString CGrammarManagerFactory::STR_MACRO_END = "END";

const QString CGrammarManagerFactory::STR_MACRO_SET_TOOLFRAME = "SET TOOLFRAME";
const QString CGrammarManagerFactory::STR_MACRO_SET_USERFRAME = "SET USERFRAME";

const QString CGrammarManagerFactory::STR_MACRO_ARC = "ARC";
const QString CGrammarManagerFactory::STR_MACRO_SET_ARC = "SET ARC";
const QString CGrammarManagerFactory::STR_MACRO_ARC_OFF = "ARC OFF";

const QString CGrammarManagerFactory::STR_MACRO_DIN = "DIN";
const QString CGrammarManagerFactory::STR_MACRO_DOUT = "DOUT";
const QString CGrammarManagerFactory::STR_MACRO_AIN = "AIN";
const QString CGrammarManagerFactory::STR_MACRO_AOUT = "AOUT";

const QString CGrammarManagerFactory::STR_MACRO_SPOT_ON = "SPOT ON";
const QString CGrammarManagerFactory::STR_MACRO_SPOT_OFF = "SPOT OFF";
const QString CGrammarManagerFactory::STR_MACRO_SET_SPOT = "SET SPOT";

const QString CGrammarManagerFactory::STR_MACRO_WAIT = "WAIT";

/*命令格式*/
const char* CGrammarManagerFactory::STR_MACRO_FORMAT_MOVL = "MOVL %1, %2, %3";
const char* CGrammarManagerFactory::STR_MACRO_FORMAT_MOVC = "MOVC %1, %2, %3, %4";

CGrammarManagerFactory::CGrammarManagerFactory()
{
}

CGrammarManagerFactory::~CGrammarManagerFactory()
{
}

CGrammarManagerFactory* CGrammarManagerFactory::GetInstance()
{
	return CSingleTon<CGrammarManagerFromProgram>::GetInstance();
}

/*更新分类模型*/
void CGrammarManagerFactory::UpdateCategoryData()
{
}

//
///*选取宏定义*/
//void CGrammarManagerFactory::SelectMacro(QModelIndex& indexSelected)
//{
//}
//
//void CGrammarManagerFactory::EditText(const QString& text)
//{
//
//}

QList<QString> CGrammarManagerFactory::GetTokens(const QString& text)
{
	QString strText = text;

	QList<QString> strList;

	/*若文本为空*/
	if (strText.isEmpty())
	{
		return strList;
	}
	/*跳过间隔*/
	SkipBlank(strText);
	strList.append(GetToken(strText));

	/*若间隔为等号*/
	if (strText.at(0)=='=')
	{
		strList.append("=");
		strText.remove(0, 1);
		strList.append(strText);
	}
	else
	{
		QString macro = strList.at(0).toUpper();

		/*若为命令*/
		if (macro==CGrammarManagerFactory::STR_MACRO_MOVL
			|| macro==CGrammarManagerFactory::STR_MACRO_MOVC)
		{
			while (!strText.isEmpty())
			{
				SkipInterval(strText);
				if (!strText.isEmpty())
				{
					strList.append(GetToken(strText));
				}
			}

		}
		/*若为for命令*/
		else if (macro == CGrammarManagerFactory::STR_MACRO_FOR)
		{
			GetForList(strList, strText);
		}
	}

	/*while (!strText.isEmpty())
	{
	SkipInterval(strText);
	if (!strText.isEmpty())
	{
	strList.append(GetToken(strText));
	}
	}*/
	
	return strList;
}

//QString CGrammarManagerFactory::GetText()
//{
//	/*获取光标所在行文本*/
//	CCodeEditor* codeEditor = CCodeEditor::GetInstance();
//	QTextCursor cursor = codeEditor->textCursor();
//	cursor.select(QTextCursor::LineUnderCursor);
//	QString text = cursor.selectedText();
//	return text;
//}

void CGrammarManagerFactory::GetForList(QList<QString>& strList, QString& strText)
{
	SkipBlank(strText);
	/*添加第一个参数*/
	strList.append(GetToken(strText));
	SkipBlank(strText);

	if (strText.isEmpty())
	{
		return;
	}

	/*若为等号*/
	if (strText.at(0) == "=")
	{
		/*删除等号*/
		strText.remove(0, 1);
		SkipBlank(strText);
		if (strText.isEmpty())
		{
			return;
		}

		/*添加第二个参数*/
		strList.append(GetToken(strText));
		SkipBlank(strText);
		if (strText.isEmpty())
		{
			return;
		}

		/*若为To*/
		if (strText.toUpper().left(2) == "TO")
		{

			/*删除To*/
			strText.remove(0, 2);
			SkipBlank(strText);
			if (strText.isEmpty())
			{
				return;
			}

			/*添加第三个参数*/
			strList.append(GetToken(strText));
			SkipBlank(strText);
			if (strText.isEmpty())
			{
				return;
			}

			/*若为Step*/
			if (strText.toUpper().left(4) == "STEP")
			{

				/*删除Step*/
				strText.remove(0, 4);
				SkipBlank(strText);
				if (strText.isEmpty())
				{
					return;
				}

				/*添加第四个参数*/
				strList.append(GetToken(strText));
				SkipBlank(strText);
			}
		}
		else
		{
			return;
		}

	}
	else
	{
		return;
	}
}

QString CGrammarManagerFactory::GetToken(QString& strText)
{
	QString strToken;

	int i = 0;
	for (; i < strText.size(); i++)
	{
		if (!IsInterval(strText.at(i)))
		{
			continue;
		}
		else
		{
			break;
		}
	}

	strToken = strText.left(i);
	strText.remove(0, i);

	return strToken;
}

void CGrammarManagerFactory::SkipInterval(QString& strText)
{
	if (strText.isEmpty())
	{
		return;
	}
	int i = 0;
	for (; i < strText.size(); i++)
	{
		if (IsInterval(strText.at(i)))
		{
			continue;
		}
		else
		{
			break;
		}
	}
	strText.remove(0, i);
}

void CGrammarManagerFactory::SkipBlank(QString& strText)
{
	if (strText.isEmpty())
	{
		return;
	}
	int i = 0;
	for (; i < strText.size(); i++)
	{
		if (IsBlank(strText.at(i)))
		{
			continue;
		}
		else
		{
			break;
		}
	}
	strText.remove(0, i);
}

bool CGrammarManagerFactory::IsBlank(QChar ch)
{
	return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0');
}

bool CGrammarManagerFactory::IsInterval(QChar ch)
{
	return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0' || ch == '=' || ch == ',');
}

