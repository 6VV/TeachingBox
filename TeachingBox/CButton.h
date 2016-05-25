#ifndef _TEACHING_BOX_C_BUTTON_H_
#define _TEACHING_BOX_C_BUTTON_H_

#include "QPushButton"

class CButton :public QPushButton
{
	Q_OBJECT
public:
	explicit CButton(const QString &text, QWidget* parent = 0);
	explicit CButton(QWidget* parent = 0);
	~CButton();

private:
	void Init();
};

#endif