#ifndef _TEACHING_BOX_C_MACRO_PARAMETER_PARENT_H_
#define _TEACHING_BOX_C_MACRO_PARAMETER_PARENT_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CMacroParameterParent.h
//  Author:			刘巍
//  Version: 		2.0
//  Date: 			2016/03/07
//  Description:    作为各种编辑参数的界面的父类，定义基本操作及接口
//  Others:			对代码进行重构，该类及其子类不再被使用
//  History:
//    <author>      刘巍
//    <time>        2016/01/18
//    <version>     1.0
//    <desc>        build this moudle
*************************************************/

#include "CScreenMainParent.h"

class CMacroParameterParent:public CScreenMainParent
{
	Q_OBJECT

public:
	CMacroParameterParent(QWidget* parent=0);
	~CMacroParameterParent();

	/*自定义公有函数*/
public:
	static void SetOperType(int operType);

	static void SetOldParameterName(QString strOldParameterName);

	/*公有变量*/
public:
	const static int OPER_ADD = 1;	/*操作类型为添加*/
	const static int OPER_MODIFY = 2;	/*操作类型为修改*/

	/*重写父类函数*/
protected:
	void RefreshText() Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

	/*槽函数*/
protected slots:
	void SlotOnButtonConfirmClicked();	/*确认按钮按下事件*/
	//void SlotOnButtonCancleClicked();	/*取消按钮按下事件*/
	void SlotOnButtonEditClicked();		/*编辑按钮按下事件*/

	//需要被子类重写的函数
protected:
	virtual void OnButtonConfirmClicked() = 0;	//确认按钮按下事件调用函数
	virtual void OnButtonEditClicked() = 0;		//编辑按钮按下事件调用函数

	bool IsCorrectName(QString& strName);

	/*自定义私有函数*/
private:
	void Init();
	void InitLayout();
	void InitSignalSlot();

	bool IsCorrectName(std::string& strName);	/*是否是正确的变量名*/

	/*自定义保护变量*/
protected:
	static int m_operType;	/*编辑方式（添加/修改）*/
	static QString m_newParameterName;	/*新参数名*/
	static QString m_oldParameterName;	/*旧参数名*/

	QTableWidget* m_tableWidget;	/*表格窗口*/

	CButton* m_btnConfirm;	/*确认按钮*/
	CButton* m_btnEdit;	/*编辑按钮*/
	//CButton* m_btnBack;	/*返回按钮*/

private:
	const char* CLASS_NAME = "CMacroPosition";
};

#endif
