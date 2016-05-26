#include "stdafx.h"
#include "CMacroParameterLabel.h"
#include "CKeyBoard.h"
#include "CGrammarManagerFactory.h"



CMacroParameterLabel::CMacroParameterLabel(QWidget* parent/*=0*/) :CMacroParameterParent(parent)
{
	Init();
}

CMacroParameterLabel::~CMacroParameterLabel()
{

}

void CMacroParameterLabel::KeyboardEdit()
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

	/*若不是正确的变量名*/
	if (!IsCorrectName(strText))
	{
		return;
	}

	QLabel* currentLabel = static_cast<QLabel*>(m_tableWidget->cellWidget(rowNum, 1));

	/*设置新的数据*/
	currentLabel->setText(strText);
}

void CMacroParameterLabel::OnButtonConfirmClicked()
{
	///*获取参数名及参数值*/
	//std::string strVariableName = m_newParameterName.toStdString();

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
	//		//CInterpreterAdapter::GetInstance()->InsertLabelValue(CValue::SCOPE_LOCAL, strVariableName);
	//	}
	//}
	///*若操作类型为修改*/
	//else if (m_operType == OPER_MODIFY)
	//{
	//	/*若名字改变*/
	//	if (m_oldParameterName != m_newParameterName)
	//	{
	//		//若已存在
	//		if (CInterpreterAdapter::GetInstance()->IsSymbolExist(strVariableName))
	//		{
	//			QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Already exist"));
	//			return;
	//		}
	//		else
	//		{
	//			//CInterpreterAdapter::GetInstance()->UpdateLabelValue(CValue::SCOPE_LOCAL, m_oldParameterName.toStdString(), strVariableName);
	//		}
	//	}
	//}

	///*返回编辑命令界面，并设置当前变量名*/
	//CScreenEditMacroParameter::GetInstance()->UpdateParameterName(m_newParameterName);
	///*CScreenMain::GetInstance()->ChangeToScreenEditMacro();*/
	//Back();
}

void CMacroParameterLabel::OnButtonEditClicked()
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
	keyboard->SetConfirmCommand(CKeyboard::CONFIRM_MACRO_LABEL_EDIT);
	keyboard->SetCurrentWidget(this);
	keyboard->SetCurrentText(currentText);
	keyboard->show();
}

void CMacroParameterLabel::Init()
{
	InitLayout();
}

void CMacroParameterLabel::InitLayout()
{
	m_tableWidget->setColumnCount(2);
	m_tableWidget->setRowCount(1);

	QLabel* lbType = new QLabel(CGrammarManagerFactory::STR_PARAMETER_TYPE_LABEL);
	m_lbParameterName = new QLabel;

	m_tableWidget->setCellWidget(0, 0, lbType);
	m_tableWidget->setCellWidget(0, 1, m_lbParameterName);

	/*若类型为修改*/
	if (m_operType == OPER_MODIFY)
	{
		m_lbParameterName->setText(m_oldParameterName);
	}
}

//bool CMacroParameterLabel::IsCorrectName(QString& strName)
//{
//	if (strName.size() > STRING_MAX_LENGTH)
//	{
//		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "The length is too long"));
//		return false;
//	}
//	return true;
//}
