#ifndef _TEACHING_BOX_C_LINEEDIT_WITH_REGEXP_AND_KEYBOARD_H_
#define _TEACHING_BOX_C_LINEEDIT_WITH_REGEXP_AND_KEYBOARD_H_

#include "QLineEdit"
#include "CKeyBoardInterface.h"
#include "CRegExpManager.h"

class CLineEditWithRegExpAndKeyboard:public QLineEdit,public CKeyBoardInterface
{
	Q_OBJECT

public:
		/*显示模式*/
	enum KEYBOARD_SHOW_MODE
	{
		KEYBOARD_SHOW_TEXT,	/*显示文本*/
		KEYBOARD_NOT_SHOW_TEXT,	/*不显示文本*/
	};

public:
	CLineEditWithRegExpAndKeyboard(const QString& text = "", const QString& regExp = CRegExpManager::STR_REG_ANY_CHAR, QWidget* const widget = 0);
	~CLineEditWithRegExpAndKeyboard();

	virtual void KeyboardEdit() override;

	void SetRegExp(const QString& regExp);
	void SetKeyboardShowMode(const KEYBOARD_SHOW_MODE mode);

protected:
	void mousePressEvent(QMouseEvent *event) override;

protected:
	QString m_regExp=CRegExpManager::STR_REG_ANY_CHAR;
	KEYBOARD_SHOW_MODE m_keyboardShowMode=KEYBOARD_SHOW_TEXT;
	
};

#endif