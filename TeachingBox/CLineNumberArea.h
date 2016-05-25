/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CLineNumberArea.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/12/09
//  Description:	文本编辑控件左侧用于显示行数的区域
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2015/12/09
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _TEACHING_BOX_C_LINE_NUMBER_AREA_H_
#define _TEACHING_BOX_C_LINE_NUMBER_AREA_H_

#include "QWidget"
#include "CCodeEditor.h"

class CLineNumberArea : public QWidget
{
	Q_OBJECT

public:
	CLineNumberArea(CCodeEditor *editor);

	/*重定义父类函数*/
protected:
	QSize sizeHint() const Q_DECL_OVERRIDE;	/*控件默认大小*/
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;	/*绘制控件*/

private:
	CCodeEditor *codeEditor;
};



#endif