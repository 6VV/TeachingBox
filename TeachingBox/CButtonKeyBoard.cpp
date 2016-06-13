#include "stdafx.h"
#include "CButtonKeyBoard.h"



CButtonKeyBoard::CButtonKeyBoard(QWidget* parent/*=0*/) :QToolButton(parent)
{
	Init();
}

CButtonKeyBoard::CButtonKeyBoard(const QString& text, QWidget* parent /*= 0*/) : QToolButton(parent)
{
	Init();
	this->setText(text);
}

CButtonKeyBoard::~CButtonKeyBoard()
{

}

//QSize CButtonKeyBoard::sizeHint() const
//{
//	QSize size = QToolButton::sizeHint();
//	int maxSize = qMax(size.width(), size.height());
//	size.rheight() = maxSize;
//	size.rwidth() = maxSize;
//	return size;
//}

void CButtonKeyBoard::Init()
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);/*¿Ø¼ş³äÂúÊ£Óà¿Õ¼ä*/
}
