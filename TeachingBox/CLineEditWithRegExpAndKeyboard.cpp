#include "stdafx.h"
#include "CLineEditWithRegExpAndKeyboard.h"
#include "CKeyBoard.h"
#include "CWarningManager.h"


CLineEditWithRegExpAndKeyboard::CLineEditWithRegExpAndKeyboard(const QString& text /*= ""*/, 
	const QString& regExp /*= CRegExpManager::STR_REG_ANY_CHAR*/, QWidget* const widget /*= 0*/)
	: QLineEdit(text, widget)
	, m_regExp(regExp)
{

}

CLineEditWithRegExpAndKeyboard::~CLineEditWithRegExpAndKeyboard()
{

}

void CLineEditWithRegExpAndKeyboard::KeyboardEdit()
{
	QString strText = CKeyboard::GetInstance()->GetCurrentText();

	QRegExp regExp(m_regExp);
	if (regExp.exactMatch(strText))
	{
		this->setText(strText);
	}
	else
	{
		CWarningManager::GetInstance()->Warning(this, QString("not match"));
	}
}

void CLineEditWithRegExpAndKeyboard::SetRegExp(const QString& regExp)
{
	m_regExp = regExp;
}

void CLineEditWithRegExpAndKeyboard::SetKeyboardShowMode(const KEYBOARD_SHOW_MODE mode)
{
	m_keyboardShowMode = mode;
}

void CLineEditWithRegExpAndKeyboard::mousePressEvent(QMouseEvent *event)
{
	this->selectAll();
	CKeyboard* keyboard = CKeyboard::GetInstance();

	switch (m_keyboardShowMode)
	{
	case CLineEditWithRegExpAndKeyboard::KEYBOARD_SHOW_TEXT:
	{
		keyboard->SetCurrentText(this->text());
	}break;
	default:
		break;
	}

	keyboard->SetKeyboardInterface(this);
	keyboard->show();
}
