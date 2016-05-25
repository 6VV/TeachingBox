#include "stdafx.h"
#include "CScreen.h"
#include "CTeachingBox.h"

CScreen* CScreen::GetInstance()
{
	return CSingleTon<CScreen>::GetInstance();
}

bool CScreen::IsCreated()
{
	return CSingleTon<CScreen>::IsCreated();
}

int CScreen::GetLeftPosition()
{
	return this->geometry().left() + CTeachingBox::GetInstance()->geometry().left();
}

int CScreen::GetRightPosition()
{
	return this->geometry().right() + CTeachingBox::GetInstance()->geometry().left();
}

int CScreen::GetTopPosition()
{
	return this->geometry().top() + CTeachingBox::GetInstance()->geometry().top();
}

int CScreen::GetBottomPosition()
{
	return this->geometry().bottom() + CTeachingBox::GetInstance()->geometry().top();
}

CScreen::CScreen(QWidget* parent /*= 0*/) :QVBoxLayout(parent)
{
	Init();
}

CScreen::~CScreen()
{

}

void CScreen::Init()
{
	//this->setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	/*初始化各布局*/
	m_layoutHeader = CScreenState::GetInstance();		/*顶端部分*/
	m_layoutWarning = CScreenWarning::GetInstance();	/*警告部分*/
	m_layoutRight = CScreenRight::GetInstance();		/*右侧坐标显示部分*/
	m_layoutScreenMain = CScreenMain::GetInstance();	/*主界面*/

	/*将主界面以及警告部分合成竖直布局*/
	QVBoxLayout* vblCenter = new QVBoxLayout;
	vblCenter->addLayout(m_layoutWarning);
	vblCenter->addLayout(m_layoutScreenMain);

	vblCenter->setStretch(0, SCREEN_WARING_HEIGHT_RATIO);	
	vblCenter->setStretch(1,SCREEN_CENTER_HEIGHT_RATIO);

	vblCenter->setSpacing(0);

	/*将右侧坐标部分添加到布局中*/
	QHBoxLayout* hblCenter = new QHBoxLayout;
	hblCenter->addLayout(vblCenter);
	hblCenter->addLayout(m_layoutRight);

	hblCenter->setStretch(0, SCREEN_CENTER_WIDTH_RATIO);
	hblCenter->setStretch(1, SCREEN_RIGHT_WIDTH_RATIO);

	hblCenter->setSpacing(0);

	/*构建总布局*/
	this->addLayout(m_layoutHeader);
	this->addLayout(hblCenter);


	this->setStretch(0, SCREEN_TOP_HEIGHT_RATIO);
	this->setStretch(1, SCREEN_CENTER_HEIGHT_RATIO+SCREEN_WARING_HEIGHT_RATIO);


	this->setSpacing(0);
}

