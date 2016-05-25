#ifndef _TEACHING_BOX_C_MODEL_MACRO_H_
#define _TEACHING_BOX_C_MODEL_MACRO_H_

#include "CSingleTon.h"
#include "CTreeModel.h"

class CModelMacro:public CTreeModel
{
	friend CSingleTon<CModelMacro>;

	Q_OBJECT

private:
	CModelMacro();
	CModelMacro(const CModelMacro&);
	CModelMacro& operator=(const CModelMacro&);
	~CModelMacro();

	/*自定义公有函数*/
public:
	static CModelMacro* GetInstance();

	void UpdateData();
	void UpdateRootData();

	void ChangeToData(QString category);

	/*自定义私有变量*/
private:
	void GetChildItem(CTreeItem* parentItem, QString childName);
	void ChangeToMovementData();	/*显示运动命令*/
	void ChangeToSystemMathData();	/*显示系统分类下数学命令*/
	void ChangeToSystemStructureData();	/*显示系统分类下结构命令*/
	void ChangeToSystemPragramData();	/*显示系统分类下程序命令*/
	void ChangeToSystemFunctionData();	/*显示系统分类下功能命令*/
	void ChangeToSystemOthersData();	/*显示系统分类下其它命令*/
	void ChangeToIODigitalData();	/*显示输入输出分类下数字量命令*/
	void ChangeToIOAnalogData();	/*显示输入输出分类下模拟量命令*/
	void ChangeToWeldArcData();		/*显示焊接分类下弧焊命令*/
	void ChangeToWeldSpotData();	/*显示焊接分类下点焊命令*/


	/*自定义私有函数*/
private:
	const char* CLASS_NAME = "CModelMacro";

	QList<QVariant> m_rootData;
};

#endif