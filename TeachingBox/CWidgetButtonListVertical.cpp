#include "stdafx.h"
#include "CWidgetButtonListVertical.h"
#include "QBoxLayout"
#include "Init.h"
#include "CScreenMain.h"
#include "CTeachingBox.h"


CWidgetButtonListVertical::CWidgetButtonListVertical(QList<CButton*>& btnlist, QWidget* parent /*= 0*/) : QWidget(parent)
{
	this->m_btnList = btnlist;
	Init();

}

CWidgetButtonListVertical::~CWidgetButtonListVertical()
{
	if (m_btnList.size() > 0)
	{
		qDeleteAll(m_btnList);
		m_btnList.clear();
	}
}

void CWidgetButtonListVertical::Init()
{
	this->setWindowFlags(Qt::Popup);
	m_layout = new QVBoxLayout(this);

	for each (CButton* button in m_btnList)
	{
		m_layout->addWidget(button);
	}

	m_layout->setMargin(0);
	m_layout->setSpacing(0);

	RefreshLayout();
}

void CWidgetButtonListVertical::RefreshLayout()
{
	int buttonHeight = CScreenMain::GetInstance()->geometry().height()*SCREEN_BOTTOM_HEIGHT_RATIO / (SCREEN_BOTTOM_HEIGHT_RATIO + SCREEN_CENTER_HEIGHT_RATIO);

	this->resize(CScreenMain::GetInstance()->geometry().width() * 3 / 20, buttonHeight*m_btnList.size());	
}

/*设置窗口相对于底部按钮的位置*/
void CWidgetButtonListVertical::SetBottomPosition(CButton* button)
{
	CScreenMain* screenMain = CScreenMain::GetInstance();
	int positionLeft = button->geometry().left() + screenMain->GetLeftPosition();
	int positionBottom = button->geometry().top() + screenMain->GetTopPosition();
	
	int positionMaxRight = screenMain->GetLeftPosition() + screenMain->geometry().width();
	if (positionLeft + this->width() > positionMaxRight)
	{
		this->move(positionMaxRight - this->width(), positionBottom - this->height());
	}
	else
	{
		this->move(positionLeft, positionBottom - this->height());
	}
}

void CWidgetButtonListVertical::SetLeftPosition(int positionTop)
{

}

void CWidgetButtonListVertical::showEvent(QShowEvent *event)
{
	RefreshLayout();
}


