#ifndef _TEACHING_BOX_C_EDIT_MACRO_PARAMETER_H_
#define _TEACHING_BOX_C_EDIT_MACRO_PARAMETER_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreenEditMacroParameter.h
//  Author:			刘巍
//  Version: 		1.0
//  Date: 			2016/01/18
//  Description:	用于加载各种编辑参数的界面，在加载新界面前会首先释放掉原来加载的界面,该类用于生成各编辑界面的父窗口，而非作为父类存在
//  Others:			已废弃
//  History:
//    <author>      刘巍
//    <time>        2016/01/18
//    <version>     1.0
//    <desc>        build this moudle
*************************************************/

#include "CSingleTon.h"
#include "CButton.h"
#include "QTableWidget"

class CScreenEditMacroParameter:public QWidget
{
	friend class CSingleTon<CScreenEditMacroParameter>;

	Q_OBJECT
	
private:
	CScreenEditMacroParameter();
	CScreenEditMacroParameter(const CScreenEditMacroParameter&);
	CScreenEditMacroParameter& operator = (const CScreenEditMacroParameter&);
	~CScreenEditMacroParameter();

	/*自定义公有函数*/
public:
	static CScreenEditMacroParameter* GetInstance();
	void ChangeWidget(QString paremeterType);
	//void ChangeToNewParameterWidget();	/*切换到新建变量视图*/

	void UpdateParameterName(QString& strName);

protected:
	void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

	/*自定义私有函数*/
private:
	void Init();

	void ChangeToPositionWidget();
	void ChangeToVelocityWidget();
	void ChangeToZoneWidget();
	void ChangeToDoubleWidget();
	void ChangeToLableWidget();
	void ChangeToIntWidget();
	void ChangeToBoolWidget();
	void ChangeToStringWidget();

	/*自定义私有变量*/
private:
	const char* CLASS_NAME = "CScreenEditParameter";
	
	QHBoxLayout* m_layout;	/*包含窗口的布局*/
	QWidget* m_currentWidget;	/*当前显示的窗口*/

};

#endif