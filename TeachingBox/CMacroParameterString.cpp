#include "stdafx.h"
#include "CMacroParameterString.h"
#include "CGrammarManagerFactory.h"
#include "CInterpreterAdapter.h"
#include "CKeyBoard.h"



CMacroParameterString::CMacroParameterString(QWidget* parent /*= 0*/)
{
	Init();
}

CMacroParameterString::~CMacroParameterString()
{

}

void CMacroParameterString::KeyboardEdit()
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

	QLabel* currentLabel = static_cast<QLabel*>(m_tableWidget->cellWidget(rowNum, 1));

	/*设置新的数据*/
	currentLabel->setText(strText);
}

void CMacroParameterString::OnButtonConfirmClicked()
{

}

void CMacroParameterString::OnButtonEditClicked()
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
	keyboard->SetConfirmCommand(CKeyboard::CONFIRM_MACRO_STRING_EDIT);
	keyboard->SetCurrentWidget(this);
	keyboard->SetCurrentText(currentText);
	keyboard->show();
}

void CMacroParameterString::Init()
{
	InitLayout();
}

void CMacroParameterString::InitLayout()
{
	//m_tableWidget->setColumnCount(2);
	//m_tableWidget->setRowCount(2);

	//QLabel* lbType = new QLabel(CGrammarManagerFactory::STR_PARAMETER_TYPE_FLOAT);
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
	//	std::string velValue;
	//	CInterpreterAdapter::GetInstance()->GetStringValue(m_oldParameterName, velValue);
	//	m_lbParameterValue->setText(QString::fromStdString(velValue));
	//}
	///*若类型为添加*/
	//else if (m_operType == OPER_ADD)
	//{
	//	m_lbParameterValue->setText("");
	//}
}
