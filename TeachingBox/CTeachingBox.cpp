#include "stdafx.h"
#include "CTeachingBox.h"
#include "QBoxLayout"
#include "CRefreshDateTime.h"
#include "UserManager.h"
#include "CInterpreterManager.h"
#include "CInterpreterAdapter.h"
#include "CInterprerterState.h"
#include "CWarningManager.h"
#include "CTcpManager.h"
#include "CTcpSpecialCommand.h"
#include "CInterpreterTcpAdapter.h"

CTeachingBox::CTeachingBox(QWidget *parent)
	: QWidget(parent)
{
	Init();
}

CTeachingBox::~CTeachingBox()
{
	m_timerRefreshDateTime->stop();
	delete(m_timerRefreshDateTime);
	delete(m_thrdRefreshDateTime);
	delete(m_thrdInterpreterManager);
	delete(m_thrdIntepreter);
}

CTeachingBox* CTeachingBox::GetInstance()
{
	return CSingleTon<CTeachingBox>::GetInstance();
}

bool CTeachingBox::IsCreated()
{
	return CSingleTon<CTeachingBox>::IsCreated();
}

void CTeachingBox::Init()
{
	InitLayout();
	InitSignalSlots();
	InitThread();
	SlotChangeToScreenSetting();
	SlotOnButtonModelClicked();

	CInterpreterAdapter::GetInstance();	/*初始化部分参数*/
	CTcpManager::GetInstance();	/*初始化net，用于在程序启动时建立连接*/
}

