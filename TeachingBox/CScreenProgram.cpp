#include "stdafx.h"
#include "CScreenProgram.h"
#include "CScreenProject.h"
#include "QFile"
#include "CKeyBoard.h"
#include "CCodeEditor.h"
#include "CScreenMain.h"


CScreenProgram::CScreenProgram(QWidget* parent/*=0*/) :CScreenMainParent(parent)
{
	Init();
	qDebug() << "ScreenProgram Created";
}

CScreenProgram::~CScreenProgram()
{
	qDebug() << "ScreenProgram Destroyed";
	m_btnList.clear();

	delete(m_widgetEdit);
}

void CScreenProgram::showEvent(QShowEvent *event)
{
	CScreenMainParent::showEvent(event);
	//QString fileName=CScreenProject::GetInstance()->GetOpenedFilePath();
	///*若已打开该文档，则返回*/
	//if (m_strCurrentFileName == fileName)
	//{
	//	return;
	//}
	//else
	//{
	//	m_strCurrentFileName = fileName;
	//	m_codeEditor->clear();
	//	m_strCurrentFileWithSuffix = m_strCurrentFileName + ".txt";
	//	QFile file(m_strCurrentFileWithSuffix);
	//	//qDebug() << "test"<<m_strCurrentFileName;
	//	if (file.open(QFile::ReadOnly))
	//	{
	//		QString text;

	//		QTextStream fileIn(&file);
	//		fileIn.setCodec("UTF-8");
	//		fileIn.seek(0);

	//		text = fileIn.readAll();
	//		m_codeEditor->setPlainText(text);
	//		file.close();
	//	}
	//}

}

void CScreenProgram::RemoveLineEditor()
{
	CScreenLineEditor* lineEditor = CScreenLineEditor::GetInstance(this);
	m_layoutMain->removeWidget(lineEditor);
	lineEditor->hide();

	m_layoutMain->addLayout(m_screenBottom);

	/*重置控件大小*/
	ResizeControlers();
}

void CScreenProgram::UpdateCurrentLine(const QString& text)
{
	m_codeEditor->UpdateLineText(text);
}

void CScreenProgram::EditMacroInterface(const QString& strText)
{
	(this->*EditMacroCallback)(strText);
}

void CScreenProgram::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();

	m_highLighter = new CHighLighter(m_codeEditor->document());
}

void CScreenProgram::InitLayout()
{
	m_codeEditor = CCodeEditor::GetInstance();

	/*创建底部按钮*/
	m_btnModify = new CButton;
	m_btnMacro = new CButton;
	m_btnNew = new CButton;
	m_btnSetPC = new CButton;
	m_btnEdit = new CButton;
	m_btnAddvance = new CButton;
	//m_btnBack = new CButton;

	m_btnList.append(m_btnModify);
	m_btnList.append(m_btnMacro);
	m_btnList.append(m_btnNew);
	m_btnList.append(m_btnSetPC);
	m_btnList.append(m_btnEdit);
	m_btnList.append(m_btnAddvance);
	m_btnList.append(m_btnBack);

	/*总布局*/
	m_layoutCenter->addWidget(m_codeEditor);
	m_screenBottom->AddButtonList(m_btnList);

	/*编辑按钮列表*/
	m_btnEditSelectAll = new CButton;
	m_btnEditCut = new CButton;
	m_btnEditCopy = new CButton;
	m_btnEditPaste = new CButton;
	m_btnEditDelete = new CButton;
	m_btnEditUndo = new CButton;
	m_btnEditRedo = new CButton;

	m_btnListEdit.append(m_btnEditSelectAll);
	m_btnListEdit.append(m_btnEditCut);
	m_btnListEdit.append(m_btnEditCopy);
	m_btnListEdit.append(m_btnEditPaste);
	m_btnListEdit.append(m_btnEditDelete);
	m_btnListEdit.append(m_btnEditUndo);
	m_btnListEdit.append(m_btnEditRedo);

	m_widgetEdit = new CWidgetButtonListVertical(m_btnListEdit);
	
	/*高级按钮列表*/
	m_btnAddvanceKeyboard = new CButton;
	m_btnAddvanceSubroutine = new CButton;
	m_btnAddvanceFormat = new CButton;
	m_btnAddvanceSearch = new CButton;
	m_btnAddvanceNote = new CButton;
	m_btnAddvanceDisable = new CButton;

	m_btnListAddvance.append(m_btnAddvanceKeyboard);
	m_btnListAddvance.append(m_btnAddvanceSubroutine);
	m_btnListAddvance.append(m_btnAddvanceFormat);
	m_btnListAddvance.append(m_btnAddvanceSearch);
	m_btnListAddvance.append(m_btnAddvanceNote);
	m_btnListAddvance.append(m_btnAddvanceDisable);

	m_widgetAddvance = new CWidgetButtonListVertical(m_btnListAddvance);
}

