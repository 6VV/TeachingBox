#ifndef _TEACHING_BOX_C_SCREEN_EDIT_MACRO_H_
#define _TEACHING_BOX_C_SCREEN_EDIT_MACRO_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreenEditMacro.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2016/03/08
//  Description:	编辑命令
//  Others:			已废弃
//  History:
//    <author>      刘巍 
//    <time>        2016/03/08
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/
#include "CSingleTon.h"
#include "CScreenMainParent.h"
#include "CButton.h"
#include "QTableWidget"
#include "CValue.h"

class CScreenEditMacro:public CScreenMainParent
{
	friend CSingleTon<CScreenEditMacro>;

	Q_OBJECT

private:
	CScreenEditMacro(QWidget* parent=0);
	CScreenEditMacro(const CScreenEditMacro&);
	CScreenEditMacro& operator=(const CScreenEditMacro&);
	~CScreenEditMacro();

	/*自定义公有函数*/
public:
	static CScreenEditMacro* GetInstance();
	void ChangeContent(QString& strMacroName,QList<QString>& listParameter);

	void UpdateParameterName(QString& strNewParameterName);	/*设置当前行的新的参数名*/

	void KeyboardEdit();

	/*自定义私有函数*/
private:
	void Init();
	void InitLayout();
	void InitSignalSlot();

	void InitMovlWidget();	/*初始化movl窗口*/
	void InitMovcWidget();  /*初始化movc窗口*/
	void InitForWidget();	/*初始化For窗口*/
	void InitSwitchWidget(); /*初始化Switch窗口*/

	QWidget* GetPositionValueWidget();	/*获取位置值控件*/
	QWidget* GetVelocityValueWidget();	/*获取速度值控件*/
	QWidget* GetZoneValueWidget();	/*获取过渡区域值控件*/
	QWidget* GetDoubleValueWidget();	/*获取浮点数值控件*/
	QWidget* GetLabelValueWidget();	/*获取标签控件*/
	QWidget* GetIntValueWidget();	/*获取整数控件*/
	QWidget* GetBoolValueWidget();	/*获取布尔控件*/
	QWidget* GetStringValueWidget();	/*获取字符串控件*/
	QWidget* GetNumValueWidget();	/*获取数值变量控件*/

	void SetMacroName(QString&);	/*设置命令名*/
	void SetParameterNameList(QStringList&);	/*设置参数名列表*/
	void SetParameterTypeList(QStringList&);	/*设置参数类型列表*/

	QString& GetMacroName();		/*获取命令名*/
	QStringList& GetParameterNameList();	/*获取参数名列表*/
	QStringList& GetParameterTypeList();	/*获取参数类型列表*/

	void SetForContext(QString& strText);	/*编辑For命令文本*/

	//void Back();	/*返回*/
	void Destroy();	/*销毁实例*/

protected:
	void RefreshText() Q_DECL_OVERRIDE;	/*更新控件文本*/
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

	/*槽函数*/
protected slots:
	void SlotOnButtonModifyClicked();	/*修改按钮点击事件*/
	void SlotOnButtonConfrimClicked();	/*确认按钮点击事件*/
	void SlotOnButtonAddClicked();		/*添加按钮点击事件*/
	//void SlotOnButtonBackClicked();		/*返回按钮点击事件*/

	/*私有变量*/
private:
	const char* CLASS_NAME = "CScreenEditMacro";
	QTableWidget* m_tableWidget;
	CValue* m_varValue;

	/*按钮*/
	CButton* m_btnModify;	/*修改按钮*/
	CButton* m_btnTeach;	/*示教按钮*/
	CButton* m_btnAdd;		/*添加按钮*/
	CButton* m_btnValue;	/*值按钮*/
	CButton* m_btnCancle;	/*取消按钮*/
	CButton* m_btnConfirm;	/*确认按钮*/
	//CButton* m_btnBack;		/*返回按钮*/

	/*宏定义相关*/
	QString m_strMacroName;	/*宏名*/
	QStringList m_strListParameterName;	/*参数名列表*/
	QStringList m_strListParameterType;	/*参数类型列表*/
};

#endif