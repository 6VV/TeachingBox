#ifndef _TEACHING_BOX_C_NEW_PARAMETER_H_
#define _TEACHING_BOX_C_NEW_PARAMETER_H_

#include "CSingleTon.h"
#include "CScreenMainParent.h"
#include "QTableWidget"
#include "CButton.h"
#include "CParameterManager.h"
#include "CInterpreterAdapter.h"
#include "CLineEditWithRegExpAndKeyboard.h"


class CScreenNewVariable:public CScreenMainParent
{
	friend CSingleTon<CScreenNewVariable>;

	Q_OBJECT

public:
	static CScreenNewVariable* GetInstance();
	void SetScope(QString& strScopeName);

private:
	CScreenNewVariable(QWidget* parent = 0);
	CScreenNewVariable(const CScreenNewVariable&);
	CScreenNewVariable& operator=(const CScreenNewVariable&);
	~CScreenNewVariable();

/*父类函数*/
protected:
	void RefreshText() Q_DECL_OVERRIDE;	/*更新文本*/
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;	/*大小改变*/
	void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;	/*显示时*/
	void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;	/*隐藏时*/

protected slots:
	void SlotOnButtonConfirmClicked();	//确认按钮按下事件调用函数
	void SlotTypeChanged(QTreeWidgetItem* item, int column);	/*类型改变*/

private:
	void Init();
	void InitLayout();
	void InitLayoutTableWidget();
	void InitLayoutTreeWidget();
	void InitSignalSlot();

	bool IsIntValue(QString& strValue);		/*是否为整数*/
	bool IsFloatValue(QString& strValue);	/*是否为浮点数*/

	void InitIntParameter();				/*初始化整数参数*/
	void InitDoubleParameter();
	void InitBoolParameter();
	void InitStringParameter();

	void InitPositionParameter();
	void InitDynamicParameter();
	void InitOverlapParameter();

private:
	const char* CLASS_NAME = "CMacroNewParameter";
	const int INIT_ROW_NUM = 3;

	CLineEditWithRegExpAndKeyboard* m_lineEidtName;	/*变量名*/
	CLineEditWithRegExpAndKeyboard* m_lineEditScope;		/*作用域*/
	CLineEditWithRegExpAndKeyboard* m_lineEditType;		/*类型*/

private:
	QTreeWidget* m_treeWidget;	/*变量类型*/

	QTableWidget* m_tableWidget;	/*变量表格*/
	CButton* m_btnConfirm;		/*确认按钮*/
	//CButton* m_btnBack;			/*返回按钮*/

	QString m_strScopeName;		/*当前作用域名*/
	QString m_strScopeInDatabase;	/*数据库中的作用域名*/

};

#endif