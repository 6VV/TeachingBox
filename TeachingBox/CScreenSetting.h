/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreenSetting.h
//  Author:			��Ρ      
//  Version: 		1.0     
//  Date: 			2015/12/01
//  Description:	������ʾ���ý���
//  Others:			�̳���CScreenMainParent��
//  History:
//    <author>      ��Ρ 
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
	friend class CSingleTon<CScreenSetting>;	/*����ģʽ*/

	Q_OBJECT

private:
	CScreenSetting(QWidget* parent=0);
	CScreenSetting(const CScreenSetting&);
	CScreenSetting& operator=(const CScreenSetting&);
	~CScreenSetting();
	
	/*���г�Ա����*/
public:
	static CScreenSetting* GetInstance();
	static bool IsCreated();

	QLabel* GetDateLabel();
	QLabel* GetTimeLabel();
	QComboBox* GetUserComboBox();

	void KeyboardEdit();	/*����ȷ�Ϻ��¼*/

	/*˽�г�Ա����*/
private:
	void Init();
	void InitSignalSlot();
	void InitLayout();

	void RefreshText(); /*���ÿؼ��ı�*/

	void CreateWidgetLogin();
	void CreateWidgetSystemSettings();
	void CreateWidgetRobotControlAuthority();
	void CreateWidgetLockScreenSetting();

	void UpdateState();
	void UpdateUserList();
	/*�����Ա����*/
private:
	void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent *event)	Q_DECL_OVERRIDE;

signals:
	void SignalsUserChanged();

private slots:
	void SlotChangeLanguage(int);	/*�л�����*/
	void SlotLogin(int);	/*��¼*/
	void SlotLogout();		/*�ǳ�*/
	void SlotControlAuthorityChanged(int state);	/*����Ȩ�޸ı�*/
	void SlotOnButtonLockScreenClicked();	/*������ť����*/

	/*˽�г�Ա����*/
private:
	const char* CLASS_NAME = "CScreenSetting";	/*����*/

	QTimer* m_timerDateTime;	

	QGroupBox* m_groupboxLogin;
	QGroupBox* m_groupboxSystemSettings;
	QGroupBox* m_groupboxRobotControlAuthority;
	QGroupBox* m_groupboxDispaySettings;


	/*��¼����*/
	QLabel* m_lbUser;
	QComboBox* m_cmbUser;
	QLabel* m_lbAuthority;
	QLabel* m_lbAuthorityValue;
	QPushButton* m_btnLogout;

	/*ϵͳ����*/
	QLabel* m_lbLanguage;
	QComboBox* m_cmbLanguage;
	QLabel* m_lbDate;
	QLabel* m_lbDateValue;
	QLabel* m_lbTime;
	QLabel* m_lbTimeValue;

	/*�����˿���Ȩ�޴���*/
	QLabel* m_lbControlAuthority;
	QCheckBox* m_checkboxControlAuthority;
	QLabel* m_lbDevice;
	QLabel* m_lbDeviceValue;
	QLabel* m_lbDeviceIp;
	QLabel* m_lbDeviceIpValue;

	/*��ʾ���ô���*/
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