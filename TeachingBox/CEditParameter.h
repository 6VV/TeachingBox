#ifndef _TEACHING_BOX_C_EDIT_PARAMETER_H_
#define _TEACHING_BOX_C_EDIT_PARAMETER_H_

#include "QWidget"
#include "CKeyBoardInterface.h"
#include "DataStruct.h"
#include "CScreenMainParent.h"
#include "CSingleTon.h"
#include "QList"
#include "CLineEditWithClickedSignal.h"
#include "CRegExpManager.h"
#include "CComboBoxWithTreeItem.h"
#include "CInterpreterAdapter.h"
#include "MacroInterface.h"


class CEditParameter:public CScreenMainParent
{
	friend CSingleTon<CEditParameter>;

	Q_OBJECT

private:
	CEditParameter(QWidget* parent=0);
	CEditParameter(const CEditParameter&);
	CEditParameter& operator=(const CEditParameter&);
	~CEditParameter();

public:
	static CEditParameter* GetInstance();

	void ChangeContent(const QString& macroName, const QStringList& strListParameterName);

	void SetMacroInterface(MacroInterface* macroInterface);

	/*重写父类函数*/
protected:
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
	void RefreshText() Q_DECL_OVERRIDE;

	void BackToWidget(QWidget*);
	void Back() override;

	/*键盘回调函数*/
protected slots:
	void SlotOnButtonConfirmClicked();	/*确认按钮按下*/

	void SlotOnComboBoxItemChanged(const QString&);	/*ComboBox文本改变事件*/

private:
	void Init();			/*初始化*/
	void InitLayout();		/*初始化布局*/
	void InitSignalSlot();	/*初始化信号槽*/

	void AddPositionParameter(const QString& strText);	/*获取位置参数*/
	void AddDynamicParameter(const QString& strText);		/*获取速度参数*/
	void AddOverlapParameter(const QString& strText);		/*获取过渡区域参数*/

	CComboBoxWithTreeItem* GetComboBoxPosition();	/*获取位置列表*/
	CComboBoxWithTreeItem* GetComboBoxDynamic();	/*获取速度列表*/
	CComboBoxWithTreeItem* GetComboBoxOverlap();	/*获取过渡区域列表*/

	void RefreshPositionParameter(QString strName,QTreeWidgetItem* widgetItem);	/*刷新位置参数*/
	void RefreshDynamicParameter(QString strName, QTreeWidgetItem* widgetItem);	/*刷新位置参数*/
	void RefreshOverlapParameter(QString strName, QTreeWidgetItem* widgetItem);	/*刷新位置参数*/

	void ChangeToMovl();
	void ChangeToMovc();

private:
	const char* CLASS_NAME = "CEditParamter";

	QTreeWidget* m_treeWidget;	/*树形控件*/

	CButton* m_btnConfirm;		/*确认按钮*/
	CButton* m_btnEdit;			/*编辑按钮*/
	//CButton* m_btnBack;			/*返回按钮*/

private:
	QString m_strMacroName;
	QStringList m_strListParameterName;
	CInterpreterAdapter* m_interpreterAdapter;
	QString m_strScope;		/*当前文件作用域*/

	MacroInterface* m_macroInterface;
};

#endif