#include "stdafx.h"
#include "CScreenUser.h"
#include "QStringList"
#include "UserManager.h"
#include "QCheckBox"
#include "QBoxLayout"


CScreenUser::CScreenUser(QWidget* parent/*=0*/) :QWidget(parent)
{
	Init();
}

CScreenUser::~CScreenUser()
{
}

void CScreenUser::showEvent(QShowEvent *event)
{
	RefreshInformation();
}

void CScreenUser::SlotControlAuthorityChanged(int state)
{
	CUserManager::GetInstance()->SetControlAuthority(state);
}

void CScreenUser::Init()
{
	/*创建表格控件*/
	m_tableWidget = new QTableWidget(this);

	/*平均分布各行*/
	m_tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	QHBoxLayout* layoutMain = new QHBoxLayout(this);
	layoutMain->setMargin(0);
	layoutMain->addWidget(m_tableWidget);

	RefreshInformation();
}

void CScreenUser::RefreshInformation()
{
	/*获取当前用户*/
	CUser user = CUserManager::GetInstance()->GetCurrentUser();

	/*设置行列*/
	m_tableWidget->setRowCount(1);
	m_tableWidget->setColumnCount(4);

	/*添加表头*/
	QStringList strList;
	strList << QCoreApplication::translate(CLASS_NAME, "User")
		<< QCoreApplication::translate(CLASS_NAME,"Address")
		<< QCoreApplication::translate(CLASS_NAME, "Authority")
		<< QCoreApplication::translate(CLASS_NAME, "Control");
	m_tableWidget->setHorizontalHeaderLabels(strList);

	/*添加列*/
	/*添加用户名*/
	QTableWidgetItem* itemName = new QTableWidgetItem(user.GetName());

	QTableWidgetItem* itemAddress = new QTableWidgetItem(CUserManager::GetInstance()->GetIpAddress());

	/*添加权限*/
	QTableWidgetItem* itemAuhority = new QTableWidgetItem(QString::number(user.GetAuthority()));
	QCheckBox* checkBox = new QCheckBox;

	/*添加控制权限*/
	checkBox->setChecked(CUserManager::GetInstance()->GetControlAuthority());
	connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(SlotControlAuthorityChanged(int)));

	m_tableWidget->setItem(0, 0, itemName);
	m_tableWidget->setItem(0, 1, itemAddress);
	m_tableWidget->setItem(0, 2, itemAuhority);
	m_tableWidget->setCellWidget(0, 3, checkBox);
}
