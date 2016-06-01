#include "stdafx.h"
#include "CEditParameter.h"
#include "CValue.h"
#include "QApplication"
#include "CGrammarManagerFactory.h"
#include "QListWidget"
#include "QBoxLayout"
#include "CParameterManager.h"
#include "QAbstractItemModel"
#include "CWidgetStack.h"
#include "CScreenPragram.h"
#include "CScreenProject.h"
#include "TVariateManager.h"
#include "TPosition.h"
#include "TDynamic.h"
#include "CLineEditWithRegExpAndKeyboard.h"
#include "TOverlap.h"



CEditParameter::CEditParameter(QWidget* parent/*=0*/) :CScreenMainParent(parent)
{
	Init();
}

CEditParameter::~CEditParameter()
{

}

CEditParameter* CEditParameter::GetInstance()
{
	return CSingleTon<CEditParameter>::GetInstance();
}

void CEditParameter::ChangeContent(const QString& macroName, const QStringList& strListParameterName)
{
	m_treeWidget->clear();

	m_strMacroName = macroName;
	m_strListParameterName = strListParameterName;

	if (macroName==CGrammarManagerFactory::STR_MACRO_MOVL)
	{
		ChangeToMovl();
	}
	else if (macroName==CGrammarManagerFactory::STR_MACRO_MOVC)
	{
		ChangeToMovc();
	}
}

void CEditParameter::SetMacroInterface(MacroInterface* macroInterface)
{
	m_macroInterface = macroInterface;
}

void CEditParameter::resizeEvent(QResizeEvent *event)
{
	/*平分各行*/
	m_treeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	m_treeWidget->setColumnWidth(0, m_treeWidget->width() / 2);

}

void CEditParameter::RefreshText()
{
	/*设置列名*/
	QStringList strListHead;
	strListHead << QCoreApplication::translate(CLASS_NAME, "Name") << QCoreApplication::translate(CLASS_NAME, "Value");
	m_treeWidget->setHeaderLabels(strListHead);

	/*设置按钮文本*/
	m_btnConfirm->setText(QCoreApplication::translate(CLASS_NAME, "Confirm"));
	m_btnEdit->setText(QCoreApplication::translate(CLASS_NAME, "Edit"));
	m_btnBack->setText(QCoreApplication::translate(CLASS_NAME, "Back"));
}


void CEditParameter::BackToWidget(QWidget* widget)
{
	CWidgetStack* widgetStack = CWidgetStack::GetInstance();
	widgetStack->BackToWidget(widget);
}

void CEditParameter::Back()
{
	BackToWidget(CScreenPragram::GetInstance());
}

void CEditParameter::SlotOnButtonConfirmClicked()
{
	/*清除原有参数文本*/
	m_strListParameterName.clear();

	/*获取参数文本*/
	QModelIndex rootIndex = m_treeWidget->rootIndex();
	QTreeWidgetItem* item=m_treeWidget->invisibleRootItem();

	for (int i = 1; i < item->childCount();++i)
	{
		QWidget* widget = m_treeWidget->itemWidget(item->child(i), 1);

		if (typeid(*widget)==typeid(CComboBoxWithTree_Old))
		{
			m_strListParameterName.append(static_cast<CComboBoxWithTree_Old*>(widget)->currentText());
		}
		else if (typeid(*widget) == typeid(CLineEditWithRegExpAndKeyboard))
		{
			m_strListParameterName.append(static_cast<CLineEditWithRegExpAndKeyboard*>(widget)->text());
		}
	}

	/*生成命令文本*/
	QString strMacroText;

	/*若为Movl命令*/
	if (m_strMacroName==CGrammarManagerFactory::STR_MACRO_MOVL)
	{
		strMacroText = QString(CGrammarManagerFactory::STR_MACRO_FORMAT_MOVL);
	}
	else if (m_strMacroName==CGrammarManagerFactory::STR_MACRO_MOVC)
	{
		strMacroText = QString(CGrammarManagerFactory::STR_MACRO_FORMAT_MOVC);
	}

	/*更新文本*/
	for (int i = 0; i < m_strListParameterName.size(); ++i)
	{
		strMacroText = strMacroText.arg(m_strListParameterName.at(i));
	}

	///*检查语法错误*/
	//try
	//{
	//	CInterpreterManager interpreterManager;
	//	interpreterManager.ParseSemantic(stdStrText);
	//}
	//catch (CExceptionInterpreter& e)
	//{
	//	std::string warningInfo;
	//	CInterpreterManager::GetWarningInfo(e, warningInfo);
	//	QMessageBox::warning(this, "", QString::fromStdString(warningInfo));
	//	return;
	//}

	m_macroInterface->EditMacroInterface(strMacroText);
	Back();
}

