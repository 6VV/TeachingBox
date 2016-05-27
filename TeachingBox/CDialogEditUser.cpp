#include "stdafx.h"
#include "CDialogEditUser.h"
#include "QBoxLayout"
#include "QGridLayout"
#include "CRegExpManager.h"
#include "CWarningManager.h"
#include "UserManager.h"
#include "CScreen.h"


CDialogEditUser::CDialogEditUser(CScreenUserManager* screenUserManager)
{
	m_screenUserManager = screenUserManager;
	Init();
}

CDialogEditUser::CDialogEditUser(CScreenUserManager* screenUserManager, CUser& user)
{
	m_screenUserManager = screenUserManager;
	m_user = user;
	Init();
}

CDialogEditUser::~CDialogEditUser()
{
	qDebug() << "delete edit user";
}


void CDialogEditUser::SetUser(const CUser& user)
{
	m_user = user;
}

void CDialogEditUser::changeEvent(QEvent *event)
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

void CDialogEditUser::showEvent(QShowEvent *event)
{
	/*设置固定大小*/
	this->setFixedSize(this->width(), this->height());

	/*移动到屏幕中央*/
	this->move((CScreenMain::GetInstance()->GetLeftPosition() + CScreenMain::GetInstance()->GetRightPosition())/2 + -this->width() / 2,
		CScreen::GetInstance()->GetTopPosition() 
		+ CScreen::GetInstance()->geometry().height()*(SCREEN_HEIGHT_RATIO-SCREEN_BOTTOM_HEIGHT_RATIO)/SCREEN_HEIGHT_RATIO/2
		- this->height() / 2);

	/*若用户名为空*/
	if (m_user.GetName().isEmpty())
	{
		return;
	}

	/*禁用部分控件*/
	DisableWidget();
}

void CDialogEditUser::SlotOnButtonConfirmClicked()
{
	QString name = m_lineEditName->text();
	QString password = m_lineEditPassword->text();
	QString confirmPassword = m_lineEditConfirmPassword->text();
	QString authority = m_comboBoxAuthority->currentText();
	QString language = m_comboBoxLanguage->currentText();
	QString identity;
	if (m_checkBoxIdencity->isChecked())
	{
		identity = CUserManager::IDENTITY_ADMINISTRATOR;
	}
	else
	{
		identity = CUserManager::IDENTITY_NORMAL;
	}

	/*若名字为空*/
	if (name.isEmpty())
	{
		CWarningManager::GetInstance()->Warning(this, "Name is empty");
		return;
	}

	if (password.isEmpty())
	{
		CWarningManager::GetInstance()->Warning(this, "Password is empty");
		return;
	}

	if (password != confirmPassword)
	{
		CWarningManager::GetInstance()->Warning(this, "Password not match");
		return;
	}

	CUser user(name, password, authority.toInt(), language, identity);

	/*若为添加*/
	if (m_user.GetName().isEmpty())
	{
		CUserManager::GetInstance()->InsertUser(user);
	}
	/*若为编辑*/
	else
	{
		CUserManager::GetInstance()->UpdateUser(user.GetName(), user);
	}

	m_screenUserManager->RefreshUserTable();	/*更新表格 */

	delete(this);
}

void CDialogEditUser::SlotOnButtonCancelClicked()
{
	delete(this);
}

void CDialogEditUser::SlotOnLineEditConfirmPasswordTextChanged()
{
	if (m_lineEditConfirmPassword->text().isEmpty())
	{
		return;
	}
	if (m_lineEditConfirmPassword->text()!=m_lineEditPassword->text())
	{
		m_lineEditConfirmPassword->clear();
		CWarningManager::GetInstance()->Warning(this, "Password Not Match");
	}
}

void CDialogEditUser::SlotOnLineEditUserTextChanged()
{
	/*若用户名为空，则返回*/
	if (m_lineEditName->text().isEmpty())
	{
		return;
	}

	CUser::TYPE_MAP_USER& mapUser = CUserManager::GetInstance()->GetUserMap();
	auto iter = mapUser.find(m_lineEditName->text());

	/*若已存在该用户*/
	if (iter!=mapUser.end())
	{
		CWarningManager::GetInstance()->Warning(this, "Already Exist This User");
		m_lineEditName->clear();
		return;
	}
}

void CDialogEditUser::Init()
{
	InitLayout();
	InitUserInfo();
	InitSignalSlot();
	RefreshText();
}

