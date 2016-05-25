#include "stdafx.h"
#include "CScreenNetwork.h"
#include "QBoxLayout"
#include "QLabel"
#include "UserManager.h"
#include "CKeyBoard.h"
#include "CTcpManager.h"


CScreenNetWork::CScreenNetWork(QWidget* parent/*=0*/) :QWidget(parent)
{
	Init();
}

CScreenNetWork::~CScreenNetWork()
{

}

void CScreenNetWork::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();
}

void CScreenNetWork::InitLayout()
{
	InitLayoutHost();
	InitLayoutNetwork();
	
	/*左侧界面*/
	QVBoxLayout* layoutLeft = new QVBoxLayout;
	layoutLeft->addWidget(m_groupboxHostManager);
	layoutLeft->addWidget(m_groupboxNetwork);

	/*VNC连接界面*/
	m_groupboxVNCConnect = new QGroupBox;
	QVBoxLayout* layoutVNC = new QVBoxLayout(m_groupboxVNCConnect);
	QGridLayout* layoutVNCBottom = new QGridLayout;
	layoutVNC->addLayout(GetLayoutVNCTop());
	layoutVNC->addLayout(GetLayoutVNCBottom());

	/*整体界面*/
	QHBoxLayout* layoutMain = new QHBoxLayout(this);
	layoutMain->addLayout(layoutLeft);
	layoutMain->addWidget(m_groupboxVNCConnect);
}

void CScreenNetWork::InitLayoutHost()
{
	/*主机管理界面*/
	m_groupboxHostManager = new QGroupBox;
	QHBoxLayout* layoutHostManger = new QHBoxLayout(m_groupboxHostManager);
	QVBoxLayout* layoutBtnHost = new QVBoxLayout;

	m_listWidgetHost = new QListWidget;
	m_btnAddHost = new CButton;
	m_btnDeleteHost = new CButton;
	m_btnConnectHost = new CButton;

	layoutBtnHost->addWidget(m_btnAddHost);
	layoutBtnHost->addWidget(m_btnDeleteHost);
	layoutBtnHost->addWidget(m_btnConnectHost);
	layoutBtnHost->addStretch(1);

	layoutHostManger->addWidget(m_listWidgetHost);
	layoutHostManger->addLayout(layoutBtnHost);

	layoutHostManger->setStretch(0, 3);
}

void CScreenNetWork::InitLayoutNetwork()
{
	/*网络设置界面*/
	m_groupboxNetwork = new QGroupBox;
	QGridLayout* layoutNewWork = new QGridLayout(m_groupboxNetwork);

	QLabel* lbLocalIp = new QLabel("Local IP");
	QLabel* lbControlIp = new QLabel("Controler IP");
	m_lbLocalIpValue = new QLabel;
	m_lbLocalIpValue->setText(CUserManager::GetInstance()->GetIpAddress());
	m_lbControlerIpValue = new QLabel;

	layoutNewWork->addWidget(lbLocalIp, 0, 0);
	layoutNewWork->addWidget(m_lbLocalIpValue, 0, 1);
	layoutNewWork->addWidget(lbControlIp, 1, 0);
	layoutNewWork->addWidget(m_lbControlerIpValue, 1, 1);
}

QHBoxLayout* CScreenNetWork::GetLayoutVNCTop()
{
	/*VNC上部界面*/
	QVBoxLayout* layoutBtnVNC = new QVBoxLayout;

	m_listWidgetVNC = new QListWidget;
	m_btnAddVNC = new CButton;
	m_btnDeleteVNC = new CButton;
	m_btnConnectVNC = new CButton;

	layoutBtnVNC->addWidget(m_btnAddVNC);
	layoutBtnVNC->addWidget(m_btnDeleteVNC);
	layoutBtnVNC->addWidget(m_btnConnectVNC);
	layoutBtnVNC->addStretch(1);

	QHBoxLayout* layoutVNCTop = new QHBoxLayout;
	layoutVNCTop->addWidget(m_listWidgetVNC);
	layoutVNCTop->addLayout(layoutBtnVNC);

	layoutVNCTop->setStretch(0, 3);

	return layoutVNCTop;
}

QGridLayout* CScreenNetWork::GetLayoutVNCBottom()
{
	/*VNC底部界面*/
	QLabel* lbName = new QLabel("Name");
	QLabel* lbVisiable = new QLabel("Visiable");
	QLabel* lbFullScreen = new QLabel("Full Screen");
	QLabel* lbContentAdaption = new QLabel("Content Adaption");

	m_lbVNCName = new QLabel;
	m_lbVNCName->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	m_lbVisiable = new QLabel;
	m_lbVisiable->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	m_checkBoxFullScreen = new QCheckBox;
	m_checkBoxContentAdaption = new QCheckBox;

	QGridLayout* layoutVNCBottom = new QGridLayout;
	layoutVNCBottom->addWidget(lbName, 0, 0);
	layoutVNCBottom->addWidget(m_lbVNCName, 0, 1);
	layoutVNCBottom->addWidget(lbVisiable, 1, 0);
	layoutVNCBottom->addWidget(m_lbVisiable, 1, 1);
	layoutVNCBottom->addWidget(lbFullScreen, 2, 0);
	layoutVNCBottom->addWidget(m_checkBoxFullScreen, 2, 1);
	layoutVNCBottom->addWidget(lbContentAdaption, 3, 0);
	layoutVNCBottom->addWidget(m_checkBoxContentAdaption, 3, 1);

	return layoutVNCBottom;
}

void CScreenNetWork::InitSignalSlot()
{
	connect(m_btnAddHost, SIGNAL(clicked()), this, SLOT(SlotOnButtonAddClicked()));
}

void CScreenNetWork::RefreshText()
{
	m_groupboxHostManager->setTitle(QCoreApplication::translate(CLASS_NAME, "Host Manager"));
	m_btnAddHost->setText(QCoreApplication::translate(CLASS_NAME, "Add"));
	m_btnDeleteHost->setText(QCoreApplication::translate(CLASS_NAME, "Delete"));
	m_btnConnectHost->setText(QCoreApplication::translate(CLASS_NAME, "Connect"));

	m_groupboxNetwork->setTitle(QCoreApplication::translate(CLASS_NAME, "Network Settings"));

	m_groupboxVNCConnect->setTitle(QCoreApplication::translate(CLASS_NAME, "VNC Connect"));
	m_btnAddVNC->setText(QCoreApplication::translate(CLASS_NAME, "Add"));
	m_btnDeleteVNC->setText(QCoreApplication::translate(CLASS_NAME, "Delete"));
	m_btnConnectVNC->setText(QCoreApplication::translate(CLASS_NAME, "Connect"));
}

void CScreenNetWork::SlotOnButtonAddClicked()
{
	CKeyboard* keyboard = CKeyboard::GetInstance();
	keyboard->SetKeyboardInterface(this);
	keyboard->show();
}

void CScreenNetWork::KeyboardEdit()
{
	QHostAddress address;
	address.setAddress(CKeyboard::GetInstance()->GetCurrentText());
	CTcpManager::GetInstance()->ConnectAddress(address);
}
