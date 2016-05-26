#include "stdafx.h"
#include "CScreenVariables.h"
#include "CWidgetButtonListVertical.h"
#include "CScreenMain.h"
#include "CWarningManager.h"
#include "CVariableTreeItemManager.h"
#include "CScreenProject.h"
#include "CFileManager.h"
#include "CKeyBoard.h"
#include "QRegExp"
#include "TVariateManager.h"
#include "TPosition.h"
#include "CTcpSpecialCommand.h"




CScreenVariables* CScreenVariables::GetInstance()
{
	return CSingleTon<CScreenVariables>::GetInstance();
}

CScreenVariables::CScreenVariables(QWidget* parent/*=0*/) : CScreenMainParent(parent)
{
	Init();
}

CScreenVariables::~CScreenVariables()
{

}

void CScreenVariables::SetVariableInterface(CVariableInterface* variableInterface)
{
	m_variableInterface = variableInterface;
}

void CScreenVariables::SetConfirmButtonState(bool state)
{
	m_btnConfirm->setEnabled(state);
}

void CScreenVariables::KeyboardEdit()
{
	QString strNewName = CKeyboard::GetInstance()->GetCurrentText();
	QString strOldName = m_treeWidget->currentItem()->text(0);
	QString strType = m_treeWidget->currentItem()->text(1);

	/*若变量名不合规范*/
	QRegExp reg(CRegExpManager::STR_REG_STRING_NAME);
	if (!reg.exactMatch(strNewName))
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::WrongName);
		return;
	}

	QTreeWidgetItem* currentItem = m_treeWidget->currentItem();

	/*若变量名未改变*/
	if (currentItem->text(0)==strNewName)
	{
		return;
	}

	/*作用域*/
	QString strScope = currentItem->parent()->text(2);

	/*已存在新变量名*/
	//bool isExist = TVariateManager::GetInstance()->IsExistVariateScrollUp(strScope, strNewName);
	//bool isExist = CInterpreterAdapter::GetInstance()->IsExist(strScope, strNewName.toStdString());
	if (TVariateManager::GetInstance()->IsExistVariateScrollUp(strScope, strNewName))
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::AlreadyExitVariable);
		return;
	}

	/*更新树形控件*/
	currentItem->setText(0, strNewName);

	/*更新数据库、内存*/
	CInterpreterAdapter::GetInstance()->UpdateVariableName(strOldName, strNewName, strScope, strType);

	/*更新文件文本*/
	CFileManager fileManager;

	/*若为全局变量*/
	if (strScope ==CScope::STR_SCOPE_GLOBAL)
	{
		QString strProjectLoaded = CScreenProject::GetInstance()->GetLoadedProjectNameInDatabase();

        for(auto projectName : m_screenProject->GetAllFiles().keys())
		{
			/*若存在同名项目变量*/
			if (m_databaseManager->IsExistVariable(projectName, strOldName))
			{
				continue;
			}
			else
			{
                for (auto fileName : m_screenProject->GetAllFiles()[projectName].keys())
				{
					/*若存在同名程序变量*/
					if (m_databaseManager->IsExistVariable(fileName, strOldName))
					{
						continue;
					}
					else
					{
						fileManager.RenameFileText(m_screenProject->GetAllFiles()[projectName][fileName], strOldName, strNewName);
					}
				}
			}
		}
	}
	/*若为项目变量*/
	else if (strScope==m_screenProject->GetLoadedProjectNameInDatabase())
	{
        for (auto fileScope : m_screenProject->GetAllFiles()[strScope].keys())
		{
			CScope* scope = TVariateManager::GetInstance()->GetRootScope().FindScopeScrollDown(fileScope);

			/*若程序中存在同名变量*/
			if (scope->FindSymbolInSelf(strOldName))
			{
				continue;
			}
			else
			{
				fileManager.RenameFileText(m_screenProject->GetAllFiles()[strScope][fileScope], strOldName, strNewName);
			}
		}
	}
	/*若为程序变量*/
	else
	{
		/*获取程序路径*/
		QString strPath;
		bool isFind = false;
        for (auto mapProject : m_screenProject->GetAllFiles())
		{
            for (auto scope : mapProject.keys())
			{
				if (scope == strScope)
				{
					strPath = mapProject[scope];
					isFind = true;
					break;
				}
			}
			if (isFind)
			{
				break;
			}
		}

		fileManager.RenameFileText(strPath, strOldName, strNewName);
	}
}

