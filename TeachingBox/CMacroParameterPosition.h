#ifndef _TEACHING_BOX_C_MACRO_POSITION_H_
#define _TEACHING_BOX_C_MACRO_POSITION_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CMacroPosition.h
//  Author:			刘巍
//  Version: 		1.0
//  Date: 			2015/12/29
//  Description:	用于显示及设置位置变量
//  Others:
//  History:
//    <author>      刘巍
//    <time>        2015/12/29
//    <version>     1.0
//    <desc>        build this moudle
*************************************************/

#include "CButton.h"
#include "QTableWidget"
#include "QLineEdit"
#include "CScreenMainParent.h"
#include "CMacroParameterParent.h"

class CMacroParameterPosition:public CMacroParameterParent
{
	Q_OBJECT

public:
	CMacroParameterPosition(QWidget* parent = 0);
	~CMacroParameterPosition();

	void KeyboardEdit();

protected:
	void OnButtonConfirmClicked() Q_DECL_OVERRIDE;
	void OnButtonEditClicked() Q_DECL_OVERRIDE;

private:
	void Init();
	void InitLayout();

	//bool IsCorrectName(QString& strName);
	bool IsCorrectValue(QString& strValue);

private:
	const char* CLASS_NAME = "CMacroParameterPosition";

	QLabel* m_lbPositionName;
	QLabel* m_lbValueA1;
	QLabel* m_lbValueA2;
	QLabel* m_lbValueA3;
	QLabel* m_lbValueA4;
	QLabel* m_lbValueA5;
	QLabel* m_lbValueA6;

};

#endif