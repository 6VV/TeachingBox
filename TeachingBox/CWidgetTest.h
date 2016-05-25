#ifndef _TEACHING_BOX_C_WIDGET_TEST_H_
#define _TEACHING_BOX_C_WIDGET_TEST_H_

#include "QWidget"
#include "QPushButton"

class CWidgetTest:public QWidget
{
	Q_OBJECT
public:
	CWidgetTest(QWidget* parent=0);
	~CWidgetTest();
protected:
	void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

protected slots:
	void SlotOnButtonCreateTableClicked();
	void SlotOnButtonInsertTableClicked();
	void SlotOnButtonDeleteTableClicked();
	void SlotOnButtonSelectTableClicked();
	void SlotOnButtonSelectOneClicked();
	void SlotOnButtonUpdateClicked();

	void SlotTest();

private:
	void Init();

private:
	const char* CLASS_NAME = "CWidgetTest";

	QPushButton* m_btnCreateTable;
	QPushButton* m_btnInsertTable;
	QPushButton* m_btnDeleteTable;
	QPushButton* m_btnSelectTable;
	QPushButton* m_btnSelectOne;
	QPushButton* m_btnUpdateTable;

	QPushButton* m_btnTest;
};

#endif