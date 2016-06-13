#include "stdafx.h"
#include "CFileManager.h"
#include "QFile"
#include "QTextStream"


CFileManager::CFileManager()
{

}

CFileManager::~CFileManager()
{

}

bool CFileManager::FindVariable(const QString& strFilePath, const QString& strVariableName)
{
	//QString path = "E:/Project/QT/TeachPendant_qt5/VS/TeachingBox/TeachingBox/Projects/B/a.txt";

	/*获取文件文本*/
	QString strFileText;
	GetFileText(strFileText, strFilePath);

	int pChar = 0;
	int length = strFileText.size();
	QChar currentChar = strFileText[0];
	while (pChar<length)
	{
		currentChar = strFileText[pChar];

		/*若为字符"/"*/
		if (currentChar == '/')
		{
			/*若为注释*/
			if (strFileText[pChar+1]=='/')
			{
				/*跳过本行*/
				while (!IsLineEnd(strFileText[++pChar]));
			}
		}
		/*若为引号*/
		else if (currentChar == '\"')
		{
			++pChar;
			/*寻找下一个引号或跳过本行*/
			while (!IsLineEnd(strFileText[pChar]) && strFileText[pChar] != '\"')
			{
				++pChar;
			}
		}
		/*若为变量*/
		else if (IsWord(currentChar))
		{
			/*获取字符串*/
			QString str;
			while (IsWord(strFileText[pChar]))
			{
				str.append(strFileText[pChar]);
				++pChar;
			}

			/*若找到变量*/
			if (str==strVariableName)
			{
				return true;
			}
			else
			{
				continue;
			}

		}

		++pChar;
	}

	return false;
}

void CFileManager::GetFileText(QString& strFileText, const QString& strFilePath)
{
	QFile file(strFilePath);
	if (file.open(QFile::ReadOnly))
	{
		QTextStream fileIn(&file);
		fileIn.setCodec("UTF-8");
		fileIn.seek(0);
		strFileText = fileIn.readAll();
		file.close();
	}
}

void CFileManager::SetFileText(const QString& strFileText, const QString& strFilePath)
{
	QFile file(strFilePath);
	if (file.open(QFile::WriteOnly))
	{
		QTextStream fileIn(&file);
		fileIn.setCodec("UTF-8");
		fileIn.seek(0);

		fileIn << strFileText;

		file.close();
	}
}

void CFileManager::RenameFileText(const QString& strFilePath, const QString& strOldName, const QString& strNewName)
{
	//QString path = "E:/Project/QT/TeachPendant_qt5/VS/TeachingBox/TeachingBox/Projects/B/a.txt";

	/*新的文件文本*/
	QString strNewText;

	/*获取文件文本*/
	QString strOldText;
	GetFileText(strOldText, strFilePath);

	int pChar = 0;
	int length = strOldText.size();
	QChar currentChar = strOldText[0];

	while (pChar < length)
	{
		currentChar = strOldText[pChar];

		/*若为字符"/"*/
		if (currentChar == '/')
		{
			strNewText.append(currentChar);
			/*若为注释*/
			if (strOldText[pChar + 1] == '/')
			{
				/*跳过本行*/
				while (!IsLineEnd(strOldText[++pChar]))
				{
					strNewText.append(strOldText[pChar]);
				}
				strNewText.append(strOldText[pChar]);
			}
		}
		/*若为引号*/
		else if (currentChar == '\"')
		{
			strNewText.append(currentChar);
			++pChar;
			strNewText.append(strOldText[pChar]);
			/*寻找下一个引号或跳过本行*/
			while (!IsLineEnd(strOldText[pChar]) && strOldText[pChar] != '\"')
			{
				++pChar;
				strNewText.append(strOldText[pChar]);
			}
		}
		/*若为变量*/
		else if (IsWord(currentChar))
		{
			/*获取字符串*/
			QString str;
			while (IsWord(strOldText[pChar]))
			{
				str.append(strOldText[pChar]);
				++pChar;
			}

			/*若找到变量*/
			if (str == strOldName)
			{
				strNewText.append(strNewName);
			}
			else
			{
				strNewText.append(str);
			}

			strNewText.append(strOldText[pChar]);

		}
		else
		{
			strNewText.append(currentChar);
		}

		++pChar;
	}

	SetFileText(strNewText, strFilePath);
}

bool CFileManager::IsWord(QChar currentChar)
{
	return (currentChar >= 'a'&& currentChar <= 'z')
		|| (currentChar >= 'A' && currentChar <= 'Z')
		|| (currentChar >= '0' && currentChar <= '9')
		|| currentChar == '_';
}

bool CFileManager::IsLineEnd(QChar c)
{
	return c == '\n' || c == '\0';
}
