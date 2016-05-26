#include "stdafx.h"
#include "CScreenEditMacroParameter.h"
#include "CGrammarManagerFactory.h"
#include "CMacroParameterPosition.h"
#include "CWidgetStack.h"
#include "CScreenEditMacro.h"
#include "CMacroParameterVelocity.h"
#include "CMacroParameterLabel.h"
#include "CMacroParameterFloat.h"
#include "CMacroParameterZone.h"
#include "CMacroParameterBool.h"
#include "CMacroParameterInt.h"
#include "CMacroParameterString.h"


CScreenEditMacroParameter::CScreenEditMacroParameter()
{
	Init();
}

CScreenEditMacroParameter::~CScreenEditMacroParameter()
{
}

CScreenEditMacroParameter* CScreenEditMacroParameter::GetInstance()
{
	return CSingleTon<CScreenEditMacroParameter>::GetInstance();
}

/*************************************************
//  Function: 		ChangeWidget
//  Description: 	根据参数类型转到不同界面
//  Calls:		 	ChangeWidget
//  Called By: 		CScreenEditMacro::SlotOnButtonAddClicked
//  Parameter:      QString
//  Return: 		void
//  Others: 		
*************************************************/
void CScreenEditMacroParameter::ChangeWidget(QString parameterType)
{
	/*若已存在其它控件，则清除*/
	QObjectList listObject = m_layout->children();
	if (listObject.size()>0)
	{
		qDeleteAll(listObject);
		listObject.clear();
	}

	if (parameterType==CGrammarManagerFactory::STR_PARAMETER_TYPE_POSITON)
	{
		ChangeToPositionWidget();
	}
	else if (parameterType==CGrammarManagerFactory::STR_PARAMETER_TYPE_VELOCITY)
	{
		ChangeToVelocityWidget();
	}
	else if (parameterType==CGrammarManagerFactory::STR_PARAMETER_TYPE_ZONE)
	{
		ChangeToZoneWidget();
	}
	else if (parameterType==CGrammarManagerFactory::STR_PARAMETER_TYPE_FLOAT)
	{
		ChangeToDoubleWidget();
	}
	else if (parameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_LABEL)
	{
		ChangeToLableWidget();
	}
	else if (parameterType==CGrammarManagerFactory::STR_PARAMETER_TYPE_INT)
	{
		ChangeToIntWidget();
	}
	else if (parameterType==CGrammarManagerFactory::STR_PARAMETER_TYPE_BOOL)
	{
		ChangeToBoolWidget();
	}
	else if (parameterType == CGrammarManagerFactory::STR_PARAMETER_TYPE_STRING)
	{
		ChangeToStringWidget();
	}
}

void CScreenEditMacroParameter::hideEvent(QHideEvent *event)
{
	delete(m_currentWidget);
}

void CScreenEditMacroParameter::Init()
{
	m_layout = new QHBoxLayout(this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
}


//void CScreenEditMacroParameter::ChangeToNewParameterWidget()
//{
//	CScreenNewParameter* widget = new CScreenNewParameter;
//	m_layout->addWidget(widget);
//	m_currentWidget = widget;
//}

void CScreenEditMacroParameter::UpdateParameterName(QString& strName)
{
	/*若上一界面为编辑命令界面*/
	if (typeid(*(CWidgetStack::GetInstance()->GetBackWidget()))==typeid(CScreenEditMacro))
	{
		CScreenEditMacro::GetInstance()->UpdateParameterName(strName);
	}
	
}

void CScreenEditMacroParameter::ChangeToPositionWidget()
{
	CMacroParameterPosition* widget = new CMacroParameterPosition;
	m_layout->addWidget(widget);
	m_currentWidget = widget;
}

void CScreenEditMacroParameter::ChangeToVelocityWidget()
{
	CMacroParameterVelocity* widget = new CMacroParameterVelocity;
	m_layout->addWidget(widget);
	m_currentWidget = widget;
}

void CScreenEditMacroParameter::ChangeToZoneWidget()
{
	CMacroParameterZone* widget = new CMacroParameterZone;
	m_layout->addWidget(widget);
	m_currentWidget = widget;
}

void CScreenEditMacroParameter::ChangeToDoubleWidget()
{
	CMacroParameterFloat* widget = new CMacroParameterFloat;
	m_layout->addWidget(widget);
	m_currentWidget = widget;
}

void CScreenEditMacroParameter::ChangeToLableWidget()
{
	CMacroParameterLabel* widget = new CMacroParameterLabel;
	m_layout->addWidget(widget);
	m_currentWidget = widget;
}

void CScreenEditMacroParameter::ChangeToIntWidget()
{
	CMacroParameterInt* widget = new CMacroParameterInt;
	m_layout->addWidget(widget);
	m_currentWidget = widget;
}

void CScreenEditMacroParameter::ChangeToBoolWidget()
{
	CMacroParameterBool* widget = new CMacroParameterBool;
	m_layout->addWidget(widget);
	m_currentWidget = widget;
}

void CScreenEditMacroParameter::ChangeToStringWidget()
{
	CMacroParameterString* widget = new CMacroParameterString;
	m_layout->addWidget(widget);
	m_currentWidget = widget;
}
