/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CLabel.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/11/24
//  Description:	自定义Label控件
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2015/11/24
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _TEACHINGBOX_C_LABEL_H_
#define _TEACHINGBOX_C_LABEL_H_

#include "QLabel"
class CLabel :public QLabel
{
	Q_OBJECT
public:
	explicit CLabel(const QString &text, QWidget* parent = 0);
	explicit CLabel();
private:
	void Init();
};


#endif