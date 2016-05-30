#ifndef _TEACHING_BOX_C_SCREEN_MAIN_H_
#define _TEACHING_BOX_C_SCREEN_MAIN_H_

#include "QVBoxLayout"
#include "CScreenProject.h"
#include "CScreenPragram.h"
#include "CScreenBottom.h"
#include "CScreenSetting.h"
#include "CSingleTon.h"
#include "CScreenNewMacro.h"
#include "CScreenNewVariable.h"
#include "CEditParameter.h"
#include "CScreenService.h"
#include "CScreenVariables.h"

class CScreenMain:public QVBoxLayout
{
	friend class CSingleTon<CScreenMain>;	/*单例模式*/

	Q_OBJECT

private:
	CScreenMain(QWidget* parent=0);
	CScreenMain(const CScreenMain&);
	CScreenMain& operator=(const CScreenMain&);
	~CScreenMain();

/*公共函数*/
public:
	static CScreenMain* GetInstance();	/*获取唯一实例*/
	static bool IsCreated();
	
	QStackedWidget* GetStackedWidget();

	void SetIsPush(bool isPush);	/*设置是否存储窗口*/

	int GetLeftPosition();	/*获取窗口左边位置*/
	int GetTopPosition();
	int GetRightPosition();
	int GetBottomPosition();

	void ChangeToScreenProgram();	/*切换到Program视图*/
	void ChangeToScreenProject();	/*切换到Project视图*/
	void ChangeToScreenService();	/*切换到Setting视图*/
	void ChangeToScreenNewMacro();	/*切换到NewMacro视图*/
	void ChangeToScreenEditMacro();	/*切换到EditMacro视图*/
	void ChangeToScreenNewVariable();	/*切换到NewParameter视图*/

	void ChangeToScreenVariables();	/*切换到变量视图*/

	//void RemoveScreenEditParameter();	/*移除EditParameter视图，并释放内存*/
	void RemoveScreenNewParameter();	/*移除NewParameter视图，并释放内存*/
/*私有函数*/
private:
	void Init();

	//template<typename Type>
	//void ChangeToScreen(QWidget*, Type);

private:
	QHBoxLayout* m_layoutCenter;	/*中央界面*/
	//QList<QWidget*> m_widgetList;	/*记录已打开的窗口*/
	QStackedWidget* m_stackedWidget;	/*堆栈窗体*/
	//QWidget* m_currentWidgetItem = NULL;	/*当前显示的窗口*/
	CScreenProject* m_screenProject=NULL;
	CScreenPragram* m_screenProgram=NULL;
	CScreenService* m_screenService = NULL;
	CScreenNewMacro* m_screenNewMacro = NULL;

	/*此部分可优先销毁*/
	CEditParameter* m_screenEditMacro = NULL;
	CScreenNewVariable* m_screenNewParameter=NULL;

	CScreenVariables* m_screenVariables = NULL;
private:
	bool m_isPush = true;	/*是否存储当前窗口，用于返回按钮*/
};

//template<typename Type>
//void CScreenMain::ChangeToScreen(QWidget* screen, Type instance)
//{
//	if (screen==NULL)
//	{
//		qDebug() << "create screen";
//		screen = instance;
//		m_stackedWidget->addWidget(screen);
//	}
//	m_stackedWidget->setCurrentWidget(screen);
//}

#endif