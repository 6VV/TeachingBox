#include "stdafx.h"
#include "CScreenSetting.h"
#include "QLabel"
#include "QLine"
#include "QAbstractSpinBox"
#include "QCoreApplication"
#include "CSingleTon.h"
#include "CKeyBoard.h"
#include "UserManager.h"
#include "CWarningManager.h"
#include "CScreenState.h"
#include "qlogging.h"
#include "CDialogLockScreen.h"

CScreenSetting::CScreenSetting(QWidget* parent/*=0*/) :QWidget(parent)
{
	Init();
}

CScreenSetting::~CScreenSetting()
{
	
}

/*初始化*/
void CScreenSetting::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();

	//////////////////////////////////////////////////////////////////////////
	/*仅为方便测试*/
	CUser user("Administor", "123", CUserManager::AUTHORITY_MAX, CUserManager::LANGUAGE_CHINESE, CUserManager::IDENTITY_ADMINISTRATOR);
	CUserManager::GetInstance()->SetCurrentUser(user);
	UpdateState();
	//////////////////////////////////////////////////////////////////////////
}

void CScreenSetting::InitSignalSlot()
{
	connect(m_cmbLanguage, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotChangeLanguage(int)));
	connect(m_cmbUser, SIGNAL(activated(int)), this, SLOT(SlotLogin(int)));
	connect(m_btnLogout, SIGNAL(clicked()), this, SLOT(SlotLogout()));
	connect(m_checkboxControlAuthority, SIGNAL(stateChanged(int)), this, SLOT(SlotControlAuthorityChanged(int)));
	connect(m_btnLockScreen, SIGNAL(clicked()), this, SLOT(SlotOnButtonLockScreenClicked()));
}

/*初始化布局*/
void CScreenSetting::InitLayout()
{


	/*创建四个窗口*/
	CreateWidgetLogin();
	CreateWidgetSystemSettings();
	CreateWidgetRobotControlAuthority();
	CreateWidgetLockScreenSetting();

	/*创建总布局*/
	QGridLayout* gridlayoutMain = new QGridLayout(this);

	gridlayoutMain->addWidget(m_groupboxLogin, 0, 0);
	gridlayoutMain->addWidget(m_groupboxSystemSettings, 0, 1);
	gridlayoutMain->addWidget(m_groupboxRobotControlAuthority, 1, 0);
	gridlayoutMain->addWidget(m_groupboxDispaySettings, 1, 1);

	gridlayoutMain->setRowStretch(0, 1);
	gridlayoutMain->setRowStretch(1, 1);
	gridlayoutMain->setColumnStretch(0, 1);
	gridlayoutMain->setColumnStretch(1, 1);
	gridlayoutMain->setSpacing(5);
	gridlayoutMain->setMargin(5);

}

/*创建登录窗口*/
void CScreenSetting::CreateWidgetLogin()
{
	/*新窗口*/
	m_groupboxLogin = new QGroupBox;

	/*布局*/
	QVBoxLayout* layoutLogin = new QVBoxLayout(m_groupboxLogin);
	QHBoxLayout* layoutUser = new QHBoxLayout;
	QHBoxLayout* layoutLevel = new QHBoxLayout;
	QHBoxLayout* layoutLogout = new QHBoxLayout;

	layoutLogin->addStretch(0);
	layoutLogin->addLayout(layoutUser);
	layoutLogin->addStretch(0);
	layoutLogin->addLayout(layoutLevel);
	layoutLogin->addStretch(0);
	layoutLogin->addLayout(layoutLogout);
	layoutLogin->addStretch(0);

	/*创建窗口控件*/
	m_lbUser = new QLabel;
	m_lbAuthority = new QLabel;
	m_lbAuthorityValue = new QLabel("1");
	m_cmbUser = new QComboBox;
	m_btnLogout = new QPushButton;

	/*添加控件并布局*/
	layoutUser->addWidget(m_lbUser);
	layoutUser->addWidget(m_cmbUser);
	layoutUser->setStretch(0, 1);
	layoutUser->setStretch(1, 1);

	layoutLevel->addWidget(m_lbAuthority);
	layoutLevel->addWidget(m_lbAuthorityValue);
	layoutLevel->setStretch(0, 1);
	layoutLevel->setStretch(1, 1);

	layoutLogout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
	layoutLogout->addWidget(m_btnLogout);
	layoutLogout->setStretch(0, 1);
	layoutLogout->setStretch(1, 1);

	/*设置控件样式*/
	m_lbAuthorityValue->setAlignment(Qt::AlignRight | Qt::AlignCenter);
	m_lbAuthorityValue->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

}

