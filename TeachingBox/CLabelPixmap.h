/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CLight.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/11/25
//  Description:	指示灯，基础控件为QLabel控件，可通过UpdateImage函数修改背景图片
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2015/11/25
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _TEACHING_BOX_C_LABEL_PIXMAP_H_
#define _TEACHING_BOX_C_LABEL_PIXMAP_H_

#include "QLabel"
#include "QPixmap"

class CLabelPixmap :public QLabel
{
	Q_OBJECT
public:
	CLabelPixmap(const QString &text, QWidget* parent = 0);
	CLabelPixmap(QWidget* parent = 0);
	~CLabelPixmap();

	/*重写父类函数*/
private:
	void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

public:
	void UpdateImage(const QString &text);

private:
	void ResizeImage();
	void ClearImage();

private:
	QPixmap* m_pixmap=NULL;
	bool m_isResized=false;	/*控件是否已经重置大小*/
};

#endif