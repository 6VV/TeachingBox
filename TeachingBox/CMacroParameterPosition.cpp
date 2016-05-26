#include "stdafx.h"
#include "CMacroParameterPosition.h"
#include "CKeyBoard.h"


CMacroParameterPosition::CMacroParameterPosition(QWidget* parent /*= 0*/) :CMacroParameterParent(parent)
{
	Init();
}

CMacroParameterPosition::~CMacroParameterPosition()
{
	qDebug() << "delete";
}

/*键盘确认按钮按下事件*/
void CMacroParameterPosition::KeyboardEdit()
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
	if (rowNum==0)
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

void CMacroParameterPosition::OnButtonConfirmClicked()
{
	///*获取参数名及参数值*/
	//std::string strPositionName = m_newParameterName.toStdString();
	//std::vector<double> vecPosition;

	//for (int i = 1; i <= 6;i++)
	//{
	//	QLabel* label = static_cast<QLabel*>(m_tableWidget->cellWidget(i, 1));
	//	vecPosition.push_back(label->text().toDouble());
	//}

	//CInterpreterAdapter::TYPE_PAIR_POSITION pairPosition(strPositionName, vecPosition);

	///*若操作类型为添加*/
	//if (m_operType==OPER_ADD)
	//{
	//	//若已存在
	//	if (CInterpreterAdapter::GetInstance()->IsSymbolExist(strPositionName))
	//	{
	//		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Already exist"));
	//		return;
	//	}
	//	else
	//	{
	//		CInterpreterAdapter::GetInstance()->InsertPositionValue(CValue::SCOPE_LOCAL, pairPosition);
	//	}
	//}
	///*若操作类型为修改*/
	//else if (m_operType==OPER_MODIFY)
	//{
	//	/*若名字未改变*/
	//	if (m_oldParameterName==m_newParameterName)
	//	{
	//		CInterpreterAdapter::GetInstance()->UpdatePositionValue(CValue::SCOPE_LOCAL, pairPosition);
	//	}
	//	/*若名字改变*/
	//	else
	//	{
	//		//若已存在
	//		if (CInterpreterAdapter::GetInstance()->IsSymbolExist(strPositionName))
	//		{
	//			QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Already exist"));
	//			return;
	//		}
	//		else
	//		{
	//			CInterpreterAdapter::GetInstance()->UpdatePositionValue(CValue::SCOPE_LOCAL, m_oldParameterName.toStdString(), pairPosition);
	//		}
	//	}
	//}

	///*返回编辑命令界面，并设置当前变量名*/
	//CScreenEditMacroParameter::GetInstance()->UpdateParameterName(m_newParameterName);
	////CScreenMain::GetInstance()->ChangeToScreenEditMacro();
	//Back();
}

void CMacroParameterPosition::OnButtonEditClicked()
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
	keyboard->SetConfirmCommand(CKeyboard::CONFIRM_MACRO_POSITION_EIDT);
	keyboard->SetCurrentWidget(this);
	keyboard->SetCurrentText(currentText);
	keyboard->show();
}


void CMacroParameterPosition::Init()
{
	InitLayout();
}

void CMacroParameterPosition::InitLayout()
{
	///*表格*/
	////m_tableWidget = new QTableWidget(7,2);
	//m_tableWidget->setRowCount(7);
	//m_tableWidget->setColumnCount(2);

	//QLabel* lbParameterType = new QLabel(CGrammarManagerFactory::STR_PARAMETER_TYPE_POSITON);
	//QLabel* lbA1 = new QLabel(QCoreApplication::translate(CLASS_NAME, "A1"));
	//QLabel* lbA2 = new QLabel(QCoreApplication::translate(CLASS_NAME, "A2"));
	//QLabel* lbA3 = new QLabel(QCoreApplication::translate(CLASS_NAME, "A3"));
	//QLabel* lbA4 = new QLabel(QCoreApplication::translate(CLASS_NAME, "A4"));
	//QLabel* lbA5 = new QLabel(QCoreApplication::translate(CLASS_NAME, "A5"));
	//QLabel* lbA6 = new QLabel(QCoreApplication::translate(CLASS_NAME, "A6"));

	//m_lbPositionName = new QLabel;
	//m_lbValueA1 = new QLabel;
	//m_lbValueA2 = new QLabel;
	//m_lbValueA3 = new QLabel;
	//m_lbValueA4 = new QLabel;
	//m_lbValueA5 = new QLabel;
	//m_lbValueA6 = new QLabel;

	//m_tableWidget->setCellWidget(0, 0, lbParameterType);
	//m_tableWidget->setCellWidget(1, 0, lbA1);
	//m_tableWidget->setCellWidget(2, 0, lbA2);
	//m_tableWidget->setCellWidget(3, 0, lbA3);
	//m_tableWidget->setCellWidget(4, 0, lbA4);
	//m_tableWidget->setCellWidget(5, 0, lbA5);
	//m_tableWidget->setCellWidget(6, 0, lbA6);
	//
	//m_tableWidget->setCellWidget(0, 1, m_lbPositionName);
	//m_tableWidget->setCellWidget(1, 1, m_lbValueA1);
	//m_tableWidget->setCellWidget(2, 1, m_lbValueA2);
	//m_tableWidget->setCellWidget(3, 1, m_lbValueA3);
	//m_tableWidget->setCellWidget(4, 1, m_lbValueA4);
	//m_tableWidget->setCellWidget(5, 1, m_lbValueA5);
	//m_tableWidget->setCellWidget(6, 1, m_lbValueA6);

	///*若类型为修改*/
	//if (m_operType==OPER_MODIFY)
	//{
	//	std::vector<double> pairPosition;
	//	CInterpreterAdapter::GetInstance()->GetPositionValue(m_oldParameterName, pairPosition);

	//	m_lbPositionName->setText(m_oldParameterName);
	//	m_lbValueA1->setText(QString::number(pairPosition.at(0)));
	//	m_lbValueA2->setText(QString::number(pairPosition.at(1)));
	//	m_lbValueA3->setText(QString::number(pairPosition.at(2)));
	//	m_lbValueA4->setText(QString::number(pairPosition.at(3)));
	//	m_lbValueA5->setText(QString::number(pairPosition.at(4)));
	//	m_lbValueA6->setText(QString::number(pairPosition.at(5)));
	//}
	///*若类型为添加*/
	//else if (m_operType==OPER_ADD)
	//{
	//	m_lbValueA1->setText("0");
	//	m_lbValueA2->setText("0");
	//	m_lbValueA3->setText("0");
	//	m_lbValueA4->setText("0");
	//	m_lbValueA5->setText("0");
	//	m_lbValueA6->setText("0");
	//}
}

//bool CMacroParameterPosition::IsCorrectName(QString& strName)
//{
//	if (strName.size()>STRING_MAX_LENGTH)
//	{
//		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "The length is too long"));
//		return false;
//	}
//	return true;
//}

bool CMacroParameterPosition::IsCorrectValue(QString& strValue)
{
	bool isCorrectNum;
	double num=strValue.toDouble(&isCorrectNum);
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