void CScreenProgram::InitSignalSlot()
{
	/*修改按钮*/
	connect(m_btnModify, SIGNAL(clicked()), this, SLOT(SlotOnButtonModifyClicked()));

	/*编辑按钮*/
	connect(m_btnEdit, SIGNAL(clicked()), this, SLOT(SlotOnButtonEditClicked()));

	/*高级按钮*/
	connect(m_btnAddvance, SIGNAL(clicked()), this, SLOT(SlotOnButtonAddvanceClicked()));

	/*新建按钮*/
	connect(m_btnNew, SIGNAL(clicked()), this, SLOT(SlotOnButtonNewClicked()));

	/*宏定义按钮*/
	connect(m_btnMacro, SIGNAL(clicked()), this, SLOT(SlotOnButtonMacroClicked()));

	connect(m_btnSetPC, SIGNAL(clicked()), this, SLOT(SlotOnSetPcClicked()));

	/*编辑按钮列表点击事件*/
	for each (CButton* button in m_btnListEdit)
	{
		connect(button, SIGNAL(clicked()), this, SLOT(SlotEdit()));
	}

	/*高级按钮列表点击事件*/
	for each (CButton* button in m_btnListAddvance)
	{
		connect(button, SIGNAL(clicked()), this, SLOT(SlotAddvance()));
	}

	/*返回按钮*/
	//connect(m_btnBack, SIGNAL(clicked()), this, SLOT(SlotOnButtonBackClicked()));
}

/*************************************************
//  Function: 		RefreshText
//  Description: 	刷新控件文本，用于国际化支持
//  Calls:		 	
//  Called By: 		changeEvent
//  Parameter:      void
//  Return: 		void
//  Others: 		本函数重写父类函数，父类函数在CScreenMainParent中定义
*************************************************/
void CScreenProgram::RefreshText()
{
	/*底部按钮文本*/
	m_btnModify->setText(QCoreApplication::translate("CScreenProgram", "Modefy"));
	m_btnMacro->setText(QCoreApplication::translate("CScreenProgram", "Macro"));
	m_btnNew->setText(QCoreApplication::translate("CScreenProgram", "New"));
	m_btnSetPC->setText(QCoreApplication::translate("CScreenProgram", "Set PC"));
	m_btnEdit->setText(QCoreApplication::translate("CScreenProgram", "Edit"));
	m_btnAddvance->setText(QCoreApplication::translate("CScreenProgram", "Addvance"));
	m_btnBack->setText(QCoreApplication::translate("CScreenProgram", "Back"));

	/*编辑按钮文本*/
	m_btnEditSelectAll->setText(QCoreApplication::translate("CScreenProgram", "Select All"));
	m_btnEditCut->setText(QCoreApplication::translate("CScreenProgram", "Cut"));
	m_btnEditCopy->setText(QCoreApplication::translate("CScreenProgram", "Copy"));
	m_btnEditPaste->setText(QCoreApplication::translate("CScreenProgram", "Paste"));
	m_btnEditDelete->setText(QCoreApplication::translate("CScreenProgram", "Delete"));
	m_btnEditUndo->setText(QCoreApplication::translate("CScreenProgram", "Undo"));
	m_btnEditRedo->setText(QCoreApplication::translate("CScreenProgram", "Redo"));

	/*高级按钮文本*/
	m_btnAddvanceKeyboard->setText(QCoreApplication::translate("CScreenProgram", "Keyboard"));
	m_btnAddvanceSubroutine->setText(QCoreApplication::translate("CScreenProgram", "Subroutine"));
	m_btnAddvanceFormat->setText(QCoreApplication::translate("CScreenProgram", "Format"));
	m_btnAddvanceSearch->setText(QCoreApplication::translate("CScreenProgram", "Search"));
	m_btnAddvanceNote->setText(QCoreApplication::translate("CScreenProgram", "Note"));
	m_btnAddvanceDisable->setText(QCoreApplication::translate("CScreenProgram", "Disable"));
}

