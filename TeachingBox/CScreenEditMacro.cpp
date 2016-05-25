#include "stdafx.h"
#include "CScreenEditMacro.h"
#include "CGrammarManagerFactory.h"
#include "CInterpreterAdapter.h"
#include "CScreenMain.h"
#include "CWidgetTest.h"
#include "CMacroParameterPosition.h"
#include "CMacroParameterParent.h"
#include "CKeyBoard.h"
#include "CInterpreterManager.h"
#include "CTokenCategory.h"
#include "CValue.h"

CScreenEditMacro::CScreenEditMacro(QWidget* parent/*=0*/) :CScreenMainParent(parent)
{
	Init();
}

CScreenEditMacro::~CScreenEditMacro()
{
	qDebug() << "CScreenEditMacro delete";
}

CScreenEditMacro* CScreenEditMacro::GetInstance()
{
	return CSingleTon<CScreenEditMacro>::GetInstance();
}

/*切换显示内容*/
void CScreenEditMacro::ChangeContent(QString& strMacroName, QList<QString>& listParameterName)
{
	/*清除原内容，释放内存*/
	m_tableWidget->clearContents();
	m_strMacroName.clear();
	m_strListParameterName.clear();
	m_strListParameterType.clear();

	/*命令相关*/
	m_strMacroName = strMacroName;
	m_strListParameterName = listParameterName;
	//m_strListParameterType = listParameterType;

	/*行数*/
	m_tableWidget->setRowCount(listParameterName.size()+1);
	m_tableWidget->setSpan(0, 0, 1, 2);

	if (strMacroName==CGrammarManagerFactory::STR_MACRO_MOVL)
	{
		InitMovlWidget();
	}
	else if (strMacroName==CGrammarManagerFactory::STR_MACRO_MOVC)
	{
		InitMovcWidget();
	}
	else if (strMacroName == CGrammarManagerFactory::STR_MACRO_FOR)
	{
		InitForWidget();
	}
	else if (strMacroName==CGrammarManagerFactory::STR_MACRO_SWITCH)
	{
		InitSwitchWidget();
	}
}

/*************************************************
//  Function: 		SetNewParameterName
//  Description: 	设置新的参数名
//  Calls:		 	用于获取各种控件的函数
//  Called By: 		CMacroParameterParent
//  Parameter:      QString&
//  Return: 		void
//  Others: 		在参数设置成功后，重新切换到此类时调用此函数
*************************************************/
void CScreenEditMacro::UpdateParameterName(QString& strNewParameterName)
{
	/*获取当前行*/
	int rowNum = m_tableWidget->currentIndex().row();
	QString strParameterType = m_strListParameterType.at(rowNum - 1);

	/*若参数为位置变量*/
	if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_POSITON)
	{
		QComboBox* comboBox = static_cast<QComboBox*>(GetPositionValueWidget());
		comboBox->setCurrentText(strNewParameterName);
		m_tableWidget->setCellWidget(rowNum, 1, comboBox);
	}
	/*若参数为速度变量*/
	else if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_VELOCITY)
	{
		QComboBox* comboBox = static_cast<QComboBox*>(GetVelocityValueWidget());
		comboBox->setCurrentText(strNewParameterName);
		m_tableWidget->setCellWidget(rowNum, 1, comboBox);
	}
	/*若参数为过渡区域变量*/
	else if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_ZONE)
	{
		QComboBox* comboBox = static_cast<QComboBox*>(GetZoneValueWidget());
		comboBox->setCurrentText(strNewParameterName);
		m_tableWidget->setCellWidget(rowNum, 1,comboBox);
	}
	/*若参数为浮点数变量*/
	else if (strParameterType==CGrammarManagerFactory::STR_PARAMETER_TYPE_FLOAT)
	{
		QComboBox* comboBox = static_cast<QComboBox*>(GetDoubleValueWidget());
		comboBox->setCurrentText(strNewParameterName);
		m_tableWidget->setCellWidget(rowNum, 1, comboBox);
	}
	/*若参数为标签变量*/
	else if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_LABEL)
	{
		QComboBox* comboBox = static_cast<QComboBox*>(GetLabelValueWidget());
		comboBox->setCurrentText(strNewParameterName);
		m_tableWidget->setCellWidget(rowNum, 1, comboBox);
	}
	/*若参数为整数变量*/
	else if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_INT)
	{
		QComboBox* comboBox = static_cast<QComboBox*>(GetIntValueWidget());
		comboBox->setCurrentText(strNewParameterName);
		m_tableWidget->setCellWidget(rowNum, 1, comboBox);
	}
	/*若参数为布尔变量*/
	else if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_BOOL)
	{
		QComboBox* comboBox = static_cast<QComboBox*>(GetBoolValueWidget());
		comboBox->setCurrentText(strNewParameterName);
		m_tableWidget->setCellWidget(rowNum, 1, comboBox);
	}
	/*若参数为字符串变量*/
	else if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_STRING)
	{
		QComboBox* comboBox = static_cast<QComboBox*>(GetStringValueWidget());
		comboBox->setCurrentText(strNewParameterName);
		m_tableWidget->setCellWidget(rowNum, 1, comboBox);
	}
	/*若参数为整数变量*/
	else if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_VAR_NUMBER)
	{
		QComboBox* comboBox = static_cast<QComboBox*>(GetNumValueWidget());
		comboBox->setCurrentText(strNewParameterName);
		m_tableWidget->setCellWidget(rowNum, 1, comboBox);
	}
}