void CDialogEditUser::InitLayout()
{
	this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
	this->setWindowModality(Qt::ApplicationModal);

	///*标题*/
	//m_lbTitle = new QLabel("test");
	//m_lbTitle->setStyleSheet("color:red;font-size:18px");
	//m_lbTitle->setAlignment(Qt::AlignCenter);

	/*文本控件*/
	m_lbName = new QLabel;
	m_lbPassword = new QLabel;
	m_lbConfirmPassword = new QLabel;
	m_lbAuthority = new QLabel;
	m_lbLanguage = new QLabel;
	m_lbIdentity = new QLabel;

	/*操作控件*/
	m_lineEditName = new CLineEditWithRegExpAndKeyboard;	/*用户名*/
	m_lineEditName->SetRegExp(QString(CRegExpManager::STR_REG_NUM_CHAR_UNDERLINE));	/*用户名格式*/

	m_lineEditPassword = new CLineEditWithRegExpAndKeyboard;	/*密码*/
	m_lineEditPassword->setEchoMode(QLineEdit::Password);	/*不显示密码*/
	m_lineEditPassword->SetKeyboardShowMode(CLineEditWithRegExpAndKeyboard::KEYBOARD_NOT_SHOW_TEXT);	/*键盘不显示密码*/

	m_lineEditConfirmPassword = new CLineEditWithRegExpAndKeyboard;
	m_lineEditConfirmPassword->setEchoMode(QLineEdit::Password);
	m_lineEditConfirmPassword->SetKeyboardShowMode(CLineEditWithRegExpAndKeyboard::KEYBOARD_NOT_SHOW_TEXT);	/*键盘不显示密码*/

	CUser user = CUserManager::GetInstance()->GetCurrentUser();	/*获取当前用户*/

	m_comboBoxAuthority = new QComboBox;
	/*添加权限，最大权限不得超过当前用户的权限*/
	for (int i = 1; i <= user.GetAuthority();++i)
	{
		m_comboBoxAuthority->addItem(QString::number(i));
	}

	m_comboBoxLanguage = new QComboBox;
	m_comboBoxLanguage->addItem(CUserManager::LANGUAGE_ENGLISH);
	m_comboBoxLanguage->addItem(CUserManager::LANGUAGE_CHINESE);
	m_comboBoxLanguage->setCurrentText(user.GetLanguage());

	m_checkBoxIdencity = new QCheckBox;

	/*按钮控件*/
	m_btnCancel = new CButton;
	m_btnConfirm = new CButton;

	/*布局*/
	QVBoxLayout* layoutMain = new QVBoxLayout(this);
	QHBoxLayout* layoutTitle = new QHBoxLayout;
	QGridLayout* layoutUser = new QGridLayout;
	QHBoxLayout* layoutButton = new QHBoxLayout;

	//layoutMain->addLayout(layoutTitle);
	layoutMain->addLayout(layoutUser);
	layoutMain->addLayout(layoutButton);

	//layoutTitle->addWidget(m_lbTitle);

	layoutUser->setMargin(10);
	layoutUser->setSpacing(10);

	layoutUser->addWidget(m_lbName, 0, 0);
	layoutUser->addWidget(m_lbPassword, 1, 0);
	layoutUser->addWidget(m_lbConfirmPassword, 2, 0);
	layoutUser->addWidget(m_lbAuthority, 3, 0);
	layoutUser->addWidget(m_lbLanguage, 4, 0);
	layoutUser->addWidget(m_lbIdentity, 5, 0);

	layoutUser->addWidget(m_lineEditName, 0, 1);
	layoutUser->addWidget(m_lineEditPassword, 1, 1);
	layoutUser->addWidget(m_lineEditConfirmPassword, 2, 1);
	layoutUser->addWidget(m_comboBoxAuthority, 3, 1);
	layoutUser->addWidget(m_comboBoxLanguage, 4, 1);
	layoutUser->addWidget(m_checkBoxIdencity, 5, 1);

	layoutButton->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));
	layoutButton->addWidget(m_btnConfirm);
	layoutButton->addWidget(m_btnCancel);
	layoutButton->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));
}

void CDialogEditUser::InitSignalSlot()
{
	connect(m_btnConfirm, SIGNAL(clicked()), this, SLOT(SlotOnButtonConfirmClicked()));
	connect(m_btnCancel, SIGNAL(clicked()), this, SLOT(SlotOnButtonCancelClicked()));

	connect(m_lineEditConfirmPassword, SIGNAL(textChanged(const QString&)), this, SLOT(SlotOnLineEditConfirmPasswordTextChanged()));
	connect(m_lineEditName, SIGNAL(textChanged(const QString&)), this, SLOT(SlotOnLineEditUserTextChanged()));
}

void CDialogEditUser::RefreshText()
{
	m_lbName->setText(QCoreApplication::translate(CLASS_NAME, "User"));
	m_lbPassword->setText(QCoreApplication::translate(CLASS_NAME, "Password"));
	m_lbConfirmPassword->setText(QCoreApplication::translate(CLASS_NAME, "Confirm Password"));
	m_lbAuthority->setText(QCoreApplication::translate(CLASS_NAME, "Authority"));
	m_lbLanguage->setText(QCoreApplication::translate(CLASS_NAME, "Language"));
	m_lbIdentity->setText(QCoreApplication::translate(CLASS_NAME, "Identity"));

	m_btnCancel->setText(QCoreApplication::translate(CLASS_NAME, "Cancel"));
	m_btnConfirm->setText(QCoreApplication::translate(CLASS_NAME, "Confirm"));
}

void CDialogEditUser::DisableWidget()
{
	m_lineEditName->setEnabled(false);

	/*若编辑的用户权限小于当前操作者的用户权限*/
	if (m_user.GetAuthority()<CUserManager::GetInstance()->GetCurrentUser().GetAuthority())
	{
		return;
	}

	/*若编辑的对象为本身*/
	if (m_user.GetName()==CUserManager::GetInstance()->GetCurrentUser().GetName())
	{
		m_comboBoxAuthority->setEnabled(false);
		m_checkBoxIdencity->setEnabled(false);
		return;
	}

}

void CDialogEditUser::InitUserInfo()
{
	/*若存在用户,显示用户信息*/
	m_lineEditName->setText(m_user.GetName());
	m_lineEditPassword->setText(m_user.GetPassword());
	m_lineEditConfirmPassword->setText(m_user.GetPassword());
	m_comboBoxAuthority->setCurrentText(QString::number(m_user.GetAuthority()));
	m_comboBoxLanguage->setCurrentText(m_user.GetLanguage());
	if (m_user.GetIdentity() == CUserManager::IDENTITY_ADMINISTRATOR)
	{
		m_checkBoxIdencity->setChecked(true);
	}
	else
	{
		m_checkBoxIdencity->setChecked(false);
	}
}
