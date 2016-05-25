#ifndef _TEACHING_BOX_C_SCREEN_LINE_EDITOR_H_
#define _TEACHING_BOX_C_SCREEN_LINE_EDITOR_H_

#include "QWidget"
#include "CButton.h"
#include "QBoxLayout"
#include "CScreenBottom.h"
#include "CLineEditor.h"
#include "CVariableInterface.h"
#include "CKeyBoardInterface.h"
#include "CProgramInterface.h"
#include "CSingleTon.h"
#include "MacroInterface.h"

class CScreenLineEditor:public QWidget,public CVariableInterface,public CKeyBoardInterface
{
	friend CSingleTon<CScreenLineEditor>;

	Q_OBJECT

public:
	static CScreenLineEditor* GetInstance(MacroInterface*);

private:
	CScreenLineEditor();
	CScreenLineEditor(const CScreenLineEditor&);
	CScreenLineEditor& operator=(const CScreenLineEditor&);
	~CScreenLineEditor();

public:
	void SetInterface(MacroInterface*);
	void SetButtonSize(const QSize size);
	void SetText(const QString& strText);

	void SelectVariable(const QString name) Q_DECL_OVERRIDE;
	void KeyboardEdit() Q_DECL_OVERRIDE;

protected:
	void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
	void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

protected slots:
	void SlotOnButtonVariableClicked();
	void SlotOnButtonMacroClicked();
	void SlotOnButtonKeyboardClicked();
	void SlotOnButtonAddClicked();
	void SlotOnButtonDeleteClicked();
	void SlotOnButtonConfirmClicked();
	void SlotOnButtonBackClicked();

private:
	void Init();
	void InitLayout();
	void InitSignalSlot();

	void RefreshText();

private:
	void(CScreenLineEditor::*KeyboardOperator)();
	void EditVariable();
	void NewVariable();

private:
	const char* CLASS_NAME = "ClineEditor";

	MacroInterface* m_interface;

	CLineEditor* m_lineEditor;

	CButton* m_btnForward;
	CButton* m_btnBackward;

	CScreenBottom* m_screenBottom;
	CButton* m_btnVariable;
	CButton* m_btnMacro;
	CButton* m_btnKeyboard;
	CButton* m_btnAdd;
	CButton* m_btnDelete;
	CButton* m_btnConfirm;
	CButton* m_btnBack;
};

#endif