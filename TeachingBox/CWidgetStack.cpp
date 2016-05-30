#include "stdafx.h"
#include "CWidgetStack.h"
#include "CScreenPragram.h"
#include "CScreenProject.h"
#include "CScreenMain.h"

CWidgetStack::CWidgetStack()
{

}

CWidgetStack::~CWidgetStack()
{
	m_listWidget.clear();
}

CWidgetStack* CWidgetStack::GetInstance()
{
	return CSingleTon<CWidgetStack>::GetInstance();
}

void CWidgetStack::PushWidget(QWidget* widget)
{
	m_listWidget.push_back(widget);
}

QWidget* CWidgetStack::PopWidget()
{
	if (m_listWidget.isEmpty())
	{
		return NULL;
	}
	QWidget* widget = m_listWidget.back();
	m_listWidget.pop_back();
	return widget;
}

QWidget* CWidgetStack::GetBackWidget()
{
	return m_listWidget.back();
}

void CWidgetStack::Clear()
{
	m_listWidget.clear();
}

void CWidgetStack::Back()
{
	/*返回上一窗口时，不再存储当前窗口*/
	CScreenMain::GetInstance()->SetIsPush(false);

	QWidget* widget = PopWidget();
	if (widget == NULL)
	{
		CScreenMain::GetInstance()->SetIsPush(true);

		return;
	}
	/*若为程序窗口*/
	if (typeid(*widget)==typeid(CScreenPragram))
	{
		CScreenMain::GetInstance()->ChangeToScreenProgram();
	}
	/*若为项目窗口*/
	else if (typeid(*widget) == typeid(CScreenProject))
	{
		CScreenMain::GetInstance()->ChangeToScreenProject();
	}
	/*若为设置窗口*/
	else if (typeid(*widget)==typeid(CScreenSetting))
	{
		CScreenMain::GetInstance()->ChangeToScreenService();
	}
	///*若为编辑命令窗口*/
	//else if (typeid(*widget)==typeid(CScreenEditMacro))
	//{
	//	CScreenMain::GetInstance()->ChangeToScreenEditMacro();
	//}
	///*若为编辑参数窗口*/
	//else if (typeid(*widget)==typeid(CScreenEditMacroParameter))
	//{
	//	CScreenMain::GetInstance()->ChangeToScreenEditParameter();
	//}
	else if (typeid(*widget) == typeid(CScreenNewVariable))
	{
		CScreenMain::GetInstance()->ChangeToScreenNewVariable();
	}
	else if (typeid(*widget)==typeid(CScreenVariables))
	{
		CScreenMain::GetInstance()->ChangeToScreenVariables();
	}
	else if (typeid(*widget)==typeid(CScreenNewMacro))
	{
		CScreenMain::GetInstance()->ChangeToScreenNewMacro();
	}

	CScreenMain::GetInstance()->SetIsPush(true);
}

void CWidgetStack::BackToWidget(QWidget* widget)
{
	int index=m_listWidget.indexOf(widget)+1;

	while (m_listWidget.size()>index)
	{
		m_listWidget.pop_back();
	}

	Back();
}

bool CWidgetStack::IsEmpty()
{
	return m_listWidget.isEmpty();
}
