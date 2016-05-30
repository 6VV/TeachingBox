#include "stdafx.h"
#include "CScreenMain.h"
#include "CTeachingBox.h"
#include "CWidgetStack.h"

CScreenMain::CScreenMain(QWidget* parent/*=0*/) :QVBoxLayout(parent)
{
	//qDebug() << "ScreenMain Create";
	Init();
}

CScreenMain::~CScreenMain()
{
	//qDebug() << "ScreenMain Delete";
}

CScreenMain* CScreenMain::GetInstance()
{
	return CSingleTon<CScreenMain>::GetInstance();
}

bool CScreenMain::IsCreated()
{
	return CSingleTon<CScreenMain>::IsCreated();
}

QStackedWidget* CScreenMain::GetStackedWidget()
{
	return m_stackedWidget;
}

void CScreenMain::SetIsPush(bool isPush)
{
	m_isPush = isPush;
}

int CScreenMain::GetLeftPosition()
{
	return this->geometry().left() + CTeachingBox::GetInstance()->geometry().left();
}

int CScreenMain::GetTopPosition()
{
	return this->geometry().top() + CTeachingBox::GetInstance()->geometry().top();
}


int CScreenMain::GetRightPosition()
{
	return this->geometry().right() + CTeachingBox::GetInstance()->geometry().left();
}

int CScreenMain::GetBottomPosition()
{
	return this->geometry().bottom() + CTeachingBox::GetInstance()->geometry().top();
}

void CScreenMain::ChangeToScreenProgram()
{
	/*if (m_screenProgram == NULL)
	{
		m_screenProgram =CScreenProgram::GetInstance();
		m_stackedWidget->addWidget(m_screenProgram);
	}
	m_stackedWidget->setCurrentWidget(m_screenProgram);*/
	//CWidgetStack::GetInstance()->Clear();
	/*若当前未打开任何程序*/
	if (CScreenProject::GetInstance()->GetOpenedFileName().isEmpty())
	{
		return;
	}

	///*清除窗口返回栈*/
	//CWidgetStack::GetInstance()->Clear();

	if (m_isPush)
	{
		CWidgetStack::GetInstance()->PushWidget(m_stackedWidget->currentWidget());
	}

	if (m_screenProgram == NULL)
	{
		m_screenProgram = CScreenPragram::GetInstance();
		m_stackedWidget->addWidget(m_screenProgram);
	}
	m_stackedWidget->setCurrentWidget(m_screenProgram);
}

void CScreenMain::ChangeToScreenProject()
{
	/*if (m_screenProject == NULL)
	{
		m_screenProject = CScreenProject::GetInstance();
		m_stackedWidget->addWidget(m_screenProject);
	}
	m_stackedWidget->setCurrentWidget(m_screenProject);*/

	/*清除窗口返回栈*/
	//CWidgetStack::GetInstance()->Clear();

	if (m_isPush)
	{
		CWidgetStack::GetInstance()->PushWidget(m_stackedWidget->currentWidget());
	}

	/*切换窗口*/
	if (m_screenProject == NULL)
	{
		m_screenProject = CScreenProject::GetInstance();
		m_stackedWidget->addWidget(m_screenProject);
	}
	m_stackedWidget->setCurrentWidget(m_screenProject);
}

void CScreenMain::ChangeToScreenService()
{
	/*if (m_screenSetting == NULL)
	{
		m_screenSetting = CScreenSetting::GetInstance();
		m_stackedWidget->addWidget(m_screenSetting);
	}
	m_stackedWidget->setCurrentWidget(m_screenSetting);*/
	
	/*清除窗口返回栈*/
	//CWidgetStack::GetInstance()->Clear();

	if (m_isPush)
	{
		CWidgetStack::GetInstance()->PushWidget(m_stackedWidget->currentWidget());
	}

	/*切换窗口*/
	if (m_screenService == NULL)
	{
		m_screenService = CScreenService::GetInstance();
		m_stackedWidget->addWidget(m_screenService);
	}
	m_stackedWidget->setCurrentWidget(m_screenService);
}

void CScreenMain::ChangeToScreenNewMacro()
{
	/*if (m_screenNewMacro==NULL)
	{
	m_screenNewMacro = CScreenNewMacro::GetInstance();
	m_stackedWidget->addWidget(m_screenNewMacro);
	}
	m_stackedWidget->setCurrentWidget(m_screenNewMacro);*/
	if (m_isPush)
	{
		CWidgetStack::GetInstance()->PushWidget(m_stackedWidget->currentWidget());
	}

	/*切换窗口*/
	if (m_screenNewMacro == NULL)
	{
		m_screenNewMacro = CScreenNewMacro::GetInstance();
		m_stackedWidget->addWidget(m_screenNewMacro);
	}
	m_stackedWidget->setCurrentWidget(m_screenNewMacro);
}

void CScreenMain::ChangeToScreenEditMacro()
{
	if (m_isPush)
	{
		CWidgetStack::GetInstance()->PushWidget(m_stackedWidget->currentWidget());
	}

	/*切换窗口*/
	if (m_screenEditMacro == NULL)
	{
		m_screenEditMacro = CEditParameter::GetInstance();
		m_stackedWidget->addWidget(m_screenEditMacro);
	}
	m_stackedWidget->setCurrentWidget(m_screenEditMacro);

}

void CScreenMain::ChangeToScreenNewVariable()
{
	if (m_isPush)
	{
		CWidgetStack::GetInstance()->PushWidget(m_stackedWidget->currentWidget());
	}

	/*切换窗口*/
	if (m_screenNewParameter == NULL)
	{
		m_screenNewParameter = CScreenNewVariable::GetInstance();
		m_stackedWidget->addWidget(m_screenNewParameter);
	}
	m_stackedWidget->setCurrentWidget(m_screenNewParameter);
}

void CScreenMain::ChangeToScreenVariables()
{
	if (m_isPush)
	{
		CWidgetStack::GetInstance()->PushWidget(m_stackedWidget->currentWidget());
	}

	/*切换窗口*/
	if (m_screenVariables == NULL)
	{
		m_screenVariables = CScreenVariables::GetInstance();
		m_stackedWidget->addWidget(m_screenVariables);
	}
	m_stackedWidget->setCurrentWidget(m_screenVariables);
}

//void CScreenMain::RemoveScreenEditParameter()
//{
//	if (m_screenEditParatmter != NULL)
//	{
//		m_stackedWidget->removeWidget(m_screenEditParatmter);
//		m_screenEditParatmter = NULL;
//		CSingleTon<CScreenEditMacroParameter>::Destroy();
//	}
//}

void CScreenMain::RemoveScreenNewParameter()
{
	if (m_screenNewParameter != NULL)
	{
		m_stackedWidget->removeWidget(m_screenNewParameter);
		m_screenNewParameter = NULL;
		CSingleTon<CScreenNewVariable>::Destroy();
	}
}

void CScreenMain::Init()
{
	m_stackedWidget = new QStackedWidget;

	this->addWidget(m_stackedWidget);
}

