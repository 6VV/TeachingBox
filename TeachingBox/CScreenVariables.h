#ifndef _TEACHING_BOX_C_SCREEN_VARIABLES_H_
#define _TEACHING_BOX_C_SCREEN_VARIABLES_H_

#include "QWidget"
#include "QTreeWidget"
#include "CScreenMainParent.h"
#include "CButton.h"
#include "CWidgetButtonListVertical.h"
#include "CValue.h"
#include "CDatabaseManager.h"
#include "CScreenProject.h"
#include "CKeyBoardInterface.h"
#include "CVariableInterface.h"
#include "CTreeWidgetWithMonitor.h"

class CScreenVariables:public CScreenMainParent,public CKeyBoardInterface
{
	friend CSingleTon<CScreenVariables>;

	Q_OBJECT

public:
	static CScreenVariables* GetInstance();

	void SetVariableInterface(CVariableInterface* variableInterface);
	void SetConfirmButtonState(bool state);

private:
	CScreenVariables(QWidget* parent=0);
	CScreenVariables(const CScreenVariables&);
	CScreenVariables& operator=(const CScreenVariables&);
	~CScreenVariables();
	

protected:
	void KeyboardEdit();

	void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
	void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

protected slots:
	void SlotOnButtonVariableClicked();	/*变量按钮按下事件*/

	void SlotOnButtonNewVariableClicked();		/*新建变量按钮按下*/
	void SlotOnButtonCopyVariableClicked()	;	/*复制按钮按下*/
	void SlotOnButtonPasteVariableClicked();	/*粘贴按钮按下*/
	void SlotOnButtonDeleteVariableClicked();	/*删除按钮按下*/
	void SlotOnButtonCutVariableClicked(); 		/*剪切按钮按下*/
	void SlotOnButtonRenameVariableClicked();	/*重命名按钮按下*/

	void SlotOnButtonConfirmClicked();			/*确认按钮按下*/
	void SlotOnButtonTeachClicked();			/*示教按钮按下*/

	void SlotOnResivePosition(tAxesAllPositions& position);	/*位置改变*/
private:
	enum VariableUsingState
	{
		USED,
		NOT_USED,
	};

private:
	void Init();
	void InitLayout();
	void InitSignalSlot();
	void InitState();

	void InitTreeWidgetData();	/*初始化树形控件*/
	void InitTreeWidgetDataFromScope(QTreeWidgetItem* item,const std::string& strScopeName);

	void RefreshText();

	void PasteNewVariable(const QString& strName, const QString& strScope);

	void DeleteVariableWithoutCheck(const QString& strName, const QString& strType, const QString& strScope);	/*直接删除变量*/
	void DeleteLocalVariable(const QString& strName, const QString& strType, const QString& strScope);	/*删除局部变量*/
	void DeleteProjectVariable(const QString& strName, const QString& strType, const QString& strScope);	/*删除项目变量*/
	void DeleteGlobalVariable(const QString& strName, const QString& strType, const QString& strScope);	/*删除全局变量*/

	VariableUsingState CheckVariableState(const QString& strName, const QString& strType, const QString& strScope);	/*判断变量的使用状态*/
	VariableUsingState CheckVariableStateInGlobalFiles(const QString& strName, const QString& strType);
	VariableUsingState CheckVariableStateInProjectFiles(const QString& strName, const QString& strType, const QString& strScope);
	VariableUsingState CheckVariableStateInFile(const QString& strName, const QString& strType, const QString& strScope);
	QString GetScope(QTreeWidgetItem* itemCurrent);
	
private:
	CVariableInterface* m_variableInterface=NULL;
	CValue* m_value;
	CDatabaseManager* m_databaseManager;
	CScreenProject* m_screenProject;
	QTreeWidgetItem* m_treeItemNeedToDelete=NULL;
	enum CopyOperator
	{
		COPY,
		CUT,
	};
	/*复制操作*/
	struct VariableCopied
	{
		QString varName;
		QString varType;
		QString varScope;
		CopyOperator copyOperator;
		QTreeWidgetItem* itemCut;

		void Clear()
		{
			varName.clear();
			varType.clear();
			varScope.clear();
		}
	}m_varCopied;

private:
	const char* CLASS_NAME = "CScreenVariable";


	CTreeWidgetWithMonitor* m_treeWidget;

	CButton* m_btnVariable;
	CButton* m_btnTeach;
	CButton* m_btnClearUnusedVariable;
	CButton* m_btnCheck;


	CButton* m_btnDeleteVariable;
	CButton* m_btnPasteVariable;
	CButton* m_btnCopyVariable;
	CButton* m_btnCutVaribale;
	CButton* m_btnRenameVariable;
	CButton* m_btnNewVariable;
	CButton* m_btnConfirm;
	CWidgetButtonListVertical* m_btnListWidgetVariable;
};

#endif