void CScreenEditMacro::KeyboardEdit()
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

	
	QLabel* currentLabel = static_cast<QLabel*>(m_tableWidget->cellWidget(rowNum, 1));

	/*设置新的数据*/
	currentLabel->setText(strText);
}

void CScreenEditMacro::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();
	m_varValue = CValue::GetInstance();
}

void CScreenEditMacro::InitLayout()
{
	m_tableWidget = new QTableWidget(this);
	m_tableWidget->setColumnCount(2);

	m_btnModify = new CButton;
	m_btnTeach = new CButton;
	m_btnAdd = new CButton;
	m_btnValue = new CButton;
	m_btnCancle = new CButton;
	m_btnConfirm = new CButton;
	//m_btnBack = new CButton;

	m_btnList.append(m_btnModify);
	m_btnList.append(m_btnTeach);
	m_btnList.append(m_btnAdd);
	m_btnList.append(m_btnValue);
	m_btnList.append(m_btnCancle);
	m_btnList.append(m_btnConfirm);
	m_btnList.append(m_btnBack);

	m_layoutCenter->addWidget(m_tableWidget);
	m_screenBottom->AddButtonList(m_btnList);
}

void CScreenEditMacro::InitSignalSlot()
{
	connect(m_btnModify, SIGNAL(clicked()), this, SLOT(SlotOnButtonModifyClicked()));
	connect(m_btnConfirm, SIGNAL(clicked()), this, SLOT(SlotOnButtonConfrimClicked()));
	connect(m_btnAdd, SIGNAL(clicked()), this, SLOT(SlotOnButtonAddClicked()));
	//connect(m_btnBack, SIGNAL(clicked()), this, SLOT(SlotOnButtonBackClicked()));
}

void CScreenEditMacro::InitMovlWidget()
{
	/*添加命令行*/
	QLabel* lbMacro = new QLabel("MOVL Position , Velocity , Zone");
	m_tableWidget->setCellWidget(0, 0, lbMacro);

	m_tableWidget->setCellWidget(1, 0, new QLabel("Position"));
	m_tableWidget->setCellWidget(2, 0, new QLabel("Velocity"));
	m_tableWidget->setCellWidget(3, 0, new QLabel("Zone"));

	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_POSITON);
	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_VELOCITY);
	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_ZONE);

	/*添加position节点*/
	QComboBox* comboBoxPosition = static_cast<QComboBox*>(GetPositionValueWidget());
	comboBoxPosition->setCurrentText(m_strListParameterName.at(0));
	m_tableWidget->setCellWidget(1, 1, comboBoxPosition);

	/*添加velocity节点*/
	QComboBox* comboBoxVelocity = static_cast<QComboBox*>(GetVelocityValueWidget());
	comboBoxVelocity->setCurrentText(m_strListParameterName.at(1));
	m_tableWidget->setCellWidget(2, 1, comboBoxVelocity);

	/*添加zone节点*/
	QComboBox* comboBoxZone = static_cast<QComboBox*>(GetZoneValueWidget());
	comboBoxZone->setCurrentText(m_strListParameterName.at(2));
	m_tableWidget->setCellWidget(3, 1, comboBoxZone);
}