/*创建系统设置窗口*/
void CScreenSetting::CreateWidgetSystemSettings()
{
	/*新窗口*/
	m_groupboxSystemSettings = new QGroupBox;

	/*布局*/
	QVBoxLayout* layoutSystemSettings = new QVBoxLayout(m_groupboxSystemSettings);
	QHBoxLayout* layoutLanguage = new QHBoxLayout;
	QHBoxLayout* layoutDate = new QHBoxLayout;
	QHBoxLayout* layoutTime = new QHBoxLayout;

	layoutSystemSettings->addStretch(0);
	layoutSystemSettings->addLayout(layoutLanguage);
	layoutSystemSettings->addStretch(0);
	layoutSystemSettings->addLayout(layoutDate);
	layoutSystemSettings->addStretch(0);
	layoutSystemSettings->addLayout(layoutTime);
	layoutSystemSettings->addStretch(0);

	/*创建窗口控件并布局*/
	m_lbLanguage = new QLabel;
	m_lbDate = new QLabel;
	m_lbTime = new QLabel;
	m_cmbLanguage = new QComboBox;
	m_lbDateValue = new QLabel;
	m_lbTimeValue = new QLabel;

	layoutLanguage->addWidget(m_lbLanguage);
	layoutLanguage->addWidget(m_cmbLanguage);
	layoutLanguage->setStretch(0, 1);
	layoutLanguage->setStretch(1, 1);
	
	layoutDate->addWidget(m_lbDate);
	layoutDate->addWidget(m_lbDateValue);
	layoutDate->setStretch(0, 1);
	layoutDate->setStretch(1, 1);
	
	layoutTime->addWidget(m_lbTime);
	layoutTime->addWidget(m_lbTimeValue);
	layoutTime->setStretch(0, 1);
	layoutTime->setStretch(1, 1);

	/*为控件添加内容*/
	m_cmbLanguage->addItem(QCoreApplication::translate(CLASS_NAME, "English"));
	m_cmbLanguage->addItem(QCoreApplication::translate(CLASS_NAME, "Chinese"));

	/*设置控件样式*/
	m_lbDateValue->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	m_lbTimeValue->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
}

/*创建机器人控制权限窗口*/
void CScreenSetting::CreateWidgetRobotControlAuthority()
{
	/*创建窗口*/
	m_groupboxRobotControlAuthority = new QGroupBox;

	/*布局*/
	QVBoxLayout* layoutRobotControlAuthority = new QVBoxLayout(m_groupboxRobotControlAuthority);
	QHBoxLayout* layoutControlAuthority = new QHBoxLayout;
	QHBoxLayout* layoutDevice = new QHBoxLayout;
	QHBoxLayout* layoutDeviceIp = new QHBoxLayout;

	layoutRobotControlAuthority->addStretch(0);
	layoutRobotControlAuthority->addLayout(layoutControlAuthority);
	layoutRobotControlAuthority->addStretch(0);
	layoutRobotControlAuthority->addLayout(layoutDevice);
	layoutRobotControlAuthority->addStretch(0);
	layoutRobotControlAuthority->addLayout(layoutDeviceIp);
	layoutRobotControlAuthority->addStretch(0);

	/*创建窗口控件并布局*/
	m_lbControlAuthority = new QLabel;
	m_checkboxControlAuthority = new QCheckBox;
	m_checkboxControlAuthority->setChecked(false);
	m_lbDevice = new QLabel;
	m_lbDeviceValue = new QLabel;
	m_lbDeviceIp = new QLabel;
	m_lbDeviceIpValue = new QLabel;

	layoutControlAuthority->addWidget(m_lbControlAuthority);
	layoutControlAuthority->addWidget(m_checkboxControlAuthority);
	layoutControlAuthority->setStretch(0, 1);
	layoutControlAuthority->setStretch(1, 1);
	layoutControlAuthority->setAlignment(m_checkboxControlAuthority, Qt::AlignRight);

	layoutDevice->addWidget(m_lbDevice);
	layoutDevice->addWidget(m_lbDeviceValue);
	layoutDevice->setStretch(0, 1);
	layoutDevice->setStretch(1, 1);

	layoutDeviceIp->addWidget(m_lbDeviceIp);
	layoutDeviceIp->addWidget(m_lbDeviceIpValue);
	layoutDeviceIp->setStretch(0, 1);
	layoutDeviceIp->setStretch(1, 1);

	/*设置控件样式*/
	m_lbDeviceValue->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	m_lbDeviceIpValue->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
}

