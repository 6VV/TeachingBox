#include "stdafx.h"
#include "CScreenNewVariable.h"
#include "CScreenProject.h"
#include "CWarningManager.h"
#include "TVariateManager.h"
#include "TInteger.h"
#include "TDouble.h"
#include "TBool.h"
#include "TPosition.h"
#include "TDynamic.h"
#include "TString.h"
#include "CRegExpManager.h"



void CScreenNewVariable::SetScope(QString& strScopeName)
{
	this->m_strScopeInDatabase = strScopeName;

	if (m_strScopeInDatabase == CScope::STR_SCOPE_SYSTEM
		|| m_strScopeInDatabase == CScope::STR_SCOPE_SYNERGIC
		|| m_strScopeInDatabase == CScope::STR_SCOPE_GLOBAL)
	{
		m_strScopeName = strScopeName;
	}
	/*若为项目*/
	else if (m_strScopeInDatabase == CScreenProject::GetInstance()->GetLoadedProjectNameInDatabase())
	{
		m_strScopeName = CScope::STR_SCOPE_PROJECT;
	}
	/*若为程序*/
	else
	{
		m_strScopeName = CScope::STR_SCOPE_LOCAL;
	}
}

CScreenNewVariable::CScreenNewVariable(QWidget* parent /*= 0*/) :CScreenMainParent(parent)
{
	Init();
}

CScreenNewVariable::~CScreenNewVariable()
{
	//qDebug() << "delete screen new parameter";
}

CScreenNewVariable* CScreenNewVariable::GetInstance()
{
	return CSingleTon<CScreenNewVariable>::GetInstance();
}