void CScreenProgram::RefreshPCLineNumber(const int lineNumber)
{
	m_codeEditor->HighlightPCLine(lineNumber);
}

void CScreenProgram::KeyboardEdit()
{
	/*更新当前行数据*/
	QTextCursor cursor = m_codeEditor->textCursor();
	cursor.select(QTextCursor::LineUnderCursor);
	cursor.insertText(CKeyboard::GetInstance()->GetLineEdit()->text());

	/*保存文件*/
	m_codeEditor->SaveFile();
}

void CScreenProgram::InsertTextBeforeLine(const QString& strText)
{
	QStringList strList = strText.split(' ');
	QString macro = strList.at(0);
	QString newText=strText;
	if (macro == CGrammarManagerFactory::STR_MACRO_FOR)
	{
		newText.append("\nNEXT");
	}
	else if (macro == CGrammarManagerFactory::STR_MACRO_SWITCH)
	{
		newText.append("\nDEFAULT\nENDSWITCH");
	}
	else if (macro==CGrammarManagerFactory::STR_MACRO_IF)
	{
		newText.append("\nENDIF");
	}
	m_codeEditor->InsertTextBeforeLine(newText);
}

/*************************************************
//  Function: 		SlotOnButtonModifyClicked
//  Description: 	响修改按钮事件，根据选取的文本不同显示不同的交互界面
//  Calls:		 	CGrammarManagerFactory::EditMacro
//  Called By: 		InitSignalSlot
//  Parameter:      void
//  Return: 		void
//  Others: 		
*************************************************/
void CScreenProgram::SlotOnButtonModifyClicked()
{
	EditMacroCallback = &CScreenProgram::UpdateCurrentLine;
	CEditParameter::GetInstance()->SetMacroInterface(this);;
	CGrammarManagerFactory::GetInstance()->EditText(GetCurrentLineText());
}

void CScreenProgram::SlotOnButtonEditClicked()
{
	m_widgetEdit->show();
	m_widgetEdit->SetBottomPosition(m_btnEdit);

}

void CScreenProgram::SlotOnButtonAddvanceClicked()
{
	m_widgetAddvance->show();
	m_widgetAddvance->SetBottomPosition(m_btnAddvance);
}

void CScreenProgram::SlotOnButtonNewClicked()
{
	//CScreenEditMacroParameter::GetInstance()->ChangeToNewParameterWidget();
	//CScreenMain::GetInstance()->ChangeToScreenNewVariable();
	EditMacroCallback = &CScreenProgram::InsertTextBeforeLine;
	CEditParameter::GetInstance()->SetMacroInterface(this);
	CScreenMain::GetInstance()->ChangeToScreenNewMacro();
}

//void CScreenProgram::SlotOnButtonBackClicked()
//{
//	CInterpreterAdapter::GetInstance()->Interpreter(m_codeEditor->toPlainText().toStdString());
//}

void CScreenProgram::SlotOnButtonMacroClicked()
{
	CScreenMain::GetInstance()->ChangeToScreenNewMacro();
}

void CScreenProgram::SlotOnSetPcClicked()
{
	m_codeEditor->HighlightPCLine(m_codeEditor->textCursor().blockNumber()+1);
}

