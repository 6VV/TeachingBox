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


void CVariableTreeItemManager::GetDynamicTreeWidgetItem(const QString& strName, const CValue::TYPE_DYNAMIC& dynamic,
	QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	item->setText(0, strName);
	item->setText(1, CParameterManager::STR_TYPE_DYNAMIC);

	QTreeWidgetItem* itemVelocity = new QTreeWidgetItem(item, QStringList("Velocity"));
	QTreeWidgetItem* itemAcceleration = new QTreeWidgetItem(item, QStringList("Acceleration"));
	QTreeWidgetItem* itemDeceleration = new QTreeWidgetItem(item, QStringList("Deceleration"));
	QTreeWidgetItem* itemPostureVelocity = new QTreeWidgetItem(item, QStringList("PostureVelocity"));
	QTreeWidgetItem* itemPostureAcceleration = new QTreeWidgetItem(item, QStringList("PostureAcceleration"));
	QTreeWidgetItem* itemPostureDeceleration = new QTreeWidgetItem(item, QStringList("PostureDeceleration"));

	CLineEditWithClickedSignal* lineEditVelocity = new CLineEditWithClickedSignal(QString::number(dynamic.m_Velocity), CRegExpManager::STR_REG_FLOAT);
	CLineEditWithClickedSignal* lineEditAcceleration = new CLineEditWithClickedSignal(QString::number(dynamic.m_Acceleration), CRegExpManager::STR_REG_FLOAT);
	CLineEditWithClickedSignal* lineEditDeceleration = new CLineEditWithClickedSignal(QString::number(dynamic.m_Deceleration), CRegExpManager::STR_REG_FLOAT);
	CLineEditWithClickedSignal* lineEditPostureVelocity = new CLineEditWithClickedSignal(QString::number(dynamic.m_PostureVelocity), CRegExpManager::STR_REG_FLOAT);
	CLineEditWithClickedSignal* lineEditPostureAcceleration = new CLineEditWithClickedSignal(QString::number(dynamic.m_PostureAcceleration), CRegExpManager::STR_REG_FLOAT);
	CLineEditWithClickedSignal* lineEditPostureDeceleration = new CLineEditWithClickedSignal(QString::number(dynamic.m_PostureDeceleration), CRegExpManager::STR_REG_FLOAT);

	treeWidget->setItemWidget(itemVelocity, 1, lineEditVelocity);
	treeWidget->setItemWidget(itemAcceleration, 1, lineEditAcceleration);
	treeWidget->setItemWidget(itemDeceleration, 1, lineEditDeceleration);
	treeWidget->setItemWidget(itemPostureVelocity, 1, lineEditPostureVelocity);
	treeWidget->setItemWidget(itemPostureAcceleration, 1, lineEditPostureAcceleration);
	treeWidget->setItemWidget(itemPostureDeceleration, 1, lineEditPostureDeceleration);

	/*添加信号槽*/
	lineEditVelocity->SetTreeWidgetItem(itemVelocity, treeWidget);
	lineEditAcceleration->SetTreeWidgetItem(itemAcceleration, treeWidget);
	lineEditDeceleration->SetTreeWidgetItem(itemDeceleration, treeWidget);
	lineEditPostureVelocity->SetTreeWidgetItem(itemPostureVelocity, treeWidget);
	lineEditPostureAcceleration->SetTreeWidgetItem(itemPostureAcceleration, treeWidget);
	lineEditPostureDeceleration->SetTreeWidgetItem(itemPostureDeceleration, treeWidget);
}

void CVariableTreeItemManager::GetDynamicTreeWidgetItem(const QString& strScope, const QString& strName, QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	GetDynamicTreeWidgetItem(strName, CValue::GetInstance()->m_mapScopeDynamic[strScope.toStdString()][strName.toStdString()], item, treeWidget);
}

void CVariableTreeItemManager::GetAllDynamicTreeWidgetItemInExactScope(const QString& strScope, QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget)
{
	for each (auto var in CValue::GetInstance()->m_mapScopeDynamic[strScope.toStdString()])
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
		item->setText(2, strScope);
		GetDynamicTreeWidgetItem(QString::fromStdString(var.first), var.second, item, treeWidget);
	}
}
