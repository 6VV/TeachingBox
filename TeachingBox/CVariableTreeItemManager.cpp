#include "stdafx.h"
#include "CVariableTreeItemManager.h"
#include "CLineEditWithClickedSignal.h"
#include "CRegExpManager.h"
#include "CValue.h"
#include "CParameterManager.h"
#include "CComboBoxWithTreeItem.h"
#include "CTreeWidgetWithMonitor.h"
#include "DataStruct.h"
#include "TVariateManager.h"
#include "TInteger.h"
#include "CTreeWidgetItemWithVariate.h"


CVariableTreeItemManager::CVariableTreeItemManager()
{
}

CVariableTreeItemManager::~CVariableTreeItemManager()
{

}


void CVariableTreeItemManager::GetVariableTreeWidgetItem(const QString& strName, const QString& strType, const QString& strScope,
	QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	TVariateManager::GetInstance()->GetVariate(strScope, strName)->ReadTreeWidgetItem(item, treeWidget);

	//if (strType==CParameterManager::STR_TYPE_INT)
	//{
	//	TVariateManager::GetInstance()->GetVariate(strScope, strName)->ReadTreeWidgetItem(item, treeWidget);
	//}
	//else if (strType==CParameterManager::STR_TYPE_DOUBLE)
	//{
	//	TVariateManager::GetInstance()->GetVariate(strScope, strName)->ReadTreeWidgetItem(item, treeWidget);
	//}
	//else if (strType==CParameterManager::STR_TYPE_BOOL)
	//{
	//	GetBoolTreeWidgetItem(strScope, strName, item, treeWidget);
	//}
	//else if (strType==CParameterManager::STR_TYPE_STRING)
	//{
	//	GetStringTreeWidgetItem(strScope, strName, item, treeWidget);
	//}
	//else if (strType==CParameterManager::STR_TYPE_POSITION)
	//{
	//	GetPositionTreeWidgetItem(strScope, strName, item, treeWidget);
	//}
	//else if (strType==CParameterManager::STR_TYPE_DYNAMIC)
	//{
	//	GetDynamicTreeWidgetItem(strScope, strName, item, treeWidget);
	//}
	//else if (strType==CParameterManager::STR_TYPE_OVERLAP)
	//{
	//	GetOverlapTreeWidgetItem(strScope, strName, item, treeWidget);
	//}
}