void CScreenProgram::SlotEdit()
{
	CButton* button = (CButton*)sender();
	QTextCursor currentCursor = m_codeEditor->textCursor();

	if (button == m_btnEditSelectAll)
	{
		currentCursor.select(QTextCursor::Document);

		/*高亮显示所有行*/
		QList<QTextEdit::ExtraSelection> extraSelections;
		if (!m_codeEditor->isReadOnly()) {
			QTextEdit::ExtraSelection selection;

			QColor lineColor = QColor(Qt::blue).lighter(160);	/*高亮显示的颜色*/

			selection.format.setBackground(lineColor);
			selection.format.setProperty(QTextFormat::FullWidthSelection, true);
			selection.cursor = currentCursor;
			extraSelections.append(selection);
		}

		m_codeEditor->setExtraSelections(extraSelections);
	}
	else if (button==m_btnEditCut)
	{
		currentCursor.select(QTextCursor::LineUnderCursor);
		m_strShearPlate = currentCursor.selectedText();
		m_codeEditor->DeleteCurrentLine();
	}
	else if (button == m_btnEditCopy)
	{
		currentCursor.select(QTextCursor::LineUnderCursor);
		m_strShearPlate = currentCursor.selectedText();
	}
	/*将剪切板内容添加到光标所在行的上部*/
	else if (button == m_btnEditPaste)
	{
		currentCursor.movePosition(QTextCursor::StartOfLine);
		currentCursor.insertText(m_strShearPlate);
		currentCursor.insertBlock();
	}
	else if (button==m_btnEditDelete)
	{
		m_codeEditor->DeleteCurrentLine();
	}
	else if (button == m_btnEditUndo)
	{
		m_codeEditor->undo();
	}
	else if (button==m_btnEditRedo)
	{
		m_codeEditor->redo();
	}
	m_widgetEdit->hide();
}

void CScreenProgram::SlotAddvance()
{
	CButton* button = (CButton*)sender();

	if (button==m_btnAddvanceKeyboard)
	{
		CKeyboard* keyboard = CKeyboard::GetInstance();
		keyboard->SetConfirmCommand(CKeyboard::CONFIRM_SCREEN_PROGRAM_EDIT_PROGRAM);
		keyboard->GetLineEdit()->setText(m_codeEditor->textCursor().block().text());
		keyboard->show();
	}
	else if (button==m_btnAddvanceSubroutine)
	{

	}
	else if (button == m_btnAddvanceFormat)
	{
		m_codeEditor->DealText();
	}
	else if (button==m_btnAddvanceSearch)
	{

	}
	else if (button == m_btnAddvanceNote)
	{

	}
	else if (button == m_btnAddvanceDisable)
	{
	}
	m_widgetAddvance->hide();
}

void CScreenProgram::EditCurrentLine()
{
	QTextCursor cursor = m_codeEditor->textCursor();
	cursor.select(QTextCursor::LineUnderCursor);
	QString str = cursor.selectedText();
	EditNormalParameter(str);
}

void CScreenProgram::EditNormalParameter(const QString& strLineText)
{
	CScreenLineEditor* lineEditor = CScreenLineEditor::GetInstance(this);
	lineEditor->SetButtonSize(m_btnModify->size());
	lineEditor->SetText(strLineText);

	m_layoutMain->removeItem(m_screenBottom);

	m_layoutMain->addWidget(lineEditor);
	lineEditor->show();
}

QString CScreenProgram::GetCurrentLineText()
{
	/*获取光标所在行文本*/
	CCodeEditor* codeEditor = CCodeEditor::GetInstance();
	QTextCursor cursor = codeEditor->textCursor();
	cursor.select(QTextCursor::LineUnderCursor);
	QString text = cursor.selectedText();
	return text;
}

CScreenProgram* CScreenProgram::GetInstance()
{
	return CSingleTon<CScreenProgram>::GetInstance();
}

bool CScreenProgram::IsCreated()
{
	return CSingleTon<CScreenProgram>::IsCreated();
}