void CEditParameter::SlotOnComboBoxItemChanged(const QString& strText)
{
	CValue* value = CValue::GetInstance();

	CSymbol* symbol = TVariateManager::GetInstance()->GetRootScope().FindScopeScrollDown(
		CScreenProject::GetInstance()->GetOpenedFileName())
		->FindSymbolScrollUp(strText);

	/*若未找到*/
	if (symbol == NULL)
	{
		return;
	}

	int valueType = symbol->GetType();

	CComboBoxWithTree_Old* comboBox = static_cast<CComboBoxWithTree_Old*>(sender());
	/*加载数据*/
	switch (valueType)
	{
	/*若为位置参数*/
	case CSymbol::TYPE_POSITION:
	{
		RefreshPositionParameter(strText, comboBox->GetTreeWidgetItem());
	}break;
	/*若为动态参数*/
	case CSymbol::TYPE_DYNAMIC:
	{
		RefreshDynamicParameter(strText, comboBox->GetTreeWidgetItem());
	}break;
	/*若为过渡参数*/
	case CSymbol::TYPE_OVERLAP:
	{
		RefreshOverlapParameter(strText, comboBox->GetTreeWidgetItem());
	}break;
	}
}

void CEditParameter::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();

	m_strScope = CScreenProject::GetInstance()->GetOpenedFileName();
}

void CEditParameter::InitLayout()
{
	m_treeWidget = new QTreeWidget;
	m_treeWidget->setColumnCount(2);

	m_btnConfirm = new CButton;
	m_btnEdit = new CButton;
	//m_btnBack = new CButton;

	CButton* btnNull1 = new CButton;
	CButton* btnNull2 = new CButton;
	CButton* btnNull3 = new CButton;

	m_btnList.append(m_btnConfirm);
	m_btnList.append(m_btnEdit);
	m_btnList.append(btnNull1);
	m_btnList.append(btnNull2);
	m_btnList.append(btnNull3);
	m_btnList.append(m_btnBack);

	m_layoutCenter->addWidget(m_treeWidget);
	m_screenBottom->AddButtonList(m_btnList);
}

void CEditParameter::InitSignalSlot()
{
	connect(m_btnConfirm, SIGNAL(clicked()), this, SLOT(SlotOnButtonConfirmClicked()));
	//connect(m_btnBack, SIGNAL(clicked()), this, SLOT(SlotOnButtonBackClicked()));
}


void CEditParameter::AddPositionParameter(const QString& strText)
{
	QTreeWidgetItem *rootItem = new QTreeWidgetItem(m_treeWidget);
	rootItem->setText(0, "Pos");
	CComboBoxWithTree_Old* comboBoxPosition = GetComboBoxPosition();
	comboBoxPosition->setCurrentText(strText);
	comboBoxPosition->SetTreeWidgetItem(rootItem);

	m_treeWidget->setItemWidget(rootItem, 1, comboBoxPosition);

	QTreeWidgetItem *itemAxis1 = new QTreeWidgetItem(rootItem, QStringList("Axis1"));
	QTreeWidgetItem *itemAxis2 = new QTreeWidgetItem(rootItem, QStringList("Axis2"));
	QTreeWidgetItem *itemAxis3 = new QTreeWidgetItem(rootItem, QStringList("Axis3"));
	QTreeWidgetItem *itemAxis4 = new QTreeWidgetItem(rootItem, QStringList("Axis4"));
	QTreeWidgetItem *itemAxis5 = new QTreeWidgetItem(rootItem, QStringList("Axis5"));
	QTreeWidgetItem *itemAxis6 = new QTreeWidgetItem(rootItem, QStringList("Axis6"));

	QLineEdit* lineEditAxis1 = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);
	QLineEdit* lineEditAxis2 = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);
	QLineEdit* lineEditAxis3 = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);
	QLineEdit* lineEditAxis4 = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);
	QLineEdit* lineEditAxis5 = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);
	QLineEdit* lineEditAxis6 = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);


	lineEditAxis1->setAlignment(Qt::AlignHCenter);
	lineEditAxis2->setAlignment(Qt::AlignHCenter);
	lineEditAxis3->setAlignment(Qt::AlignHCenter);
	lineEditAxis4->setAlignment(Qt::AlignHCenter);
	lineEditAxis5->setAlignment(Qt::AlignHCenter);
	lineEditAxis6->setAlignment(Qt::AlignHCenter);

	m_treeWidget->setItemWidget(itemAxis1, 1, lineEditAxis1);
	m_treeWidget->setItemWidget(itemAxis2, 1, lineEditAxis2);
	m_treeWidget->setItemWidget(itemAxis3, 1, lineEditAxis3);
	m_treeWidget->setItemWidget(itemAxis4, 1, lineEditAxis4);
	m_treeWidget->setItemWidget(itemAxis5, 1, lineEditAxis5);
	m_treeWidget->setItemWidget(itemAxis6, 1, lineEditAxis6);

	/*comboBox切换事件*/
	connect(comboBoxPosition, SIGNAL(currentTextChanged(const QString&)), this, SLOT(SlotOnComboBoxItemChanged(const QString&)));

	RefreshPositionParameter(comboBoxPosition->currentText(), rootItem);
}

