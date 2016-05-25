#include "stdafx.h"
#include "CScreenNewMacro.h"
#include "CModelCategory.h"
#include "CModelMacro.h"
#include "CModelCategoryFromXml.h"
#include "CModeMacroFromXml.h"
#include "CScreenMain.h"

CScreenNewMacro::CScreenNewMacro(QWidget* parent/*=0*/) :CScreenMainParent(parent)
{
	Init();
}

CScreenNewMacro::~CScreenNewMacro()
{

}

void CScreenNewMacro::RefreshText()
{

	m_btnConfirm->setText(QCoreApplication::translate(CLASS_NAME, "Confirm"));
	m_btnCancle->setText(QCoreApplication::translate(CLASS_NAME, "Cancle"));
	m_btnBack->setText(QCoreApplication::translate(CLASS_NAME, "Back"));

	CModelMacro::GetInstance()->UpdateData();
}

CScreenNewMacro* CScreenNewMacro::GetInstance()
{
	return CSingleTon<CScreenNewMacro>::GetInstance();
}

void CScreenNewMacro::SlotViewCategoryClicked(QModelIndex indexClicked)
{
	m_grammarManager->UpdateMacroData(indexClicked);
}

void CScreenNewMacro::SlotButtonConfirmClicked()
{
	QModelIndex index = m_viewMacro->currentIndex();
	m_grammarManager->SelectMacro(index);
}

//void CScreenNewMacro::SlotButtonBackClicked()
//{
//	CScreenMain::GetInstance()->ChangeToScreenProgram();
//}

void CScreenNewMacro::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();
}

void CScreenNewMacro::InitLayout()
{
	/*初始化底部按钮*/
	m_btnConfirm = new CButton;
	m_btnCancle = new CButton;
	//m_btnBack = new CButton;
	m_btnNull1 = new CButton;
	m_btnNull2 = new CButton;
	m_btnNull3 = new CButton;
	m_btnNull4 = new CButton;

	m_btnNull1->setEnabled(false);
	m_btnNull2->setEnabled(false);
	m_btnNull3->setEnabled(false);
	m_btnNull4->setEnabled(false);

	m_btnList.append(m_btnConfirm);
	m_btnList.append(m_btnCancle);
	m_btnList.append(m_btnNull1);
	m_btnList.append(m_btnNull2);
	m_btnList.append(m_btnNull3);
	m_btnList.append(m_btnNull4);
	m_btnList.append(m_btnBack);

	/*分类树形视图*/
	m_grammarManager = CGrammarManagerFactory::GetInstance();

	m_viewCategory = new QTreeView;
	m_viewCategory->setModel(CModelCategory::GetInstance());

	m_viewMacro = new QTreeView;
	m_viewMacro->setModel(CModelMacro::GetInstance());

	/*主布局*/
	m_layoutCenter->addWidget(m_viewCategory);
	m_layoutCenter->addWidget(m_viewMacro);
	m_layoutCenter->setStretch(0, 1);
	m_layoutCenter->setStretch(1, 1);
	m_screenBottom->AddButtonList(m_btnList);
}

void CScreenNewMacro::InitSignalSlot()
{
	connect(m_viewCategory, SIGNAL(clicked(QModelIndex)), this, SLOT(SlotViewCategoryClicked(QModelIndex)));
	connect(m_btnConfirm, SIGNAL(clicked()), this, SLOT(SlotButtonConfirmClicked()));
	//connect(m_btnBack, SIGNAL(clicked()), this, SLOT(SlotButtonBackClicked()));
	//connect(m_btnCancle, SIGNAL(clicked()), this, SLOT(SlotButtonBackClicked()));
}