void CScreenNewVariable::SlotOnButtonConfirmClicked()
{
	QString varName = (static_cast<CLineEditWithClickedSignal*>(m_tableWidget->cellWidget(0, 1)))->text();

	/*若变量名为空*/
	if (varName.isEmpty())
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::NameIsEmpty);
		return;
	}

	/*是否已存在该变量*/
	//bool isExist = CInterpreterAdapter::GetInstance()->IsSymbolExist(m_strScopeInDatabase, varName.toStdString());
	if (TVariateManager::GetInstance()->IsExistVariateScrollUp(m_strScopeInDatabase,varName))
	{
		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Already Exist"));
		return;
	}

	/*判断当前类型*/
	QString currentType = m_treeWidget->currentItem()->text(0);

	/*若为整数类型*/
	if (currentType == CParameterManager::STR_TYPE_INT)
	{
		int varValue = (static_cast<CLineEditWithClickedSignal*>(
			m_tableWidget->cellWidget(INIT_ROW_NUM, 1)))->text().toInt();

		/*存储变量*/
		TVariateManager::GetInstance()->Add(
			new TInteger(m_strScopeInDatabase, varName, varValue));
	}
	else if (currentType == CParameterManager::STR_TYPE_DOUBLE)
	{
		double varValue = (static_cast<CLineEditWithClickedSignal*>(m_tableWidget->cellWidget(INIT_ROW_NUM, 1)))->text().toDouble();

		/*存储变量*/
		TVariateManager::GetInstance()->Add(new TDouble(m_strScopeInDatabase, varName, varValue));

		//CInterpreterAdapter::GetInstance()->InsertDoubleValue(m_strScopeInDatabase, pairVariable);
	}
	else if (currentType == CParameterManager::STR_TYPE_BOOL)
	{
		int boolValue=0;

		QString strValue = static_cast<QComboBox*>(m_tableWidget->cellWidget(INIT_ROW_NUM, 1))->currentText();
		if (strValue == CParameterManager::STR_BOOL_TRUE)
		{
			boolValue = 1;
		}

		/*存储变量*/
		//CInterpreterAdapter::GetInstance()->InsertBoolValue(m_strScopeInDatabase, pairVariable);
		TVariateManager::GetInstance()->Add(new TBool(m_strScopeInDatabase, varName, boolValue));
	}
	else if (currentType == CParameterManager::STR_TYPE_STRING)
	{
		std::string varValue = (static_cast<CLineEditWithClickedSignal*>(
			m_tableWidget->cellWidget(INIT_ROW_NUM, 1)))->text().toStdString();

		/*存储变量*/
		TVariateManager::GetInstance()->Add(
			new TString(m_strScopeInDatabase, varName, QString::fromStdString(varValue)));
		//CInterpreterAdapter::GetInstance()->InsertStringValue(m_strScopeInDatabase, pairVariable);
	}

	else if (currentType == CParameterManager::STR_TYPE_POSITION)
	{
		CValue::TYPE_POSITION position;
		for (int i = INIT_ROW_NUM; i < INIT_ROW_NUM+6; i++)
		{
			CLineEditWithClickedSignal* lineEdit = static_cast<CLineEditWithClickedSignal*>(m_tableWidget->cellWidget(i, 1));
			position.m_AxisPosition[i-INIT_ROW_NUM]=lineEdit->text().toDouble();
		}

		/*存储变量*/
		TPosition::TYPE_POSITION vecPosition;
		for (int i = 0; i < TPosition::AXIS_SIZE;++i)
		{
			vecPosition.push_back(position.m_AxisPosition[i]);
		}
		TVariateManager::GetInstance()->Add(
			new TPosition(m_strScopeInDatabase, varName, vecPosition));
		//CInterpreterAdapter::GetInstance()->InsertPositionValue(m_strScopeInDatabase, pairPosition);
	}
	else if (currentType == CParameterManager::STR_TYPE_DYNAMIC)
	{
		CValue::TYPE_DYNAMIC dynamic;
		QVector<double> vectorDynamic;
		for (int i = INIT_ROW_NUM; i < INIT_ROW_NUM + 6; i++)
		{
			CLineEditWithClickedSignal* lineEdit = static_cast<CLineEditWithClickedSignal*>(m_tableWidget->cellWidget(i, 1));
			vectorDynamic.push_back(lineEdit->text().toDouble());
		}

		dynamic.m_Velocity = vectorDynamic.at(0);
		dynamic.m_Acceleration = vectorDynamic.at(1);
		dynamic.m_Deceleration = vectorDynamic.at(2);
		dynamic.m_PostureVelocity = vectorDynamic.at(3);
		dynamic.m_PostureAcceleration = vectorDynamic.at(4);
		dynamic.m_PostureDeceleration = vectorDynamic.at(5);


		/*存储变量*/
		TVariateManager::GetInstance()->Add(
			new TDynamic(m_strScopeInDatabase, varName, dynamic));
		//CInterpreterAdapter::GetInstance()->InsertDynamicValue(m_strScopeInDatabase, pairVariable);
		
	}
	else if (currentType == CParameterManager::STR_TYPE_OVERLAP)
	{
		int mode=0;
		QString strMode = (static_cast<QComboBox*>(m_tableWidget->cellWidget(INIT_ROW_NUM, 1)))->currentText();

		if (strMode==CParameterManager::STR_TRANSITION_MODE_ABSOLUTE)
		{
			mode = kTransitionModeAbsolute;
		}
		else if (strMode == CParameterManager::STR_TRANSITION_MODE_RELATIVE)
		{
			mode = kTransitionModeRelative;
		}

		int value = (static_cast<CLineEditWithClickedSignal*>(m_tableWidget->cellWidget(INIT_ROW_NUM + 1, 1)))->text().toDouble();
		
		CValue::TYPE_OVERLAP overlap{ mode, value };
		CValue::TYPE_PAIR_OVERLAP pairVariable(varName.toStdString(), overlap);

		/*存储变量*/
		CInterpreterAdapter::GetInstance()->InsertOverlapValue(m_strScopeInDatabase,pairVariable);
		
	}

	Back();

}

void CScreenNewVariable::RefreshText()
{
	/*底部按钮*/
	m_btnConfirm->setText(QCoreApplication::translate(CLASS_NAME, "Confirm"));
	m_btnBack->setText(QCoreApplication::translate(CLASS_NAME, "Back"));

	/*树形*/
	m_treeWidget->setHeaderLabel(QCoreApplication::translate(CLASS_NAME, "TYPE"));

	/*表格*/
	QStringList strListHeader;
	strListHeader.append(QCoreApplication::translate(CLASS_NAME, "Name"));
	strListHeader.append(QCoreApplication::translate(CLASS_NAME, "Value"));
	m_tableWidget->setHorizontalHeaderLabels(strListHeader);
}

void CScreenNewVariable::resizeEvent(QResizeEvent *event)
{
	m_tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	m_tableWidget->setColumnWidth(0, m_tableWidget->width() / 2);
	m_tableWidget->horizontalHeader()->setStretchLastSection(true);
}

void CScreenNewVariable::showEvent(QShowEvent *event)
{
	CScreenMainParent::showEvent(event);

	/*设置当前作用域*/
	m_lineEditScope->setText(m_strScopeName);

	/*设置当前类型为Int类型*/
	//m_lineEditType->setCurrentIndex(0);
	//static_cast<CLineEditWithClickedSignal*>(m_tableWidget->cellWidget(0, 1))->setText("");
	//static_cast<QComboBox*>(m_tableWidget->cellWidget(1, 1))->setCurrentIndex(0);
	//static_cast<QComboBox*>(m_tableWidget->cellWidget(2, 1))->setCurrentIndex(0);

	//SlotTypeChanged(0);
}