void CEditParameter::AddDynamicParameter(const QString& strText)
{
	QTreeWidgetItem *rootItem = new QTreeWidgetItem(m_treeWidget);
	rootItem->setText(0, "Dyn");
	CComboBoxWithTree_Old* comboBoxDynamic = GetComboBoxDynamic();
	comboBoxDynamic->setCurrentText(strText);
	comboBoxDynamic->SetTreeWidgetItem(rootItem);

	m_treeWidget->setItemWidget(rootItem, 1, comboBoxDynamic);

	QTreeWidgetItem* itemVelocity = new QTreeWidgetItem(rootItem, QStringList("Velocity"));
	QTreeWidgetItem* itemAcceleration = new QTreeWidgetItem(rootItem, QStringList("Acceleration"));
	QTreeWidgetItem* itemDeceleration = new QTreeWidgetItem(rootItem, QStringList("Deceleration"));
	QTreeWidgetItem* itemPostureVelocity = new QTreeWidgetItem(rootItem, QStringList("PostureVelocity"));
	QTreeWidgetItem* itemPostureAcceleration = new QTreeWidgetItem(rootItem, QStringList("PostureAcceleration"));
	QTreeWidgetItem* itemPostureDeceleration = new QTreeWidgetItem(rootItem, QStringList("PostureDeceleration"));

	QLineEdit* lineEditVelocity = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);
	QLineEdit* lineEditAcceleration = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);
	QLineEdit* lineEditDeceleration = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);
	QLineEdit* lineEditPostureVelocity = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);
	QLineEdit* lineEditPostureAcceleration = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);
	QLineEdit* lineEditPostureDeceleration = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);

	m_treeWidget->setItemWidget(itemVelocity, 1, lineEditVelocity);
	m_treeWidget->setItemWidget(itemAcceleration, 1, lineEditAcceleration);
	m_treeWidget->setItemWidget(itemDeceleration, 1, lineEditDeceleration);
	m_treeWidget->setItemWidget(itemPostureVelocity, 1, lineEditPostureVelocity);
	m_treeWidget->setItemWidget(itemPostureAcceleration, 1, lineEditPostureAcceleration);
	m_treeWidget->setItemWidget(itemPostureDeceleration, 1, lineEditPostureDeceleration);

	/*comboBox切换事件*/
	connect(comboBoxDynamic, SIGNAL(currentTextChanged(const QString&)), this, SLOT(SlotOnComboBoxItemChanged(const QString&)));

	RefreshDynamicParameter(comboBoxDynamic->currentText(), rootItem);
}

