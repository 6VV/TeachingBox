#include "stdafx.h"
#include "CModeMacroFromXml.h"



CModelMacroFromXml::CModelMacroFromXml(QWidget* parent/*=0*/) :CTreeModel(parent)
{
	UpdateRootData();
}

CModelMacroFromXml::~CModelMacroFromXml()
{

}

CModelMacroFromXml* CModelMacroFromXml::GetInstance()
{
	return CSingleTon<CModelMacroFromXml>::GetInstance();
}

void CModelMacroFromXml::UpdateData(QList<QString>& childrenItems)
{
	beginResetModel();

	rootItem->ClearChildren();

	QList<QVariant> data;

	for (int i = 0; i < childrenItems.size();++i)
	{
		data << childrenItems.at(i);
		CTreeItem* childItem = new CTreeItem(data, rootItem);
		rootItem->appendChild(childItem);
		data.clear();
	}

	endResetModel();
}

void CModelMacroFromXml::UpdateRootData()
{
	if (m_rootData.isEmpty())
	{
		m_rootData << QCoreApplication::translate("CModelMacro", "Macro");
	}

	if (rootItem == NULL)
	{
		rootItem = new CTreeItem(m_rootData);
	}
	else
	{
		rootItem->SetItemData(m_rootData);
	}
}