void CScreenNewVariable::hideEvent(QHideEvent *event)
{
	m_lineEidtName->setText("");
	m_lineEidtName->setEnabled(false);
	m_lineEditScope->setText("");
	m_lineEditType->setText("");
	m_tableWidget->setRowCount(INIT_ROW_NUM);

	m_treeWidget->reset();
}

void CScreenNewVariable::SlotTypeChanged(QTreeWidgetItem* item, int column)
{
	/*若当前节点为顶部节点*/
	if (item->parent()==NULL)
	{
		return;
	}

	/*设置名称可编辑*/
	m_lineEidtName->setEnabled(true);

	/*获取当前类型*/
	QString currentType = item->text(0);
	m_lineEditType->setText(currentType);

	/*若为Int类*/
	if (currentType == CParameterManager::STR_TYPE_INT)
	{
		InitIntParameter();
	}
	/*若为Double类*/
	else if (currentType == CParameterManager::STR_TYPE_DOUBLE)
	{
		InitDoubleParameter();
	}
	/*若为Bool类*/
	else if (currentType == CParameterManager::STR_TYPE_BOOL)
	{
		InitBoolParameter();
	}
	/*若为String类*/
	else if (currentType == CParameterManager::STR_TYPE_STRING)
	{
		InitStringParameter();
	}
	/*若为Position类*/
	else if (currentType == CParameterManager::STR_TYPE_POSITION)
	{
		InitPositionParameter();
	}
	/*若为Dynamic类*/
	else if (currentType == CParameterManager::STR_TYPE_DYNAMIC)
	{
		InitDynamicParameter();
	}
	/*若为Overlap类*/
	else if (currentType == CParameterManager::STR_TYPE_OVERLAP)
	{
		InitOverlapParameter();
	}
}

void CScreenNewVariable::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();
}

void CScreenNewVariable::InitLayout()
{
	InitLayoutTreeWidget();
	InitLayoutTableWidget();

	/*初始化控件*/
	m_btnConfirm = new CButton;
	//m_btnBack = new CButton;

	CButton* btnNull1 = new CButton;
	CButton* btnNull2 = new CButton;
	CButton* btnNull3 = new CButton;
	CButton* btnNull4 = new CButton;
	CButton* btnNull5 = new CButton;

	btnNull1->setEnabled(false);
	btnNull2->setEnabled(false);
	btnNull3->setEnabled(false);
	btnNull4->setEnabled(false);
	btnNull5->setEnabled(false);

	/*布局*/
	m_btnList.append(m_btnConfirm);
	m_btnList.append(btnNull1);
	m_btnList.append(btnNull2);
	m_btnList.append(btnNull3);
	m_btnList.append(btnNull4);
	m_btnList.append(btnNull5);
	m_btnList.append(m_btnBack);

	//m_layoutCenter->addWidget(m_tableWidget);
	m_layoutCenter->addWidget(m_treeWidget);
	m_layoutCenter->addWidget(m_tableWidget);

	m_layoutCenter->setStretch(0, 1);
	m_layoutCenter->setStretch(1, 2);

	m_screenBottom->AddButtonList(m_btnList);
}

void CScreenNewVariable::InitLayoutTableWidget()
{
	m_tableWidget = new QTableWidget;
	m_tableWidget->setColumnCount(2);
	m_tableWidget->setRowCount(INIT_ROW_NUM);

	QLabel* lbName = new QLabel(QCoreApplication::translate(CLASS_NAME,"Name"));
	QLabel* lbScope = new QLabel(QCoreApplication::translate(CLASS_NAME, "Scope"));
	QLabel* lbType = new QLabel(QCoreApplication::translate(CLASS_NAME, "Type"));

	m_lineEidtName = new CLineEditWithClickedSignal("",CRegExpManager::STR_REG_STRING_NAME);
	m_lineEidtName->setEnabled(false);

	m_lineEditScope = new CLineEditWithClickedSignal;
	m_lineEditScope->setEnabled(false);
	//m_lineEditScope->addItem(QString::fromStdString(CScope::STR_SCOPE_GLOBAL));
	//m_lineEditScope->addItem(QString::fromStdString(CScope::STR_SCOPE_GLOBAL));

	m_lineEditType = new CLineEditWithClickedSignal;
	m_lineEditType->setEnabled(false);
	//m_lineEditType->addItem(CParameterManager::STR_TYPE_INT);
	//m_lineEditType->addItem(CParameterManager::STR_TYPE_FLOAT);
	//m_lineEditType->addItem(CParameterManager::STR_TYPE_BOOL);
	//m_lineEditType->addItem(CParameterManager::STR_TYPE_STRING);
	//m_lineEditType->addItem(CParameterManager::STR_TYPE_POSITION);
	//m_lineEditType->addItem(CParameterManager::STR_TYPE_DYNAMIC);
	//m_lineEditType->addItem(CParameterManager::STR_TYPE_OVERLAP);

	m_tableWidget->setCellWidget(0, 0, lbName);
	m_tableWidget->setCellWidget(0, 1, m_lineEidtName);

	m_tableWidget->setCellWidget(1, 0, lbScope);
	m_tableWidget->setCellWidget(1, 1, m_lineEditScope);

	m_tableWidget->setCellWidget(2, 0, lbType);
	m_tableWidget->setCellWidget(2, 1, m_lineEditType);
}