void CEditParameter::AddOverlapParameter(const QString& strText)
{
	QTreeWidgetItem *rootItem = new QTreeWidgetItem(m_treeWidget);
	rootItem->setText(0, "Ovl");
	CComboBoxWithTree_Old* comboBoxOverlap = GetComboBoxOverlap();
	comboBoxOverlap->setCurrentText(strText);
	comboBoxOverlap->SetTreeWidgetItem(rootItem);

	m_treeWidget->setItemWidget(rootItem, 1, comboBoxOverlap);

	QComboBox* comboBoxTransitionMode = new QComboBox;
	comboBoxTransitionMode->addItem(CParameterManager::STR_TRANSITION_MODE_ABSOLUTE);
	comboBoxTransitionMode->addItem(CParameterManager::STR_TRANSITION_MODE_RELATIVE);

	QTreeWidgetItem* itemVelocity = new QTreeWidgetItem(rootItem, QStringList("Mode"));
	QTreeWidgetItem* itemAcceleration = new QTreeWidgetItem(rootItem, QStringList("Parameter"));

	QLineEdit* lineEditTransitionParamter = new CLineEditWithRegExpAndKeyboard("0", CRegExpManager::STR_REG_FLOAT);

	m_treeWidget->setItemWidget(itemVelocity, 1, comboBoxTransitionMode);
	m_treeWidget->setItemWidget(itemAcceleration, 1, lineEditTransitionParamter);

	/*comboBox切换事件*/
	connect(comboBoxOverlap, SIGNAL(currentTextChanged(const QString&)), this, SLOT(SlotOnComboBoxItemChanged(const QString&)));

	RefreshOverlapParameter(comboBoxOverlap->currentText(), rootItem);
}

CComboBoxWithTree_Old* CEditParameter::GetComboBoxPosition()
{
	CComboBoxWithTree_Old* comboBox = new CComboBoxWithTree_Old;
	CValue* value = CValue::GetInstance();

	QStringList strList;
	TVariate::SET collection;
	TVariateManager::GetInstance()->GetCollectionScollUp(collection, m_strScope, CSymbol::TYPE_POSITION);
	for (auto var:collection)
	{
		strList.push_back(var->GetName());
	}
	//m_interpreterAdapter->GetPositionListFromEnclosingScope(strList,m_strScope);

	for each (auto var in strList)
	{
		comboBox->addItem(var);
	}

	return comboBox;
}

CComboBoxWithTree_Old* CEditParameter::GetComboBoxDynamic()
{
	CComboBoxWithTree_Old* comboBox = new CComboBoxWithTree_Old;

	QStringList strList;
	//m_interpreterAdapter->GetDynamicListFromEnclosingScope(strList, m_strScope);

	TVariate::SET collection;
	TVariateManager::GetInstance()->GetCollectionScollUp(collection, m_strScope, CSymbol::TYPE_DYNAMIC);
	for (auto var:collection)
	{
		strList.push_back(var->GetName());
	}
	for each (auto var in strList)
	{
		comboBox->addItem(var);
	}

	return comboBox;
}

CComboBoxWithTree_Old* CEditParameter::GetComboBoxOverlap()
{
	CComboBoxWithTree_Old* comboBox = new CComboBoxWithTree_Old;

	//QStringList strList;
	TVariate::SET collection;
	TVariateManager::GetInstance()->GetCollectionScollUp(collection, m_strScope, CSymbol::TYPE_OVERLAP);
	//m_interpreterAdapter->GetOverlapListFromEnclosingScope(strList, m_strScope);

	for each (auto var in collection)
	{
		comboBox->addItem(var->GetName());
	}

	return comboBox;
}


void CEditParameter::RefreshPositionParameter(QString strName,QTreeWidgetItem* widgetItem)
{
	/*若不存在该变量*/
	if (strName.size()<=0)
	{
		return;
	}
	/*添加当前item下的所有LineEidt控件*/
	QList<QLineEdit*> listLineEdit;
	for (int i = 0; i < widgetItem->childCount(); ++i)
	{
		listLineEdit.append(static_cast<QLineEdit*>(m_treeWidget->itemWidget(widgetItem->child(i), 1)));
	}

	CValue* value = CValue::GetInstance();

	tAxesAllPositions vecDouble = static_cast<TPosition*>(TVariateManager::GetInstance()
		->GetVariateSrollUp(CScreenProject::GetInstance()->GetOpenedFileName(), strName))
		->GetValue();;
	//m_interpreterAdapter->GetPositionValueFromEnclosingScope(CScreenProject::GetInstance()->GetOpenedFileName(), strName.toStdString(), vecDouble);

	//TPosition::TYPE_POSITION position = static_cast<TPosition*>(TVariateManager::GetInstance()
	//	->GetVariateSrollUp(CScreenProject::GetInstance()->GetOpenedFileName(), strName))
	//	->GetValue();
	//for (int i = 0; i < position.size();++i)
	//{
	//	vecDouble.m_AxisPosition[i] = position[i];
	//}

	///*更新字符*/
	//CValue::TYPE_POSITION vecDouble = iterValue->second;
	for (int i = 0; i < listLineEdit.size(); ++i)
	{
		listLineEdit.at(i)->setText(QString::number(vecDouble.m_AxisPosition[i]));
	}
}

