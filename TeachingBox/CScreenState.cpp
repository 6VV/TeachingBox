#include "stdafx.h"
#include "CScreenState.h"
#include "QBoxLayout"
#include "QTimer"

CScreenState* CScreenState::GetInstance()
{
	return CSingleTon<CScreenState>::GetInstance();
}

bool CScreenState::IsCreated()
{
	return CSingleTon<CScreenState>::IsCreated();
}

CScreenState::CScreenState(QWidget* parent /*= 0*/) :QVBoxLayout(parent)
{
	Init();
}

CScreenState::~CScreenState()
{
}

/*************************************************
//  Function: 		CScreenHeader::Init
//  Description: 	用于各控件及参数的初始化
//  Calls:		 	
//  Called By: 		CScreenHeader::CScreenHeader
//  Parameter:      void
//  Return: 		void
//  Others: 		
*************************************************/
void CScreenState::Init()
{
	InitLayout();
	InitSignalSlot();
}

void CScreenState::InitLayout()
{
	/*初始化第一栏控件*/
	m_lbOperationMode = new CLabel(QStringLiteral("T1"));	/*初始化操作模式*/
	m_lbServo = new CLabel(QStringLiteral("ER20"));			/*初始化伺服*/
	m_lbCoordinate = new CLabel(QStringLiteral("World"));	/*初始化坐标系*/
	m_lbTool = new CLabel(QStringLiteral("DefaultTool"));	/*初始化工具*/
	m_lbSpeedPersent = new CLabel(QStringLiteral("50%"));	/*初始化速度百分比*/
	m_lbLogo = new CLabel(QStringLiteral("EFORT"));			/*初始LOGO*/

	/*第一栏，添加控件并布局*/
	QHBoxLayout* layoutTop = new QHBoxLayout;
	layoutTop->addWidget(m_lbOperationMode);
	layoutTop->addWidget(m_lbServo);
	layoutTop->addWidget(m_lbCoordinate);
	layoutTop->addWidget(m_lbTool);
	layoutTop->addWidget(m_lbSpeedPersent);
	layoutTop->addWidget(m_lbLogo);

	layoutTop->setStretch(0, 2);
	layoutTop->setStretch(1, 4);
	layoutTop->setStretch(2, 6);
	layoutTop->setStretch(3, 6);
	layoutTop->setStretch(4, 2);
	layoutTop->setStretch(5, 4);

	layoutTop->setSpacing(0);/*各控件间隔为0*/

	/*初始化第二栏控件*/
	m_lbProjectAndProgram = new CLabel;
	m_lbProgramState = new CLabel;
	m_lbProgramRunningMode = new CLabel;
	m_lb3 = new CLabel;
	m_lb4 = new CLabel;
	m_lb5 = new CLabel;
	m_lbSpace = new CLabel;
	m_lbSecurityState = new CLabel;
	m_lbUserAuthority = new CLabel(QStringLiteral("16"));
	m_lbCurrentTime = new CLabel;

	/*第二栏，添加控件并布局*/
	QHBoxLayout* layoutCenter = new QHBoxLayout;
	layoutCenter->addWidget(m_lbProjectAndProgram);
	layoutCenter->addWidget(m_lbProgramState);
	layoutCenter->addWidget(m_lbProgramRunningMode);
	layoutCenter->addWidget(m_lb3);
	layoutCenter->addWidget(m_lb4);
	layoutCenter->addWidget(m_lb5);
	layoutCenter->addWidget(m_lbSpace);
	layoutCenter->addWidget(m_lbSecurityState);
	layoutCenter->addWidget(m_lbUserAuthority);
	layoutCenter->addWidget(m_lbCurrentTime);

	layoutCenter->setStretch(0, 12);
	layoutCenter->setStretch(1, 1);
	layoutCenter->setStretch(2, 1);
	layoutCenter->setStretch(3, 1);
	layoutCenter->setStretch(4, 1);
	layoutCenter->setStretch(5, 1);
	layoutCenter->setStretch(6, 1);
	layoutCenter->setStretch(7, 1);
	layoutCenter->setStretch(8, 1);
	layoutCenter->setStretch(9, 4);

	layoutCenter->setSpacing(0);

	/*添加一、二栏*/
	this->addLayout(layoutTop);
	this->addLayout(layoutCenter);

	this->setSpacing(0);

	/*显示时间*/
	QDateTime dateTime = QDateTime::currentDateTime();
	QString time = dateTime.toString("hh:mm:ss");
	m_lbCurrentTime->setText(time);
}

void CScreenState::InitSignalSlot()
{
}


CLabel* CScreenState::GetTimeLabel()
{
	return m_lbCurrentTime;
}

void CScreenState::ChangeUserAuthority(const QString& userAuthority)
{
	m_lbUserAuthority->setText(userAuthority);
}