void CScreenNewVariable::InitLayoutTreeWidget()
{
	m_treeWidget = new QTreeWidget;

	/*添加基本类型*/
	QTreeWidgetItem* itemBase = new QTreeWidgetItem(QStringList{ CParameterManager::STR_CATEGORY_BASE });
	QStringList strListBase;

	itemBase->addChild(new QTreeWidgetItem(QStringList{ CParameterManager::STR_TYPE_INT }));
	itemBase->addChild(new QTreeWidgetItem(QStringList{ CParameterManager::STR_TYPE_DOUBLE }));
	itemBase->addChild(new QTreeWidgetItem(QStringList{ CParameterManager::STR_TYPE_BOOL }));
	itemBase->addChild(new QTreeWidgetItem(QStringList{ CParameterManager::STR_TYPE_STRING }));

	/*添加位置类型*/
	QTreeWidgetItem* itemPosition = new QTreeWidgetItem(QStringList{ CParameterManager::STR_TYPE_POSITION });
	itemPosition->addChild(new QTreeWidgetItem(QStringList{ CParameterManager::STR_TYPE_POSITION }));

	/*添加动态类型*/
	QTreeWidgetItem* itemDynamic = new QTreeWidgetItem(QStringList{ CParameterManager::STR_TYPE_DYNAMIC });
	itemDynamic->addChild(new QTreeWidgetItem(QStringList{ CParameterManager::STR_TYPE_DYNAMIC }));

	/*添加过渡类型*/
	QTreeWidgetItem* itemOverlap = new QTreeWidgetItem(QStringList{ CParameterManager::STR_TYPE_OVERLAP });
	itemOverlap->addChild(new QTreeWidgetItem(QStringList{ CParameterManager::STR_TYPE_OVERLAP }));


	m_treeWidget->addTopLevelItem(itemBase);
	m_treeWidget->addTopLevelItem(itemPosition);
	m_treeWidget->addTopLevelItem(itemDynamic);
	m_treeWidget->addTopLevelItem(itemOverlap);

}

void CScreenNewVariable::InitSignalSlot()
{
	/*树形控件事件*/
	connect(m_treeWidget, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(SlotTypeChanged(QTreeWidgetItem*, int)));

	/*表格事件*/
	connect(m_btnConfirm, SIGNAL(clicked()), this, SLOT(SlotOnButtonConfirmClicked()));
	//connect(m_btnBack, SIGNAL(clicked()), this, SLOT(SlotOnButtonBackClicked()));
	connect(m_lineEditType, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotTypeChanged(int)));
}


bool CScreenNewVariable::IsIntValue(QString& strValue)
{
	bool ok;
	int value=strValue.toInt(&ok);

	if (!ok)
	{
		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Not a int number"));
	}

	strValue = QString::number(value);
	return ok;
}

bool CScreenNewVariable::IsFloatValue(QString& strValue)
{
	bool ok;
	double value=strValue.toDouble(&ok);
	if (!ok)
	{
		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Not a number"));
	}
	strValue = QString::number(value);
	return ok;
}

void CScreenNewVariable::InitIntParameter()
{
	m_tableWidget->setRowCount(INIT_ROW_NUM + 1);
	QLabel* lbValue = new QLabel(QCoreApplication::translate(CLASS_NAME, "Value"));
	m_tableWidget->setCellWidget(INIT_ROW_NUM, 0, lbValue);

	m_tableWidget->setCellWidget(INIT_ROW_NUM, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_INT));
}

