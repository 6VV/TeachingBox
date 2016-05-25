#ifndef _TEACHING_BOX_C_SCREEN_PROGRAM_H_
#define _TEACHING_BOX_C_SCREEN_PROGRAM_H_

#include "QHBoxLayout"
#include "QTextEdit"
#include "Init.h"
#include "CButton.h"
#include "CScreenBottom.h"
#include "CScreenMainParent.h"
#include "CSingleTon.h"
#include "CCodeEditor.h"
#include "QTextCursor"
#include "QList"
#include "CWidgetButtonListVertical.h"
#include "CHighLighter.h"
#include "CProgramInterface.h"
#include "CScreenLineEditor.h"
#include "MacroInterface.h"

class CScreenPragram: public CScreenMainParent,public CProgramInterface,public MacroInterface
{
	friend class CSingleTon<CScreenPragram>;

	Q_OBJECT

private:
	CScreenPragram(QWidget* parent=0);
	CScreenPragram(const CScreenPragram&);
	CScreenPragram& operator=(const CScreenPragram&);
	~CScreenPragram();

	/*自定义父类函数*/
protected:
	void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
	void RefreshText() Q_DECL_OVERRIDE;	/*刷新*/

	void RemoveLineEditor() override;
	void UpdateCurrentLine(const QString& text) override;

	void EditMacroInterface(const QString& strText) override;

	/*回调函数*/
private:
	typedef void (CScreenPragram::*EditMacro)(const QString& strText);
	EditMacro EditMacroCallback;

	/*自定义公有函数*/
public:
	static CScreenPragram* GetInstance();
	static bool IsCreated();
	void EditNormalParameter(const QString& strLineText);	/*编辑某一行*/

	void InsertTextBeforeLine(const QString& strText);	/*插入文本*/
	void KeyboardEdit();	/*键盘确认按钮按下*/
	void RefreshPCLineNumber(const int lineNumber);	/*刷新行*/

	/*自定义私有函数*/
private:
	void Init();
	void InitLayout();
	void InitSignalSlot();

	void EditCurrentLine();		/*编辑当前行*/

	/*自定义槽函数*/
protected slots:
	void SlotOnButtonModifyClicked();	/*修改按钮*/
	void SlotOnButtonEditClicked();		/*编辑按钮*/
	void SlotOnButtonAddvanceClicked();	/*高级按钮*/
	void SlotOnButtonNewClicked();		/*新建按钮*/
	void SlotOnButtonMacroClicked();	/*宏定义命令按钮*/
	void SlotOnSetPcClicked();			/*SetPC按钮*/

	void SlotEdit();		/*编辑按钮列表事件*/
	void SlotAddvance();	/*高级按钮列表事件*/

private:
	CCodeEditor* m_codeEditor;	/*显示控件*/
	QString m_strCurrentFileName;	/*当前打开的文件名*/
	QString m_strCurrentFileWithSuffix;	/*当前打开的文件名（带后缀）*/
	QString m_strShearPlate;	/*剪切板*/

	
	QString GetCurrentLineText();	/*获取当前行文本*/
private:
	const char* CLASS_NAME = "CScreenProgram";
	CHighLighter* m_highLighter;

	/*底部按钮*/
	CButton* m_btnModify;
	CButton* m_btnMacro;
	CButton* m_btnNew;
	CButton* m_btnSetPC;
	CButton* m_btnEdit;
	CButton* m_btnAddvance;
	//CButton* m_btnBack;

	/*编辑按钮窗口*/
	CWidgetButtonListVertical* m_widgetEdit;
	QList<CButton*> m_btnListEdit;
	CButton* m_btnEditSelectAll;
	CButton* m_btnEditCut;
	CButton* m_btnEditCopy;
	CButton* m_btnEditPaste;
	CButton* m_btnEditDelete;
	CButton* m_btnEditUndo;
	CButton* m_btnEditRedo;

	/*高级按钮窗口*/
	CWidgetButtonListVertical* m_widgetAddvance;
	QList<CButton*> m_btnListAddvance;
	CButton* m_btnAddvanceKeyboard;
	CButton* m_btnAddvanceSubroutine;
	CButton* m_btnAddvanceFormat;
	CButton* m_btnAddvanceSearch;
	CButton* m_btnAddvanceNote;
	CButton* m_btnAddvanceDisable;

};

#endif