void CScreenEditMacro::InitMovcWidget()
{
	/*添加命令行*/
	QLabel* lbMacro = new QLabel("MOVC Position1 , Position1 , Velocity , Zone");
	m_tableWidget->setCellWidget(0, 0, lbMacro);

	m_tableWidget->setCellWidget(1, 0, new QLabel("Position1"));
	m_tableWidget->setCellWidget(2, 0, new QLabel("Position2"));
	m_tableWidget->setCellWidget(3, 0, new QLabel("Velocity"));
	m_tableWidget->setCellWidget(4, 0, new QLabel("Zone"));

	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_POSITON);
	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_POSITON);
	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_VELOCITY);
	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_ZONE);

	/*添加position1节点*/
	QComboBox* comboBoxPosition1 = static_cast<QComboBox*>(GetPositionValueWidget());
	comboBoxPosition1->setCurrentText(m_strListParameterName.at(0));
	m_tableWidget->setCellWidget(1, 1, comboBoxPosition1);

	/*添加position2节点*/
	QComboBox* comboBoxPosition2 = static_cast<QComboBox*>(GetPositionValueWidget());
	comboBoxPosition2->setCurrentText(m_strListParameterName.at(1));
	m_tableWidget->setCellWidget(2, 1, comboBoxPosition2);

	/*添加velocity节点*/
	QComboBox* comboBoxVelocity = static_cast<QComboBox*>(GetVelocityValueWidget());
	comboBoxVelocity->setCurrentText(m_strListParameterName.at(2));
	m_tableWidget->setCellWidget(3, 1, comboBoxVelocity);

	/*添加zone节点*/
	QComboBox* comboBoxZone = static_cast<QComboBox*>(GetZoneValueWidget());
	comboBoxZone->setCurrentText(m_strListParameterName.at(3));
	m_tableWidget->setCellWidget(4, 1, comboBoxZone);
}

void CScreenEditMacro::InitForWidget()
{
	/*添加命令行*/
	QLabel* lbMacro = new QLabel("FOR Variant = Para1 TO Para2 STEP Para3");
	m_tableWidget->setCellWidget(0, 0, lbMacro);

	m_tableWidget->setCellWidget(1, 0, new QLabel("Variant"));
	m_tableWidget->setCellWidget(2, 0, new QLabel("Para1"));
	m_tableWidget->setCellWidget(3, 0, new QLabel("Para2"));
	m_tableWidget->setCellWidget(4, 0, new QLabel("Para3"));

	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_INT);
	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_NUMBER);
	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_NUMBER);
	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_NUMBER);

	/*添加num变量节点*/
	QComboBox* comboBoxNum = static_cast<QComboBox*>(GetIntValueWidget());
	comboBoxNum->setCurrentText(m_strListParameterName.at(0));
	m_tableWidget->setCellWidget(1, 1, comboBoxNum);

	m_tableWidget->setCellWidget(2, 1, new QLabel(m_strListParameterName.at(1)));
	m_tableWidget->setCellWidget(3, 1, new QLabel(m_strListParameterName.at(2)));
	m_tableWidget->setCellWidget(4, 1, new QLabel(m_strListParameterName.at(3)));

}

void CScreenEditMacro::InitSwitchWidget()
{
	/*添加命令行*/
	QLabel* lbMacro = new QLabel("SWITCH Para");
	m_tableWidget->setCellWidget(0, 0, lbMacro);

	m_tableWidget->setCellWidget(1, 0, new QLabel("Para"));

	m_strListParameterType.append(CGrammarManagerFactory::STR_PARAMETER_TYPE_INT);

	/*添加num变量节点*/
	QComboBox* comboBoxInt = static_cast<QComboBox*>(GetIntValueWidget());
	comboBoxInt->setCurrentText(m_strListParameterName.at(0));
	m_tableWidget->setCellWidget(1, 1, comboBoxInt);

}

/*************************************************
//  Function: 		GetPositionValueWidget
//  Description: 	获取位置值控件,查询已存在的位置变量并显示
//  Calls:		 	
//  Called By: 		// 调用本函数的函数清单
//  Parameter:      void
//  Return: 		QWidget
//  Others: 		// 其它说明
*************************************************/
QWidget* CScreenEditMacro::GetPositionValueWidget()
{
	QComboBox* positonBox = new QComboBox;
	QStringList strListPositions;
	CInterpreterAdapter::GetInstance()->GetPositionListFromEnclosingScope(strListPositions,"");
	for each (QString strPosition in strListPositions)
	{
		positonBox->addItem(strPosition);
	}
	return positonBox;
}

