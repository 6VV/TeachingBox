#include "stdafx.h"
#include <QtWidgets>
#include "CCodeEditor.h"
#include "CLineNumberArea.h"
#include "CScreenProject.h"
#include "CWarningManager.h"
#include "CFileManager.h"

CCodeEditor::CCodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
	m_lineNumberArea = new CLineNumberArea(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(SlotUpdateCodeAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(SlotUpdateLineNumberArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(SlotHighlightCurrentLine()));

	SlotUpdateCodeAreaWidth(0);
	SlotHighlightCurrentLine();

	this->setCursorWidth(0);
}

CCodeEditor::~CCodeEditor()
{
}

/*返回左侧区域宽度*/
int CCodeEditor::GetLineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10)
	{
		max /= 10;
		++digits;
	}

	/*宽度取决于行数的位数*/
	int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}


void CCodeEditor::HighlightPCLine(const int lineNumber)
{
	QTextBlock block = this->document()->findBlockByLineNumber(lineNumber - 1);
	QTextCursor cursor(block);

	m_extraSelections.clear();
	if (!isReadOnly())
	{
		m_pcSelection.format.setBackground(COLOR_PC);
		m_pcSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
		m_pcSelection.cursor = cursor;
		m_pcSelection.cursor.clearSelection();

		m_extraSelections.append(m_selectSelection);
		m_extraSelections.append(m_pcSelection);
		m_extraSelections.append(m_wrongSelection);
	}
	setExtraSelections(m_extraSelections);
}

void CCodeEditor::InsertTextBeforeLine(const QString& strText)
{
	OperatorText(INSERT_LINE, strText);
}

void CCodeEditor::InsertTextBeforeLineUnsafely(const QString& strText)
{
	_InsertTextBeforeLine(strText);
	FormatCurrentText();
}

void CCodeEditor::HighlightWrongLine(const int lineNumber)
{
	QTextBlock wrongBlock = this->document()->findBlockByLineNumber(lineNumber);
	QTextCursor wrongCursor(wrongBlock);

	m_extraSelections.clear();
	if (!isReadOnly())
	{
		m_wrongSelection.format.setBackground(COLOR_WRONG);
		m_wrongSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
		m_wrongSelection.cursor = wrongCursor;
		m_wrongSelection.cursor.clearSelection();

		m_extraSelections.append(m_selectSelection);
		m_extraSelections.append(m_pcSelection);
		m_extraSelections.append(m_wrongSelection);
	}
	setExtraSelections(m_extraSelections);
}

void CCodeEditor::_DeleteCurrentLine()
{
	QTextCursor currentCursor = this->textCursor();
	currentCursor.select(QTextCursor::BlockUnderCursor);
	currentCursor.removeSelectedText();
	++m_textChangeTime;
	if (currentCursor.atStart())
	{
		currentCursor.deleteChar();
		++m_textChangeTime;
	}
}

void CCodeEditor::_UpdateCurrentLine(const QString& text)
{
	QTextCursor textCursor = this->textCursor();
	textCursor.select(QTextCursor::LineUnderCursor);
	textCursor.removeSelectedText();
	++m_textChangeTime;
	textCursor.insertText(text);
	++m_textChangeTime;
}

void CCodeEditor::_InsertTextBeforeLine(const QString& text)
{
	moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	insertPlainText(text + "\n");
	++m_textChangeTime;
}

void CCodeEditor::OperatorText(OperatorType operatorType, const QString& text)
{
	int lastLineNumber = textCursor().blockNumber();

	switch (operatorType)
	{
	case CCodeEditor::UPDATE_LINE:
	{
		_UpdateCurrentLine(text);
	}
	break;
	case CCodeEditor::DELETE_LINE:
	{
		_DeleteCurrentLine();
	}
	break;
	case CCodeEditor::INSERT_LINE:
	{
		_InsertTextBeforeLine(text);
	}
	break;
	default:
		break;
	}

	DealText();
	SaveFile();

	while (lastLineNumber-- > 0)
	{
		this->moveCursor(QTextCursor::Down, QTextCursor::MoveAnchor);
	}
	SlotHighlightCurrentLine();
}

void CCodeEditor::SaveFile()
{
	CFileManager fileManager;
	fileManager.SetFileText(toPlainText(), CScreenProject::GetInstance()->GetOpenedFilePath());
}

void CCodeEditor::ClearWrongLine()
{
	m_extraSelections.clear();

	m_wrongSelection.format.clearBackground();
	m_wrongSelection.format.clearProperty(QTextFormat::FullWidthSelection);

	m_extraSelections.append(m_selectSelection);
	m_extraSelections.append(m_pcSelection);
	m_extraSelections.append(m_wrongSelection);

	setExtraSelections(m_extraSelections);
}

void CCodeEditor::DealText()
{
	if (ParseText())
	{
		FormatText(this->toPlainText());
	}
}

void CCodeEditor::DeleteCurrentLine()
{
	OperatorText(DELETE_LINE);
}

