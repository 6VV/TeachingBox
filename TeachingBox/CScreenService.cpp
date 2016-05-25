#include "stdafx.h"
#include "CScreenService.h"
#include "CScreenSetting.h"
#include "UserManager.h"
#include "CScreenNetwork.h"

CScreenService::CScreenService(QWidget* parent/*=0*/) :CScreenMainParent(parent)
{
	Init();
}

CScreenService::~CScreenService()
{

}

void CScreenService::SlotOnButtonSettingClicked()
{
	ChangeToScreenSetting();
}

void CScreenService::SlotOnButtonUserClicked()
{
	ChangeToScreenUser();
}

void CScreenService::SlotOnButtonUserAdminClicked()
{
	ChangeToScreenUserManager();
}

void CScreenService::SlotOnButtonNetworkClicked()
{
	ChangeToScreenNetwork();
}

void CScreenService::SlotUserChanged()
{
	CUserManager* userManager = CUserManager::GetInstance();

	/*若没有用户登录*/
	if (!userManager->IsUserLogin())
	{
		m_btnUserInformation->setEnabled(false);
		m_btnUserAdmin->setEnabled(false);
	}
	else
	{
		/*若为管理员*/
		if (userManager->GetCurrentUser().GetIdentity()==CUserManager::IDENTITY_ADMINISTRATOR)
		{
			m_btnUserInformation->setEnabled(true);
			m_btnUserAdmin->setEnabled(true);
		}
		/*若为普通用户*/
		else
		{
			m_btnUserInformation->setEnabled(true);
			m_btnUserAdmin->setEnabled(false);
		}
	}
	
}

CScreenService* CScreenService::GetInstance()
{
	return CSingleTon<CScreenService>::GetInstance();
}

void CScreenService::RefreshText()
{
	/*底部按钮控件*/
	m_btnSettings->setText(QCoreApplication::translate("CScreenSetting", "Settings"));
	m_btnUserInformation->setText(QCoreApplication::translate("CScreenSetting", "Users"));
	m_btnUserAdmin->setText(QCoreApplication::translate("CScreenSetting", "UserAdmin"));
	m_btnVersion->setText(QCoreApplication::translate("CScreenSetting", "Version"));
	m_btnInfo->setText(QCoreApplication::translate("CScreenSetting", "Info"));
	m_btnNetWork->setText(QCoreApplication::translate("CScreenSetting", "NetWork"));
	m_btnBack->setText(QCoreApplication::translate("CScreenSetting", "Back"));
}

void CScreenService::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();
	SlotUserChanged();
}

void CScreenService::InitLayout()
{
	m_stackedWidget = new QStackedWidget;

	/*初始化按钮*/
	m_btnSettings = new CButton;
	m_btnUserInformation = new CButton;
	m_btnUserAdmin = new CButton;
	m_btnVersion = new CButton;
	m_btnInfo = new CButton;
	m_btnNetWork = new CButton;
	//m_btnBack = new CButton;

	/*将按钮添加到列表中*/
	m_btnList.append(m_btnSettings);
	m_btnList.append(m_btnUserInformation);
	m_btnList.append(m_btnUserAdmin);
	m_btnList.append(m_btnVersion);
	m_btnList.append(m_btnInfo);
	m_btnList.append(m_btnNetWork);
	m_btnList.append(m_btnBack);


	/*添加屏幕下端按钮布局*/
	m_layoutCenter->addWidget(m_stackedWidget);
	m_screenBottom->AddButtonList(m_btnList);

	/*初始化窗口*/
	ChangeToScreenSetting();
}

void CScreenService::InitSignalSlot()
{
	connect(m_btnSettings, SIGNAL(clicked()), this, SLOT(SlotOnButtonSettingClicked()));
	connect(m_btnUserInformation, SIGNAL(clicked()), this, SLOT(SlotOnButtonUserClicked()));
	connect(m_screenSetting, SIGNAL(SignalsUserChanged()), this, SLOT(SlotUserChanged()));
	connect(m_btnUserAdmin, SIGNAL(clicked()), this, SLOT(SlotOnButtonUserAdminClicked()));
	connect(m_btnNetWork, SIGNAL(clicked()), this, SLOT(SlotOnButtonNetworkClicked()));
}

void CScreenService::ChangeToScreenSetting()
{
	if (m_screenSetting==NULL)
	{
		m_screenSetting = CScreenSetting::GetInstance();
		m_stackedWidget->addWidget(m_screenSetting);
	}

	m_stackedWidget->setCurrentWidget(m_screenSetting);
}

void CScreenService::ChangeToScreenUser()
{
	if (m_screenUser==NULL)
	{
		m_screenUser = new CScreenUser(m_stackedWidget);
		m_stackedWidget->addWidget(m_screenUser);
	}

	m_stackedWidget->setCurrentWidget(m_screenUser);
}

void CScreenService::ChangeToScreenUserManager()
{
	if (m_screenUserManager==NULL)
	{
		m_screenUserManager = new CScreenUserManager(m_stackedWidget);
		m_stackedWidget->addWidget(m_screenUserManager);
	}

	m_stackedWidget->setCurrentWidget(m_screenUserManager);
}

void CScreenService::ChangeToScreenNetwork()
{
	if (m_screenNetwork==NULL)
	{
		m_screenNetwork = new CScreenNetWork(m_stackedWidget);
		m_stackedWidget->addWidget(m_screenNetwork);
	}
	m_stackedWidget->setCurrentWidget(m_screenNetwork);
}

