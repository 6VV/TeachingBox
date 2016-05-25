/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreenWarning.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/12/01
//  Description:	用于显示屏幕中警告部分
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2015/12/01
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/
#ifndef _TEACHING_BOX_C_SCREEN_WARNING_H_
#define _TEACHING_BOX_C_SCREEN_WARNING_H_

#include "CLabel.h"
#include "QBoxLayout"
#include "CButton.h"
#include "CSingleTon.h"

class CScreenWarning :public QHBoxLayout
{
	friend class CSingleTon<CScreenWarning>;

	Q_OBJECT

private:
	CScreenWarning(QWidget* parent = 0);
	CScreenWarning(const CScreenWarning&);
	CScreenWarning& operator=(const CScreenWarning&);
	~CScreenWarning();

	/*自定义公有函数*/
public:
	static CScreenWarning* GetInstance();
	static bool IsCreated();

	CLabel* GetLabel();

	/*自定义私有函数*/
private:
	void Init();

private:
	CLabel* m_lbWarning;		//警告信息
	CButton* m_btnConfirm;		//确认按钮
};

#endif