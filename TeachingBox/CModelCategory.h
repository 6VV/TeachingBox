#ifndef _TEACHING_BOX_C_MODEL_CATEGORY_H_
#define _TEACHING_BOX_C_MODEL_CATEGORY_H_

#include "CSingleTon.h"
#include "CTreeModel.h"
#include "QVariant"

class CModelCategory:public CTreeModel
{
	friend class CSingleTon<CModelCategory>;

	Q_OBJECT

private:
	CModelCategory(QWidget* parent=0);
	CModelCategory(const CModelCategory&);
	CModelCategory& operator=(const CModelCategory&);
	~CModelCategory();

	/*自定义公有函数*/
public:
	static CModelCategory* GetInstance();
	void UpdateData();
	void UpdateRootData();	/*获取根节点*/
	void UpdateCategoryText();	/*更新文本*/

	/*自定义私有函数*/
private:
	void Init();
	void GetCategoryData(CTreeItem* parentItem);	/*获取命令类型*/
	void GetSystemData(CTreeItem* parentItem);	/*获取系统分类下各命令*/
	void GetIOData(CTreeItem* parentItem);		/*获取输入输出分类下各命令*/
	void GetWeldData(CTreeItem* parentItem);	/*获取焊接分类下各命令*/

	void GetChildItem(CTreeItem* parentItem, QString childName);
	
	/*自定义公有变量*/
	/*此变量被CModelMacro类调用，用于判断显示何种界面*/
public:
	QString STR_MOVEMENT;
	QString STR_FUNCTIONS;
	QString STR_IO;
	QString STR_WELD;

	QString STR_SYSTEM_MATH;
	QString STR_SYSTEM_STRUCTURE;
	QString STR_SYSTEM_PROGRAM;
	QString STR_SYSTEM_FUNCTION;
	QString STR_SYSTEM_OTHERS;

	QString STR_IO_DIGITAL;
	QString STR_IO_ANALOG;

	QString STR_WELD_ARC;
	QString STR_WELD_SPOT;

	/*自定义私有变量*/
private:
	const char* CLASS_NAME = "CModelCategory";

};

#endif