QWidget* CScreenEditMacro::GetVelocityValueWidget()
{
	QComboBox* velocityBox = new QComboBox;

	/*QStringList strList;
	CInterpreterAdapter::GetInstance()->GetDynamicList(strList);
	for each (QString strVariable in strList)
	{
		velocityBox->addItem(strVariable);

	}*/

	return velocityBox;
}

QWidget* CScreenEditMacro::GetZoneValueWidget()
{
	QComboBox* zoneBox = new QComboBox;

	/*QStringList strList;
	CInterpreterAdapter::GetInstance()->GetOverlapList(strList);
	for each (QString strVariable in strList)
	{
		zoneBox->addItem(strVariable);
	}*/
	return zoneBox;
}

QWidget* CScreenEditMacro::GetDoubleValueWidget()
{
	QComboBox* doubleBox = new QComboBox;

	/*QStringList strList;
	CInterpreterAdapter::GetInstance()->GetDoubleList(strList);
	for each (QString strVariable in strList)
	{
		doubleBox->addItem(strVariable);

	}*/

	return doubleBox;
}

QWidget* CScreenEditMacro::GetLabelValueWidget()
{
	QComboBox* labelBox = new QComboBox;

	//QStringList strList;
	//CInterpreterAdapter::GetInstance()->GetLabelList(strList);
	//for each (QString strVariable in strList)
	//{
	//	labelBox->addItem(strVariable);

	//}

	return labelBox;
}

QWidget* CScreenEditMacro::GetIntValueWidget()
{
	QComboBox* intBox = new QComboBox;

	/*QStringList strList;
	CInterpreterAdapter::GetInstance()->GetIntList(strList);
	for each (QString strVariable in strList)
	{
		intBox->addItem(strVariable);

	}*/

	return intBox;
}

QWidget* CScreenEditMacro::GetBoolValueWidget()
{
	QComboBox* boolBox = new QComboBox;

	/*QStringList strList;
	CInterpreterAdapter::GetInstance()->GetBoolList(strList);
	for each (QString strVariable in strList)
	{
		boolBox->addItem(strVariable);

	}*/

	return boolBox;
}

QWidget* CScreenEditMacro::GetStringValueWidget()
{
	QComboBox* stringBox = new QComboBox;

	/*QStringList strList;
	CInterpreterAdapter::GetInstance()->GetStringList(strList);
	for each (QString strVariable in strList)
	{
		stringBox->addItem(strVariable);

	}*/

	return stringBox;
}

QWidget* CScreenEditMacro::GetNumValueWidget()
{
	QComboBox* numBox = new QComboBox;

	QStringList strIntList;
	QStringList strDoubleList;
	//CInterpreterAdapter::GetInstance()->GetIntList(strIntList);
	//CInterpreterAdapter::GetInstance()->GetDoubleList(strDoubleList);

	QStringList strNumList;
	strNumList = strIntList;
	strNumList.append(strDoubleList);

	for each (QString strVariable in strNumList)
	{
		numBox->addItem(strVariable);

	}

	return numBox;
}

void CScreenEditMacro::SetMacroName(QString& strMacroName)
{
	m_strMacroName = strMacroName;
}

void CScreenEditMacro::SetParameterNameList(QStringList& strList)
{
	m_strListParameterName = strList;
}

void CScreenEditMacro::SetParameterTypeList(QStringList& strList)
{
	m_strListParameterType = strList;
}

QString& CScreenEditMacro::GetMacroName()
{
	return m_strMacroName;
}

QStringList& CScreenEditMacro::GetParameterNameList()
{
	return m_strListParameterName;
}

QStringList& CScreenEditMacro::GetParameterTypeList()
{
	return m_strListParameterType;
}

void CScreenEditMacro::SetForContext(QString& strText)
{
	strText.append("FOR ");
	strText.append(m_strListParameterName.at(0));
	strText.append(" = ");
	strText.append(m_strListParameterName.at(1));
	strText.append(" TO ");
	strText.append(m_strListParameterName.at(2));
	strText.append(" STEP ");
	strText.append(m_strListParameterName.at(3));
}

//void CScreenEditMacro::Back()
//{
//	CScreenMain::GetInstance()->ChangeToScreenProgram();
//}

void CScreenEditMacro::Destroy()
{
	CSingleTon<CScreenEditMacro>::Destroy();
}

