#ifndef _TEACHING_BOX_C_LAYOUT_BUTTON_LIST_VERTICAL_H_
#define _TEACHING_BOX_C_LAYOUT_BUTTON_LIST_VERTICAL_H_

#include "QList"
#include "CButton.h"

class CWidgetButtonListVertical:public QWidget
{
public:
	CWidgetButtonListVertical(QList<CButton*>& btnlist, QWidget* parent = 0);
	~CWidgetButtonListVertical();

	void SetBottomPosition(CButton*);
	void SetLeftPosition(int positionTop);

protected:
	void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
	void Init();
	void RefreshLayout();

private:
	QList<CButton*> m_btnList;
	QVBoxLayout* m_layout;
};

#endif