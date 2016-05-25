#include "stdafx.h"
#include "CScreenWarning.h"
#include "QBoxLayout"



CScreenWarning::CScreenWarning(QWidget* parent /*= 0*/) :QHBoxLayout(parent)
{
	Init();
}

CScreenWarning::~CScreenWarning()
{
	
}

CScreenWarning* CScreenWarning::GetInstance()
{
	return CSingleTon<CScreenWarning>::GetInstance();
}

bool CScreenWarning::IsCreated()
{
	return CSingleTon<CScreenWarning>::IsCreated();
}

CLabel* CScreenWarning::GetLabel()
{
	return m_lbWarning;
}

void CScreenWarning::Init()
{
	m_lbWarning = new CLabel;
	m_btnConfirm = new CButton(QStringLiteral("Confirm"));

	this->addWidget(m_lbWarning);
	this->addWidget(m_btnConfirm);

	this->setStretch(0, 1);

	this->setSpacing(0);
}