/*创建显示设置窗口*/
void CScreenSetting::CreateWidgetLockScreenSetting()
{
	/*创建窗口*/
	m_groupboxDispaySettings = new QGroupBox;

	/*布局*/
	QVBoxLayout* layoutDisplaySetting = new QVBoxLayout(m_groupboxDispaySettings);
	QHBoxLayout* layoutDisplayLock = new QHBoxLayout;

	layoutDisplaySetting->addStretch(0);
	layoutDisplaySetting->addLayout(layoutDisplayLock);
	layoutDisplaySetting->addStretch(0);

	/*创建窗口控件并布局*/
	m_lbLockScreen = new QLabel;
	m_btnLockScreen = new QPushButton;

	layoutDisplayLock->addWidget(m_lbLockScreen);
	layoutDisplayLock->addWidget(m_btnLockScreen);
	layoutDisplayLock->setStretch(0, 1);
	layoutDisplayLock->setStretch(1, 1);
}

void CScreenSetting::UpdateState()
{
	/*更新用户列表*/
	UpdateUserList();

	CUserManager* userManager = CUserManager::GetInstance();

	m_lbDeviceValue->setText("My Device");
	m_lbDeviceIpValue->setText(userManager->GetIpAddress());

	/*设置控制权限*/
	m_checkboxControlAuthority->setChecked(userManager->GetControlAuthority());

	/*若不存在登录用户*/
	if (!userManager->IsUserLogin())
	{
		m_cmbUser->setCurrentText("");
		m_lbAuthorityValue->setText("0");	/*设置权限为0*/
        CScreenState::GetInstance()->ChangeUserAuthority(QString("0"));	/*状态栏显示*/
		m_checkboxControlAuthority->setChecked(false);
		m_checkboxControlAuthority->setEnabled(false);
		return;
	}

	CUser user = userManager->GetCurrentUser();
	m_cmbUser->setCurrentText(user.GetName());
	m_checkboxControlAuthority->setEnabled(true);
	m_checkboxControlAuthority->setChecked(true);
	/*更新权限*/
	m_lbAuthorityValue->setText(QString::number(user.GetAuthority()));
	CScreenState::GetInstance()->ChangeUserAuthority(QString::number(user.GetAuthority()));

	/*更新语言*/
	QString userLanguage = user.GetLanguage();
	if (userLanguage == CUserManager::LANGUAGE_ENGLISH)
	{
		m_cmbLanguage->setCurrentIndex(LANGUAGE_INDEX_ENGLISH);
	}
	else if (userLanguage == CUserManager::LANGUAGE_CHINESE)
	{
		m_cmbLanguage->setCurrentIndex(lANGUAGE_INDEX_CHINESE);
	}

	/*控制权限*/
	m_checkboxControlAuthority->setCheckable(true);
	m_checkboxControlAuthority->setChecked(userManager->GetControlAuthority());
}

void CScreenSetting::UpdateUserList()
{
	/*为用户列表控件添加内容*/
	m_cmbUser->clear();
	QStringList strListUserName;
	CUserManager::GetInstance()->GetUserList(strListUserName);
	m_cmbUser->addItem("");
    for (auto var : strListUserName)
	{
		m_cmbUser->addItem(var);
	}
}

void CScreenSetting::changeEvent(QEvent *event)
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

void CScreenSetting::showEvent(QShowEvent *event)
{
	UpdateState();
}