void CScreenVariables::changeEvent(QEvent *event)
{
	QWidget::changeEvent(event);

	switch (event->type())
	{
	case QEvent::LanguageChange:
	{
		RefreshText();
	}break;
	default:
		break;
	}
}

void CScreenVariables::resizeEvent(QResizeEvent *event)
{
	m_treeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	m_treeWidget->setColumnWidth(0, m_treeWidget->width() / 2);
}

void CScreenVariables::showEvent(QShowEvent *event)
{
	CScreenMainParent::showEvent(event);

	InitTreeWidgetData();
	InitState();
}

void CScreenVariables::hideEvent(QHideEvent *event)
{
	m_btnConfirm->setEnabled(false);
}

void CScreenVariables::SlotOnButtonVariableClicked()
{
	m_btnListWidgetVariable->SetBottomPosition(m_btnVariable);
	m_btnListWidgetVariable->show();
}

void CScreenVariables::SlotOnButtonNewVariableClicked()
{
	QTreeWidgetItem* item = m_treeWidget->currentItem();

	/*若未选项任何节点*/
	if (item==NULL)
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::PleaseSelectScope);
		return;
	}

	QString strScope;

	QTreeWidgetItem* currentItem = item;

	/*若选择的不为顶部节点*/
	if (item->parent()!=NULL)
	{
		/*获取顶部节点*/
		while (currentItem->parent()!=NULL)
		{
			currentItem = currentItem->parent();
		}
	}

	strScope = currentItem->text(2);

	/*设置新建变量作用域*/
	CScreenNewVariable::GetInstance()->SetScope(strScope);

	CScreenMain::GetInstance()->ChangeToScreenNewVariable();
	m_btnListWidgetVariable->hide();
}

void CScreenVariables::SlotOnButtonCopyVariableClicked()
{
	QTreeWidgetItem* itemCurrent = m_treeWidget->currentItem();

	/*若为无效节点或作用域*/
	if (itemCurrent == NULL && itemCurrent->parent()==NULL)
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::PleaseSelectVariable);
		return;
	}

	/*若不是变量节点*/
	while(itemCurrent->parent()->parent() != NULL)
	{
		itemCurrent = itemCurrent->parent();
	}

	/*复制变量信息*/
	m_varCopied.varName = itemCurrent->text(0);
	m_varCopied.varType = itemCurrent->text(1);
	m_varCopied.varScope = itemCurrent->text(2);
	m_varCopied.copyOperator = CopyOperator::COPY;

	/*允许粘贴变量*/
	m_btnPasteVariable->setEnabled(true);
}

void CScreenVariables::SlotOnButtonPasteVariableClicked()
{
	/*若未选择任何变量*/
	if (m_varCopied.varName.isEmpty())
	{
		return;
	}

	/*若目标节点为无效节点*/
	QTreeWidgetItem* itemCurrent = m_treeWidget->currentItem();
	if (itemCurrent==NULL)
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::PleaseSelectScope);
		return;
	}

	/*若原作用域不存在该变量*/
	//bool isExist = CInterpreterAdapter::GetInstance()->IsSymbolExist(m_varCopied.varScope, m_varCopied.varName.toStdString());
	
	if (!TVariateManager::GetInstance()->IsExistVariateScrollUp(m_varCopied.varScope, m_varCopied.varName))
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::NotFindVariable);
		m_btnPasteVariable->setEnabled(false);
		return;
	}

	QString strNewScope = GetScope(itemCurrent);

	/*新的变量名*/
	QString newName;

	/*若为复制，重命名*/
	if (m_varCopied.copyOperator==CopyOperator::COPY)
	{
		newName =  m_varCopied.varName + "_copy";
	}
	/*若为剪切，判断是否允许删除*/
	else
	{
		newName = m_varCopied.varName;
		if (CheckVariableState(m_varCopied.varName,m_varCopied.varType,m_varCopied.varScope)==USED)
		{
			return;
		}
	}

	/*若目标作用域内已存在变量*/
	//bool isExist = CInterpreterAdapter::GetInstance()->IsSymbolExist(strNewScope, newName.toStdString());
	if (TVariateManager::GetInstance()->IsExistVariateScrollUp(strNewScope,newName))
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::AlreadyExitVariable);
		return;
	}

	/*粘贴变量*/
	PasteNewVariable(newName, strNewScope);

	/*添加树节点*/
	QTreeWidgetItem* itemNew = new QTreeWidgetItem(itemCurrent,QStringList{newName,m_varCopied.varType,strNewScope});
	CVariableTreeItemManager manager;
	manager.GetVariableTreeWidgetItem(newName, m_varCopied.varType, strNewScope, itemNew, m_treeWidget);

	/*若为剪切*/
	if (m_varCopied.copyOperator==CopyOperator::CUT)
	{
		DeleteVariableWithoutCheck(m_varCopied.varName, m_varCopied.varType, m_varCopied.varScope);
		m_varCopied.itemCut->parent()->removeChild(m_varCopied.itemCut);
	}
}

