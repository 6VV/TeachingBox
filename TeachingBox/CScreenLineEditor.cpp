#include "stdafx.h"
#include "CScreenLineEditor.h"
#include "QBoxLayout"
#include "CLabelWithSignal.h"
#include "CScreenMain.h"
#include "CScreenVariables.h"
#include "CKeyBoard.h"


CScreenLineEditor* CScreenLineEditor::GetInstance(MacroInterface* newInterface)
{
	CScreenLineEditor* newIntance = CSingleTon<CScreenLineEditor>::GetInstance();
	newIntance->SetInterface(newInterface);
	return newIntance;
}


CScreenLineEditor::CScreenLineEditor()
{
	Init();
}

CScreenLineEditor::~CScreenLineEditor()
{
	qDebug() << "delete";
}

void CScreenLineEditor::SetInterface(MacroInterface* newInterface)
{
	m_interface = newInterface;
}

void CScreenLineEditor::SetButtonSize(const QSize size)
{
	m_btnConfirm->setFixedSize(size);
	m_btnForward->setFixedHeight(size.height());
}

void CScreenLineEditor::SetText(const QString& strText)
{
	m_lineEditor->SetAllText(strText);
}


void CScreenLineEditor::SelectVariable(const QString name)
{
	m_lineEditor->SetLabelText(name);
}

void CScreenLineEditor::KeyboardEdit()
{
	(this->*KeyboardOperator)();
}

void CScreenLineEditor::changeEvent(QEvent *event)
{
	QWidget::changeEvent(event);

	switch (event->type())
	{
	case QEvent::LanguageChange:
	{
		RefreshText();
	}break;
	}
}

void CScreenLineEditor::hideEvent(QHideEvent *event)
{
	//delete(this);
}

void CScreenLineEditor::SlotOnButtonVariableClicked()
{
	CScreenMain::GetInstance()->ChangeToScreenVariables();
	CScreenVariables::GetInstance()->SetConfirmButtonState(true);
	CScreenVariables::GetInstance()->SetVariableInterface(this);
}

void CScreenLineEditor::SlotOnButtonMacroClicked()
{

}

void CScreenLineEditor::SlotOnButtonKeyboardClicked()
{
	KeyboardOperator = &CScreenLineEditor::EditVariable;
	CKeyboard* keyboard = CKeyboard::GetInstance();
	keyboard->SetCurrentText(m_lineEditor->GetLabelText());
	keyboard->SetKeyboardInterface(this);
	keyboard->show();
}

void CScreenLineEditor::SlotOnButtonAddClicked()
{
	KeyboardOperator = &CScreenLineEditor::NewVariable;
	CKeyboard* keyboard = CKeyboard::GetInstance();
	keyboard->SetCurrentText(m_lineEditor->GetLabelText());
	keyboard->SetKeyboardInterface(this);
	keyboard->show();
}

void CScreenLineEditor::SlotOnButtonDeleteClicked()
{
	m_lineEditor->DeleteCurrentLabel();
}

void CScreenLineEditor::SlotOnButtonConfirmClicked()
{
	m_interface->EditMacroInterface(m_lineEditor->GetAllText());
	SlotOnButtonBackClicked();
}

void CScreenLineEditor::SlotOnButtonBackClicked()
{
	m_interface->RemoveLineEditor();
}

void CScreenLineEditor::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();
}

void CScreenLineEditor::InitLayout()
{
	/*第一行*/
	QHBoxLayout* layoutTop = new QHBoxLayout;
	m_btnForward = new CButton;
	m_btnBackward = new CButton;

	m_lineEditor = new CLineEditor();

	layoutTop->addWidget(m_btnForward);
	layoutTop->addWidget(m_lineEditor);
	layoutTop->addWidget(m_btnBackward);

	layoutTop->setStretch(1, 1);

	/*第二行*/
	m_screenBottom = new CScreenBottom;

	m_btnVariable = new CButton;
	m_btnMacro = new CButton;
	m_btnKeyboard = new CButton;
	m_btnAdd = new CButton;
	m_btnDelete = new CButton;
	m_btnConfirm = new CButton;
	m_btnBack = new CButton;

	QList<CButton*> btnList;
	btnList.append(m_btnVariable);
	btnList.append(m_btnMacro);
	btnList.append(m_btnKeyboard);
	btnList.append(m_btnAdd);
	btnList.append(m_btnDelete);
	btnList.append(m_btnConfirm);
	btnList.append(m_btnBack);

	m_screenBottom->AddButtonList(btnList);

	/*全部*/
	QVBoxLayout* layoutMain = new QVBoxLayout(this);
	layoutMain->setMargin(0);
	layoutMain->setSpacing(0);
	layoutMain->addLayout(layoutTop);
	layoutMain->addLayout(m_screenBottom);
}

void CScreenLineEditor::InitSignalSlot()
{
	connect(m_btnVariable, SIGNAL(clicked()), this, SLOT(SlotOnButtonVariableClicked()));
	connect(m_btnMacro, SIGNAL(clicked()), this, SLOT(SlotOnButtonMacroClicked()));
	connect(m_btnKeyboard, SIGNAL(clicked()), this, SLOT(SlotOnButtonKeyboardClicked()));
	connect(m_btnAdd, SIGNAL(clicked()), this, SLOT(SlotOnButtonAddClicked()));
	connect(m_btnDelete, SIGNAL(clicked()), this, SLOT(SlotOnButtonDeleteClicked()));
	connect(m_btnConfirm, SIGNAL(clicked()), this, SLOT(SlotOnButtonConfirmClicked()));
	connect(m_btnBack, SIGNAL(clicked()), this, SLOT(SlotOnButtonBackClicked()));
}

void CScreenLineEditor::RefreshText()
{
	m_btnVariable->setText(QCoreApplication::translate(CLASS_NAME, "Variable"));
	m_btnMacro->setText(QCoreApplication::translate(CLASS_NAME, "Macro"));
	m_btnKeyboard->setText(QCoreApplication::translate(CLASS_NAME, "Keyboard"));
	m_btnAdd->setText(QCoreApplication::translate(CLASS_NAME, "Add"));
	m_btnDelete->setText(QCoreApplication::translate(CLASS_NAME, "Delete"));
	m_btnConfirm->setText(QCoreApplication::translate(CLASS_NAME, "Confirm"));
	m_btnBack->setText(QCoreApplication::translate(CLASS_NAME, "Back"));

	m_btnForward->setText(QCoreApplication::translate(CLASS_NAME, "Forward"));
	m_btnBackward->setText(QCoreApplication::translate(CLASS_NAME, "Backward"));
}

void CScreenLineEditor::EditVariable()
{
	m_lineEditor->SetLabelText(CKeyboard::GetInstance()->GetCurrentText());
}

void CScreenLineEditor::NewVariable()
{
	m_lineEditor->AddLabel(CKeyboard::GetInstance()->GetCurrentText());
}
