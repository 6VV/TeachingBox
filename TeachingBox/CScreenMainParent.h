/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreenMainParent.cpp
//  Author:			刘巍
//  Version: 		1.0
//  Date: 			2015/12/01
//  Description:	主界面的父类，用于构造主界面的结构,主界面包括一个中央界面以及底部的按钮布局
//  Others:
//  History:
//    <author>      刘巍
//    <time>        2015/12/01
//    <version>     1.0
//    <desc>        build this moudle
*************************************************/

#ifndef _TEACHING_BOX_C_SCREEN_MAIN_PARENT_H_
#define _TEACHING_BOX_C_SCREEN_MAIN_PARENT_H_

#include "QWidget"
#include "QBoxLayout"
#include "CScreenBottom.h"
#include "Init.h"

class CScreenMainParent:public QWidget
{
	Q_OBJECT
public:
	CScreenMainParent(QWidget* parent=0);
	~CScreenMainParent();

	/*重写父类函数*/
protected:
	void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
	virtual void Back();

protected slots:
	void SlotOnButtonBackClicked();

	/*自定义保护函数*/
protected:
	QVBoxLayout* m_layoutMain;		/*主界面*/
	QHBoxLayout* m_layoutCenter;	/*中央界面*/
	CScreenBottom* m_screenBottom;	/*底部控件布局*/

	void Init();
	void InitLayout();	
	void InitSignalSlot();

	void ResizeControlers();

	/*自定义私有函数*/
protected:
	virtual void RefreshText()=0;	/*刷新控件文本，需在子类实现*/

protected:
	QList<CButton*> m_btnList; /*底部按钮列表*/

	CButton* m_btnBack;
};

#endif