void CScreenVariables::SlotOnButtonDeleteVariableClicked()
{
	QTreeWidgetItem* itemCurrent = m_treeWidget->currentItem();

	/*若为无效节点或顶部节点*/
	if (itemCurrent == NULL || itemCurrent->parent() == NULL)
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::PleaseSelectVariable);
		return;
	}
	/*若为变量节点*/
	else if (itemCurrent->parent()->parent()==NULL)
	{
		QString strScope = itemCurrent->parent()->text(2);
		m_treeItemNeedToDelete = itemCurrent;

		if (strScope == CScope::STR_SCOPE_SYNERGIC
			|| strScope == CScope::STR_SCOPE_SYSTEM)
		{
			CWarningManager::GetInstance()->Warning(this, CWarningManager::ForbidDelete);
			return;
		}
		/*若为全局变量*/
		if (strScope == CScope::STR_SCOPE_GLOBAL)
		{
			DeleteGlobalVariable(itemCurrent->text(0), itemCurrent->text(1), strScope);
		}
		/*若为项目变量*/
		else if (strScope==CScreenProject::GetInstance()->GetLoadedProjectNameInDatabase())
		{
			DeleteProjectVariable(itemCurrent->text(0), itemCurrent->text(1), strScope);
		}
		/*若为局部变量*/
		else
		{
			DeleteLocalVariable(itemCurrent->text(0), itemCurrent->text(1), strScope);
		}
	}
	/*若为其它节点（值节点）*/
	else
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::PleaseSelectVariable);
		return;
	}
}

void CScreenVariables::SlotOnButtonCutVariableClicked()
{
	QTreeWidgetItem* itemCurrent = m_treeWidget->currentItem();

	/*若为无效节点或作用域*/
	if (itemCurrent == NULL && itemCurrent->parent() == NULL)
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::PleaseSelectVariable);
		return;
	}

	/*若不是变量节点*/
	while (itemCurrent->parent()->parent() != NULL)
	{
		itemCurrent = itemCurrent->parent();
	}

	/*复制变量信息*/
	m_varCopied.varName = itemCurrent->text(0);
	m_varCopied.varType = itemCurrent->text(1);
	m_varCopied.varScope = itemCurrent->text(2);
	m_varCopied.copyOperator = CopyOperator::CUT;
	m_varCopied.itemCut = itemCurrent;

	/*允许粘贴变量*/
	m_btnPasteVariable->setEnabled(true);

}

void CScreenVariables::SlotOnButtonRenameVariableClicked()
{
	QTreeWidgetItem* item = m_treeWidget->currentItem();

	/*若为无效节点或作用域*/
	if (item==NULL || item->parent()==NULL)
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::PleaseSelectVariable);
		return;
	}

	CKeyboard* keyboard = CKeyboard::GetInstance();
	keyboard->SetKeyboardInterface(this);
	keyboard->show();
}

void CScreenVariables::SlotOnButtonConfirmClicked()
{
	QTreeWidgetItem* item = m_treeWidget->currentItem();

	if (item == NULL || item->parent() == NULL || m_variableInterface == NULL)
	{
		return;
	}

	m_variableInterface->SelectVariable(item->text(0));
	m_variableInterface = NULL;
	Back();
}

void CScreenVariables::SlotOnButtonTeachClicked()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandGetPosition());
}

