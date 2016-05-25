#ifndef _TEACHING_BOX_C_FILE_MANAGER_H_
#define _TEACHING_BOX_C_FILE_MANAGER_H_

#include "QString"

class CFileManager
{
public:
	CFileManager();
	~CFileManager();

	bool FindVariable(const QString& strFilePath, const QString& strVariableName);	/*查找文件内是否存在变量（不包括注释及引号范围内的字符）*/

	void GetFileText(QString& strFileText, const QString& strFilePath);	/*读取文件文本*/
	void SetFileText(QString& strFileText, const QString& strFilePath);	/*写入文本文件*/

	void RenameFileText(const QString& strFilePath,const QString& strOldName,const QString& strNewName);

	inline bool IsWord(QChar);	/*是否为字母、数据或下划线*/
	inline bool IsLineEnd(QChar);	/*是否达到行尾（文件尾）*/

private:
	const char* CLASS_NAME = "CFileManager";
};

#endif