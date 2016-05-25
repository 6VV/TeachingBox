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

void CVariableTreeItemManager::GetPositionTreeWidgetItem(const QString& strName, const CValue::TYPE_POSITION& position,
	QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget)
{
	parentItem->setText(0, strName);
	parentItem->setText(1, CParameterManager::STR_TYPE_POSITION);

	QTreeWidgetItem *itemAxis1 = new QTreeWidgetItem(parentItem, QStringList("Axis1"));
	QTreeWidgetItem *itemAxis2 = new QTreeWidgetItem(parentItem, QStringList("Axis2"));
	QTreeWidgetItem *itemAxis3 = new QTreeWidgetItem(parentItem, QStringList("Axis3"));
	QTreeWidgetItem *itemAxis4 = new QTreeWidgetItem(parentItem, QStringList("Axis4"));
	QTreeWidgetItem *itemAxis5 = new QTreeWidgetItem(parentItem, QStringList("Axis5"));
	QTreeWidgetItem *itemAxis6 = new QTreeWidgetItem(parentItem, QStringList("Axis6"));

	CLineEditWithClickedSignal* lineEditAxis1 = new CLineEditWithClickedSignal(QString::number(position.m_AxisPosition[0]), CRegExpManager::STR_REG_FLOAT);
	CLineEditWithClickedSignal* lineEditAxis2 = new CLineEditWithClickedSignal(QString::number(position.m_AxisPosition[1]), CRegExpManager::STR_REG_FLOAT);
	CLineEditWithClickedSignal* lineEditAxis3 = new CLineEditWithClickedSignal(QString::number(position.m_AxisPosition[2]), CRegExpManager::STR_REG_FLOAT);
	CLineEditWithClickedSignal* lineEditAxis4 = new CLineEditWithClickedSignal(QString::number(position.m_AxisPosition[3]), CRegExpManager::STR_REG_FLOAT);
	CLineEditWithClickedSignal* lineEditAxis5 = new CLineEditWithClickedSignal(QString::number(position.m_AxisPosition[4]), CRegExpManager::STR_REG_FLOAT);
	CLineEditWithClickedSignal* lineEditAxis6 = new CLineEditWithClickedSignal(QString::number(position.m_AxisPosition[5]), CRegExpManager::STR_REG_FLOAT);

	//lineEditAxis1->setAlignment(Qt::AlignHCenter);
	//lineEditAxis2->setAlignment(Qt::AlignHCenter);
	//lineEditAxis3->setAlignment(Qt::AlignHCenter);
	//lineEditAxis4->setAlignment(Qt::AlignHCenter);
	//lineEditAxis5->setAlignment(Qt::AlignHCenter);
	//lineEditAxis6->setAlignment(Qt::AlignHCenter);

	treeWidget->setItemWidget(itemAxis1, 1, lineEditAxis1);
	treeWidget->setItemWidget(itemAxis2, 1, lineEditAxis2);
	treeWidget->setItemWidget(itemAxis3, 1, lineEditAxis3);
	treeWidget->setItemWidget(itemAxis4, 1, lineEditAxis4);
	treeWidget->setItemWidget(itemAxis5, 1, lineEditAxis5);
	treeWidget->setItemWidget(itemAxis6, 1, lineEditAxis6);

	/*信号槽*/
	lineEditAxis1->SetTreeWidgetItem(itemAxis1,treeWidget);
	lineEditAxis2->SetTreeWidgetItem(itemAxis2, treeWidget);
	lineEditAxis3->SetTreeWidgetItem(itemAxis3, treeWidget);
	lineEditAxis4->SetTreeWidgetItem(itemAxis4, treeWidget);
	lineEditAxis5->SetTreeWidgetItem(itemAxis5, treeWidget);
	lineEditAxis6->SetTreeWidgetItem(itemAxis6, treeWidget);
}


void CVariableTreeItemManager::GetPositionTreeWidgetItem(const QString& strScope, const QString& strName, QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	GetPositionTreeWidgetItem(strName, CValue::GetInstance()->m_mapScopePosition[strScope.toStdString()][strName.toStdString()], item, treeWidget);
}

void CVariableTreeItemManager::GetAllPositionTreeWidgetItemInExactScope(const QString& strScope, QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget)
{
	for each (auto var in CValue::GetInstance()->m_mapScopePosition[strScope.toStdString()])
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
		item->setText(2, strScope);
		GetPositionTreeWidgetItem(QString::fromStdString(var.first), var.second, item, treeWidget);
	}
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

