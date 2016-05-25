#include "stdafx.h"
#include "CMacroParameterInt.h"
#include "CGrammarManagerFactory.h"
#include "CInterpreterAdapter.h"
#include "CScreenEditMacroParameter.h"
#include "CKeyBoard.h"



CMacroParameterInt::CMacroParameterInt(QWidget* parent /*= 0*/) :CMacroParameterParent(parent)
{
	Init();
}

CMacroParameterInt::~CMacroParameterInt()
{

}

void CMacroParameterInt::KeyboardEdit()
{
	/*获取键盘当前数据*/
	QString strText = CKeyboard::GetInstance()->GetCurrentText();

	/*获取当前编辑的控件*/
	QModelIndex index = m_tableWidget->currentIndex();
	if (!index.isValid())
	{
		return;
	}
	int rowNum = index.row();

	/*若是变量名*/
	if (rowNum == 0)
	{
		/*若不是正确的变量名*/
		if (!IsCorrectName(strText))
		{
			return;
		}
	}
	else
	{
		/*若不是正确的变量值*/
		if (!IsCorrectValue(strText))
		{
			return;
		}
	}
	QLabel* currentLabel = static_cast<QLabel*>(m_tableWidget->cellWidget(rowNum, 1));

	/*设置新的数据*/
	currentLabel->setText(strText);
}

void CMacroParameterInt::OnButtonConfirmClicked()
{
	///*获取参数名及参数值*/
	//std::string strVariableName = m_newParameterName.toStdString();
	//int variableValue;

	//QLabel* label = static_cast<QLabel*>(m_tableWidget->cellWidget(1, 1));
	//variableValue = label->text().toInt();

	//CInterpreterAdapter::TYPE_PAIR_INT pairVariable(strVariableName, variableValue);

	///*若操作类型为添加*/
	//if (m_operType == OPER_ADD)
	//{
	//	//若已存在
	//	if (CInterpreterAdapter::GetInstance()->IsSymbolExist(strVariableName))
	//	{
	//		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Already exist"));
	//		return;
	//	}
	//	else
	//	{
	//		CInterpreterAdapter::GetInstance()->InsertIntValue(CValue::SCOPE_LOCAL, pairVariable);
	//	}
	//}
	///*若操作类型为修改*/
	//else if (m_operType == OPER_MODIFY)
	//{
	//	/*若名字未改变*/
	//	if (m_oldParameterName == m_newParameterName)
	//	{
	//		CInterpreterAdapter::GetInstance()->UpdateIntValue(CValue::SCOPE_LOCAL, pairVariable);
	//	}
	//	/*若名字改变*/
	//	else
	//	{
	//		//若已存在
	//		if (CInterpreterAdapter::GetInstance()->IsSymbolExist(strVariableName))
	//		{
	//			QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Already exist"));
	//			return;
	//		}
	//		else
	//		{
	//			CInterpreterAdapter::GetInstance()->UpdateIntValue(CValue::SCOPE_LOCAL, m_oldParameterName.toStdString(), pairVariable);
	//		}
	//	}
	//}

	///*返回编辑命令界面，并设置当前变量名*/
	//CScreenEditMacroParameter::GetInstance()->UpdateParameterName(m_newParameterName);
	////CScreenMain::GetInstance()->ChangeToScreenEditMacro();
	//Back();
}

void CMacroParameterInt::OnButtonEditClicked()
{
	QModelIndex index = m_tableWidget->currentIndex();
	if (!index.isValid())
	{
		return;
	}

	/*获取当前行信息*/
	int rowNum = index.row();
	QLabel* currentLabel = static_cast<QLabel*>(m_tableWidget->cellWidget(rowNum, 1));
	QString currentText = currentLabel->text();

	/*显示键盘*/
	CKeyboard* keyboard = CKeyboard::GetInstance();
	keyboard->SetConfirmCommand(CKeyboard::CONFIRM_MACRO_INT_EDIT);
	keyboard->SetCurrentWidget(this);
	keyboard->SetCurrentText(currentText);
	keyboard->show();
}

void CMacroParameterInt::Init()
{
	InitLayout();
}

void CMacroParameterInt::InitLayout()
{
	//m_tableWidget->setColumnCount(2);
	//m_tableWidget->setRowCount(2);

	//QLabel* lbType = new QLabel(CGrammarManagerFactory::STR_PARAMETER_TYPE_INT);
	//QLabel* lbValue = new QLabel(QCoreApplication::translate(CLASS_NAME, "value"));
	//m_lbParameterName = new QLabel;
	//m_lbParameterValue = new QLabel;

	//m_tableWidget->setCellWidget(0, 0, lbType);
	//m_tableWidget->setCellWidget(1, 0, lbValue);
	//m_tableWidget->setCellWidget(0, 1, m_lbParameterName);
	//m_tableWidget->setCellWidget(1, 1, m_lbParameterValue);

	///*若类型为修改*/
	//if (m_operType == OPER_MODIFY)
	//{
	//	m_lbParameterName->setText(m_oldParameterName);
	//	int velValue;
	//	CInterpreterAdapter::GetInstance()->GetIntValue(m_oldParameterName, velValue);
	//	m_lbParameterValue->setText(QString::number(velValue));
	//}
	///*若类型为添加*/
	//else if (m_operType == OPER_ADD)
	//{
	//	m_lbParameterValue->setText(QString::number(0));
	//}
}

bool CMacroParameterInt::IsCorrectValue(QString& strValue)
{
	bool isCorrectNum;
	int num = strValue.toInt(&isCorrectNum);
	if (!isCorrectNum)
	{
		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Not a interger"));
	}
	else
	{
		strValue = QString::number(num);
	}
	return isCorrectNum;
}
