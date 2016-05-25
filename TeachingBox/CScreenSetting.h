/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreenSetting.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/12/01
//  Description:	用于显示设置界面
//  Others:			继承自CScreenMainParent类
//  History:
//    <author>      刘巍 
//    <time>        2015/12/01
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _TEACHING_BOX_C_SCREEN_SETTING_H_
#define _TEACHING_BOX_C_SCREEN_SETTING_H_

#include "QWidget"
#include "CButton.h"
#include "CScreenMainParent.h"
#include "QMap"
#include "QComboBox"
#include "QThread"
#include "QTimer"
#include "CSingleTon.h"
#include "CKeyBoardInterface.h"

class CScreenSetting:public QWidget,public CKeyBoardInterface
{
	friend class CSingleTon<CScreenSetting>;	/*单例模式*/

	Q_OBJECT

private:
	CScreenSetting(QWidget* parent=0);
	CScreenSetting(const CScreenSetting&);
	CScreenSetting& operator=(const CScreenSetting&);
	~CScreenSetting();
	
	/*公有成员函数*/
public:
	static CScreenSetting* GetInstance();
	static bool IsCreated();

	QLabel* GetDateLabel();
	QLabel* GetTimeLabel();
	QComboBox* GetUserComboBox();

	void KeyboardEdit();	/*键盘确认后登录*/

	/*私有成员函数*/
private:
	void Init();
	void InitSignalSlot();
	void InitLayout();

	void RefreshText(); /*设置控件文本*/

	void CreateWidgetLogin();
	void CreateWidgetSystemSettings();
	void CreateWidgetRobotControlAuthority();
	void CreateWidgetLockScreenSetting();

	void UpdateState();
	void UpdateUserList();
	/*父类成员函数*/
private:
	void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent *event)	Q_DECL_OVERRIDE;

signals:
	void SignalsUserChanged();

private slots:
	void SlotChangeLanguage(int);	/*切换语言*/
	void SlotLogin(int);	/*登录*/
	void SlotLogout();		/*登出*/
	void SlotControlAuthorityChanged(int state);	/*控制权限改变*/
	void SlotOnButtonLockScreenClicked();	/*锁屏按钮按下*/

	/*私有成员变量*/
private:
	const char* CLASS_NAME = "CScreenSetting";	/*类名*/

	QTimer* m_timerDateTime;	

	QGroupBox* m_groupboxLogin;
	QGroupBox* m_groupboxSystemSettings;
	QGroupBox* m_groupboxRobotControlAuthority;
	QGroupBox* m_groupboxDispaySettings;


	/*登录窗口*/
	QLabel* m_lbUser;
	QComboBox* m_cmbUser;
	QLabel* m_lbAuthority;
	QLabel* m_lbAuthorityValue;
	QPushButton* m_btnLogout;

	/*系统窗口*/
	QLabel* m_lbLanguage;
	QComboBox* m_cmbLanguage;
	QLabel* m_lbDate;
	QLabel* m_lbDateValue;
	QLabel* m_lbTime;
	QLabel* m_lbTimeValue;

	/*机器人控制权限窗口*/
	QLabel* m_lbControlAuthority;
	QCheckBox* m_checkboxControlAuthority;
	QLabel* m_lbDevice;
	QLabel* m_lbDeviceValue;
	QLabel* m_lbDeviceIp;
	QLabel* m_lbDeviceIpValue;

	/*显示设置窗口*/
	QLabel* m_lbLockScreen;
	QPushButton* m_btnLockScreen;

private:
	enum LANGUAGE_INDEX
	{
		LANGUAGE_INDEX_ENGLISH=0,
		lANGUAGE_INDEX_CHINESE,
	};
};

#endif