/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreenBottom.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/11/24
//  Description:	1、屏幕下端控制按钮栏，其布局固定，但显示内容随用户操作变化
					2、该类被CScreen类所调用，显示于Screen界面中
					3、此布局与主界面共用按钮，用于响应用户操作，并与主界面中显示
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2015/11/24
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _TEACHING_BOX_C_BOTTOM_H_
#define _TEACHING_BOX_C_BOTTOM_H_

#include "QPushButton"
#include "QBoxLayout"
#include "CButton.h"


class CScreenBottom :public QHBoxLayout
{
	Q_OBJECT
public:
	CScreenBottom(QWidget* parent = 0);
	CScreenBottom(QList<CButton*>& btnList, QWidget* parent = 0);
	~CScreenBottom();

protected slots:


public:
	void AddButtonList(QList<CButton*>&);	/*添加按钮列表*/
	void RemoveButtonList();
private:
	void Refresh();	/*刷新布局*/
	
private:
	QList<CButton*> m_btnList;	/*按钮列表*/
};

#endif