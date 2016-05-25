#include "stdafx.h"
#include "CLineNumberArea.h"


CLineNumberArea::CLineNumberArea(CCodeEditor *editor) : QWidget(editor)
{
	codeEditor = editor;
}

QSize CLineNumberArea::sizeHint() const
{
	return QSize(codeEditor->GetLineNumberAreaWidth(), 0);
}

void CLineNumberArea::paintEvent(QPaintEvent *event) 
{
	codeEditor->PaintLineNumberArea(event);
}
