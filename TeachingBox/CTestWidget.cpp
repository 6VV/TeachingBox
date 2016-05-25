#include "stdafx.h"
#include "CTestWidget.h"
#include "CInterpreterAdapter.h"
#include "CVariableTreeItemManager.h"
#include "CFileManager.h"


CTestWiget::CTestWiget(QWidget* parent/*=0*/)
{
	Init();
}

CTestWiget::~CTestWiget()
{

}

void CTestWiget::Init()
{
	//CInterpreterAdapter::GetInstance();
	//CVariableTreeItemManager itemManager;

	//m_treeWidget = new CTreeWidgetMonitorChangeDatabase(this);
	//m_treeWidget->setColumnCount(2);

	//QTreeWidgetItem* item = new QTreeWidgetItem;
	//m_treeWidget->addTopLevelItem(item);
	//itemManager.GetAllPositionTreeWidgetItemInExactScope(QString::fromStdString(CScope::STR_SCOPE_SYNERGIC), item, m_treeWidget);
	//m_treeWidget->addTopLevelItem(GetTreeWidgetItem());
	QVBoxLayout* layout = new QVBoxLayout(this);
	QPushButton* button = new QPushButton();
	m_lineEditOldName = new QLineEdit();
	m_lineEditNewName = new QLineEdit;
	m_label = new QLabel;
	layout->addWidget(m_lineEditOldName);
	layout->addWidget(m_lineEditNewName);
	layout->addWidget(button);
	layout->addWidget(m_label);

	connect(button, SIGNAL(clicked()), this, SLOT(SlotOnButtonClicked()));

	
}

void CTestWiget::SlotOnButtonClicked()
{
	CFileManager file;
	QString strOldName = m_lineEditOldName->text();
	QString strNewName = m_lineEditNewName->text();
	file.RenameFileText("", strOldName, strNewName);
}

