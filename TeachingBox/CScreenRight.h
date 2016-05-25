/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreenLeft.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/11/24
//  Description:	1、警告信息栏，用于显示出现的警告或错误信息等，按下确认按钮后消除当前信息，并显示上一次信息
					2、该类被CScreen类所调用，用于在主界面上显示信息
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2015/11/24
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _TEACHING_BOX_C_SCREEN_LEFT_H_
#define _TEACHING_BOX_C_SCREEN_LEFT_H_

#include "QBoxLayout"
#include "CSingleTon.h"

class CScreenRight :public QVBoxLayout
{
	friend class CSingleTon<CScreenRight>;

	Q_OBJECT

public:
	static CScreenRight* GetInstance();
	static bool IsCreated();

private:
	CScreenRight(QWidget* parent = 0);
	CScreenRight(const CScreenRight&);
	CScreenRight& operator=(const CScreenRight&);
	~CScreenRight();

private:
	void Init();	/*初始化*/
};

#endif