void CScreenEditMacro::RefreshText()
{
	QString strName = QCoreApplication::translate("CScreenEditMacro", "Name");
	QString strValue = QCoreApplication::translate("CScreenEditMacro", "Value");
	QStringList strList;
	strList << strName << strValue;
	m_tableWidget->setHorizontalHeaderLabels(strList);

	m_btnModify->setText(QCoreApplication::translate("CScreenEditMacro", "Modify"));
	m_btnTeach->setText(QCoreApplication::translate("CScreenEditMacro", "Teach"));
	m_btnAdd->setText(QCoreApplication::translate("CScreenEditMacro", "Add"));
	m_btnValue->setText(QCoreApplication::translate("CScreenEditMacro", "Value"));
	m_btnCancle->setText(QCoreApplication::translate("CScreenEditMacro", "Cancle"));
	m_btnConfirm->setText(QCoreApplication::translate("CScreenEditMacro", "Confirm"));
	m_btnBack->setText(QCoreApplication::translate("CScreenEditMacro", "Back"));
}

void CScreenEditMacro::resizeEvent(QResizeEvent *event)
{
	m_tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	m_tableWidget->setColumnWidth(0, m_tableWidget->width() / 2);
	m_tableWidget->horizontalHeader()->setStretchLastSection(true);
}

void CScreenEditMacro::SlotOnButtonModifyClicked()
{
	//QModelIndex index = m_tableWidget->currentIndex();
	///*若索引无效*/
	//if (!index.isValid())
	//{
	//	return;
	//}
	///*若选择的为第一行*/
	//if (index.row() < 1)
	//{
	//	return;
	//}

	//if (m_strListParameterType.size()<1)
	//{
	//	return;
	//}

	///*获取参数类型*/
	//QString strParameterType = m_strListParameterType.at(index.row() - 1);

	///*设置变量名*/
	//QString strParameterName;
	//QWidget* widget = m_tableWidget->cellWidget(index.row(), 1);
	//if (typeid(*widget) == typeid(QComboBox))
	//{
	//	QComboBox* comboBox = static_cast<QComboBox*>(widget);
	//	strParameterName = comboBox->currentText();
	//}

	///*若为数值*/
	//if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_NUMBER)
	//{
	//	CKeyboard* keyboard = CKeyboard::GetInstance();
	//	keyboard->SetConfirmCommand(CKeyboard::CONFIRM_MACRO_NUMBER_EDIT);
	//	keyboard->SetCurrentWidget(this);
	//	keyboard->SetCurrentText(((QLabel*)m_tableWidget->cellWidget(index.row(),1))->text());
	//	keyboard->show();
	//	return;
	//}
	///*若为数值变量*/
	//else if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_VAR_NUMBER)
	//{
	//	/*若为整数*/
	//	if (m_varValue->m_mapSymbol[strParameterName.toStdString()].type == CValue::CATEGORY_INT)
	//	{
	//		strParameterType=CGrammarManagerFactory::STR_PARAMETER_TYPE_INT;
	//	}
	//	/*若为浮点数*/
	//	else
	//	{
	//		strParameterType = CGrammarManagerFactory::STR_PARAMETER_TYPE_FLOAT;
	//	}
	//}

	///*设置操作方式为修改*/
	//CMacroParameterParent::SetOperType(CMacroParameterParent::OPER_MODIFY);

	//

	///*若未选取任何变量*/
	//if (strParameterName.isEmpty())
	//{
	//	return;
	//}

	//CMacroParameterParent::SetOldParameterName(strParameterName);
	//CScreenEditMacroParameter::GetInstance()->ChangeWidget(strParameterType);

	//CScreenMain::GetInstance()->ChangeToScreenEditParameter();
}