/*************************************************
//  Function: 		init
//  Description: 	初始化示教盒参数，包括示教盒大小，控件,布局等
//  Calls:
//  Called By: 		TeachingBox()
//  Parameter:      void
//  Return: 		void
//  Others: 		// 其它说明
*************************************************/
void CTeachingBox::InitLayout()
{

	this->resize(BOX_WIDTH, BOX_HEIGHT);

	/*主屏幕*/
	m_screen = CScreen::GetInstance();
	m_layoutScreenMain = CScreenMain::GetInstance(); /*获取屏幕主界面*/

	/*示教盒左侧控件*/
	/*功能按钮*/
	m_btnU = new CButton(QStringLiteral("U"));
	m_btnSetting = new CButton(QStringLiteral("Setting"));
	m_btnVariate = new CButton(QStringLiteral("Variate"));
	m_btnProject = new CButton(QStringLiteral("Project"));
	m_btnProgram = new CButton(QStringLiteral("Program"));
	m_btnCoordinate = new CButton(QStringLiteral("Coordinate"));
	m_btnWarning = new CButton(QStringLiteral("Warning"));

	/*指示灯文本*/
	m_lbRun = new CLabel(QStringLiteral("RUN"));
	m_lbError = new CLabel(QStringLiteral("ERR"));
	m_lbPro = new CLabel(QStringLiteral("PRO"));
	m_lbMot = new CLabel(QStringLiteral("MOT"));

	/*指示灯*/
	m_clRun = new CLabelLight;
	m_clError = new CLabelLight;
	m_clPro = new CLabelLight;
	m_clMot = new CLabelLight;

	/*指示灯布局*/
	QHBoxLayout* layoutRun = new QHBoxLayout;
	layoutRun->addWidget(m_lbRun);
	layoutRun->addWidget(m_clRun);
	layoutRun->setStretch(0, 2);
	layoutRun->setStretch(1, 1);
	
	QHBoxLayout* layoutError = new QHBoxLayout;
	layoutError->addWidget(m_lbError);
	layoutError->addWidget(m_clError);
	layoutError->setStretch(0,2);
	layoutError->setStretch(1, 1);

	QHBoxLayout* layoutPro = new QHBoxLayout;
	layoutPro->addWidget(m_lbPro);
	layoutPro->addWidget(m_clPro);
	layoutPro->setStretch(0, 2);
	layoutPro->setStretch(1, 1);

	QHBoxLayout*layoutMot = new QHBoxLayout;
	layoutMot->addWidget(m_lbMot);
	layoutMot->addWidget(m_clMot);
	layoutMot->setStretch(0, 2);
	layoutMot->setStretch(1, 1);

	QVBoxLayout* layoutLight = new QVBoxLayout;
	layoutLight->addLayout(layoutRun);
	layoutLight->addLayout(layoutError);
	layoutLight->addLayout(layoutPro);
	layoutLight->addLayout(layoutMot);

	layoutLight->addStretch(0);

	/*左侧按钮*/
	QVBoxLayout* layoutLeftControl = new QVBoxLayout;
	layoutLeftControl->addWidget(m_btnU);
	layoutLeftControl->addWidget(m_btnSetting);
	layoutLeftControl->addWidget(m_btnVariate);
	layoutLeftControl->addWidget(m_btnProject);
	layoutLeftControl->addWidget(m_btnProgram);
	layoutLeftControl->addWidget(m_btnCoordinate);
	layoutLeftControl->addWidget(m_btnWarning);

	/*左侧总布局*/
	QHBoxLayout* layoutLeft = new QHBoxLayout;
	layoutLeft->addLayout(layoutLight);
	layoutLeft->addLayout(layoutLeftControl);
	
	layoutLeft->setStretch(0, 2);
	layoutLeft->setStretch(1, 1);

	/*示教盒上侧控件*/
	m_modelButtonGroup = new QButtonGroup;
	m_btnMode1 = new QRadioButton;
	m_btnMode2 = new QRadioButton;
	m_btnMode3 = new QRadioButton;
	m_btnScram = new CButton(QStringLiteral("Scram"));

	/*上侧布局一*/
	m_modelButtonGroup->addButton(m_btnMode1, AUTO_EXECUTE);
	m_modelButtonGroup->addButton(m_btnMode2, MANUAL_EXECUTE);
	m_modelButtonGroup->addButton(m_btnMode3, STEP_EXECUTE);

	QHBoxLayout* layoutTop1 = new QHBoxLayout;
	layoutTop1->addStretch();
	layoutTop1->addWidget(m_btnMode1);
	layoutTop1->addWidget(m_btnMode2);
	layoutTop1->addWidget(m_btnMode3);
	layoutTop1->addStretch();

	m_btnMode1->setChecked(true);

	/*上侧布局二*/
	QHBoxLayout* layoutTop2 = new QHBoxLayout;
	layoutTop2->addStretch();
	layoutTop2->addWidget(m_btnScram);
	layoutTop2->addStretch();

	/*上侧总布局*/
	QHBoxLayout* layoutTop = new QHBoxLayout;
	layoutTop->addLayout(layoutTop1);
	layoutTop->addLayout(layoutTop2);


	/*示教盒右侧控件*/
	m_btnStop = new CButton(QStringLiteral("Stop"));
	m_btnStart = new CButton(QStringLiteral("Start"));
	m_A1Plus = new CButton(QStringLiteral("+"));
	m_A1Minus = new CButton(QStringLiteral("-"));
	m_A2Plus = new CButton(QStringLiteral("+"));
	m_A2Minus = new CButton(QStringLiteral("-"));
	m_A3Plus = new CButton(QStringLiteral("+"));
	m_A3Minus = new CButton(QStringLiteral("-"));
	m_A4Plus = new CButton(QStringLiteral("+"));
	m_A4Minus = new CButton(QStringLiteral("-"));
	m_A5Plus = new CButton(QStringLiteral("+"));
	m_A5Minus = new CButton(QStringLiteral("-"));
	m_A6Plus = new CButton(QStringLiteral("+"));
	m_A6Minus = new CButton(QStringLiteral("-"));
	m_btn2nd = new CButton(QStringLiteral("2nd"));

	/*右侧布局*/
	QGridLayout* layoutRightTop = new QGridLayout;
	layoutRightTop->addWidget(m_btnStart, 0, 0);
	layoutRightTop->addWidget(m_A1Plus, 1, 0);
	layoutRightTop->addWidget(m_A2Plus, 2, 0);
	layoutRightTop->addWidget(m_A3Plus, 3, 0);
	layoutRightTop->addWidget(m_A4Plus, 4, 0);
	layoutRightTop->addWidget(m_A5Plus, 5, 0);
	layoutRightTop->addWidget(m_A6Plus, 6, 0);
	layoutRightTop->addWidget(m_btn2nd, 7, 0);

	layoutRightTop->addWidget(m_btnStop, 0, 1);
	layoutRightTop->addWidget(m_A1Minus, 1, 1);
	layoutRightTop->addWidget(m_A2Minus, 2, 1);
	layoutRightTop->addWidget(m_A3Minus, 3, 1);
	layoutRightTop->addWidget(m_A4Minus, 4, 1);
	layoutRightTop->addWidget(m_A5Minus, 5, 1);
	layoutRightTop->addWidget(m_A6Minus, 6, 1);

	layoutRightTop->setSpacing(0);

	QVBoxLayout* layoutRight = new QVBoxLayout;
	layoutRight->addLayout(layoutRightTop);
	layoutRight->addLayout(new QHBoxLayout);
	layoutRight->setStretch(0,18);
	layoutRight->setStretch(1,1);
	layoutRight->setSpacing(0);

	/*下侧控件*/
	m_btnF1 = new CButton(QStringLiteral("F1"));
	m_btnF2 = new CButton(QStringLiteral("F2"));
	m_btnMot = new CButton(QStringLiteral("Mot"));
	m_btnRob = new CButton(QStringLiteral("Rob"));
	m_btnJog = new CButton(QStringLiteral("Jog"));
	m_btnFB = new CButton(QStringLiteral("F/B"));
	m_btnStep = new CButton(QStringLiteral("Step"));
	m_btnVMinus = new CButton(QStringLiteral("V-"));
	m_btnVPlus = new CButton(QStringLiteral("V+"));

	/*下侧布局*/
	QHBoxLayout* layoutBottom = new QHBoxLayout;
	layoutBottom->addWidget(m_btnF1);
	layoutBottom->addWidget(m_btnF2);
	layoutBottom->addWidget(m_btnMot);
	layoutBottom->addWidget(m_btnRob);
	layoutBottom->addWidget(m_btnJog);
	layoutBottom->addWidget(m_btnFB);
	layoutBottom->addWidget(m_btnStep);
	layoutBottom->addWidget(m_btnVMinus);
	layoutBottom->addWidget(m_btnVPlus);

	layoutBottom->setSpacing(0);

	/*总布局*/
	QGridLayout* layoutMain = new QGridLayout(this);
	layoutMain->addLayout(layoutTop, 0, 1);
	layoutMain->addLayout(layoutLeft, 1, 0);
	layoutMain->addLayout(layoutRight, 1, 2);
	layoutMain->addLayout(m_screen,1,1);
	layoutMain->addLayout(layoutBottom, 2, 1);

	layoutMain->setColumnStretch(0, BOX_LEFT_WIDTH_RATIO);
	layoutMain->setColumnStretch(1, SCREEN_WIDTH_RATIO);
	layoutMain->setColumnStretch(2, BOX_RIGHT_WIDTH_RATIO);

	layoutMain->setRowStretch(0, BOX_TOP_HEIGHT_RATIO);
	layoutMain->setRowStretch(1, SCREEN_HEIGHT_RATIO);
	layoutMain->setRowStretch(2, BOX_BOTTOM_HEIGHT_RATIO);
}