void CScreenVariables::SlotOnResivePosition(tAxesAllPositions& position)
{
	QTreeWidgetItem* currentItem = m_treeWidget->currentItem();
	/*若为无效节点或作用域*/
	if (currentItem == NULL || currentItem->parent() == NULL)
	{
		return;
	}
	if (currentItem->text(1) == CParameterManager::STR_TYPE_POSITION)
	{
		QString scope = GetScope(currentItem);
		QString name = currentItem->text(0);
		QVector<double> vecPosition;
		for (int i = 0; i < TPosition::AXIS_SIZE;++i)
		{
			vecPosition.push_back(position.m_AxisPosition[i]);
		}
		TVariateManager::GetInstance()->Update(scope, name, TPosition(scope, name, vecPosition));
		//CInterpreterAdapter::GetInstance()->UpdatePositionValue(scope, name.toStdString(), name.toStdString(), position);
		(static_cast<CLineEditWithClickedSignal*>(m_treeWidget->itemWidget(currentItem->child(0), 1)))->setText(QString::number(position.m_AxisPosition[0]));
		(static_cast<CLineEditWithClickedSignal*>(m_treeWidget->itemWidget(currentItem->child(1), 1)))->setText(QString::number(position.m_AxisPosition[1]));
		(static_cast<CLineEditWithClickedSignal*>(m_treeWidget->itemWidget(currentItem->child(2), 1)))->setText(QString::number(position.m_AxisPosition[2]));
		(static_cast<CLineEditWithClickedSignal*>(m_treeWidget->itemWidget(currentItem->child(3), 1)))->setText(QString::number(position.m_AxisPosition[3]));
		(static_cast<CLineEditWithClickedSignal*>(m_treeWidget->itemWidget(currentItem->child(4), 1)))->setText(QString::number(position.m_AxisPosition[4]));
		(static_cast<CLineEditWithClickedSignal*>(m_treeWidget->itemWidget(currentItem->child(5), 1)))->setText(QString::number(position.m_AxisPosition[5]));
	}
}

void CScreenVariables::Init()
{
	m_value = CValue::GetInstance();
	m_databaseManager = CDatabaseManager::GetInstance();
	m_screenProject = CScreenProject::GetInstance();

	InitLayout();
	InitSignalSlot();
	RefreshText();
}

void CScreenVariables::InitLayout()
{
	/*初始化树形控件*/
	m_treeWidget = new CTreeWidgetWithMonitor;
	m_treeWidget->setColumnCount(3);
	m_treeWidget->hideColumn(2);

	/*初始化底部按钮*/
	m_btnVariable = new CButton;
	m_btnTeach = new CButton;
	m_btnClearUnusedVariable = new CButton;
	m_btnCheck = new CButton;
	m_btnConfirm = new CButton;

	/*添加底部按钮*/
	m_btnList.append(m_btnVariable);
	m_btnList.append(m_btnTeach);
	m_btnList.append(m_btnClearUnusedVariable);
	m_btnList.append(m_btnCheck);
	m_btnList.append(m_btnConfirm);
	CButton* btnNull1 = new CButton;
	btnNull1->setEnabled(false);
	m_btnList.append(btnNull1);
	m_btnList.append(m_btnBack);

	/*初始化按钮列表*/
	QList<CButton*> btnListVariable;
	m_btnDeleteVariable = new CButton;
	m_btnPasteVariable = new CButton;
	m_btnCopyVariable = new CButton;
	m_btnCutVaribale = new CButton;
	m_btnRenameVariable = new CButton;
	m_btnNewVariable = new CButton;

	btnListVariable.append(m_btnDeleteVariable);
	btnListVariable.append(m_btnPasteVariable);
	btnListVariable.append(m_btnCopyVariable);
	btnListVariable.append(m_btnCutVaribale);
	btnListVariable.append(m_btnRenameVariable);
	btnListVariable.append(m_btnNewVariable);

	m_btnListWidgetVariable = new CWidgetButtonListVertical(btnListVariable, this);

	/*添加界面控件*/
	m_layoutCenter->addWidget(m_treeWidget);
	m_screenBottom->AddButtonList(m_btnList);
}