void CScreenEditMacro::SlotOnButtonConfrimClicked()
{
	/*清除原有参数文本*/
	m_strListParameterName.clear();

	/*获取新的参数文本*/
	for (int i = 0; i < m_strListParameterType.size();i++)
	{
		QWidget* widget = m_tableWidget->cellWidget(i + 1, 1);	/*顺序获取控件*/

		/*读取控件的当前文本*/
		QString currentText;
		if (typeid(*widget) == typeid(QComboBox))
		{
			QComboBox* comboBox = static_cast<QComboBox*>(widget);
			currentText=comboBox->currentText();
		}
		else if (typeid(*widget) == typeid(QLabel))
		{
			QLabel* label = static_cast<QLabel*>(widget);
			currentText = label->text();
		}

		/*若文本为空，返回*/
		if (currentText.isEmpty())
		{
			//m_strListParameterName.append("*");
			QMessageBox::warning(this, "", "parameter cann't be empty");
			return;
		}
		else
		{
			m_strListParameterName.append(currentText);
		}
	}

	QString strMacroSentence;	/*显示于程序中的文本*/
	std::string stdStrText;	/*需进行语法检查的文本*/

	/*若命令为函数*/
	if (m_strMacroName==CGrammarManagerFactory::STR_MACRO_SIN ||
		m_strMacroName == CGrammarManagerFactory::STR_MACRO_COS ||
		m_strMacroName == CGrammarManagerFactory::STR_MACRO_TAN ||
		m_strMacroName == CGrammarManagerFactory::STR_MACRO_SQRT)
	{
		strMacroSentence.append(m_strMacroName);

		strMacroSentence.append("(");

		for (int i = 0; i < m_strListParameterName.size() - 1; i++)
		{
			strMacroSentence.append(m_strListParameterName.at(i));
			strMacroSentence.append(",");
		}
		strMacroSentence.append(m_strListParameterName.at(m_strListParameterName.size() - 1));

		strMacroSentence.append(")");
	}
	/*若为Movl、Movc命令*/
	else if (m_strMacroName==CGrammarManagerFactory::STR_MACRO_MOVL
		|| m_strMacroName==CGrammarManagerFactory::STR_MACRO_MOVC)
	{
		strMacroSentence.append(m_strMacroName);

		strMacroSentence.append(" ");
		for (int i = 0; i < m_strListParameterName.size() - 1; i++)
		{
			strMacroSentence.append(m_strListParameterName.at(i));
			strMacroSentence.append(",");
		}
		strMacroSentence.append(m_strListParameterName.at(m_strListParameterName.size() - 1));
	}
	/*若为For命令*/
	else if (m_strMacroName==CGrammarManagerFactory::STR_MACRO_FOR)
	{
		SetForContext(strMacroSentence);
		stdStrText = strMacroSentence.toStdString() + "\n" + "next";
	}
	/*若为Switch命令*/
	else if (m_strMacroName==CGrammarManagerFactory::STR_MACRO_SWITCH)
	{
		strMacroSentence.append(m_strMacroName);
		strMacroSentence.append(" ");
		strMacroSentence.append(m_strListParameterName.at(0));
	}

	/*检查语法错误*/
	try
	{
		CInterpreterManager* interpreterManager=CInterpreterManager::GetInstance();
		interpreterManager->ParseSemantic(stdStrText);
	}
	catch (CExceptionInterpreter& e)
	{
		std::string warningInfo;
		CInterpreterManager::GetWarningInfo(e, warningInfo);
		QMessageBox::warning(this, "", QString::fromStdString(warningInfo));
		return;
	}

	//CScreenPragram::GetInstance()->UpdateLineText(strMacroSentence);
	Back();
}

/*************************************************
//  Function: 		SlotOnButtonAddClicked
//  Description: 	添加按钮事件，按钮按下后，根据选择的参数类型切换到对应的参数编辑界面
//  Calls:		 	界面切换函数，CMacroParameterParent::SetOperType
//  Called By: 		InitSignalSlot
//  Parameter:      void
//  Return: 		void
//  Others: 		
*************************************************/
void CScreenEditMacro::SlotOnButtonAddClicked()
{
	QModelIndex index = m_tableWidget->currentIndex();
	/*若索引无效*/
	if (!index.isValid())
	{
		return;
	}
	/*若选择的为第一行*/
	if (index.row()<1)
	{
		return;
	}

	if (m_strListParameterType.size() < 1)
	{
		return;
	}

	/*获取参数类型*/
	QString strParameterType = m_strListParameterType.at(index.row() - 1);

	/*若为数值变量或数值*/
	if (strParameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_VAR_NUMBER)
	{
		/*设置操作方式为添加*/
		CMacroParameterParent::SetOperType(CMacroParameterParent::OPER_ADD);

		CScreenEditMacroParameter::GetInstance()->ChangeWidget(CGrammarManagerFactory::STR_PARAMETER_TYPE_INT);

		CScreenMain::GetInstance()->ChangeToScreenEditParameter();
		return;
	}
	else if(strParameterType==CGrammarManagerFactory::STR_PARAMETER_TYPE_NUMBER)
	{
		return;
	}


	/*设置操作方式为添加*/
	CMacroParameterParent::SetOperType(CMacroParameterParent::OPER_ADD);

	CScreenEditMacroParameter::GetInstance()->ChangeWidget(strParameterType);

	CScreenMain::GetInstance()->ChangeToScreenEditParameter();
}

//void CScreenEditMacro::SlotOnButtonBackClicked()
//{
//	Back();
//}
