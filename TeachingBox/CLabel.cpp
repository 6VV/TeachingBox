#include "stdafx.h"
#include "CLabel.h"

CLabel::CLabel(const QString &text, QWidget* parent /*= 0*/) :QLabel(parent)
{
	Init();
	setText(text);
}

CLabel::CLabel()
{
	Init();
}


void CLabel::Init()
{
	this->setStyleSheet("border: 1px groove gray; background-color: rgba(255, 193, 245, 0%); ");
	this->setAlignment(Qt::AlignCenter);
}
