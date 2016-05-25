#include "stdafx.h"
#include "CModelCategoryFromXml.h"


CModelCategoryFromXml::CModelCategoryFromXml(QWidget* parent/*=0*/) :CTreeModel(parent)
{
	UpdateRootData();
}

CModelCategoryFromXml::~CModelCategoryFromXml()
{

}

CModelCategoryFromXml* CModelCategoryFromXml::GetInstance()
{
	return CSingleTon<CModelCategoryFromXml>::GetInstance();
}

void CModelCategoryFromXml::UpdateRootData()
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

CTreeItem* CModelCategoryFromXml::GetRootItem()
{
	return rootItem;
}
