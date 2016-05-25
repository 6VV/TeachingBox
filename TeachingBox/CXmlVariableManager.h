#ifndef _TEACHING_BOX_C_XML_CONTROLER_VARIABLE_H_
#define _TEACHING_BOX_C_XML_CONTROLER_VARIABLE_H_

class CXmlVariableManager
{
public:
	CXmlVariableManager();
	~CXmlVariableManager();

	/*自定义公有函数*/
public:
	void SetProjectName(QString strProjectName);
	void SetProgramName(QString strProgramName);

	void WriteProgramVariable();
	void WriteAllVariable(QXmlStreamWriter& writer);
	void WritePositionVariable(QXmlStreamWriter& writer);
	void WriteVelocityVariable(QXmlStreamWriter& writer);
	void WriteZoneVariable(QXmlStreamWriter& writer);

	/*自定义私有变量*/
private:
	const QString FILE_NAME = "variable.xml";
	QString m_filePath;
	QString m_projectName;
	QString m_programName;
};

#endif