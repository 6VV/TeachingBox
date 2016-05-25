/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreen.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/11/24
//  Description:	示教盒主屏幕
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2015/11/24
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _TEACHING_BOX_C_SCREEN_H_
#define _TEACHING_BOX_C_SCREEN_H_

#include "QtWidgets/QWidget"
#include "QWidget"

#include "Init.h"
#include "CScreenState.h"
#include "CScreenBottom.h"
#include "CScreenWarning.h"
#include "CScreenRight.h"
#include "QBoxLayout"
#include "CScreenMain.h"
#include "CSingleTon.h"

class CScreen :public QVBoxLayout
{
	friend class CSingleTon<CScreen>;

	Q_OBJECT

public:
	static CScreen* GetInstance();
	static bool IsCreated();

	int GetLeftPosition();
	int GetRightPosition();
	int GetTopPosition();
	int GetBottomPosition();

private:
	CScreen(QWidget* parent = 0);
	CScreen(const CScreen&);
	CScreen& operator=(const CScreen&);
	~CScreen();


private:
	void Init();

private:
	CScreenState* m_layoutHeader;
	CScreenWarning* m_layoutWarning;
	CScreenRight* m_layoutRight;
	CScreenMain* m_layoutScreenMain;
};

#endif