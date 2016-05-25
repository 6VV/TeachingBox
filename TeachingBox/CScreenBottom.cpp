#include "stdafx.h"
#include "CScreenBottom.h"
#include "QBoxLayout"
#include "qlogging.h"

CScreenBottom::CScreenBottom(QWidget* parent /*= 0*/) :QHBoxLayout(parent)
{
}

CScreenBottom::CScreenBottom(QList<CButton*>& btnList, QWidget* parent /*= 0*/) 
	: QHBoxLayout(parent)
{
	this->m_btnList = btnList;
	Refresh();
}

CScreenBottom::~CScreenBottom()
{
	if (m_btnList.size() > 0){
		qDeleteAll(m_btnList);
		m_btnList.clear();
	}
}

void CScreenBottom::AddButtonList(QList<CButton*>& btnList)
{
	if (m_btnList.size() > 0)
	{
		return;
	}
	this->m_btnList = btnList;
	Refresh();
}

void CScreenBottom::RemoveButtonList()
{
	for each (auto var in this->children())
	{
		this->removeWidget(static_cast<CButton*>(var));
	}
}

/*************************************************
//  Function: 		CScreenBottom::Refresh
//  Description: 	按m_btnList中内容将控件添加到布局中，并设置比例大小
//  Calls:
//  Called By: 		该函数在AddButton函数之后被调用
//  Parameter:      void
//  Return: 		void
//  Others:
*************************************************/
void CScreenBottom::Refresh()
{
	/*添加列表中的按钮*/
	for (int i = 0; i < m_btnList.size(); i++)
	{
		this->addWidget(m_btnList[i]);
	}

	//this->setDirection(QBoxLayout::RightToLeft);/*设置方向有从右向左*/

	/*设置各按钮长度比例*/
	this->setStretch(0, 3);
	this->setStretch(1, 3);
	this->setStretch(2, 3);
	this->setStretch(3, 3);
	this->setStretch(4, 3);
	this->setStretch(5, 3);
	this->setStretch(6, 2);

	this->setSpacing(0);
}
