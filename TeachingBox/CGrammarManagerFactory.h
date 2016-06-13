#ifndef _TEACHING_BOX_C_GRAMMAR_MANAGER_H_
#define _TEACHING_BOX_C_GRAMMAR_MANAGER_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CGrammarManager.h
//  Author:			刘巍
//  Version: 		1.0
//  Date: 			2015/12/22
//  Description:    用于控制语法的显示，包括将语法的分类、宏定义添加到相应模型中，
以及在对应的视图中的显示，编译器语法的生成等
//  Others:			主要有两种生成语法的方式：从程序中直接编写；从xml文件中读取。
两种方式分别存于两个类中，可在GetInstance函数中选择生成方式所对应的类。
//  History:
//    <author>      刘巍
//    <time>        2015/12/22
//    <version>     1.0
//    <desc>        build this moudle
*************************************************/

#include "CSingleTon.h"
#include "CModelCategoryFromXml.h"
#include "CModeMacroFromXml.h"
#include "qdom.h"
#include "Init.h"
#include "CModelMacro.h"
#include "CModelCategory.h"

class CGrammarManagerFactory
{
	friend class CSingleTon<CGrammarManagerFactory>;

protected:
	CGrammarManagerFactory();
	CGrammarManagerFactory(const CGrammarManagerFactory&);
	CGrammarManagerFactory& operator=(const CGrammarManagerFactory&);
	~CGrammarManagerFactory();

public:
	static CGrammarManagerFactory* GetInstance();
	
	virtual void UpdateCategoryData(); /*更新分类模型*/
	virtual void UpdateMacroData(QModelIndex& indexCategory){};	/*更新宏定义模型*/
	virtual void SelectMacro(QModelIndex& indexSelected){};	/*选取宏定义*/
	virtual void EditText(const QString& text){};	/*编辑程序文本*/

protected:
	QList<QString> GetTokens(const QString& text);
	//QString GetText();
	void GetForList(QList<QString>& strList,QString& strText);

private:
	QString GetToken(QString& strText);
	void SkipInterval(QString& strText);
	void SkipBlank(QString& strText);
	bool IsBlank(QChar ch);
	bool IsInterval(QChar ch);

protected:
	CModelCategory* m_modelCategory;	/*宏定义分类模型*/
	CModelMacro* m_modelMacro;	/*宏定义模型*/

public:
	/*命令参数*/
    enum MACRO_TYPE
	{
		INT_PARAMETER_TYPE_POSITON,
		INT_PARAMETER_TYPE_VELOCITY,
		INT_PARAMETER_TYPE_ZONE,
		INT_PARAMETER_TYPE_FLOAT,
		INT_PARAMETER_TYPE_LABEL,
		INT_PARAMETER_TYPE_INT,
		INT_PARAMETER_TYPE_BOOL,
		INT_PARAMETER_TYPE_STRING,
		INT_PARAMETER_TYPE_NUMBER,
		INT_PARAMETER_TYPE_VAR_NUMBER,

		INT_MACRO_MOVL,
		INT_MACRO_MOVC,
		INT_MACRO_MOVJ,
		INT_MACRO_MOVX,
		INT_MACRO_MOVJX,
		INT_MACRO_MOVY,
		INT_MACRO_MOVZ,
		INT_MACRO_ROTX,
		INT_MACRO_ROTY,
		INT_MACRO_ROTZ,

		INT_MACRO_SIN,
		INT_MACRO_COS,
		INT_MACRO_TAN,
		INT_MACRO_SQRT,
		INT_MACRO_LABEL,
		INT_MACRO_GOTO,
		INT_MACRO_GOSUB,
		INT_MACRO_FOR,
		INT_MACRO_FOR_NEXT,
		INT_MACRO_IF_ENDIF,
		INT_MACRO_ELSEIF,
		INT_MACRO_SWITCH,
		INT_MACRO_SWITCH_DEFAULT_ENDSWITCH,
		INT_MACRO_CASE,
		INT_MACRO_BREAK,
		INT_MACRO_ABORT,
		INT_MACRO_END,

		INT_MACRO_SET_TOOLFRAME,
		INT_MACRO_SET_USERFRAME,

		INT_MACRO_ARC,
		INT_MACRO_ARC_OFF,
		INT_MACRO_SET_ARC,

		INT_MACRO_DOUT,
		INT_MACRO_DIN,
		INT_MACRO_AOUT,
		INT_MACRO_AIN,

		INT_MACRO_SPOT_ON,
		INT_MACRO_SPOT_OFF,
		INT_MACRO_SET_SPOT,

		INT_MACRO_WAIT,
	};
	/*采用字符串而非整型，是方便与xml文件中读取的对比*/
	const static QString STR_PARAMETER_TYPE_POSITON;
	const static QString STR_PARAMETER_TYPE_VELOCITY;
	const static QString STR_PARAMETER_TYPE_ZONE;
	const static QString STR_PARAMETER_TYPE_FLOAT;
	const static QString STR_PARAMETER_TYPE_LABEL;
	const static QString STR_PARAMETER_TYPE_INT;
	const static QString STR_PARAMETER_TYPE_BOOL;
	const static QString STR_PARAMETER_TYPE_STRING;
	const static QString STR_PARAMETER_TYPE_NUMBER;
	const static QString STR_PARAMETER_TYPE_VAR_NUMBER;

	const static QString STR_MACRO_MOVL;
	const static QString STR_MACRO_MOVC;
	const static QString STR_MACRO_MOVJ;
	const static QString STR_MACRO_MOVX;
	const static QString STR_MACRO_MOVJX;
	const static QString STR_MACRO_MOVY;
	const static QString STR_MACRO_MOVZ;
	const static QString STR_MACRO_ROTX;
	const static QString STR_MACRO_ROTY;
	const static QString STR_MACRO_ROTZ;

	const static QString STR_MACRO_SIN;
	const static QString STR_MACRO_COS;
	const static QString STR_MACRO_TAN;
	const static QString STR_MACRO_SQRT;
	const static QString STR_MACRO_LABEL;
	const static QString STR_MACRO_GOTO;
	const static QString STR_MACRO_GOSUB;
	const static QString STR_MACRO_FOR;
	const static QString STR_MACRO_FOR_NEXT;
	const static QString STR_MACRO_IF;
	const static QString STR_MACRO_IF_ENDIF;
	const static QString STR_MACRO_ELSEIF;
	const static QString STR_MACRO_SWITCH;
	const static QString STR_MACRO_SWITCH_DEFAULT_ENDSWITCH;
	const static QString STR_MACRO_CASE;
	const static QString STR_MACRO_DEFAULT;
	const static QString STR_MACRO_BREAK;
	const static QString STR_MACRO_ABORT;
	const static QString STR_MACRO_END;

	const static QString STR_MACRO_SET_TOOLFRAME;
	const static QString STR_MACRO_SET_USERFRAME;

	const static QString STR_MACRO_ARC;
	const static QString STR_MACRO_ARC_OFF;
	const static QString STR_MACRO_SET_ARC;

	const static QString STR_MACRO_DOUT;
	const static QString STR_MACRO_DIN;
	const static QString STR_MACRO_AOUT;
	const static QString STR_MACRO_AIN;

	const static QString STR_MACRO_SPOT_ON;
	const static QString STR_MACRO_SPOT_OFF;
	const static QString STR_MACRO_SET_SPOT;

	const static QString STR_MACRO_WAIT;

	/*命令格式*/
	const static char* STR_MACRO_FORMAT_MOVL;
	const static char* STR_MACRO_FORMAT_MOVC;
};

#endif
