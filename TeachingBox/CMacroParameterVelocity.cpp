#include "stdafx.h"
#include "CMacroParameterVelocity.h"
#include "QLabel"
#include "CKeyBoard.h"

CMacroParameterVelocity::CMacroParameterVelocity(QWidget* widget/*=0*/) :CMacroParameterParent(widget)
{
	Init();
}

CMacroParameterVelocity::~CMacroParameterVelocity()
{

}

void CMacroParameterVelocity::KeyboardEdit()
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

void CMacroParameterVelocity::OnButtonConfirmClicked()
{
	///*获取参数名及参数值*/
	//std::string strVelocityName = m_newParameterName.toStdString();
	//double velocityValue;

	//QLabel* label = static_cast<QLabel*>(m_tableWidget->cellWidget(1, 1));
	//velocityValue = label->text().toDouble();

	//CInterpreterAdapter::TYPE_PAIR_DOUBLE pairVelocity(strVelocityName, velocityValue);

	///*若操作类型为添加*/
	//if (m_operType == OPER_ADD)
	//{
	//	//若已存在
	//	if (CInterpreterAdapter::GetInstance()->IsSymbolExist(strVelocityName))
	//	{
	//		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Already exist"));
	//		return;
	//	}
	//	else
	//	{
	//		CInterpreterAdapter::GetInstance()->InsertDynamicValue(CValue::SCOPE_LOCAL, pairVelocity);
	//	}
	//}
	///*若操作类型为修改*/
	//else if (m_operType == OPER_MODIFY)
	//{
	//	/*若名字未改变*/
	//	if (m_oldParameterName == m_newParameterName)
	//	{
	//		CInterpreterAdapter::GetInstance()->UpdateDynamicValue(CValue::SCOPE_LOCAL, pairVelocity);
	//	}
	//	/*若名字改变*/
	//	else
	//	{
	//		//若已存在
	//		if (CInterpreterAdapter::GetInstance()->IsSymbolExist(strVelocityName))
	//		{
	//			QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Already exist"));
	//			return;
	//		}
	//		else
	//		{
	//			CInterpreterAdapter::GetInstance()->UpdateDynamicValue(CValue::SCOPE_LOCAL, m_oldParameterName.toStdString(), pairVelocity);
	//		}
	//	}
	//}


	///*返回编辑命令界面，并设置当前变量名*/
	//CScreenEditMacroParameter::GetInstance()->UpdateParameterName(m_newParameterName);
	////CScreenMain::GetInstance()->ChangeToScreenEditMacro();
	//Back();
}

void CMacroParameterVelocity::OnButtonEditClicked()
{
	QModelIndex index = m_tableWidget->currentIndex();
	if (!index.isValid())
	{
		return;
	}

	int rowNum = index.row();
	QLabel* currentLabel = static_cast<QLabel*>(m_tableWidget->cellWidget(rowNum, 1));
	QString currentText = currentLabel->text();

	CKeyboard* keyboard = CKeyboard::GetInstance();
	keyboard->SetConfirmCommand(CKeyboard::CONFIRM_MACRO_VELOCITY_EDIT);
	keyboard->SetCurrentWidget(this);
	keyboard->SetCurrentText(currentText);
	keyboard->show();
}

void CMacroParameterVelocity::Init()
{
	InitLayout();
}

void CMacroParameterVelocity::InitLayout()
{
	//m_tableWidget->setRowCount(2);
	//m_tableWidget->setColumnCount(2);

	//QLabel* lbParameterType = new QLabel(CGrammarManagerFactory::STR_PARAMETER_TYPE_VELOCITY);
	//QLabel* lbValue = new QLabel(QCoreApplication::translate(CLASS_NAME,"value"));

	//m_lbParameterName = new QLabel;
	//m_lbParameterValue = new QLabel;

	//m_tableWidget->setCellWidget(0, 0, lbParameterType);
	//m_tableWidget->setCellWidget(1, 0, lbValue);
	//m_tableWidget->setCellWidget(0, 1, m_lbParameterName);
	//m_tableWidget->setCellWidget(1, 1, m_lbParameterValue);

	///*若类型为修改*/
	//if (m_operType==OPER_MODIFY)
	//{
	//	m_lbParameterName->setText(m_oldParameterName);
	//	CValue::TYPE_DYNAMIC velValue;
	//	CInterpreterAdapter::GetInstance()->GetDynamicValue(m_oldParameterName, velValue);
	//	//m_lbParameterValue->setText(QString::number(velValue));
	//}
	///*若类型为添加*/
	//else if (m_operType==OPER_ADD)
	//{
	//	m_lbParameterValue->setText(QString::number(0));
	//}
}

//bool CMacroParameterVelocity::IsCorrectName(QString& strName)
//{
//	if (strName.size() > STRING_MAX_LENGTH)
//	{
//		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "The length is too long"));
//		return false;
//	}
//	return true;
//}

bool CMacroParameterVelocity::IsCorrectValue(QString& strValue)
{
	bool isCorrectNum;
	double num = strValue.toDouble(&isCorrectNum);
	if (!isCorrectNum)
	{
		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Please input a number"));
	}
	else
	{
		strValue = QString::number(num);
	}
	return isCorrectNum;
}
