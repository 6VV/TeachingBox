#ifndef _TEACHING_BOX_C_SCREEN_NEW_MACRO_H_
#define _TEACHING_BOX_C_SCREEN_NEW_MACRO_H_

#include "CScreenMainParent.h"
#include "CSingleTon.h"
#include "CButton.h"
#include "QTreeView"
#include "CGrammarManagerFactory.h"
#include "MacroInterface.h"

class CScreenNewMacro:public CScreenMainParent
{
	friend CSingleTon<CScreenNewMacro>;

	Q_OBJECT

private:
	CScreenNewMacro(QWidget* parent=0);
	CScreenNewMacro(const CScreenNewMacro&);
	CScreenNewMacro& operator=(const CScreenNewMacro&);
	~CScreenNewMacro();

	/*重写父类函数*/
protected:
	void RefreshText() Q_DECL_OVERRIDE;

	/*自定义公有函数*/
public:
	static CScreenNewMacro* GetInstance();

	/*自定义槽函数*/
protected slots:
	void SlotViewCategoryClicked(QModelIndex);
	void SlotButtonConfirmClicked();
	//void SlotButtonBackClicked();

	/*自定义私有函数*/
private:
	void Init();
	void InitLayout();
	void InitSignalSlot();

	/*接口*/
private:
	MacroInterface* m_macroInterface;

	/*自定义私有变量*/
private:
	const char* CLASS_NAME = "CScreenNewMacro";

	QTreeView* m_viewCategory;	/*分类树形视图*/
	QTreeView* m_viewMacro;	/*宏定义视图*/

	CGrammarManagerFactory* m_grammarManager;	/*语法管理*/

	/*底部按钮*/
	CButton* m_btnConfirm;
	CButton* m_btnCancle;
	//CButton* m_btnBack;
	CButton* m_btnNull1;
	CButton* m_btnNull2;
	CButton* m_btnNull3;
	CButton* m_btnNull4;

};

#endif