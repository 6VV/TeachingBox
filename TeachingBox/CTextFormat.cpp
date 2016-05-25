#include "stdafx.h"
#include "CTextFormat.h"



CTextFormat::CTextFormat()
{

}

CTextFormat::~CTextFormat()
{
}

void CTextFormat::FormatText(const QString& strOldText, QString& strNewText)
{
	QStringList strList = strOldText.split('\n');
	int retractNum = 0;

	for each (auto var in strList)
	{
		var = var.simplified();
		if (var.size()==0)
		{
			continue;
		}
		QString strFirst = var.split(' ').at(0);

		if (strFirst == "FOR" || strFirst == "IF" || strFirst == "SWITCH")
		{
			AddRetract(strNewText, retractNum);
			FormatLineText(var);

			strNewText.append(var).append('\n');
			retractNum++;
		}
		else if (strFirst == "NEXT" || strFirst == "ENDIF" || strFirst == "ENDSWITCH")
		{
			retractNum--;

			AddRetract(strNewText, retractNum);
			FormatLineText(var);

			strNewText.append(var).append('\n');
		}
		else if (strFirst == "CASE" || strFirst=="DEFAULT" || strFirst=="ELSEIF")
		{
			retractNum--;

			AddRetract(strNewText, retractNum);
			FormatLineText(var);

			strNewText.append(var).append('\n');

			retractNum++;
		}
		else
		{
			AddRetract(strNewText, retractNum);
			FormatLineText(var);

			strNewText.append(var).append('\n');
		}
	}

	strNewText.chop(1);
}

void CTextFormat::AddRetract(QString& strText, int retractNum)
{
	for (int i = 0; i < retractNum; ++i)
	{
		strText.append(strRetract);
	}
}

void CTextFormat::FormatLineText(QString& strOldText)
{
	QString strNewText;

	for (int i = 0; i < strOldText.size(); ++i)
	{
		switch (strOldText.at(i).toLatin1())
		{
		/*若为算数运算符或&、|，则两边加空格*/
		case '+':case '-':case '*':case '/':case '^':case '&':case '|':
		{
			strNewText.append(' ').append(strOldText.at(i)).append(' ');
		}break;
		/*若为逗号，则后面加空格*/
		case ',':
		{
			strNewText.append(strOldText.at(i)).append(' ');
		}break;
		case '!':
		{
			switch (strOldText.at(i+1).toLatin1())
			{
				/*若为!=，!>，!<，则两边加空格*/
			case '=':case'>':case'<':
			{
				strNewText.append(' ').append(strOldText.at(i)).append(strOldText.at(i + 1)).append(' ');
				++i;
			}break;
			default:
			{
				strNewText.append(' ').append(strOldText.at(i)).append(' ');
			}break;
			}
		}break;
		/*若为>，<，两边加空格*/
		case '>':case '<':
		{
			switch (strOldText.at(i + 1).toLatin1())
			{
				/*若为>=，<=，则两边加空格*/
			case '=':
			{
				strNewText.append(' ').append(strOldText.at(i)).append(strOldText.at(i + 1)).append(' ');
				++i;
			}break;
			default:
			{
				strNewText.append(' ').append(strOldText.at(i)).append(' ');
			}break;
			}

		}break;
		case '=':
		{
			/*若为==*/
			if (strOldText.at(i+1)=='=')
			{
				strNewText.append(' ').append(strOldText.at(i)).append(strOldText.at(i + 1)).append(' ');
				++i;
			}
			else
			{
				strNewText.append(' ').append(strOldText.at(i)).append(' ');
			}
		}break;
		/*若为（）,不加空格*/
		case '(':case ')':
		{
			strNewText.append(strOldText.at(i));
		}break;
		/*若为变量*/
		default:
		{
			strNewText.append(strOldText.at(i));
		}break;
		}
	}

	strOldText = strNewText.simplified();
}