void CScreenVariables::InitSignalSlot()
{
	connect(m_btnVariable, SIGNAL(clicked()), this, SLOT(SlotOnButtonVariableClicked()));	/*变量按钮按下时*/

	connect(m_btnNewVariable, SIGNAL(clicked()), this, SLOT(SlotOnButtonNewVariableClicked()));			/*新建变量*/
	connect(m_btnCopyVariable, SIGNAL(clicked()), this, SLOT(SlotOnButtonCopyVariableClicked()));		/*复制变量*/
	connect(m_btnPasteVariable, SIGNAL(clicked()), this, SLOT(SlotOnButtonPasteVariableClicked()));		/*粘贴按钮按下*/
	connect(m_btnDeleteVariable, SIGNAL(clicked()), this, SLOT(SlotOnButtonDeleteVariableClicked()));	/*删除按钮按下*/
	connect(m_btnCutVaribale, SIGNAL(clicked()), this, SLOT(SlotOnButtonCutVariableClicked()));			/*剪切按钮按下*/
	connect(m_btnRenameVariable, SIGNAL(clicked()), this, SLOT(SlotOnButtonRenameVariableClicked()));	/*重命名按钮按下*/

	connect(m_btnConfirm, SIGNAL(clicked()), this, SLOT(SlotOnButtonConfirmClicked()));				/*确认按钮按下*/
	connect(m_btnTeach, SIGNAL(clicked()), this, SLOT(SlotOnButtonTeachClicked()));	/*示教按钮按下*/

	connect(CCommandRemoteParser::GetInstance(), SIGNAL(SignalOnResivePosition(tAxesAllPositions&)), this, SLOT(SlotOnResivePosition(tAxesAllPositions&)));
}

void CScreenVariables::InitState()
{
	/*清空复制变量信息*/
	m_varCopied.Clear();

	/*禁止粘贴变量*/
	m_btnPasteVariable->setEnabled(false);

	/*禁止确认按钮*/
	m_btnConfirm->setEnabled(false);
}

void CScreenVariables::InitTreeWidgetData()
{
	m_treeWidget->clear();

	QTreeWidgetItem* itemSystem = new QTreeWidgetItem(QStringList{ 
		CScope::STR_SCOPE_SYSTEM, 
		"",
		CScope::STR_SCOPE_SYSTEM });
	QTreeWidgetItem* itemSynergic = new QTreeWidgetItem(QStringList{ 
		CScope::STR_SCOPE_SYNERGIC,
		"", 
		CScope::STR_SCOPE_SYNERGIC });
	QTreeWidgetItem* itemGlobal = new QTreeWidgetItem(QStringList{ 
		CScope::STR_SCOPE_GLOBAL,
		"",
		CScope::STR_SCOPE_GLOBAL });

	m_treeWidget->addTopLevelItem(itemSystem);
	m_treeWidget->addTopLevelItem(itemSynergic);
	m_treeWidget->addTopLevelItem(itemGlobal);

	CInterpreterAdapter::GetInstance();

	InitTreeWidgetDataFromScope(itemSystem, CScope::STR_SCOPE_SYSTEM.toStdString());
	InitTreeWidgetDataFromScope(itemSynergic, CScope::STR_SCOPE_SYNERGIC.toStdString());
	InitTreeWidgetDataFromScope(itemGlobal, CScope::STR_SCOPE_GLOBAL.toStdString());

	CScreenProject* projectManager = CScreenProject::GetInstance();

	/*若已加载项目*/
	if (projectManager->IsLoadProject())
	{
		QString strProject = projectManager->GetLoadedProjectNameInDatabase();
		QTreeWidgetItem* itemProject = new QTreeWidgetItem(QStringList{ strProject, "", strProject });
		m_treeWidget->addTopLevelItem(itemProject);
		InitTreeWidgetDataFromScope(itemProject, strProject.toStdString());

		/*获取文件节点*/
		QList<QString> strListFileNames;
		strListFileNames.append(projectManager->GetLoadedProjectAllFilesExactName());
		QList<QString> strListFileScopes;
		strListFileScopes=projectManager->GetLoadedProjectAllFilesScopeName();

		for (int i = 0; i < strListFileNames.size();++i)
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(QStringList{ strListFileNames[i], "", strListFileScopes[i] });
			m_treeWidget->addTopLevelItem(item);
			InitTreeWidgetDataFromScope(item, strListFileScopes.at(i).toStdString());
		}

	}
	else
	{
		return;
	}
}

