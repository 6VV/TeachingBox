#include "stdafx.h"
#include "CScreenUserManager.h"
#include "QBoxLayout"
#include "UserManager.h"
#include "CDialogEditUser.h"
#include "CWarningManager.h"

CScreenUserManager::CScreenUserManager(QWidget* parent/*=0*/) :QWidget(parent)
{
	Init();
}

CScreenUserManager::~CScreenUserManager()
{

}

void CScreenUserManager::changeEvent(QEvent *event)
{
	QWidget::changeEvent(event);

	switch (event->type())
	{
	case QEvent::LanguageChange:
	{
		RefreshText();
	}
	default:
		break;
	}
}

void CScreenUserManager::showEvent(QShowEvent *event)
{
	RefreshUserTable();
}

void CScreenUserManager::SlotOnButtonAddUserClicked()
{
	CDialogEditUser* dialogEditUser = new CDialogEditUser(this);
	dialogEditUser->setWindowTitle(QCoreApplication::translate(CLASS_NAME, "Add User"));
	dialogEditUser->show();
}

void CScreenUserManager::SlotOnButtonEditUserClicked()
{
	int row=m_tableWidget->currentRow();
	if (row<0)
	{
		CWarningManager::GetInstance()->Warning(this, QCoreApplication::translate(CLASS_NAME,"Please Select An User"));
		return;
	}

	QString userName=m_tableWidget->item(row, 0)->text();
	CUser user;
	CUserManager::GetInstance()->GetUserInfo(userName, user);

	/*若编辑对象的权限与操作者相同，则返回*/
	if (user.GetAuthority()==CUserManager::GetInstance()->GetCurrentUser().GetAuthority())
	{
		CWarningManager::GetInstance()->Warning(this, "Permission Denied");
		return;
	}

	CDialogEditUser* dialogEditUser = new CDialogEditUser(this,user);
	dialogEditUser->setWindowTitle(QCoreApplication::translate(CLASS_NAME, "Edit User"));
	//CUser user=CUserManager::GetInstance()->GetUserInfo()
	//dialogEditUser->SetUser(CUserManager::GetInstance()->getus)
	dialogEditUser->show();
}

void CScreenUserManager::SlotOnButtonDeleteUserClicked()
{
	int row = m_tableWidget->currentRow();
	if (row < 0)
	{
		CWarningManager::GetInstance()->Warning(this, QCoreApplication::translate(CLASS_NAME, "Please Select An User"));
		return;
	}

	/*获取删除的用户名*/
	QString userName = m_tableWidget->item(row, 0)->text();

	/*获取要删除的用户*/
	CUser user;
	CUserManager::GetInstance()->GetUserInfo(userName, user);

	/*用户权限低于当前操作者*/
	switch (QMessageBox::question(this, QCoreApplication::translate(CLASS_NAME, "Delete User"), userName,
		QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok))
	{
	case QMessageBox::Ok:
	{
		CUserManager::GetInstance()->DeleteUser(userName);
		RefreshUserTable();
	}break;
	case QMessageBox::Cancel:
	{
		return;
	}break;
	default:
		break;
	}
}

void CScreenUserManager::SlotTabelWidgetSelectionChanged(QTableWidgetItem* item)
{
	QString strName=m_tableWidget->item(item->row(), 0)->text();
	
	CUser user;
	CUserManager::GetInstance()->GetUserInfo(strName, user);

	/*若选择的用户权限小于当前用户权限*/
	if (user.GetAuthority()<CUserManager::GetInstance()->GetCurrentUser().GetAuthority())
	{
		m_btnDelete->setEnabled(true);
	}
	/*若选择的用户权限大于当前用户权限*/
	else if (user.GetAuthority()>CUserManager::GetInstance()->GetCurrentUser().GetAuthority())
	{
		m_btnDelete->setEnabled(false);
	}
	/*若选择的用户权限等于当前用户权限*/
	else
	{
		/*若当前用户为管理员*/
		if (CUserManager::GetInstance()->GetCurrentUser().GetName()==CUserManager::USER_ADMINISTRAOR)
		{
			/*若选择的用户为管理员*/
			if (user.GetName()==CUserManager::USER_ADMINISTRAOR)
			{
				/*管理员不得删除自身*/
				m_btnDelete->setEnabled(false);
			}
			else
			{
				m_btnDelete->setEnabled(true);
			}
		}
		else
		{
			/*用户不得删除同等权限用户*/
			m_btnDelete->setEnabled(false);
		}
	}
}