void CTeachingBox::InitSignalSlots()
{
	/*视图切换*/
	connect(m_btnProject, SIGNAL(clicked()), this, SLOT(SlotChangeToScreenProject()));
	connect(m_btnProgram, SIGNAL(clicked()), this, SLOT(SlotChangeToScreenProgram()));
	connect(m_btnSetting, SIGNAL(clicked()), this, SLOT(SlotChangeToScreenSetting()));
	connect(m_btnVariate, SIGNAL(clicked()), this, SLOT(SlotChangeToScreenVariable()));

	//connect(m_btnStart, SIGNAL(clicked()), this, SLOT(SlotOnButtonStartClicked()));

	connect(m_btnMode1, SIGNAL(clicked()), this, SLOT(SlotOnButtonModelClicked()));
	connect(m_btnMode2, SIGNAL(clicked()), this, SLOT(SlotOnButtonModelClicked()));
	connect(m_btnMode3, SIGNAL(clicked()), this, SLOT(SlotOnButtonModelClicked()));

	connect(m_btnStart, SIGNAL(pressed()), this, SLOT(SlotOnButtonStartPressed()));
	connect(m_btnStart, SIGNAL(released()), this, SLOT(SlotOnButtonStartReleased()));
	connect(m_btnStop, SIGNAL(clicked()), this, SLOT(SlotOnButtonStopClicked()));

	InitSignalSlotMoveAxis();
}