void CScreenVariables::InitTreeWidgetDataFromScope(QTreeWidgetItem* item, const std::string& strScopeName)
{
	CVariableTreeItemManager variableTreeItemManager;

	//variableTreeItemManager.GetAllPositionTreeWidgetItemInExactScope(
	//	QString::fromStdString(strScopeName), item, m_treeWidget);
	//variableTreeItemManager.GetAllDynamicTreeWidgetItemInExactScope(
	//	QString::fromStdString(strScopeName), item, m_treeWidget);


	TVariateManager::GetInstance()->ReadTreeItemCollection(item, m_treeWidget,
		QString::fromStdString(strScopeName), CSymbol::TYPE_POSITION);
	TVariateManager::GetInstance()->ReadTreeItemCollection(item, m_treeWidget,
		QString::fromStdString(strScopeName), CSymbol::TYPE_DYNAMIC);
	TVariateManager::GetInstance()->ReadTreeItemCollection(item, m_treeWidget,
		QString::fromStdString(strScopeName), CSymbol::TYPE_OVERLAP);

	TVariateManager::GetInstance()->ReadTreeItemCollection(item, m_treeWidget,
		QString::fromStdString(strScopeName), CSymbol::TYPE_INTERGER);
	TVariateManager::GetInstance()->ReadTreeItemCollection(item, m_treeWidget,
		QString::fromStdString(strScopeName), CSymbol::TYPE_DOUBLE);
	TVariateManager::GetInstance()->ReadTreeItemCollection(item,m_treeWidget,
		QString::fromStdString(strScopeName), CSymbol::TYPE_BOOL);
	TVariateManager::GetInstance()->ReadTreeItemCollection(item, m_treeWidget,
		QString::fromStdString(strScopeName), CSymbol::TYPE_STRING);
	/*variableTreeItemManager.GetAllDoubleTreeWidgetItemInExactScope(
		QString::fromStdString(strScopeName), item, m_treeWidget);*/
	//variableTreeItemManager.GetAllBoolTreeWidgetItemInExactScope(
	//	QString::fromStdString(strScopeName), item, m_treeWidget);
}

void CScreenVariables::RefreshText()
{
	/*树形控件标题*/
	QStringList strListHeader;
	strListHeader << QCoreApplication::translate(CLASS_NAME, "Variable") << QCoreApplication::translate(CLASS_NAME, "Value");
	m_treeWidget->setHeaderLabels(strListHeader);

	/*底部按钮文本*/
	m_btnVariable->setText(QCoreApplication::translate(CLASS_NAME, "Variable"));
	m_btnTeach->setText(QCoreApplication::translate(CLASS_NAME, "Teach"));
	m_btnClearUnusedVariable->setText(QCoreApplication::translate(CLASS_NAME, "ClearUnused"));
	m_btnCheck->setText(QCoreApplication::translate(CLASS_NAME, "Check"));
	m_btnBack->setText(QCoreApplication::translate(CLASS_NAME, "Back"));
	m_btnConfirm->setText(QCoreApplication::translate(CLASS_NAME, "Confirm"));

	/*变量操作列表*/
	m_btnDeleteVariable->setText(QCoreApplication::translate(CLASS_NAME, "Delete"));
	m_btnCopyVariable->setText(QCoreApplication::translate(CLASS_NAME, "Copy"));
	m_btnPasteVariable->setText(QCoreApplication::translate(CLASS_NAME, "Paste"));
	m_btnCutVaribale->setText(QCoreApplication::translate(CLASS_NAME, "Cut"));
	m_btnRenameVariable->setText(QCoreApplication::translate(CLASS_NAME, "Rename"));
	m_btnNewVariable->setText(QCoreApplication::translate(CLASS_NAME, "New"));
}

