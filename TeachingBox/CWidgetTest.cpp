#include "stdafx.h"
#include "CWidgetTest.h"
#include "QPushButton"
#include "QBoxLayout"
#include "CDatabaseManager.h"
#include <vector>
#include "CTestMessagebox.h"
#include "CXmlVariableManager.h"
#include "UserManager.h"


CWidgetTest::CWidgetTest(QWidget* parent/*=0*/) :QWidget(parent)
{
	qDebug() << "create";
	Init();
}

CWidgetTest::~CWidgetTest()
{
	qDebug() << "delete";
}

void CWidgetTest::closeEvent(QCloseEvent *event)
{
	delete(this);
}

void CWidgetTest::SlotOnButtonCreateTableClicked()
{
	//CTestMessagebox::warning(this, "test", "test");
	//CDatabaseManager::GetInstance()->CreateTable();
}

void CWidgetTest::SlotOnButtonInsertTableClicked()
{
	CUser user("Administrator", "pass", CUserManager::AUTHORITY_MAX, CUserManager::LANGUAGE_CHINESE, CUserManager::IDENTITY_ADMINISTRATOR);
	CDatabaseManager::GetInstance()->InsertUserInfo(user);
	

}

void CWidgetTest::SlotOnButtonDeleteTableClicked()
{
	//CDatabaseManager::GetInstance()->DeleteSymbolValue("project1", "program1", "name3");
}

void CWidgetTest::SlotOnButtonSelectTableClicked()
{
	/*CValue::TYPE_MAP_SYMBOL map;

	CDatabaseManager::GetInstance()->SelectSymbolValue("project1","program1",map);

	for each (auto var in map)
	{
		qDebug() << QString::fromStdString(var.first) << QString::number(var.second.scope) << QString::number(var.second.type);
	}*/
}

void CWidgetTest::SlotOnButtonSelectOneClicked()
{

	/*int v1;
	int v2;
	CDatabaseManager::GetInstance()->SelectSymbolValue("project2","program2","name2",v1,v2);
	qDebug() << "ONE:"<<QString::number(v1)<<QString::number(v2);*/
}

void CWidgetTest::SlotOnButtonUpdateClicked()
{

	/*int v1 = 3;
	CDatabaseManager::GetInstance()->UpdateSymbolValue(CValue::SCOPE_GLOBAL,"project2", "program2", "name2", v1);
	CDatabaseManager::GetInstance()->UpdateSymbolValue(CValue::SCOPE_LOCAL, "project1", "program1", "name1","name5", v1+3);*/

}

void CWidgetTest::SlotTest()
{
}

void CWidgetTest::Init()
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	m_btnCreateTable = new QPushButton(QCoreApplication::translate(CLASS_NAME,"create table"));
	m_btnInsertTable = new QPushButton(QCoreApplication::translate(CLASS_NAME,"insert value"));
	m_btnDeleteTable = new QPushButton(QCoreApplication::translate(CLASS_NAME,"delete value"));
	m_btnSelectTable = new QPushButton(QCoreApplication::translate(CLASS_NAME,"select value"));
	m_btnSelectOne = new QPushButton(QCoreApplication::translate(CLASS_NAME, "select one"));
	m_btnUpdateTable = new QPushButton(QCoreApplication::translate(CLASS_NAME, "update value"));
	m_btnTest = new QPushButton(QCoreApplication::translate(CLASS_NAME, "test"));

	layout->addWidget(m_btnCreateTable);
	layout->addWidget(m_btnInsertTable);
	layout->addWidget(m_btnDeleteTable);
	layout->addWidget(m_btnSelectTable);
	layout->addWidget(m_btnSelectOne);
	layout->addWidget(m_btnUpdateTable);
	layout->addWidget(m_btnTest);

	connect(m_btnCreateTable, SIGNAL(clicked()), this, SLOT(SlotOnButtonCreateTableClicked()));
	connect(m_btnInsertTable, SIGNAL(clicked()), this, SLOT(SlotOnButtonInsertTableClicked()));
	connect(m_btnDeleteTable, SIGNAL(clicked()), this, SLOT(SlotOnButtonDeleteTableClicked()));
	connect(m_btnSelectTable, SIGNAL(clicked()), this, SLOT(SlotOnButtonSelectTableClicked()));
	connect(m_btnSelectOne, SIGNAL(clicked()), this, SLOT(SlotOnButtonSelectOneClicked()));
	connect(m_btnUpdateTable, SIGNAL(clicked()), this, SLOT(SlotOnButtonUpdateClicked()));
	connect(m_btnTest, SIGNAL(clicked()), this, SLOT(SlotTest()));
}
