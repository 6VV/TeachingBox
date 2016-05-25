#include "stdafx.h"
#include "CScreenRight.h"
#include "CLabel.h"
#include "QBoxLayout"

CScreenRight* CScreenRight::GetInstance()
{
	return CSingleTon<CScreenRight>::GetInstance();
}

bool CScreenRight::IsCreated()
{
	return CSingleTon<CScreenRight>::IsCreated();
}

CScreenRight::CScreenRight(QWidget* parent /*= 0*/) :QVBoxLayout(parent)
{
	Init();
}

CScreenRight::~CScreenRight()
{

}

/*************************************************
//  Function: 		CScreenLeft::Init
//  Description: 	初始化控件及布局
//  Calls:		 	
//  Called By: 		CScreenLeft::CScreenLeft
//  Parameter:      void
//  Return: 		void
//  Others: 		
*************************************************/
void CScreenRight::Init()
{
	CLabel* lbA1 = new CLabel(QStringLiteral("A1"));
	CLabel* lbA2 = new CLabel(QStringLiteral("A2"));
	CLabel* lbA3 = new CLabel(QStringLiteral("A3"));
	CLabel* lbA4 = new CLabel(QStringLiteral("A4"));
	CLabel* lbA5 = new CLabel(QStringLiteral("A5"));
	CLabel* lbA6 = new CLabel(QStringLiteral("A6"));
	CLabel* lbNull = new CLabel;

	this->addWidget(lbA1);
	this->addWidget(lbA2);
	this->addWidget(lbA3);
	this->addWidget(lbA4);
	this->addWidget(lbA5);
	this->addWidget(lbA6);
	this->addWidget(lbNull);

	this->setSpacing(0);
}