void CScreenVariables::PasteNewVariable(const QString& newName, const QString& strNewScope)
{
	///*粘贴变量*/
	//CInterpreterAdapter* interpreterAdapter = CInterpreterAdapter::GetInstance();
	///*若为整数*/
	//if (m_varCopied.varType == CParameterManager::STR_TYPE_INT)
	//{
	//	int value = static_cast<TInteger*>(TVariateManager::GetInstance()
	//		->GetVariate(m_varCopied.varScope, m_varCopied.varName))->GetValue();
	//	TVariateManager::GetInstance()->Add(new TInteger(strNewScope, newName, value));
	//}
	///*若为浮点数*/
	//else if (m_varCopied.varType == CParameterManager::STR_TYPE_DOUBLE)
	//{
	//	static_cast<TDouble*>(TVariateManager::GetInstance()
	//		->GetVariate(m_varCopied.varScope, m_varCopied.varName))->GetValue();
	//	double value = static_cast<TDouble*>(TVariateManager::GetInstance()
	//		->GetVariate(m_varCopied.varScope, m_varCopied.varName))->GetValue();
	//	TVariateManager::GetInstance()->Add(new TDouble(strNewScope, newName, value));
	//	//interpreterAdapter->InsertDoubleValue(strNewScope, CValue::TYPE_PAIR_DOUBLE{ newName.toStdString(), value });
	//}
	///*若为布尔变量*/
	//else if (m_varCopied.varType == CParameterManager::STR_TYPE_BOOL)
	//{
	//	int value = m_value->m_mapScopeBool[m_varCopied.varScope.toStdString()][m_varCopied.varName.toStdString()];
	//	//interpreterAdapter->InsertIntValue(strNewScope, CValue::TYPE_PAIR_BOOL{ newName.toStdString(), value });
	//	TVariateManager::GetInstance()->Add(new TInteger(strNewScope, newName, value));
	//}
	///*若为字符串变量*/
	//else if (m_varCopied.varType == CParameterManager::STR_TYPE_STRING)
	//{
	//	std::string value = m_value->m_mapScopeString[m_varCopied.varScope.toStdString()][m_varCopied.varName.toStdString()];
	//	interpreterAdapter->InsertStringValue(strNewScope, CValue::TYPE_PAIR_STRING{ newName.toStdString(), value });
	//}
	///*若为位置变量*/
	//else if (m_varCopied.varType == CParameterManager::STR_TYPE_POSITION)
	//{
	//	CValue::TYPE_POSITION value = m_value->m_mapScopePosition[m_varCopied.varScope.toStdString()][m_varCopied.varName.toStdString()];
	//	interpreterAdapter->InsertPositionValue(strNewScope, CValue::TYPE_PAIR_POSITION{ newName.toStdString(), value });
	//}
	///*若为动态变量*/
	//else if (m_varCopied.varType == CParameterManager::STR_TYPE_DYNAMIC)
	//{
	//	CValue::TYPE_DYNAMIC value = m_value->m_mapScopeDynamic[m_varCopied.varScope.toStdString()][m_varCopied.varName.toStdString()];
	//	interpreterAdapter->InsertDynamicValue(strNewScope, CValue::TYPE_PAIR_DYNAMIC{ newName.toStdString(), value });
	//}
	///*若为过渡变量*/
	//else if (m_varCopied.varType == CParameterManager::STR_TYPE_DYNAMIC)
	//{
	//	CValue::TYPE_OVERLAP value = m_value->m_mapScopeOverlap[m_varCopied.varScope.toStdString()][m_varCopied.varName.toStdString()];
	//	interpreterAdapter->InsertOverlapValue(strNewScope, CValue::TYPE_PAIR_OVERLAP{ newName.toStdString(), value });
	//}
}


void CScreenVariables::DeleteVariableWithoutCheck(const QString& strName, const QString& strType, const QString& strScope)
{
	/*从符号表，内存，数据库中删除*/
	//CInterpreterAdapter::GetInstance()->DeleteVariable(strName, strType, strScope);
	TVariateManager::GetInstance()->Delete(strScope, strName);

	/*从树形结构上删除*/
	if (m_treeItemNeedToDelete==NULL)
	{
		return;
	}
	m_treeItemNeedToDelete->parent()->removeChild(m_treeItemNeedToDelete);
	m_treeItemNeedToDelete = NULL;
}

void CScreenVariables::DeleteLocalVariable(const QString& strName, const QString& strType, const QString& strScope)
{
	CFileManager fileManager;

	/*获取文件路径*/
	QString filePath = CScreenProject::GetInstance()->GetAllFiles()[CScreenProject::GetInstance()->GetLoadedProjectNameInDatabase()][strScope];

	/*该文件内是否正在使用该变量*/
	if (fileManager.FindVariable(filePath, strName))
	{
		CWarningManager::GetInstance()->Warning(this, "Cann't Delete\nFind In File\n" + strScope);
		return;
	}
	else
	{
		DeleteVariableWithoutCheck(strName, strType, strScope);
	}
}

void CScreenVariables::DeleteProjectVariable(const QString& strName, const QString& strType, const QString& strScope)
{
	/*若变量可以被删除*/
	if (CheckVariableStateInProjectFiles(strName,strType,strScope)==NOT_USED)
	{
		DeleteVariableWithoutCheck(strName, strType, strScope);
	}

}

void CScreenVariables::DeleteGlobalVariable(const QString& strName, const QString& strType, const QString& strScope)
{
	/*若可以被删除*/
	if (CheckVariableStateInGlobalFiles(strName,strType)==NOT_USED)
	{
		DeleteVariableWithoutCheck(strName, strType, strScope);
	}
}