void CEditParameter::RefreshDynamicParameter(QString strName, QTreeWidgetItem* widgetItem)
{
	/*若不存在该变量*/
	if (strName.size() <= 0)
	{
		return;
	}
	/*添加当前item下的所有LineEidt控件*/
	QList<QLineEdit*> listLineEdit;
	for (int i = 0; i < widgetItem->childCount(); ++i)
	{
		listLineEdit.append(static_cast<QLineEdit*>(m_treeWidget->itemWidget(widgetItem->child(i), 1)));
	}

	CValue* value = CValue::GetInstance();


	CValue::TYPE_DYNAMIC dynamic = static_cast<TDynamic*>(TVariateManager::GetInstance()
		->GetVariateSrollUp(CScreenProject::GetInstance()->GetOpenedFileName(), strName))
		->GetValue();

	///*更新字符*/
	//CValue::TYPE_DYNAMIC dynamic = iterValue->second;
	listLineEdit.at(0)->setText(QString::number(dynamic.m_Velocity));
	listLineEdit.at(1)->setText(QString::number(dynamic.m_Acceleration));
	listLineEdit.at(2)->setText(QString::number(dynamic.m_Deceleration));
	listLineEdit.at(3)->setText(QString::number(dynamic.m_PostureVelocity));
	listLineEdit.at(4)->setText(QString::number(dynamic.m_PostureAcceleration));
	listLineEdit.at(5)->setText(QString::number(dynamic.m_PostureDeceleration));
}

void CEditParameter::RefreshOverlapParameter(QString strName, QTreeWidgetItem* widgetItem)
{
	/*若不存在该变量*/
	if (strName.size() <= 0)
	{
		return;
	}
	/*添加当前item下的所有控件*/
	QComboBox* comboBox = static_cast<QComboBox*>(m_treeWidget->itemWidget(widgetItem->child(0), 1));
	QLineEdit* lineEdit = static_cast<QLineEdit*>(m_treeWidget->itemWidget(widgetItem->child(1), 1));

	CValue* value = CValue::GetInstance();

	
	//m_interpreterAdapter->GetOverlapValueFromEnclosingScope(CScreenProject::GetInstance()->GetOpenedFileName(), strName.toStdString(), overlap);
	CValue::TYPE_OVERLAP overlap=static_cast<TOverlap*>(TVariateManager::GetInstance()->GetVariateSrollUp(CScreenProject::GetInstance()->GetOpenedFileName(), strName))->GetValue();
	///*若未找到*/
	//if (iterValue == value->m_mapScopeOverlap.end())
	//{
	//	return;
	//}

	///*更新字符*/
	//CValue::TYPE_OVERLAP overlap = iterValue->second;

	if (overlap.m_TransitionMode == CValue::TYPE_TRANSITION_MODE::kTransitionModeAbsolute)
	{
		comboBox->setCurrentText(CParameterManager::STR_TRANSITION_MODE_ABSOLUTE);
	}
	else
	{
		comboBox->setCurrentText(CParameterManager::STR_TRANSITION_MODE_RELATIVE);
	}

	lineEdit->setText(QString::number(overlap.m_TransitionParameter));
}

void CEditParameter::ChangeToMovl()
{
	QTreeWidgetItem* itemHead = new QTreeWidgetItem(m_treeWidget, QStringList("MOVL Pos, Dyn, Ovl"));
	AddPositionParameter(m_strListParameterName.at(0));
	AddDynamicParameter(m_strListParameterName.at(1));
	AddOverlapParameter(m_strListParameterName.at(2));
}

void CEditParameter::ChangeToMovc()
{
	QTreeWidgetItem* itemHead = new QTreeWidgetItem(m_treeWidget, QStringList("MOVL Pos, Pos, Dyn, Ovl"));
	AddPositionParameter(m_strListParameterName.at(0));
	AddPositionParameter(m_strListParameterName.at(1));
	AddDynamicParameter(m_strListParameterName.at(2));
	AddOverlapParameter(m_strListParameterName.at(3));
}
