#include "stdafx.h"
#include "CTreeWidgetWithMonitor.h"
#include "CInterpreterAdapter.h"
#include "CParameterManager.h"
#include "TVariateManager.h"
#include "TInteger.h"
#include "CLineEditWithClickedSignal.h"
#include "TDouble.h"
#include "TBool.h"
#include "TString.h"



CTreeWidgetWithMonitor::CTreeWidgetWithMonitor(QWidget* parent/*=0*/) :QTreeWidget(parent)
{

}

CTreeWidgetWithMonitor::~CTreeWidgetWithMonitor()
{

}

void CTreeWidgetWithMonitor::SlotItemTextChanged(QTreeWidgetItem* item)
{
	QTreeWidgetItem* itemParent = item->parent();

	/*名字、类型、作用域*/
	QString varName = itemParent->text(0);
	QString varType = itemParent->text(1);
	QString varScope = itemParent->text(2);

	CInterpreterAdapter* interpreterAdapter = CInterpreterAdapter::GetInstance();
	CValue* valueManager = CValue::GetInstance();

	///*若为整数*/
	//if (varType==CParameterManager::STR_TYPE_INT)
	//{
	//	QLineEdit* lineEdit = static_cast<QLineEdit*>(item->treeWidget()->itemWidget(item, 1));
	//	int value = lineEdit->text().toInt();

	//	TVariateManager::GetInstance()->Update(varScope, varName,
	//		new TInteger(varScope, varName, value));
	//}
	/*若为浮点数*/
	if (varType==CParameterManager::STR_TYPE_DOUBLE)
	{
		QLineEdit* lineEdit = static_cast<QLineEdit*>(item->treeWidget()->itemWidget(item, 1));
		int value = lineEdit->text().toDouble();

		TVariateManager::GetInstance()->Update(varScope, varName,
			TDouble(varScope, varName, value));

		//interpreterAdapter->UpdateDoubleValue(varScope, varName.toStdString(), varName.toStdString(), value);
	}
	/*若为布尔值*/
	else if (varType==CParameterManager::STR_TYPE_BOOL)
	{
		QComboBox* comboBox = static_cast<QComboBox*>(item->treeWidget()->itemWidget(item, 1));

		bool value;
		if (comboBox->currentText()== CParameterManager::STR_BOOL_TRUE)
		{
			value = true;
		}
		else
		{
			value = false;
		}
		TVariateManager::GetInstance()->Update(varScope,varName,TBool(varScope, varName, value));
		//interpreterAdapter->UpdateBoolValue(varScope, varName.toStdString(), varName.toStdString(), value);
	}
	/*若为字符串*/
	else if (varType==CParameterManager::STR_TYPE_STRING)
	{
		QLineEdit* lineEdit = static_cast<QLineEdit*>(item->treeWidget()->itemWidget(item, 1));

		TVariateManager::GetInstance()->Update(varScope, varName, TString(varScope, varName, lineEdit->text()));

		//interpreterAdapter->UpdateStringValue(varScope, varName.toStdString(), varName.toStdString(), lineEdit->text().toStdString());
	}
	/*若为位置变量*/
	else if (varType==CParameterManager::STR_TYPE_POSITION)
	{
		CValue::TYPE_POSITION position;
		for (int i = 0; i < itemParent->childCount();++i)
		{
			CLineEditWithClickedSignal* lineEdit = static_cast<CLineEditWithClickedSignal*>(item->treeWidget()->itemWidget(itemParent->child(i), 1));

			position.m_AxisPosition[i]=lineEdit->text().toDouble();
		}
		interpreterAdapter->UpdatePositionValue(varScope, varName.toStdString(), varName.toStdString(), position);

	}
	/*若为动态参数*/
	else if (varType==CParameterManager::STR_TYPE_DYNAMIC)
	{
		CValue::TYPE_DYNAMIC dynamic;

		QLineEdit* lineEdit1 = static_cast<QLineEdit*>(item->treeWidget()->itemWidget(itemParent->child(0), 1));
		QLineEdit* lineEdit2 = static_cast<QLineEdit*>(item->treeWidget()->itemWidget(itemParent->child(1), 1));
		QLineEdit* lineEdit3 = static_cast<QLineEdit*>(item->treeWidget()->itemWidget(itemParent->child(2), 1));
		QLineEdit* lineEdit4 = static_cast<QLineEdit*>(item->treeWidget()->itemWidget(itemParent->child(3), 1));
		QLineEdit* lineEdit5 = static_cast<QLineEdit*>(item->treeWidget()->itemWidget(itemParent->child(4), 1));
		QLineEdit* lineEdit6 = static_cast<QLineEdit*>(item->treeWidget()->itemWidget(itemParent->child(5), 1));

		dynamic.m_Velocity = lineEdit1->text().toDouble();
		dynamic.m_Acceleration = lineEdit2->text().toDouble();
		dynamic.m_Deceleration = lineEdit3->text().toDouble();
		dynamic.m_PostureVelocity = lineEdit4->text().toDouble();
		dynamic.m_PostureAcceleration = lineEdit5->text().toDouble();
		dynamic.m_PostureDeceleration = lineEdit6->text().toDouble();

		interpreterAdapter->UpdateDynamicValue(varScope, varName.toStdString(), varName.toStdString(), dynamic);

	}
	/*若为过渡参数*/
	else if (varType==CParameterManager::STR_TYPE_OVERLAP)
	{
		CValue::TYPE_OVERLAP overlap;

		QComboBox* comboBox = static_cast<QComboBox*>(item->treeWidget()->itemWidget(itemParent->child(0), 1));

		if (comboBox->currentText() == CParameterManager::STR_TRANSITION_MODE_ABSOLUTE)
		{
			overlap.m_TransitionMode = kTransitionModeAbsolute;
		}
		else
		{
			overlap.m_TransitionMode = kTransitionModeRelative;
		}

		QLineEdit* lineEdit = static_cast<QLineEdit*>(item->treeWidget()->itemWidget(itemParent->child(1), 1));
		overlap.m_TransitionParameter = lineEdit->text().toDouble();

		interpreterAdapter->UpdateOverlapValue(varScope, varName.toStdString(), varName.toStdString(), overlap);
	}
}
