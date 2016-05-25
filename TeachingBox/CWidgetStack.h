#ifndef _TEACHING_BOX_C_WIDGET_STACK_H_
#define _TEACHING_BOX_C_WIDGET_STACK_H_

#include "CSingleTon.h"
#include "QList"

class CWidgetStack
{
	friend CSingleTon<CWidgetStack>;
public:
	CWidgetStack();
	~CWidgetStack();

	static CWidgetStack* GetInstance();

	void PushWidget(QWidget* widget);	/*添加窗口*/
	QWidget* PopWidget();	/*清除尾部窗口*/
	QWidget* GetBackWidget();	/*获取尾部窗口*/
	void Clear();	/*清除窗口列表*/
	void Back();	/*返回上一窗口*/
	void BackToWidget(QWidget* widget);	/*返回到特定窗口*/
	bool IsEmpty();	/*是否为空*/
protected:
	
private:
	QList<QWidget*> m_listWidget;
};

#endif