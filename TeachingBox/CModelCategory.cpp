#include "stdafx.h"
#include "CModelCategory.h"
#include "QList"

CModelCategory::CModelCategory(QWidget* parent/*=0*/) :CTreeModel(parent)
{
	UpdateData();
}

CModelCategory::~CModelCategory()
{

}

CModelCategory* CModelCategory::GetInstance()
{
	return CSingleTon<CModelCategory>::GetInstance();
}

void CModelCategory::UpdateData()
{
	UpdateCategoryText();

	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	UpdateRootData();
	GetCategoryData(rootItem);

	endResetModel();
}

void CModelCategory::UpdateRootData()
{
	QList<QVariant> rootData;
	rootData << QCoreApplication::translate(CLASS_NAME, "Category");

	if (rootItem)
	{
		rootItem->SetItemData(rootData);
	}
	else
	{
		rootItem = new CTreeItem(rootData);
	}
}

void CModelCategory::UpdateCategoryText()
{
	STR_MOVEMENT = QCoreApplication::translate(CLASS_NAME, "Movement");
	STR_FUNCTIONS = QCoreApplication::translate(CLASS_NAME, "Functions");
	STR_IO = QCoreApplication::translate(CLASS_NAME, "IO");
	STR_WELD = QCoreApplication::translate(CLASS_NAME, "Weld");

	STR_SYSTEM_MATH = QCoreApplication::translate(CLASS_NAME, "Math");
	STR_SYSTEM_STRUCTURE = QCoreApplication::translate(CLASS_NAME, "Structure");
	STR_SYSTEM_PROGRAM = QCoreApplication::translate(CLASS_NAME, "Pragram");
	STR_SYSTEM_FUNCTION = QCoreApplication::translate(CLASS_NAME, "Function");
	STR_SYSTEM_OTHERS = QCoreApplication::translate(CLASS_NAME, "Others");

	STR_IO_DIGITAL = QCoreApplication::translate(CLASS_NAME, "Digital");
	STR_IO_ANALOG = QCoreApplication::translate(CLASS_NAME, "Analog");

	STR_WELD_ARC = QCoreApplication::translate(CLASS_NAME, "Arc Welding");
	STR_WELD_SPOT = QCoreApplication::translate(CLASS_NAME, "Spot Welding");
}

void CModelCategory::Init()
{

}

void CModelCategory::GetCategoryData(CTreeItem* parentItem)
{
	QList<QVariant> categoryData;

	/*添加运动节点*/
	GetChildItem(parentItem, STR_MOVEMENT);

	/*添加系统节点*/
	categoryData << STR_FUNCTIONS;
	CTreeItem* settingsItem = new CTreeItem(categoryData, parentItem);
	GetSystemData(settingsItem);
	parentItem->appendChild(settingsItem);
	categoryData.clear();

	/*获取输入输出节点*/
	categoryData << STR_IO;
	CTreeItem* ioItem = new CTreeItem(categoryData, parentItem);
	GetIOData(ioItem);
	parentItem->appendChild(ioItem);
	categoryData.clear();

	/*获取焊接节点*/
	categoryData << STR_WELD;
	CTreeItem* weldItem = new CTreeItem(categoryData, parentItem);
	GetWeldData(weldItem);
	parentItem->appendChild(weldItem);
	categoryData.clear();
}

void CModelCategory::GetSystemData(CTreeItem* parentItem)
{
	GetChildItem(parentItem, STR_SYSTEM_MATH);
	GetChildItem(parentItem, STR_SYSTEM_STRUCTURE);
	GetChildItem(parentItem, STR_SYSTEM_PROGRAM);
	GetChildItem(parentItem, STR_SYSTEM_FUNCTION);
	GetChildItem(parentItem, STR_SYSTEM_OTHERS);
}

void CModelCategory::GetIOData(CTreeItem* parentItem)
{

	GetChildItem(parentItem, STR_IO_DIGITAL);
	GetChildItem(parentItem, STR_IO_ANALOG);
}

void CModelCategory::GetWeldData(CTreeItem* parentItem)
{
	GetChildItem(parentItem, STR_WELD_ARC);
	GetChildItem(parentItem, STR_WELD_SPOT);
}

void CModelCategory::GetChildItem(CTreeItem* parentItem, QString childName)
{
	QList<QVariant> childData;
	childData << childName;
	CTreeItem* childItem = new CTreeItem(childData, parentItem);
	parentItem->appendChild(childItem);
}
