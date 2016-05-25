/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreenHeader.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/11/24
//  Description:	屏幕上端状态栏，用于显示当前的各种状态，包括如下内容：
					1、第一栏显示操作方式，伺服驱动，坐标系，工具，速度，LOGO等；
					2、第二栏显示权限，时间等；
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2015/11/24
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _TEACHING_BOX_C_SCREEN_HEADER_H_
#define _TEACHING_BOX_C_SCREEN_HEADER_H_

#include "QtWidgets/QWidget"
#include "QLabel"
#include "QPushButton"
#include "CLabel.h"
#include "Init.h"
#include "QBoxLayout"
#include "QTimer"
#include "CSingleTon.h"

class CScreenState :public QVBoxLayout
{
	friend class CSingleTon<CScreenState>;

	Q_OBJECT

public:
	static CScreenState* GetInstance();
	static bool IsCreated();

	CLabel* GetTimeLabel();

	void ChangeUserAuthority(const QString& userAuthority);
private:
	CScreenState(QWidget* parent = 0);
	CScreenState(const CScreenState&);
	CScreenState& operator=(const CScreenState&);
	~CScreenState();

	/*自定义私有函数*/
private:
	void Init();	//初始化
	void InitLayout();
	void InitSignalSlot();

	/*自定义槽函数*/
private slots:
	//void SlotRefreshTime();

	/*私有变量*/
private:
	CLabel* m_lbOperationMode;	//操作模式
	CLabel* m_lbServo;			//机器人状态
	CLabel* m_lbCoordinate;		//参考坐标系
	CLabel* m_lbTool;			//工具手
	CLabel* m_lbSpeedPersent;	//机器人速度
	CLabel* m_lbLogo;			//商标

	CLabel* m_lbProjectAndProgram;		//项目与程序
	CLabel* m_lbProgramState;			//程序状态
	CLabel* m_lbProgramRunningMode;		//同上
	CLabel* m_lb3;				//用户自定义
	CLabel* m_lb4;				//同上
	CLabel* m_lb5;				//同上
	CLabel* m_lbSpace;			//空间监控
	CLabel* m_lbSecurityState;	//安全状态
	CLabel* m_lbUserAuthority;	//用户权限
	CLabel* m_lbCurrentTime;	//当前时间
};

#endif