//void CCodeEditor::FormatText()
//{
//	CTextFormat textFormat;
//	QString strNewText;
//	textFormat.FormatText(this->toPlainText(), strNewText);
//	QPlainTextEdit::setPlainText(strNewText);
//}

void CCodeEditor::FormatText(const QString& text)
{
	CTextFormat textFormat;
	QString strNewText;
	textFormat.FormatText(text, strNewText);
	QPlainTextEdit::setPlainText(strNewText);
}

void CCodeEditor::FormatCurrentText()
{
	FormatText(toPlainText());
}

bool CCodeEditor::ParseText()
{
	CInterpreterManager* interpreterManager = CInterpreterManager::GetInstance();

	/*检查语法错误并生成语法树*/
	try
	{
		interpreterManager->AddProgramNode(CScreenProject::GetInstance()->GetOpenedFileName().toStdString(), this->toPlainText().toStdString());
	}
	catch (CExceptionInterpreter& e)
	{
		while (m_textChangeTime>0)
		{
			this->undo();
			--m_textChangeTime;
		}

		std::string strWarning;
		CInterpreterManager::GetInstance()->GetWarningInfo(e, strWarning);
		CWarningManager::GetInstance()->Warning(this, QString::fromStdString(strWarning));
		return false;
	}

	return true;
}

void CCodeEditor::HighLightEditLine(const int lineNumber)
{
	QTextBlock block = this->document()->findBlockByLineNumber(lineNumber);
	QTextCursor cursor(block);

	m_extraSelections.clear();
	if (!isReadOnly())
	{
		m_wrongSelection.format.setBackground(COLOR_EDIT);
		m_wrongSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
		m_wrongSelection.cursor = cursor;
		m_wrongSelection.cursor.clearSelection();

		m_extraSelections.append(m_selectSelection);
		m_extraSelections.append(m_pcSelection);
		m_extraSelections.append(m_wrongSelection);
	}
	setExtraSelections(m_extraSelections);
}

void CCodeEditor::setPlainText(const QString &text)
{
	FormatText(text);
}

//void CCodeEditor::SetText(const QString& text)
//{
//	setPlainText(text);
//	//ParseText();
//}

/*更新右侧区域宽度*/
void CCodeEditor::SlotUpdateCodeAreaWidth(int /* newBlockCount */)
{
	setViewportMargins(GetLineNumberAreaWidth(), 0, 0, 0);
}

/*更新左侧区域*/
void CCodeEditor::SlotUpdateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
	{
		m_lineNumberArea->scroll(0, dy);
	}
	else
	{
		/*通过调用update函数，而非直接调用PaintLineNumberArea函数，是为了保证能够实时更新*/
		m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
	}
	if (rect.contains(viewport()->rect()))
	{
		SlotUpdateCodeAreaWidth(0);
	}
}


//void CCodeEditor::SlotFormatText()
//{
//	FormatText();
//}

/*行数控件大小随编辑器大小变化*/
void CCodeEditor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), GetLineNumberAreaWidth(), cr.height()));
}

void CCodeEditor::showEvent(QShowEvent *event)
{
	m_pcSelection.format.clearBackground();
	m_pcSelection.format.clearProperty(QTextFormat::FullWidthSelection);
	m_wrongSelection.format.clearBackground();
	m_wrongSelection.format.clearProperty(QTextFormat::FullWidthSelection);
}

/*高亮显示某行*/
void CCodeEditor::SlotHighlightCurrentLine()
{
	HighLightEditLine(textCursor().blockNumber());
}

CCodeEditor* CCodeEditor::GetInstance()
{
	return CSingleTon<CCodeEditor>::GetInstance();
}

int CCodeEditor::GetPCLineNumber()
{
	return m_pcSelection.cursor.blockNumber() + 1;
}

/*************************************************
//  Function: 		PaintLineNumberArea
//  Description: 	绘制左侧行号区域
//  Calls:
//  Called By: 		CLineNumberArea::paintEvent
//  Parameter:      QPaintEvent *event
//  Return: 		void
//  Others: 		通过CLineNumberArea类的paintEvent函数来调用本函数，
以保证能够实时更新该行数
*************************************************/
void CCodeEditor::PaintLineNumberArea(QPaintEvent *event)
{
	QPainter painter(m_lineNumberArea);
	painter.fillRect(event->rect(), Qt::lightGray);	/*行数背景颜色*/
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();	/*当前行(从0开始)*/


	int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top() + fontMetrics().descent();/*绘制区域的顶部*/
	int bottom = top + (int)blockBoundingRect(block).height();	/*绘制区域的底部*/

	/*绘制可见区域*/
	while (block.isValid() && top <= event->rect().bottom())
	{
		if (block.isVisible() && bottom >= event->rect().top())
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);	/*行号颜色*/
			painter.drawText(0, top, m_lineNumberArea->width(), fontMetrics().height(),
				Qt::AlignCenter, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int)blockBoundingRect(block).height();
		++blockNumber;
	}
}

void CCodeEditor::UpdateLineText(const QString& strNewText)
{
	OperatorText(UPDATE_LINE, strNewText);
}
