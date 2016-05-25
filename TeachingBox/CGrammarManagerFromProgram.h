#ifndef _TEACHING_BOX_C_GRAMMAR_MANAGER_FROM_PROGRAM_H_
#define _TEACHING_BOX_C_GRAMMAR_MANAGER_FROM_PROGRAM_H_

#include "CSingleTon.h"
#include "CGrammarManagerFactory.h"

class CGrammarManagerFromProgram:public CGrammarManagerFactory
{
	friend class CSingleTon<CGrammarManagerFromProgram>;

private:
	CGrammarManagerFromProgram();
	CGrammarManagerFromProgram(const CGrammarManagerFromProgram&);
	CGrammarManagerFromProgram& operator=(const CGrammarManagerFromProgram&);
	~CGrammarManagerFromProgram();

public:
	static CGrammarManagerFromProgram* GetInstance();

	void UpdateCategoryData() Q_DECL_OVERRIDE; /*更新分类模型*/
	void UpdateMacroData(QModelIndex& indexCategory) Q_DECL_OVERRIDE;	/*更新宏定义模型*/
	void SelectMacro(QModelIndex& indexSelected) Q_DECL_OVERRIDE;	/*选取宏定义*/
	void EditText(const QString& text) override;	/*编辑程序文本*/
	void GetMacroFromProgram(QStringList& strIndex);	/*获取命令列表*/
	//void ModifyText(const QString& text);

	/*自定义私有函数*/
private:
	void Init();	/*初始化*/

	void GetCategoryDataFromXml(QDomElement& parentElement, CTreeItem* parentItem);	/*分类模型获取数据*/
	void GetMacroDataFromXml(QList<QString>& categoryList);	/*改变命令模型中的当前数据*/

	void InsertMacro(QString strMacro);	/*构建命令*/

	void GetMovlList(QStringList& strList, QStringList& strListParameter);	/*获取Movl命令列表*/
	void GetMovcList(QStringList& strList, QStringList& strListParamter);	/*获取Movc命令列表*/
	void GetForList(QStringList& strList, QStringList& strListParamter);	/*获取For命令列表*/
	void GetSwitchList(QStringList& strList, QStringList& strListParameter);/*获取Switch命令列表*/

	void EditMacroParameter(const QString& macroName,const QStringList& macroParameterList);
	void EditNormalParameter(const QString& text);
private:


	//////////////////////////////////////////////////////////////////////////
	/*xml文档相关*/
	QDomDocument* m_domDocument;
	const QString XML_TAG_CATEGORY = "category";	/*该节点为分类节点*/
	const QString XML_TAG_MACRO = "macro";	/*该节点为宏定义节点*/
	const QString XML_ATTRIBUTE_NAME = "name";	/*名称*/
	//////////////////////////////////////////////////////////////////////////
};

#endif