CScreenVariables::VariableUsingState CScreenVariables::CheckVariableState(const QString& strName, const QString& strType, const QString& strScope)
{
	/*若为全局变量*/
	if (strScope ==CScope::STR_SCOPE_GLOBAL)
	{
		return CheckVariableStateInGlobalFiles(strName, strType);
	}
	/*若为项目变量*/
	else if (strScope==m_screenProject->GetLoadedProjectNameInDatabase())
	{
		return CheckVariableStateInProjectFiles(strName, strType, strScope);
	}
	/*若为程序变量*/
	else
	{
		return CheckVariableStateInFile(strName, strType, strScope);
	}
}

CScreenVariables::VariableUsingState CScreenVariables::CheckVariableStateInProjectFiles(const QString& strName, const QString& strType, const QString& strScope)
{
	CFileManager fileManager;
	for each (auto var in CScreenProject::GetInstance()->GetAllFiles()[strScope].keys())
	{
		/*若该程序内存在同名变量*/
		if (TVariateManager::GetInstance()->GetRootScope().FindScopeScrollDown(var)
			->FindSymbolInSelf(strName) != NULL)
		{
			continue;
		}
		else
		{
			/*若程序中正在使用该变量*/
			if (fileManager.FindVariable(CScreenProject::GetInstance()->GetAllFiles()[strScope][var], strName))
			{
				CWarningManager::GetInstance()->Warning(this, "Cann't Delete\nFind In File\n" + var);
				return USED;
			}
			else
			{
				continue;
			}
		}
	}

	return NOT_USED;
}

CScreenVariables::VariableUsingState CScreenVariables::CheckVariableStateInFile(const QString& strName, const QString& strType, const QString& strScope)
{
	/*获取程序路径*/
	QString strPath;
	bool isFind = false;
    for (auto mapProject : m_screenProject->GetAllFiles())
	{
        for (auto scope : mapProject.keys())
		{
			if (scope==strScope)
			{
				strPath = mapProject[scope];
				isFind = true;
				break;
			}
		}
		if (isFind)
		{
			break;
		}
	}

	/*若不存在同名程序变量*/
	if (!m_databaseManager->IsExistVariable(strScope, strName))
	{
		CFileManager fileManager;
		/*若正在被该项目内的程序所使用*/
		if (fileManager.FindVariable(strPath, strName))
		{
			CWarningManager::GetInstance()->Warning(this, "Cann't Delete\nFind In File\n" + strScope);
			return USED;
		}
	}

	return NOT_USED;
}

QString CScreenVariables::GetScope(QTreeWidgetItem* itemCurrent)

{
	/*获取作用域节点*/
	while (itemCurrent->parent() != NULL)
	{
		itemCurrent = itemCurrent->parent();
	}

	return itemCurrent->text(2);
}


CScreenVariables::VariableUsingState CScreenVariables::CheckVariableStateInGlobalFiles(const QString& strName, const QString& strType)
{
	QString strProjectLoaded = CScreenProject::GetInstance()->GetLoadedProjectNameInDatabase();

	/*若当前加载项目中不存在同名变量*/
	if (!strProjectLoaded.isEmpty() && TVariateManager::GetInstance()->GetRootScope().FindScopeScrollDown(strProjectLoaded)
		->FindSymbolInSelf(strName) == NULL)
	{
		/*若正在被该项目内的程序所使用*/
		if (CheckVariableStateInProjectFiles(strName, strType, strProjectLoaded) == USED)
		{
			return USED;
		}
	}

	/*检查其它项目*/
    for (auto projectName : m_screenProject->GetAllFiles().keys())
	{
		/*若为当前项目*/
		if (projectName == strProjectLoaded)
		{
			continue;
		}
		/*若为其它项目*/
		/*若存在同名项目变量*/
		if (m_databaseManager->IsExistVariable(projectName, strName))
		{
			continue;
		}
		else
		{
            for (auto fileName : m_screenProject->GetAllFiles()[projectName].keys())
			{
				/*若存在同名程序变量*/
				if (m_databaseManager->IsExistVariable(fileName, strName))
				{
					continue;
				}
				else
				{
					CFileManager fileManager;
					/*若正在被该项目内的程序所使用*/
					if (fileManager.FindVariable(CScreenProject::GetInstance()->GetAllFiles()[projectName][fileName], strName))
					{
						CWarningManager::GetInstance()->Warning(this, "Cann't Delete\nFind In File\n" + fileName);
						return USED;
					}
				}
			}
		}
	}

	return NOT_USED;
}