void CScreenUserManager::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();
}

void CScreenUserManager::InitLayout()
{

	/*添加用户列表*/
	QHBoxLayout* layoutUser=new QHBoxLayout;
	m_tableWidget = new QTableWidget;
	m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);	/*整行选中*/
	m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	/*禁止编辑*/

	layoutUser->addWidget(m_tableWidget);

	/*平均分布各列*/
	m_tableWidget->setColumnCount(3);
	m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

	/*添加按钮布局*/
	QHBoxLayout* layoutButton=new QHBoxLayout;
	m_btnEdit = new CButton;
	m_btnNew = new CButton;
	m_btnDelete = new CButton;

	layoutButton->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));
	layoutButton->addWidget(m_btnEdit);
	layoutButton->addWidget(m_btnNew);
	layoutButton->addWidget(m_btnDelete);
	layoutButton->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));

	/*总布局*/
	QVBoxLayout* layoutMain=new QVBoxLayout(this);
	layoutMain->setMargin(0);
	layoutMain->addLayout(layoutUser);
	layoutMain->addLayout(layoutButton);
	layoutMain->setStretch(0, 1);
	layoutMain->setStretch(1, 0);
}

void CScreenUserManager::InitSignalSlot()
{
	connect(m_btnEdit, SIGNAL(clicked()), this, SLOT(SlotOnButtonEditUserClicked()));
	connect(m_btnNew, SIGNAL(clicked()), this, SLOT(SlotOnButtonAddUserClicked()));
	connect(m_btnDelete, SIGNAL(clicked()), this, SLOT(SlotOnButtonDeleteUserClicked()));

	connect(m_tableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(SlotTabelWidgetSelectionChanged(QTableWidgetItem*)));
}

void CScreenUserManager::RefreshText()
{
	/*更新列表名*/
	QStringList strHeader;
	strHeader << QCoreApplication::translate(CLASS_NAME, "User")
		<< QCoreApplication::translate(CLASS_NAME, "Authority")
		<< QCoreApplication::translate(CLASS_NAME, "Language");
	m_tableWidget->setHorizontalHeaderLabels(strHeader);

	/*更新按钮名*/
	m_btnEdit->setText(QCoreApplication::translate(CLASS_NAME, "Edit"));
	m_btnNew->setText(QCoreApplication::translate(CLASS_NAME, "New"));
	m_btnDelete->setText(QCoreApplication::translate(CLASS_NAME, "Delete"));
}

void CScreenUserManager::RefreshUserTable()
{
	QStringList listUser;
	int currentUserAuthority = CUserManager::GetInstance()->GetCurrentUser().GetAuthority();

    for  (auto var : CUserManager::GetInstance()->GetUserMap())
	{
		if (var.GetAuthority() <= currentUserAuthority)
		{
			listUser.append(var.GetName());
		}
	}

	m_tableWidget->setRowCount(listUser.size());
	
	for (int i = 0; i < m_tableWidget->rowCount(); i++)
	{
		m_tableWidget->setItem(i, 0, new QTableWidgetItem(listUser.at(i)));
		m_tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(CUserManager::GetInstance()->GetUserMap()[listUser.at(i)].GetAuthority())));
		m_tableWidget->setItem(i, 2, new QTableWidgetItem(CUserManager::GetInstance()->GetUserMap()[listUser.at(i)].GetLanguage()));
	}
}
