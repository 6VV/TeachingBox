#ifndef _TEACHING_BOX_C_GRAMMAR_MANAGER_FROM_XML_H_
#define _TEACHING_BOX_C_GRAMMAR_MANAGER_FROM_XML_H_

#include "CGrammarManagerFactory.h"

class CGrammarManagerFromXml:public CGrammarManagerFactory
{
	friend class CSingleTon<CGrammarManagerFromXml>;

private:
	CGrammarManagerFromXml();
	CGrammarManagerFromXml(const CGrammarManagerFromXml&);
	CGrammarManagerFromXml& operator=(const CGrammarManagerFromXml&);
	~CGrammarManagerFromXml();

	/*自定义公有函数*/
public:
	static CGrammarManagerFactory* GetInstance();

	void UpdateCategoryData() Q_DECL_OVERRIDE; /*更新分类模型*/
	void UpdateMacroData(QModelIndex& indexCategory) Q_DECL_OVERRIDE;	/*更新宏定义模型*/
	void SelectMacro(QModelIndex& indexSelected) Q_DECL_OVERRIDE;	/*选取宏定义*/
	void EditText();	/*编辑程序文本*/

	/*自定义保护函数*/
protected:
	QDomElement SearchMacroFromXml(QModelIndex& index);		/*查找index对应的宏定义*/
	void SelectMacroFromXml(QModelIndex& indexSelected);	/*选取宏定义语法*/

	/*自定义私有函数*/
private:
	void Init();
	void GetCategoryDataFromXml(QDomElement& parentElement, CTreeItem* parentItem);	/*分类模型获取数据*/
	void GetMacroDataFromXml(QList<QString>& categoryList);	/*改变命令模型中的当前数据*/

private:
	QDomDocument* m_domDocument;

	//////////////////////////////////////////////////////////////////////////
	/*与xml文件对应的部分标签属性值*/
	/*xml文件标签类型*/
	const QString XML_TAG_CATEGORY = "category";	/*该节点为分类节点*/
	const QString XML_TAG_MACRO = "macro";	/*该节点为宏定义节点*/
	const QString XML_TAG_GRAMMAR = "grammar";	/*该节点为语法节点*/

	/*xml文件属性类型*/
	const QString XML_ATTRIBUTE_NAME = "name";	/*名称*/
	const QString XML_ATTRIBUTE_EDIT = "edit";	/*编辑方式*/
	const QString XML_ATTRIBUTE_TYPE = "type";	/*类型*/

	/*xml文件部分属性值*/
	const QString XML_ATTRIBUTE_EDIT_TABLE = "table";	/*编辑方式为table*/
	const QString XML_ATTRIBUTE_TYPE_COPY = "copy";	/*类型值为copy*/
	const QString XML_ATTRIBUTE_TYPE_POSITION = "position";	/*类型值为copy*/
	const QString XML_ATTRIBUTE_TYPE_VELOCITY = "velocity";	/*类型值为velocity*/
	const QString XML_ATTRIBUTE_TYPE_ZONE = "zone";	/*类型值为zone*/
	//////////////////////////////////////////////////////////////////////////

};

#endif