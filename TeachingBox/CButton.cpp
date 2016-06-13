#include "stdafx.h"
#include "CButton.h"


CButton::CButton(const QString &text, QWidget* parent /*= 0*/) : QPushButton(parent)
{
	Init();
	this->setText(text);
}

CButton::CButton(QWidget* parent /*= 0*/) :QPushButton(parent)
{
	Init();
}

CButton::~CButton(){}

void CButton::Init()
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);/*控件充满剩余空间*/
}