void CVariableTreeItemManager::GetOverlapTreeWidgetItem(const QString& strName, const CValue::TYPE_OVERLAP& overlap, 
	QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	item->setText(0, strName);
	item->setText(1, CParameterManager::STR_TYPE_OVERLAP);

	CComboBoxWithTreeItem* comboBoxTransitionMode = new CComboBoxWithTreeItem;
	comboBoxTransitionMode->addItem(CParameterManager::STR_TRANSITION_MODE_ABSOLUTE);
	comboBoxTransitionMode->addItem(CParameterManager::STR_TRANSITION_MODE_RELATIVE);

	QTreeWidgetItem* itemMode = new QTreeWidgetItem(item, QStringList("Mode"));
	QTreeWidgetItem* itemValue = new QTreeWidgetItem(item, QStringList("Parameter"));

	if (overlap.m_TransitionMode==tTransitionMode::kTransitionModeAbsolute)
	{
		comboBoxTransitionMode->setCurrentText(CParameterManager::STR_TRANSITION_MODE_ABSOLUTE);
	}
	else
	{
		comboBoxTransitionMode->setCurrentText(CParameterManager::STR_TRANSITION_MODE_RELATIVE);
	}

	CLineEditWithClickedSignal* lineEditTransitionParamter = new CLineEditWithClickedSignal(QString::number(overlap.m_TransitionParameter), CRegExpManager::STR_REG_FLOAT);

	treeWidget->setItemWidget(itemMode, 1, comboBoxTransitionMode);
	treeWidget->setItemWidget(itemValue, 1, lineEditTransitionParamter);

	/*添加信号槽*/
	lineEditTransitionParamter->SetTreeWidgetItem(itemValue, treeWidget);
	comboBoxTransitionMode->SetTreeWidgetItem(itemMode, treeWidget);
}

void CVariableTreeItemManager::GetOverlapTreeWidgetItem(const QString& strScope, const QString& strName, QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	GetOverlapTreeWidgetItem(strName, CValue::GetInstance()->m_mapScopeOverlap[strScope.toStdString()][strName.toStdString()], item, treeWidget);
}

void CVariableTreeItemManager::GetAllOverlapTreeWidgetItemInExactScope(const QString& strScope, QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget)
{
	for each (auto var in CValue::GetInstance()->m_mapScopeOverlap[strScope.toStdString()])
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
		item->setText(2, strScope);
		GetOverlapTreeWidgetItem(QString::fromStdString(var.first), var.second, item, treeWidget);
	}
}

void CVariableTreeItemManager::GetBoolTreeWidgetItem(const QString& strName, const int value, QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	item->setText(0, strName);
	item->setText(1, CParameterManager::STR_TYPE_BOOL);

	QTreeWidgetItem* itemBool = new QTreeWidgetItem(item, QStringList("Value"));

	CComboBoxWithTreeItem* comboValue = new CComboBoxWithTreeItem(itemBool);
	comboValue->addItem(CParameterManager::STR_BOOL_FALSE);
	comboValue->addItem(CParameterManager::STR_BOOL_TRUE);

	if (value)
	{
		comboValue->setCurrentText(CParameterManager::STR_BOOL_TRUE);
	}
	else
	{
		comboValue->setCurrentText(CParameterManager::STR_BOOL_FALSE);
	}

	treeWidget->setItemWidget(itemBool, 1, comboValue);

	/*设置信号槽*/
	comboValue->SetTreeWidgetItem(itemBool, treeWidget);
}

void CVariableTreeItemManager::GetBoolTreeWidgetItem(const QString& strScope, const QString& strName, QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	GetBoolTreeWidgetItem(strName, CValue::GetInstance()->m_mapScopeBool[strScope.toStdString()][strName.toStdString()], item, treeWidget);
}

void CVariableTreeItemManager::GetAllBoolTreeWidgetItemInExactScope(const QString& strScope, QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget)
{
	for each (auto var in CValue::GetInstance()->m_mapScopeBool[strScope.toStdString()])
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
		item->setText(2, strScope);
		GetBoolTreeWidgetItem(QString::fromStdString(var.first), var.second, item, treeWidget);
	}
}

void CVariableTreeItemManager::GetStringTreeWidgetItem(const QString& strName, const std::string& value, QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	item->setText(0, strName);
	item->setText(1, CParameterManager::STR_TYPE_STRING);

	QTreeWidgetItem* itemString = new QTreeWidgetItem(item, QStringList("Value"));

	CLineEditWithClickedSignal* lineEditValue = new CLineEditWithClickedSignal(QString::fromStdString(value));

	treeWidget->setItemWidget(itemString, 1, lineEditValue);

	/*添加信号槽*/
	lineEditValue->SetTreeWidgetItem(itemString, treeWidget);
}

void CVariableTreeItemManager::GetStringTreeWidgetItem(const QString& strScope, const QString& strName, 
	QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	GetStringTreeWidgetItem(strName, CValue::GetInstance()->m_mapScopeString[strScope.toStdString()][strName.toStdString()], 
		item, treeWidget);

}

void CVariableTreeItemManager::GetAllStringTreeWidgetItemInExactScope(const QString& strScope, QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget)
{
	for each (auto var in CValue::GetInstance()->m_mapScopeString[strScope.toStdString()])
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
		item->setText(2, strScope);
		GetStringTreeWidgetItem(QString::fromStdString(var.first), var.second, item, treeWidget);
	}
}
