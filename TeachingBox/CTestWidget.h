#ifndef _C_TEST_WIDGTE_H_
#define _C_TEST_WIDGTE_H_

#include "QWidget"
#include "QLineEdit"
#include "CRegExpManager.h"
#include "QPushButton"
#include "CTreeWidgetWithMonitor.h"


class CTestWiget:public QWidget
{
	Q_OBJECT

public:
	CTestWiget(QWidget* parent=0);
	~CTestWiget();

private:
	void Init();
	
	protected slots:
	void SlotOnButtonClicked();

private:
	CTreeWidgetWithMonitor* m_treeWidget;
	QLineEdit* m_lineEditOldName;
	QLineEdit* m_lineEditNewName;
	QLabel* m_label;
};

#endif