void CScreenNewVariable::InitDoubleParameter()
{
	m_tableWidget->setRowCount(INIT_ROW_NUM + 1);
	QLabel* lbValue = new QLabel(QCoreApplication::translate(CLASS_NAME, "Value"));
	m_tableWidget->setCellWidget(INIT_ROW_NUM, 0, lbValue);

	m_tableWidget->setCellWidget(INIT_ROW_NUM, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
}

void CScreenNewVariable::InitBoolParameter()
{
	m_tableWidget->setRowCount(INIT_ROW_NUM + 1);
	QLabel* lbValue = new QLabel(QCoreApplication::translate(CLASS_NAME, "Value"));
	m_tableWidget->setCellWidget(INIT_ROW_NUM, 0, lbValue);
	QComboBox* cmbBool = new QComboBox;
	cmbBool->addItem(CParameterManager::STR_BOOL_TRUE);
	cmbBool->addItem(CParameterManager::STR_BOOL_FALSE);
	m_tableWidget->setCellWidget(INIT_ROW_NUM, 1, cmbBool);
}

void CScreenNewVariable::InitStringParameter()
{
	m_tableWidget->setRowCount(INIT_ROW_NUM + 1);
	QLabel* lbValue = new QLabel(QCoreApplication::translate(CLASS_NAME, "Value"));
	m_tableWidget->setCellWidget(INIT_ROW_NUM, 0, lbValue);

	m_tableWidget->setCellWidget(INIT_ROW_NUM, 1, new CLineEditWithClickedSignal);
}

void CScreenNewVariable::InitPositionParameter()
{
	m_tableWidget->setRowCount(INIT_ROW_NUM + 6);
	QLabel* lbValue1 = new QLabel("Axis1");
	QLabel* lbValue2 = new QLabel("Axis2");
	QLabel* lbValue3 = new QLabel("Axis3");
	QLabel* lbValue4 = new QLabel("Axis4");
	QLabel* lbValue5 = new QLabel("Axis5");
	QLabel* lbValue6 = new QLabel("Axis6");


	m_tableWidget->setCellWidget(INIT_ROW_NUM, 0, lbValue1);
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 1, 0, lbValue2);
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 2, 0, lbValue3);
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 3, 0, lbValue4);
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 4, 0, lbValue5);
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 5, 0, lbValue6);

	m_tableWidget->setCellWidget(INIT_ROW_NUM, 1, new CLineEditWithClickedSignal("0",CRegExpManager::STR_REG_FLOAT));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 1, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 2, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 3, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 4, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 5, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
}

void CScreenNewVariable::InitDynamicParameter()
{
	m_tableWidget->setRowCount(INIT_ROW_NUM + 6);
	m_tableWidget->setCellWidget(INIT_ROW_NUM, 0, new QLabel(QCoreApplication::translate(CLASS_NAME, "Velocity")));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 1, 0, new QLabel(QCoreApplication::translate(CLASS_NAME, "Acceleration")));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 2, 0, new QLabel(QCoreApplication::translate(CLASS_NAME, "Deceleration")));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 3, 0, new QLabel(QCoreApplication::translate(CLASS_NAME, "PostureVelocity")));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 4, 0, new QLabel(QCoreApplication::translate(CLASS_NAME, "PostureAcceleration")));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 5, 0, new QLabel(QCoreApplication::translate(CLASS_NAME, "PostureDeceleration")));

	m_tableWidget->setCellWidget(INIT_ROW_NUM, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 1, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 2, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 3, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 4, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 5, 1, new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT));
}

void CScreenNewVariable::InitOverlapParameter()
{
	m_tableWidget->setRowCount(INIT_ROW_NUM + 2);
	m_tableWidget->setCellWidget(INIT_ROW_NUM, 0, new QLabel(QCoreApplication::translate(CLASS_NAME, "Mode")));
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 1, 0, new QLabel(QCoreApplication::translate(CLASS_NAME, "Value")));

	QComboBox* comboBoxMode = new QComboBox;
	comboBoxMode->addItem(CParameterManager::STR_TRANSITION_MODE_RELATIVE);
	comboBoxMode->addItem(CParameterManager::STR_TRANSITION_MODE_ABSOLUTE);
	CLineEditWithClickedSignal* lineEdit = new CLineEditWithClickedSignal("0", CRegExpManager::STR_REG_FLOAT);
	m_tableWidget->setCellWidget(INIT_ROW_NUM, 1, comboBoxMode);
	m_tableWidget->setCellWidget(INIT_ROW_NUM + 1, 1, lineEdit);
}