/*************************************************
//  Function: 		RefreshText
//  Description: 	刷新控件文本，用于国际化支持
//  Calls:
//  Called By: 		changeEvent
//  Parameter:      void
//  Return: 		void
//  Others: 		本函数重写父类函数，父类函数在CScreenMainParent中定义
*************************************************/
void CScreenSetting::RefreshText()
{
	/*登录窗口*/
	m_groupboxLogin->setTitle(QCoreApplication::translate("CScreenSetting", "Login"));
	m_lbUser->setText(QCoreApplication::translate("CScreenSetting", "User"));
	m_lbAuthority->setText(QCoreApplication::translate("CScreenSetting", "Level"));
	m_btnLogout->setText(QCoreApplication::translate("CScreenSetting", "Logout"));

	/*系统设置窗口*/
	m_groupboxSystemSettings->setTitle(QCoreApplication::translate("CScreenSetting", "System settings"));
	m_lbLanguage->setText(QCoreApplication::translate("CScreenSetting", "Language"));
	m_lbDate->setText(QCoreApplication::translate("CScreenSetting", "Date"));
	m_lbTime->setText(QCoreApplication::translate("CScreenSetting", "time"));
	m_cmbLanguage->setItemText(0,QCoreApplication::translate("CScreenSetting", "English"));
	m_cmbLanguage->setItemText(1,QCoreApplication::translate("CScreenSetting", "Chinese"));

	/*机器人控制权限窗口*/
	m_groupboxRobotControlAuthority->setTitle(QCoreApplication::translate("CScreenSetting", "Robot control authority"));
	m_lbControlAuthority->setText(QCoreApplication::translate("CScreenSetting", "Control authority"));
	m_lbDevice->setText(QCoreApplication::translate("CScreenSetting", "Device"));
	m_lbDeviceIp->setText(QCoreApplication::translate("CScreenSetting", "Device IP"));

	/*显示设置窗口*/
	m_groupboxDispaySettings->setTitle(QCoreApplication::translate("CScreenSetting", "Display setting"));
	m_lbLockScreen->setText(QCoreApplication::translate("CScreenSetting", "Displaylock"));
	m_btnLockScreen->setText(QCoreApplication::translate("CScreenSetting", "Lock"));


	/*更新日期时间*/
	QDateTime dateTime = QDateTime::currentDateTime();
	QString date = dateTime.toString("yyyy-MM-dd");
	QString time = dateTime.toString("hh:mm:ss");
	m_lbDateValue->setText(date);
	m_lbTimeValue->setText(time);
}


/*更改显示语言*/
void CScreenSetting::SlotChangeLanguage(int languageIndex)
{
	static QTranslator* translator = NULL;
	if (translator != NULL)
	{
		qApp->removeTranslator(translator);
		delete(translator);
		translator = NULL;
	}
	translator = new QTranslator;

	switch (languageIndex)
	{
	/*英文*/
	case LANGUAGE_INDEX_ENGLISH:{}break;

	/*中文*/
	case lANGUAGE_INDEX_CHINESE:
	{
		if (translator->load(":/Translate/teachingbox_zh.qm"))
		{
			qApp->installTranslator(translator);
		}
	}break;

	default:
		break;
	}
}


void CScreenSetting::SlotLogin(int index)
{
	if (m_cmbUser->currentText().size()==0)
	{
		return;
	}
	CKeyboard::GetInstance()->SetKeyboardInterface(this);
	CKeyboard::GetInstance()->show();
}

void CScreenSetting::SlotLogout()
{
	CUserManager::GetInstance()->ClearCurrentUser();
	m_cmbUser->setCurrentText("");
	UpdateState();
	emit SignalsUserChanged();
}

void CScreenSetting::SlotControlAuthorityChanged(int state)
{
	CUserManager::GetInstance()->SetControlAuthority(state);
}

void CScreenSetting::SlotOnButtonLockScreenClicked()
{
	CDialogLockScreen* dialog = new CDialogLockScreen(3);
	dialog->show();
}

CScreenSetting* CScreenSetting::GetInstance()
{
	return CSingleTon<CScreenSetting>::GetInstance();
}

bool CScreenSetting::IsCreated()
{
	return CSingleTon<CScreenSetting>::IsCreated();
}

QLabel* CScreenSetting::GetDateLabel()
{
	return m_lbDateValue;
}

QLabel* CScreenSetting::GetTimeLabel()
{
	return m_lbTimeValue;
}

QComboBox* CScreenSetting::GetUserComboBox()
{
	return m_cmbUser;
}

void CScreenSetting::KeyboardEdit()
{
	QString strInputPassword = CKeyboard::GetInstance()->GetCurrentText();
	CUser user;
	CUserManager::GetInstance()->GetUserInfo(m_cmbUser->currentText(), user);

	/*若密码正确*/
	if (strInputPassword == user.GetPassword())
	{
		/*记录当前用户*/
		CUserManager::GetInstance()->SetCurrentUser(user);
		UpdateState();
		emit SignalsUserChanged();
	}
	else
	{
		m_cmbUser->setCurrentText("");
		CWarningManager::GetInstance()->Warning(this, "Wrong Password");
	}
}

