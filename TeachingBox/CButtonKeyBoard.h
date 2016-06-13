#ifndef _TEACHING_BOX_C_BUTTON_KEY_BOARD_H_
#define _TEACHING_BOX_C_BUTTON_KEY_BOARD_H_

#include "QToolButton"
#include "QWidget"

class CButtonKeyBoard:public QToolButton
{
public:
	CButtonKeyBoard(QWidget* parent=0);
	CButtonKeyBoard(const QString& text, QWidget* parent = 0);
	~CButtonKeyBoard();

protected:
	//QSize sizeHint() const Q_DECL_OVERRIDE;
private:
	void Init();
};


#endif