void CTeachingBox::InitSignalSlotMoveAxis()
{
	/*按下*/
	connect(m_A1Plus, SIGNAL(pressed()), this, SLOT(SlotOnButtonPlusAxis1()));
	connect(m_A2Plus, SIGNAL(pressed()), this, SLOT(SlotOnButtonPlusAxis2()));
	connect(m_A3Plus, SIGNAL(pressed()), this, SLOT(SlotOnButtonPlusAxis3()));
	connect(m_A4Plus, SIGNAL(pressed()), this, SLOT(SlotOnButtonPlusAxis4()));
	connect(m_A5Plus, SIGNAL(pressed()), this, SLOT(SlotOnButtonPlusAxis5()));
	connect(m_A6Plus, SIGNAL(pressed()), this, SLOT(SlotOnButtonPlusAxis6()));

	connect(m_A1Minus, SIGNAL(pressed()), this, SLOT(SlotOnButtonMinusAxis1()));
	connect(m_A2Minus, SIGNAL(pressed()), this, SLOT(SlotOnButtonMinusAxis2()));
	connect(m_A3Minus, SIGNAL(pressed()), this, SLOT(SlotOnButtonMinusAxis3()));
	connect(m_A4Minus, SIGNAL(pressed()), this, SLOT(SlotOnButtonMinusAxis4()));
	connect(m_A5Minus, SIGNAL(pressed()), this, SLOT(SlotOnButtonMinusAxis5()));
	connect(m_A6Minus, SIGNAL(pressed()), this, SLOT(SlotOnButtonMinusAxis6()));

	/*松开*/
	connect(m_A1Plus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
	connect(m_A2Plus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
	connect(m_A3Plus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
	connect(m_A4Plus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
	connect(m_A5Plus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
	connect(m_A6Plus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));

	connect(m_A1Minus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
	connect(m_A2Minus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
	connect(m_A3Minus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
	connect(m_A4Minus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
	connect(m_A5Minus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
	connect(m_A6Minus, SIGNAL(released()), this, SLOT(SlotOnButtonStopMoveAxis()));
}

/*初始化线程*/
void CTeachingBox::InitThread()
{
	/*创建线程和定时器*/
	m_thrdRefreshDateTime = new QThread(this);
	m_timerRefreshDateTime = new QTimer(this);
	CRefreshDateTime* workerRefreshDateTime = CRefreshDateTime::GetInstance();

	/*绑定定时器及函数，并开启定时器*/
	connect(m_timerRefreshDateTime, SIGNAL(timeout()), workerRefreshDateTime, SLOT(SlotRefreshDateTime()));
	m_timerRefreshDateTime->start(1000);

	/*将对象移到新线程中，并开启线程*/
	workerRefreshDateTime->moveToThread(m_thrdRefreshDateTime);
	m_thrdRefreshDateTime->start();

	/*解释器线程*/
	m_thrdIntepreter = new QThread(this);

	m_thrdInterpreterManager = new CThreadInterpreterManager(CInterpreterManager::GetInstance());
	m_thrdInterpreterManager->moveToThread(m_thrdIntepreter);
	connect(this, SIGNAL(SignalBeginExecute()), m_thrdInterpreterManager, SLOT(SlotExecuteFromLine()));
	connect(this, SIGNAL(SignalExecuteStep()), m_thrdInterpreterManager, SLOT(SlotExecuteStep()));
	connect(this, SIGNAL(SignalStopExecute()), m_thrdInterpreterManager, SLOT(SlotStopExecute()));
	m_thrdIntepreter->start();
}

void CTeachingBox::SlotChangeToScreenProject()
{
	m_layoutScreenMain->ChangeToScreenProject();
}

void CTeachingBox::SlotChangeToScreenProgram()
{
	m_layoutScreenMain->ChangeToScreenProgram();
}

void CTeachingBox::SlotChangeToScreenSetting()
{
	m_layoutScreenMain->ChangeToScreenService();
}

void CTeachingBox::SlotChangeToScreenVariable()
{
	m_layoutScreenMain->ChangeToScreenVariables();
}

void CTeachingBox::SlotOnButtonStartPressed()
{
	/*若未加载任何程序*/
	if (CScreenProject::GetInstance()->GetLoadedFileNames().size()==0)
	{
		return;
	}

	switch (m_executeType)
	{
	case CTeachingBox::AUTO_EXECUTE:case CTeachingBox::MANUAL_EXECUTE:
	{
		emit SignalBeginExecute();
	}break;
	case CTeachingBox::STEP_EXECUTE:
	{
		emit SignalExecuteStep();
	}break;
	default:
		break;
	}
}

void CTeachingBox::SlotOnButtonStartReleased()
{
	switch (m_executeType)
	{
	case CTeachingBox::MANUAL_EXECUTE:
	{
		StopExecute();
	}
		break;
	default:
		break;
	}
}

void CTeachingBox::SlotOnButtonStopClicked()
{
	StopExecute();
}

void CTeachingBox::SlotOnButtonModelClicked()
{
	switch (m_modelButtonGroup->checkedId())
	{
	case AUTO_EXECUTE:
	{
		m_executeType = AUTO_EXECUTE;
	}break;
	case MANUAL_EXECUTE:
	{
		m_executeType = MANUAL_EXECUTE;
	}break;
	case STEP_EXECUTE:
	{
		m_executeType = STEP_EXECUTE;
	}break;
	}
}

void CTeachingBox::SlotOnButtonPlusAxis1()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandPlusAxis1());
}

void CTeachingBox::SlotOnButtonPlusAxis2()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandPlusAxis2());
}

void CTeachingBox::SlotOnButtonPlusAxis3()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandPlusAxis3());
}

void CTeachingBox::SlotOnButtonPlusAxis4()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandPlusAxis4());
}

void CTeachingBox::SlotOnButtonPlusAxis5()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandPlusAxis5());
}

void CTeachingBox::SlotOnButtonPlusAxis6()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandPlusAxis6());
}

void CTeachingBox::SlotOnButtonMinusAxis1()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandMinusAxis1());
}

void CTeachingBox::SlotOnButtonMinusAxis2()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandMinusAxis2());
}

void CTeachingBox::SlotOnButtonMinusAxis3()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandMinusAxis3());
}

void CTeachingBox::SlotOnButtonMinusAxis4()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandMinusAxis4());
}

void CTeachingBox::SlotOnButtonMinusAxis5()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandMinusAxis5());
}

void CTeachingBox::SlotOnButtonMinusAxis6()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandMinusAxis6());
}

void CTeachingBox::SlotOnButtonStopMoveAxis()
{
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandStopMove());
}

void CTeachingBox::StopExecute()
{
	CInterpreterState::GetInstance()->SetAdmit(false);
	CTcpManager::GetInstance()->SendData(CTcpSpecialCommand::CommandStopExecute());
}
