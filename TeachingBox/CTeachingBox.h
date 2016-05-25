#ifndef _TEACHING_BOX_C_TEACHINGBOX_H_
#define _TEACHING_BOX_C_TEACHINGBOX_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CTeachingBox.h
//  Author:			刘巍
//  Version: 		1.0
//  Date: 			2015/11/24
//  Description:	示教盒，包括用于交互的主屏幕，以及旁边的按钮
//  Others:			最后实际应用中只用主屏幕的内容是可以实际看到的，旁边的按钮会被示教盒所覆盖
//  History:
//    <author>      刘巍
//    <time>        2015/11/24
//    <version>     1.0
//    <desc>        build this moudle
*************************************************/

#include <QtWidgets/QWidget>
#include<QHBoxLayout>
#include <QLabel>
#include "QPushButton"
#include "QRadioButton"
#include "CLabel.h"

#include "Init.h"
#include "CButton.h"
#include "CScreen.h"
#include "CLabelPixmap.h"
#include "CLabelLight.h"
#include "QThread"
#include "QButtonGroup"
#include "CExceptionInterpreter.h"
#include "CInterpreterManager.h"
#include "CThreadInterpreterManager.h"

class CTeachingBox : public QWidget
{
	friend class CSingleTon<CTeachingBox>; /*单例模式*/

	Q_OBJECT

	enum ExecuteType
	{
		AUTO_EXECUTE,
		MANUAL_EXECUTE,
		STEP_EXECUTE,
	};
private:
	CTeachingBox(QWidget *parent = 0);
	CTeachingBox(const CTeachingBox&);
	CTeachingBox& operator=(const CTeachingBox&);
	~CTeachingBox();

	/*自定义公有函数*/
public:
	static CTeachingBox* GetInstance();	/*获取唯一实例*/
	static bool IsCreated();

	/*自定义私有函数*/
private:
	void Init();	//初始化
	void InitLayout();	
	void InitSignalSlots();
	void InitSignalSlotMoveAxis();
	void InitThread();

signals:
	void SignalBeginExecute();
	void SignalExecuteStep();
	void SignalStopExecute();

protected slots:
	void SlotChangeToScreenProject();
	void SlotChangeToScreenProgram();
	void SlotChangeToScreenSetting();
	void SlotChangeToScreenVariable();

	void SlotOnButtonStartPressed();
	void SlotOnButtonStartReleased();

	void SlotOnButtonStopClicked();	/*停止解释*/
	void SlotOnButtonModelClicked();

	void SlotOnButtonPlusAxis1();
	void SlotOnButtonPlusAxis2();
	void SlotOnButtonPlusAxis3();
	void SlotOnButtonPlusAxis4();
	void SlotOnButtonPlusAxis5();
	void SlotOnButtonPlusAxis6();
	void SlotOnButtonMinusAxis1();
	void SlotOnButtonMinusAxis2();
	void SlotOnButtonMinusAxis3();
	void SlotOnButtonMinusAxis4();
	void SlotOnButtonMinusAxis5();
	void SlotOnButtonMinusAxis6();

	void SlotOnButtonStopMoveAxis();

private:
	void StopExecute(); /*停止执行*/
private:
	ExecuteType m_executeType=MANUAL_EXECUTE;

private:
	QThread* m_thrdRefreshDateTime;
	QTimer* m_timerRefreshDateTime;

	QThread* m_thrdIntepreter;
	CThreadInterpreterManager* m_thrdInterpreterManager;
	CInterpreterManager* m_interpreterManager;

	CScreen* m_screen;/*屏幕*/
	CScreenMain* m_layoutScreenMain;	/*屏幕主界面*/

	//示教盒左侧控件
	CButton* m_btnU;	/*预留*/
	CButton* m_btnSetting;
	CButton* m_btnVariate;
	CButton* m_btnProject;
	CButton* m_btnProgram;
	CButton* m_btnCoordinate;
	CButton* m_btnWarning;

	CLabel* m_lbRun;
	CLabel* m_lbError;
	CLabel* m_lbPro;
	CLabel* m_lbMot;

	CLabelLight* m_clRun;
	CLabelLight* m_clError;
	CLabelLight* m_clPro;
	CLabelLight* m_clMot;

	//示教盒上侧控件
	QButtonGroup* m_modelButtonGroup;
	QRadioButton* m_btnMode1;
	QRadioButton* m_btnMode2;
	QRadioButton* m_btnMode3;
	CButton* m_btnScram;

	//示教盒右侧控件
	CButton* m_btnStop;
	CButton* m_btnStart;
	CButton* m_A1Plus;
	CButton* m_A1Minus;
	CButton* m_A2Plus;
	CButton* m_A2Minus;
	CButton* m_A3Plus;
	CButton* m_A3Minus;
	CButton* m_A4Plus;
	CButton* m_A4Minus;
	CButton* m_A5Plus;
	CButton* m_A5Minus;
	CButton* m_A6Plus;
	CButton* m_A6Minus;
	CButton* m_btn2nd;

	//示教盒下侧控件
	CButton* m_btnF1;
	CButton* m_btnF2;
	CButton* m_btnMot;
	CButton* m_btnRob;
	CButton* m_btnJog;
	CButton* m_btnFB;
	CButton* m_btnStep;
	CButton* m_btnVMinus;
	CButton* m_btnVPlus;
};

#endif // TEACHINGBOX_H
