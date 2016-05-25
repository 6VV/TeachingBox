#include "stdafx.h"
#include "CScreenMainParent.h"
#include "CWidgetStack.h"

CScreenMainParent::CScreenMainParent(QWidget* parent/*=0*/) :QWidget(parent)
{
	Init();
}

CScreenMainParent::~CScreenMainParent()
{

}

void CScreenMainParent::changeEvent(QEvent *event)
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

void CScreenMainParent::showEvent(QShowEvent *event)
{
	if (CWidgetStack::GetInstance()->IsEmpty())
	{
		m_btnBack->setEnabled(false);
	}
	else
	{
		m_btnBack->setEnabled(true);
	}
}

void CScreenMainParent::Back()
{
	CWidgetStack::GetInstance()->Back();
}

void CScreenMainParent::SlotOnButtonBackClicked()
{
	Back();
}

void CScreenMainParent::Init()
{
	InitLayout();
	InitSignalSlot();
}

//void CScreenMainParent::RefreshText()
//{
//}

void CScreenMainParent::InitLayout()
{
	m_layoutMain = new QVBoxLayout(this);
	m_layoutCenter = new QHBoxLayout;
	m_screenBottom = new CScreenBottom;

	m_btnBack = new CButton;

	m_layoutMain->addLayout(m_layoutCenter);
	m_layoutMain->addLayout(m_screenBottom);
	
	ResizeControlers();
}

void CScreenMainParent::InitSignalSlot()
{
	connect(m_btnBack, SIGNAL(clicked()), this, SLOT(SlotOnButtonBackClicked()));
}

void CScreenMainParent::ResizeControlers()
{
	m_layoutMain->setStretch(0, SCREEN_CENTER_HEIGHT_RATIO);
	m_layoutMain->setStretch(1, SCREEN_BOTTOM_HEIGHT_RATIO);
	m_layoutMain->setMargin(0);
	m_layoutMain->setSpacing(0);
}

